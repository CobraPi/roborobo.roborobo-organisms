/*
 *  MedeaSpecializePerceptronControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Nicolas on 15/09/10
 *
 */

#include "mEDEA-specialize/include/MedeaSpecializePerceptronControlArchitecture.h"

#include "World/World.h"

#include "Utilities/Misc.h"

#include <math.h>

MedeaSpecializePerceptronControlArchitecture::MedeaSpecializePerceptronControlArchitecture( RobotAgentWorldModel *__wm )
{
	_wm = (MedeaSpecializeAgentWorldModel*)__wm;
	//_wm->_genome.resize(18);
	_iteration = 0;

	//_initialEnergy = 2*MedeaSpecializeSharedData::gEvaluationTime*0.1;  // original CEC
	//_initialEnergy = 2*MedeaSpecializeSharedData::gEvaluationTime;  // original CEC, adapte avec nv schema
	//_initialEnergy = MedeaSpecializeSharedData::gEvaluationTime*0.1 / 2; // half the maximum lifetime.  // used to be: 2*MedeaSpecializeSharedData::gEvaluationTime*0.1;
	//_initialEnergy = MedeaSpecializeSharedData::gEvaluationTime*2; // MUST BE AT LEAST MedeaSpecializeSharedData::gEvaluationTime otw a dead agent will never be revived  // / 2.0; // half the maximum lifetime + nv schema (entier)
	
	_wm->setEnergyLevel(MedeaSpecializeSharedData::gEnergyInit);
	//_deltaEnergy = 10.0; // CEC
	_wm->setDeltaEnergy(0.0); // MedeaSpecialize

	_wm->setAngleToClosestEnergyPoint(0);
	_wm->setDistanceToClosestEnergyPoint(0);

	gProperties.checkAndGetPropertyValue("nbHiddenNeurons",&_nbHiddenNeurons,true);
	
	_wm->setActiveStatus(true);

	if ( gVerbose )
	{
		//std::cout << "robot #" << _wm->_agentId << " perceptron \n" ;
	}
}

MedeaSpecializePerceptronControlArchitecture::~MedeaSpecializePerceptronControlArchitecture()
{
	// nothing to do.
}

void MedeaSpecializePerceptronControlArchitecture::reset()
{
	_parameters.clear();
	_parameters = _wm->_genome;
}


