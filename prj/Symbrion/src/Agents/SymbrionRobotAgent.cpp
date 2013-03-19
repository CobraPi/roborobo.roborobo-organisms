#include "Symbrion/include/Agents/SymbrionRobotAgent.h"
#include "Symbrion/include/SymbrionSharedData.h"
#include "boost/lexical_cast.hpp"

int RenderTextToSurface(std::string Text, int x, int y, SDL_Surface *Dest);

SymbrionRobotAgent::SymbrionRobotAgent(AgentState state, World *__world) :
	RobotAgent( __world )
{
	this->_state = state;
    if(state!=FREERIDER){
        this->_genome = new SymbrionOrganismGenome( (SymbrionAgentWorldModel*)this->_wm );
    }
    setMask();
}

SymbrionRobotAgent::~SymbrionRobotAgent()
{
}

void SymbrionRobotAgent::setState( AgentState _state ) {
	this->_state = _state;
    setMask();
}

void SymbrionRobotAgent::setMask(){
    switch(_state){
        case EGG:
        case ADULT:
            this->setAgentMask(SymbrionSharedData::gEggAgentMaskImage);
            break;
        case SEED:
            this->setAgentMask(SymbrionSharedData::gSeedAgentMaskImage);
            break;
        default:
            this->setAgentMask(SymbrionSharedData::gFreeRiderAgentMaskImage);
            break;
    }
}

AgentState SymbrionRobotAgent::getState() const {
	return this->_state;
}

SymbrionAgentWorldModel* SymbrionRobotAgent::getWorldModel(){
    return dynamic_cast<SymbrionAgentWorldModel*>(_wm);
}

SymbrionOrganismGenome* SymbrionRobotAgent::getGenome(){
    return _genome;
}

void SymbrionRobotAgent::toSeed()
{
	SymbrionOrganismGenome* newGenome = NULL;
	bool crossover = false;
    bool mutation = false;
    
    if( _receivedGenomes.size() >= 2 )
	{
		int i = rand() % _receivedGenomes.size();
		int j = i;
		while( j == i )
		{
			j = rand() % _receivedGenomes.size();
		}
		
		newGenome = _receivedGenomes.at(i)->crossover( _receivedGenomes.at(j) );
		_receivedGenomes.clear();
        crossover = true;
	}
	else if( _receivedGenomes.size() == 1 )
	{
		int j = rand() % _receivedGenomes.size();
		
		newGenome = _genome->crossover( _receivedGenomes.at(j) );
		_receivedGenomes.clear();
        crossover = true;
	}
	else
	{
		newGenome = _genome;
        mutation = true;
	}
	
	_genome = newGenome->mutate();
    
	this->setState(SEED);
	
    if(this->getOrganism() != NULL)
        this->getOrganism()->setInactive();
    
    //	gLogFile << "Creating new organism: " << this->getOrganism()->getId() << " robots: " << this->_wm->_agentId << std::endl;
	this->setOrganism(new Organism());
	this->getOrganism()->addRobot(shared_from_this());
	Organism::add(this->getOrganism());
	
	SymbrionSharedData::gEventLogFile << this->getWorldModel()->_world->getIterations() << "," << this->_genome->getId() << ",6," << this->getWorldModel()->getPosition() << "," << ((SymbrionAgentWorldModel*)this->getWorldModel())->getStateLifetime() << ",toSeed\n";
    
    ((SymbrionAgentWorldModel*)this->getWorldModel())->setStateLifetime(0);
    
    // We are the first robot
    _genome->append();
}

void SymbrionRobotAgent::toEgg()
{
	this->_genome->log();
	//std::cerr << "size: " << this->getOrganism()->agents.size() << "\n";
	for( int i = this->getOrganism()->agents.size() - 1; i > 0 ; i-- )
	{
		//std::cerr << "i: " << i <<  "\n";
		if( this->getOrganism()->agents.at(i)->getWorldModel()->_agentId != this->getWorldModel()->_agentId )
		{
			this->getOrganism()->agents.at(i)->letGoOfOrganism();
		}
	}
    
	this->setState( EGG );
    
	SymbrionSharedData::gTravelLogFile << this->getWorldModel()->_world->getIterations() << "," << this->_genome->getId() << "," << this->getWorldModel()->getDistanceTraveled() << "\n";
	this->getWorldModel()->setDistanceTraveled(0);
	SymbrionSharedData::gEventLogFile << this->getWorldModel()->_world->getIterations() << "," << this->_genome->getId() << ",2," << this->getWorldModel()->getPosition() << "," << this->getOrganism()->getAge() << ",toEgg\n";
    
    ((SymbrionAgentWorldModel*)this->getWorldModel())->setStateLifetime(0);
    if(this->getOrganism() != NULL)
        this->getOrganism()->setInactive();
	this->setOrganism(NULL);
}

void SymbrionRobotAgent::abortion(){
    for( int i = this->getOrganism()->agents.size() - 1; i > 0 ; i-- )
	{
		//std::cerr << "i: " << i <<  "\n";
		if( this->getOrganism()->agents.at(i)->getWorldModel()->_agentId != this->getWorldModel()->_agentId )
		{
			this->getOrganism()->agents.at(i)->letGoOfOrganism();
		}
	}
//
//    this->_genome = new SymbrionOrganismGenome( (SymbrionAgentWorldModel*)_wm );
//    this->toSeed();

    this->setState( EGG );
	  
    this->getWorldModel()->setDistanceTraveled(0);
    
    SymbrionSharedData::nrAbortions++;
    
    SymbrionSharedData::gEventLogFile << this->getWorldModel()->_world->getIterations() << "," << this->_genome->getId() << ",1," << this->getWorldModel()->getPosition() << "," << ((SymbrionAgentWorldModel*)this->getWorldModel())->getStateLifetime() << ",abortion\n";
    SymbrionSharedData::gEventLogFile << this->getWorldModel()->_world->getIterations() << "," << this->_genome->getId() << ",2," << this->getWorldModel()->getPosition() << "," << ((SymbrionAgentWorldModel*)this->getWorldModel())->getStateLifetime() << ",toEgg\n";
    
    ((SymbrionAgentWorldModel*)this->getWorldModel())->setStateLifetime(0);
}

bool SymbrionRobotAgent::isComplete()
{
	return _genome->isComplete();
}

void SymbrionRobotAgent::stepBehavior() {
	// update status
	_iterations++;
	
	if (_wm->_isAlive) // note: reviving a robot is performed in stepGenetics()
	{
		// compute commands
		_behavior->step();
		((SymbrionAgentWorldModel*)_wm)->setStateLifetime( ((SymbrionAgentWorldModel*)_wm)->getStateLifetime()+1 );
	}
	
	displayInfo();
}

bool SymbrionRobotAgent::receiveGenome( SymbrionOrganismGenome* _genome )
{
	//check if already received
	for( unsigned int i = 0; i < _receivedGenomes.size(); i++ )
	{
		if( _receivedGenomes.at(i)->getId() == _genome->getId() )
			return false;
	}
    SymbrionOrganismGenome* copy = new SymbrionOrganismGenome((SymbrionAgentWorldModel*)this->getWorldModel(), _genome);
	_receivedGenomes.push_back( copy );
	return true;
}

int SymbrionRobotAgent::plannedSize()
{
	return this->_genome->getCoords().size();
}