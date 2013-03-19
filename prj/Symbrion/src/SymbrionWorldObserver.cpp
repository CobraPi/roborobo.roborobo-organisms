#include <math.h>

#include "Symbrion/include/SymbrionWorldObserver.h"

#include "World/World.h"
#include "Symbrion/include/SymbrionSharedData.h"
#include "Symbrion/include/SymbrionAgentWorldModel.h"
#include "Symbrion/include/Agents/SymbrionRobotAgent.h"
#include "BehaviorControlArchitectures/EvolutionaryControlArchitecture.h"

SymbrionWorldObserver::SymbrionWorldObserver(World *__world) : WorldObserver(__world)
{
	_world = __world;
    // override initialization of agents by the standard world.
    _world->setInitializeAgents(false);
    _world->setSetUpConnections(false);

    // load environment and agents files
	if (loadFiles() == false) {
		std::cout << "[critical] cannot load image files." << std::endl;
		exit(1);
	}
}

SymbrionWorldObserver::~SymbrionWorldObserver()
{
}

void SymbrionWorldObserver::reset()
{
	if(SymbrionSharedData::outputLogFile){
		SymbrionSharedData::outputLogFile.close();
	}
	SymbrionSharedData::outputLogFile.open(SymbrionSharedData::gOutputLogFilename.c_str()); //, std::ofstream::out | std::ofstream::app);
	if (!SymbrionSharedData::outputLogFile) {
		std::cout << "[error] Cannot open log file" << SymbrionSharedData::gOutputLogFilename <<"." << std::endl << std::endl;
		exit(1);
	}
	SymbrionSharedData::outputLogFile << "id,timestep,strategy,ring" << std::endl;
    
    resetHeatmap();
    
    initWorld();
}

void SymbrionWorldObserver::step()
{
    updateWorld();
}

void SymbrionWorldObserver::initWorld()
{

	if (gUseOrganisms) {
        Organism::reset();
	}
	   
    int nbEggs = gNbOfAgents / 10;
    int nbSeeds = gNbOfAgents / 10;
    int nbFree = (gNbOfAgents - nbEggs) - nbSeeds;
	
	// initialize eggs
	for (int i = 0; i != nbEggs; i++) {
        SymbrionRobotAgentPtr egg = boost::make_shared<SymbrionRobotAgent>(EGG, _world);
        egg->getWorldModel()->setDistanceTraveled(0);
        egg->setOrganism(NULL);
        // Set random start age
        ((SymbrionAgentWorldModel*)egg->getWorldModel())->setStateLifetime(rand()%SymbrionSharedData::maxEggLifetime);
        _world->addAgent(boost::dynamic_pointer_cast<RobotAgent>(egg));
	}
	
	// initialize seed
	for (int i = 0; i != nbSeeds; i++) {
        SymbrionRobotAgentPtr seed = boost::make_shared<SymbrionRobotAgent>(SEED, _world);
		_world->addAgent(boost::dynamic_pointer_cast<RobotAgent>(seed));
		seed->toSeed();
        ((SymbrionAgentWorldModel*)seed->getWorldModel())->setStateLifetime(rand()%SymbrionSharedData::maxSeedLifetime);
	}
	
	// initialize agents
	for (int i = 0; i != nbFree; i++) {
		_world->addAgent(boost::dynamic_pointer_cast<RobotAgent>(boost::make_shared<SymbrionRobotAgent>(FREERIDER, _world)));
	}
	
	gRadioNetworkArray.resize(gAgentCounter);
	for (int i = 0; i != gAgentCounter; i++)
		(gRadioNetworkArray.at(i)).reserve(gAgentCounter);
	
    SymbrionSharedData::nrAbortions = 0;
}