// perform one controller update
// set motor value using motor format.
void MedeaSpecializePerceptronControlArchitecture::step()
{
	_iteration++;

	if ( _wm->getNewGenomeStatus() ) // check for new NN parameters
	{
		reset();
		_wm->setNewGenomeStatus(false);
	}

/*	if ( _wm->_age < 0 ) // problem: _age is nowhere to be incremented
	{
		// ** security control (prior to a new behavior, get out of crash situation) -- random noise to avoid "STALL" status
		_wm->_desiredTranslationalValue = ( ranf()*2.-1. ) * gMaxTranslationalSpeed ;
		_wm->_desiredRotationalVelocity =( ranf()*2.-1. ) * gMaxRotationalSpeed ;
		currentObserver->setKey( ( ranf()*2.-1. ) * MedeaSpecializeSharedData::gMaxKeyRange);
		return;
	}
*/
	//_wm->_desiredTranslationalValue = 0.0;
	//_wm->_desiredRotationalVelocity = 0.0;
//	double keyToSet = 0.0;

	if ( _wm->getActiveStatus() == true )
	{
		double angleToClosestEnergyPoint = 0.0;
		double shortestDistance = 0.0;
		if ( MedeaSpecializeSharedData::gExperimentNumber == 2)
		{
			//the only energy point is on the east of the map
			angleToClosestEnergyPoint = 90;  // SOUTH!
		}
		else
		{
			//search the current active energy point

			//Point2d posRobot(_wm->_xReal,_wm->_yReal);
//			Point2d closestPoint = gEnergyPoints[MedeaSpecializeSharedData::gActiveSun].getPosition();

			//shortestDistance = getEuclidianDistance (posRobot,closestPoint);

			//compute the orientation of the active sun ( in degree between 0 and 360 )
			///angleToClosestEnergyPoint = (atan2(closestPoint.y-posRobot.y,closestPoint.x-posRobot.x)/M_PI)*180.0;
			//angleToClosestEnergyPoint += 360.0 ;
			//angleToClosestEnergyPoint = computeModulo(angleToClosestEnergyPoint,360.0);
			//if ( angleToClosestEnergyPoint > 180 ) // force btw -180 and 180
			//	angleToClosestEnergyPoint -= 360.0;
		}

		//compute the angle between the actual orientation of the robot and the orientation of the closest energy point ( in degree between -180 and 180 )
		//double diffAngleToClosestEnergyPoint = angleToClosestEnergyPoint -  _wm->_agentAbsoluteOrientation ;
		//if ( diffAngleToClosestEnergyPoint < -180.0 )
		//{
		//	diffAngleToClosestEnergyPoint += 360.0 ;
		//}
		//if ( diffAngleToClosestEnergyPoint > 180.0 )
		//{
		//	diffAngleToClosestEnergyPoint -= 360.0 ;
		//}

//		if ( gAgentIndexFocus == _wm->_agentId )
		//if ( gVerbose && gInspectAgent && gAgentIndexFocus == _wm->_agentId )
		//{
		//	std::cout << "Sun           : " << diffAngleToClosestEnergyPoint << "°, " << shortestDistance << "mm"<< std::endl;
		//}
		
		//cast the diffAngle between -1 and 1
		//diffAngleToClosestEnergyPoint = diffAngleToClosestEnergyPoint / 180.0 ;
		//_wm->setEnergyPointDirectionAngleValue(diffAngleToClosestEnergyPoint);
		
		//cast the shortest distance between 0 and 1
		if ( shortestDistance > gSensorRange )
			shortestDistance = 1.0;
		else
			shortestDistance = shortestDistance / (double)gSensorRange;
		_wm->setEnergyPointDistanceValue(shortestDistance);

//		if ( gAgentIndexFocus == _wm->_agentId )
		//if ( gVerbose && gInspectAgent && gAgentIndexFocus == _wm->_agentId )
		//{
		//	std::cout << "SunSensorValue: " << _wm->getEnergyPointDirectionAngleValue() << " , " << _wm->getEnergyPointDistanceValue() << std::endl;
		//}

		std::vector<double> hiddenLayer;
		hiddenLayer.resize(_nbHiddenNeurons);
		for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		{
			hiddenLayer[j] = 0.0;
		}

		int geneToUse = 0;

		// inputs to hidden Layer

		// distance sensors
		//for ( int i = 0 ; i < _wm->_sensorCount ; i++ )
		//{
		//	for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		//	{
		//		hiddenLayer[j] += (_wm->getSensorDistanceValue(i)/_wm->getSensorMaximumDistanceValue(i)) * _parameters[geneToUse] ;  // !N - corrected BUG di0319 - use normalized sensor value in 0...1
		//		geneToUse ++;
		//	}
		//}

		//floor sensor
		//for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		//{
		//	if ( _wm->_floorSensor != 0 )		// binary detector -- either something, or nothing.
		//		hiddenLayer[j] += 1.0  * _parameters[geneToUse];
		//	//hiddenLayer[j] += _wm->_floorSensor/255.0  * _parameters[geneToUse];
		//	geneToUse ++;
		//}

		//direction of the closest energy point
		for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		{
			hiddenLayer[j] += _wm->getEnergyPointDirectionAngleValue()  * _parameters[geneToUse];		// ??? !N - should be angle. naming problem, code correct.
			geneToUse ++;
		}

		//direction of the closest energy point
		for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		{
			hiddenLayer[j] += _wm->getEnergyPointDistanceValue()  * _parameters[geneToUse];  // ??? !N - should be reference to _wm (convention with previous)
			geneToUse ++;
		}

		//energy level
		for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		{
			hiddenLayer[j] +=  (_wm->getEnergyLevel()/MedeaSpecializeSharedData::gEnergyMax)  * _parameters[geneToUse];  // !N : added: energy value normalization btw 0 and 1.
			geneToUse ++;
		}

		//bias
		for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		{
			hiddenLayer[j] += 1.0  * _parameters[geneToUse];
			geneToUse ++;
		}

		//activation function on hidden layer
		for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		{
			hiddenLayer[j] = tanh(hiddenLayer[j]);
		}
		
		//hiddenLayer to output
		//_wm->_desiredTranslationalValue = 0;
		//_wm->_desiredRotationalVelocity = 0;
		//keyToSet = 0.0;
		//for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		//{
		//	_wm->_desiredTranslationalValue += hiddenLayer[j] * _parameters[geneToUse] ;
		//	geneToUse ++;
		//}
		//for (int j= 0 ; j < _nbHiddenNeurons ; j++ )
		//{
		//	_wm->_desiredRotationalVelocity += hiddenLayer[j] * _parameters[geneToUse] ;
		//	geneToUse ++;
		//}
		
		//_wm->_desiredTranslationalValue += 1.0 * _parameters[geneToUse] ;
		//geneToUse ++;
		//_wm->_desiredRotationalVelocity += 1.0 * _parameters[geneToUse] ;

		//activation function on output
		//_wm->_desiredTranslationalValue = tanh( _wm->_desiredTranslationalValue ) ;  // !N note that tanh is optional for ANN outputs.
		//_wm->_desiredRotationalVelocity = tanh( _wm->_desiredRotationalVelocity );
	
		// normalize to motor interval values
		//_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue * gMaxTranslationalSpeed;
		//_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity * gMaxRotationalSpeed;
		
		// penalize if needed (only experiment no.2, in energy zone)
		//if ( MedeaSpecializeSharedData::gExperimentNumber == 2 && ( (int)_wm->_xReal >= MedeaSpecializeSharedData::g_xStart_EnergyZone && (int)_wm->_yReal >= MedeaSpecializeSharedData::g_yStart_EnergyZone && (int)_wm->_xReal <= MedeaSpecializeSharedData::g_xEnd_EnergyZone && (int)_wm->_yReal <= MedeaSpecializeSharedData::g_yEnd_EnergyZone ) )
		//{
		//	double penalizationFactor = ( 1.0 - MedeaSpecializeSharedData::gMaxPenalizationRate )  + MedeaSpecializeSharedData::gMaxPenalizationRate*(MedeaSpecializeSharedData::gZoneEnergy_harvestValue/MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue);  // based on MedeaSpecializeSharedData::gZoneEnergy_harvestValue/MedeaSpecializeSharedData::gZoneEnergy_maxHarvestValue
		//	_wm->_desiredTranslationalValue = _wm->_desiredTranslationalValue*penalizationFactor;    //*(1/(1+exp(-0.17*(_deltaEnergy-5))));
		//	_wm->_desiredRotationalVelocity = _wm->_desiredRotationalVelocity*penalizationFactor;
		//}


	}

}


void MedeaSpecializePerceptronControlArchitecture::searchClosestEnergyPoint(Point2d posRobot, std::vector<Point2d> energyPoints)
{
	std::vector<Point2d>::iterator closestPoint = energyPoints.begin();
	double shortestDistance = getEuclidianDistance (posRobot,*closestPoint);

	for(std::vector<Point2d>::iterator it = energyPoints.begin()+1; it < energyPoints.end(); it++)
	{
		double newDistance = getEuclidianDistance (posRobot,*it);
		if(newDistance < shortestDistance)
		{
			shortestDistance = newDistance;
			closestPoint = it;
		}
	}

	_wm->setDistanceToClosestEnergyPoint(shortestDistance);

	//compute the angle between the robot and the closest energyPoint
	_wm->setAngleToClosestEnergyPoint(atan2(closestPoint->y,closestPoint->x) - atan2(posRobot.y,posRobot.x));

	//place this angle between 0 and 2*M_PI
	if (_wm->getAngleToClosestEnergyPoint() < 0)
	{
		_wm->setAngleToClosestEnergyPoint( _wm->getAngleToClosestEnergyPoint() + 2.0*M_PI );
	}
}

