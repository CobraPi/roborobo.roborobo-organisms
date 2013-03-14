/*
 *  HiveMindControlArchitecture.cpp
 *  roborobo-online
 *
 *  Created by Robert-Jan on 07/10/2010
 *
 */

#include "IPA-HiveMind/include/HiveMindControlArchitecture.h"

HiveMindControlArchitecture::HiveMindControlArchitecture( RobotAgentWorldModel *__wm ) : PopulationControlArchitecture ( __wm )
{
	setPopulation(new HiveMindInstance());
}

HiveMindControlArchitecture::~HiveMindControlArchitecture()
{
	// nothing here
}

ControllerPtr HiveMindControlArchitecture::createRandomGenome(){
	return Util::createRandomGenome(_wm);
}
