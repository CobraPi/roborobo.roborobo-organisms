#include "Symbrion/include/SymbrionControlArchitecture.h"
#include "Symbrion/include/SymbrionAgentWorldModel.h"
#include "Symbrion/include/SymbrionSharedData.h"
#include "Symbrion/include/Agents/SymbrionRobotAgent.h"
#include "World/World.h"
#include "Behavior/WallAvoidanceBehavior.h"

SymbrionControlArchitecture::SymbrionControlArchitecture( RobotAgentWorldModel* __wm ) : BehaviorControlArchitecture ( __wm )
//: DifferentialControlArchitecture( __wm )
{
    neuralNet = NULL;
}

SymbrionControlArchitecture::~SymbrionControlArchitecture()
{
}


void SymbrionControlArchitecture::reset() {
	SymbrionAgentWorldModel* worldModel = dynamic_cast<SymbrionAgentWorldModel*> (_wm);
    
	SymbrionSharedData::genomeLogFile << this->id << "," << gWorld->getIterations() << ",";
	for(uint i=0;i<this->weights.size();i++){
		SymbrionSharedData::genomeLogFile << this->weights[i] << ":";
	}
	SymbrionSharedData::genomeLogFile << std::endl;
    
	worldModel->setDistanceTraveled( 0 );
	//worldModel->setState(EGG);
}

void SymbrionControlArchitecture::step() {
	SymbrionAgentWorldModel* worldModel = dynamic_cast<SymbrionAgentWorldModel*> (_wm );
	SymbrionRobotAgentPtr agent = boost::dynamic_pointer_cast<SymbrionRobotAgent>(gWorld->getAgent( worldModel->_agentId ));
    
	if(agent->getState() == EGG || agent->getState() == SEED){
        _wm->_desiredRotationalVelocity = 0;
        _wm->_desiredTranslationalValue = 0;
		return;
    }

    WallAvoidanceBehavior::step(worldModel);
//    return;
//
//    if(agent->getState() == FREERIDER && !agent->isPartOfOrganism()){
//        avoidObstacles();
//        return;
//    }
//    
//	// retrieve the values of the distance sensors
//	vector<double> inputs = this->getSensorValues();
//	if (inputs.size() == 0) {
//		cerr << "ERR: robot did not return any distance sensor data - can't build neural net" << endl;
//		return;
//	}
//	
//	if( SymbrionSharedData::connectionType == 7 )
//	{
//		std::vector<RobotAgentPtr> *connected = agent->getConnected();
//		
//		int i = 0;
//		
//		for( ; i < connected->size(); i++ )
//		{
//			SymbrionAgentWorldModel* othersWorldModel = dynamic_cast<SymbrionAgentWorldModel*> (connected->at(i)->getWorldModel());
//			
//			inputs.push_back( othersWorldModel->_leftWheelSpeed );
//			inputs.push_back( othersWorldModel->_rightWheelSpeed );
//		}
//		for( ; i < 4; i++ )
//		{
//            inputs.push_back( 0 );
//            inputs.push_back( 0 );
//		}
//	}
//	
//	inputs.push_back(1); // bias node
//    
//	// Output is a vote for 1 of 5 strategies.
//	//vector<double> outputs = vector<double>(5, 0);
//	
//	vector<double> outputs = vector<double>(2,0);
//    
//	// create the neural net if it didn't exist already
//	if (neuralNet == NULL) {
//		neuralNet = new Neural::ESN( weights, inputs.size(), outputs.size(), 200, 0.05, 0.88, 1, 1, 1, 1, 1,
//                                    false,	// allowInputToOutputDirectConnections		<-- works
//                                    false,	// allowOutputSelfRecurrentConnections		<-- works, but not with allowInputToOutputDirectConnections if input == 0, and do not work with big reservoirs (why ???)
//                                    true,	// allowInputToReservoirConnections   		<-- needed
//                                    false);
//		//neuralNet->setActivationFunction(&ActivationFunctionTanh::apply,&ActivationFunctionTanh::apply);
//        
//		// load the weights
//		//neuralNet->loadParameters(&weights.front(), weights.size());
//		weights = std::vector<double>(neuralNet->computeRequiredNumberOfWeights(), 0.42);
//		neuralNet->setWeigths(weights);
//	}
//    
//	neuralNet->setInputs( inputs );
//	// set the sensor values as input neuron values
//	neuralNet->step();
//    
//	// and calculate the output
//	//neuralNet->getOutputValues(&outputs.front());
//	outputs = neuralNet->readOut();
//    
//	if(gWorld->getIterations() % gOrganismSampleFrequency == 0) {
//		SymbrionSharedData::outputLogFile << worldModel->_agentId << "," << gWorld->getIterations() << "," << outputs[0] << "," << outputs[1] << std::endl;
//	}
//    
////  double left, right;
////	left = right = outputs[0];
////	double angle = outputs[1] * 180;
////	followAngle(angle,left,right);
//	double left = (outputs[0] / 2.0) + 1;
//	double right = (outputs[1] / 2.0) + 1;
//	worldModel->_leftWheelSpeed = left;
//	worldModel->_rightWheelSpeed = right;
//    
//    setSpeed(left,right);
}

vector<double> SymbrionControlArchitecture::getSensorValues() {
	SymbrionAgentWorldModel* worldModel = dynamic_cast<SymbrionAgentWorldModel*> (_wm);
    
	vector<double> sensors;
	for (int i = 0; i < worldModel->getDefaultSensors()->getSensorCount(); i++) {
		sensors.push_back(worldModel->getDefaultSensors()->getSensorDistanceValue(i) / worldModel->getDefaultSensors()->getSensorMaximumDistanceValue(i));
	}
	
	RobotAgentPtr agent = gWorld->getAgent(worldModel->_agentId);
	if(agent->getOrganism()) {
		sensors.push_back(agent->getOrganism()->size() / gWorld->getNbOfAgent());
	} else {
		sensors.push_back(0);
	}
    
	return sensors;
}

void SymbrionControlArchitecture::followAngle(double angle, double &left, double &right) {
	double diffAngle = angle - _wm->_agentAbsoluteOrientation;
	if (diffAngle < -180.0) {
		diffAngle += 360.0;
	}
	if (diffAngle > 180.0) {
		diffAngle -= 360.0;
	}
	
	if (diffAngle > 5) {
		right -= 0.4;
	} else if (diffAngle < -5) {
		left -= 0.4;
	} else if (diffAngle > 0) {
		right -= 0.01;
	} else if (diffAngle < 0) {
		left -= 0.01;
	}
}