void SymbrionWorldObserver::updateWorld() {
	int nrOfSeeds = 0;
	int nrOfEggs = 0;
	int nrOfAdults = 0;
	int nrOfLooseFreeRiders = 0;
    
	// * move the dot -- apply (limited) physics
	for (int i = 0; i < _world->getNbOfAgent(); i++) {
		// move agent
		if (getAgent(i)->getState() == EGG ) {
			nrOfEggs++;
			if( (getAgent(i)->getWorldModel())->getStateLifetime() >= SymbrionSharedData::maxEggLifetime ) {
				getAgent(i)->toSeed();
			}
		} else if (getAgent(i)->getState() == SEED ) {
            nrOfSeeds++;
            
            if((getAgent(i)->getWorldModel())->getStateLifetime() >= SymbrionSharedData::maxSeedLifetime ){
                //                    std::cerr << "Abortion " << organism->getId() << "\n";
                getAgent(i)->abortion();
            }
            boardcastSeed(i);
        } else if (getAgent(i)->getState() == ADULT) {
			//std::cerr << agents[shuffledIndex[i]]->getOrganism()->size() << "\\" << agents.at(shuffledIndex[i])->plannedSize() << "\n";
			Organism* organism = getAgent(i)->getOrganism();
			nrOfAdults++;
            if(organism->getAge() >= SymbrionSharedData::maxOrganismLifetime ) {
//                    std::cerr << "toEgg " << organism->getId() << "\n";
                getAgent(i)->toEgg();
            }
            boardcastOrganism(i);
		} else {
			if( !getAgent(i)->isPartOfOrganism() )
			{
				nrOfLooseFreeRiders++;
			}
        }
		
		//std::cerr << "x" << agents.at( shuffledIndex[i] )->getWorldModel()->_xReal << " y: " << agents.at( shuffledIndex[i] )->getWorldModel()->_yReal << std::endl;
        Point2d position = getAgent(i)->getWorldModel()->getPosition();
		_heatmap[position.x][position.y]++;
	}
	
	if( _world->getIterations() % 5000 == 0 )
		logHeatmap();
	
	if( _world->getIterations() % 200 == 0 ) {
		SymbrionSharedData::gPopLogFile << _world->getIterations() << "," << _world->getNbOfAgent() << "," << nrOfEggs << "," << nrOfSeeds << "," << nrOfAdults << "," << nrOfLooseFreeRiders << "," << SymbrionSharedData::nrAbortions << std::endl;
        SymbrionSharedData::nrAbortions = 0;
        std::cerr << _world->getIterations() << "," << _world->getNbOfAgent() << "," << nrOfEggs << "," << nrOfSeeds << "," << nrOfAdults << "," << nrOfLooseFreeRiders << "\n";
    }
	
}

void SymbrionWorldObserver::boardcastOrganism(int index)
{
    SymbrionRobotAgentPtr broadcaster = getAgent(index);
	if( broadcaster->getState() == ADULT && broadcaster->getGenome()->isComplete() )
	{
		std::vector<SymbrionRobotAgentPtr> nearEggs;
		for( int i = 0; i < gAgentCounter; i++ )
		{
            Point2d position = getAgent(i)->getWorldModel()->getPosition();
			int distance = position.distance(broadcaster->getWorldModel()->getPosition());
			if(getAgent(i)->getState() == EGG && distance <= SymbrionSharedData::maxOrganismBoardcastRange )
				nearEggs.push_back(getAgent( i ) );
		}
		
		for( unsigned int i = 0; i < nearEggs.size(); i++ )
		{
			if(nearEggs.at(i)->receiveGenome( broadcaster->getGenome() ) );
            SymbrionSharedData::gEventLogFile << broadcaster->getWorldModel()->_world->getIterations() << "," << broadcaster->getGenome()->getId() << ",3, boardcast genome\n";
		}
	}
}

void SymbrionWorldObserver::boardcastSeed(int index)
{
    SymbrionRobotAgentPtr broadcaster = getAgent(index);
	if( broadcaster->getState() == SEED && !broadcaster->getGenome()->isComplete() )
	{
        assert(broadcaster->getOrganism() != NULL);
		std::vector<RobotAgentPtr> nearRobots;
		for(int i = 0; i < _world->getNbOfAgent(); i++)
		{
            Point2d position = getAgent(i)->getWorldModel()->getPosition();
			int distance = position.distance(broadcaster->getWorldModel()->getPosition());			if(getAgent(i)->getState() == FREERIDER && distance <= SymbrionSharedData::maxSeedBoardcastRange && !getAgent(i)->isPartOfOrganism()){
				nearRobots.push_back(getAgent(i));
            }
		}
		for(unsigned int i =0 ; i < nearRobots.size(); i++ )
		{
			SymbrionSharedData::gEventLogFile << _world->getIterations() << "," << broadcaster->getGenome()->getId() << ",4," << broadcaster->getWorldModel()->getPosition() << "," << broadcaster->getWorldModel()->getStateLifetime() << ",boardcast recruitment\n";
			RobotAgentPtr other = nearRobots.at(i);
            other->unregisterAgent();

            assert(getAgent(other->getWorldModel()->_agentId)->getState() != SEED);
            assert(broadcaster->getOrganism() != NULL);
            
            Point2d oldPosition = other->getWorldModel()->getPosition();
            Point2d broadcasterPosition = broadcaster->getWorldModel()->getPosition();
			
			Coord2d coord = broadcaster->getGenome()->current();
            
            //            std::cout << "relative location: " << coord._x << ", " << coord._y << std::endl;
			
            Point2d newPosition;
            newPosition.x = broadcasterPosition.x + coord._x * gAgentWidth;
            newPosition.y = broadcasterPosition.y + coord._y * gAgentHeight;
            
            //std::cout << this->getWorldModel()->_agentId << " at location: " << this->getWorldModel()->_xReal << ", " << this->getWorldModel()->_yReal << " found another robot to connect " << other->getWorldModel()->_agentId << " setting coordinates from: " << x_old << ", " << y_old << " to: " << x << ", " << y;
            
            assert(broadcaster->getOrganism() != NULL);
			other->setCoord((int)newPosition.x+0.5, (int)newPosition.y+0.5);
            other->setCoordReal(newPosition.x, newPosition.y);
			if(other->isCollision())
			{
				other->setCoord((int)oldPosition.x+0.5, (int)oldPosition.y+0.5 );
                other->setCoordReal(oldPosition.x, oldPosition.y);
                
                other->registerAgent();
				continue;
			}
			else
			{
                broadcaster->getOrganism()->addRobot(other);
                other->setOrganism(broadcaster->getOrganism());
                
                other->registerAgent();
                broadcaster->getGenome()->append();
			}
			
			if( broadcaster->isComplete() )
			{
                gLogFile << _world->getIterations() << "," << broadcaster->getGenome()->getId() << broadcaster->getWorldModel()->getStateLifetime() << std::endl;
				//broadcaster->dockESN();
				
				SymbrionSharedData::gEventLogFile << _world->getIterations() << "," <<  broadcaster->getGenome()->getId() << ",5," << broadcaster->getWorldModel()->getPosition() << "," << broadcaster->getWorldModel()->getStateLifetime() << ",toOrganism\n";
                
				broadcaster->getWorldModel()->setStateLifetime( 0 );
                broadcaster->setState(ADULT);
				return;
			}
			
		}
		
	}
}


SymbrionRobotAgentPtr SymbrionWorldObserver::getAgent(int index){
    return boost::dynamic_pointer_cast<SymbrionRobotAgent>(_world->getAgent(index));
}

bool SymbrionWorldObserver::loadFiles()
{
    bool returnValue = true;
    
	SymbrionSharedData::gEggAgentMaskImage = load_image(SymbrionSharedData::gEggAgentMaskImageFilename);
	if (SymbrionSharedData::gEggAgentMaskImage == NULL) {
		std::cerr << "Could not load egg agent mask image\n";
		returnValue = false;
	}
	
	SymbrionSharedData::gSeedAgentMaskImage = load_image(SymbrionSharedData::gSeedAgentMaskImageFilename);
	if (SymbrionSharedData::gSeedAgentMaskImage == NULL) {
		std::cerr << "Could not load seed agent mask image\n";
		returnValue = false;
	}
	
	SymbrionSharedData::gFreeRiderAgentMaskImage = load_image(SymbrionSharedData::gFreeRiderAgentMaskImageFilename);
	if (SymbrionSharedData::gFreeRiderAgentMaskImage == NULL) {
		std::cerr << "Could not load free rider agent mask image\n";
		returnValue = false;
	}
    
	SymbrionSharedData::gEggAgentSpecsImage = load_image(SymbrionSharedData::gEggAgentSpecsImageFilename);
	if (SymbrionSharedData::gEggAgentSpecsImage == NULL) {
		std::cerr << "Could not load egg agent mask image\n";
		returnValue = false;
	}
	
	SymbrionSharedData::gSeedAgentSpecsImage = load_image(SymbrionSharedData::gSeedAgentSpecsImageFilename);
	if (SymbrionSharedData::gSeedAgentSpecsImage == NULL) {
		std::cerr << "Could not load seed agent mask image\n";
		returnValue = false;
	}
	SymbrionSharedData::gFreeRiderAgentSpecsImage = load_image(SymbrionSharedData::gFreeRiderAgentSpecsImageFilename);
	if (SymbrionSharedData::gFreeRiderAgentSpecsImage == NULL) {
		std::cerr << "Could not load free rider agent mask image\n";
		returnValue = false;
	}
	
	return returnValue;
};

void SymbrionWorldObserver::logHeatmap()
{
	std::stringstream filename;
    //	filename << SymbrionSharedData::gHeatmapLogFilename << "." << _iterations << ".log";
    filename << "logs/heatmap." << gWorld->getIterations() << ".log";
	std::ofstream heatmapFile;
	heatmapFile.open( filename.str().c_str());
	
	for( int x = 0; x < gAreaWidth; x++ )
	{
		for( int y = 0; y < gAreaHeight; y++ )
		{
			heatmapFile << _heatmap[x][y];
			if( x != gAreaWidth - 1 )
				heatmapFile << ",";
		}
		heatmapFile << "\n";
	}
	
	heatmapFile.close();
}

void SymbrionWorldObserver::resetHeatmap(){
    for( int x = 0; x < gAreaWidth; x++ )
    {
		std::vector<int> row;
        for( int y = 0; y < gAreaHeight; y++ )
		{
			row.push_back( 0 );
		}
		_heatmap.push_back( row );
	}
}
