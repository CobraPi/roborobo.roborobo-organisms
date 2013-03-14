#include "Config/ConfigurationLoader.h"
#include <string.h>

//-------------------------
//Include below your loaders in alphabetical order
#include "Config/BasicProjectConfigurationLoader.h"
#include "Config/BattleConfigurationLoader.h"
#include "Config/ChangingWorldConfigurationLoader.h"
#include "Config/ChangeDetectionConfigurationLoader.h"
#include "Config/DynamicConfigurationLoader.h"
#include "Config/HiveMindConfigurationLoader.h"
#include "Config/MedeaConfigurationLoader.h" 
#include "Config/MedeaSpecializeConfigurationLoader.h"
#include "Config/MedeaTwoSunsConfigurationLoader.h"
#include "Config/MONEEConfigurationLoader.h"
#include "Config/MPOConfigurationLoader.h"
#include "Config/NewscastConfigurationLoader.h"
#include "Config/OnePlusOneOnlineConfigurationLoader.h" 
#include "Config/ParcoursConfigurationLoader.h"
#include "Config/ReplayConfigurationLoader.h"
#include "Config/RiverbedConfigurationLoader.h"
#include "Config/ScalePowerConfigurationLoader.h"
#include "Config/ScalePowerSimpleConfigurationLoader.h"
#include "Config/SelfRepConfigurationLoader.h" 
#include "Config/StrongCausalityConfigurationLoader.h" 
#include "Config/SymbrionConfigurationLoader.h"

ConfigurationLoader::ConfigurationLoader() {
	//nothing to do
}

ConfigurationLoader::~ConfigurationLoader() {
	//nothing to do
}

ConfigurationLoader* ConfigurationLoader::make_ConfigurationLoader(std::string configurationLoaderObjectName) {
	if (configurationLoaderObjectName == "SelfRepConfigurationLoader") {
		return new SelfRepConfigurationLoader();
	} else if (configurationLoaderObjectName == "StrongCausalityConfigurationLoader") {
		return new StrongCausalityConfigurationLoader();
	} else if (configurationLoaderObjectName == "OnePlusOneOnlineConfigurationLoader") {
		return new OnePlusOneOnlineConfigurationLoader();
	} else if (configurationLoaderObjectName == "MedeaConfigurationLoader") {
		return new MedeaConfigurationLoader();
	} else if (configurationLoaderObjectName == "BattleConfigurationLoader") {
		return new BattleConfigurationLoader();
	} else if (configurationLoaderObjectName == "ReplayConfigurationLoader") {
		return new ReplayConfigurationLoader();
	} else if (configurationLoaderObjectName == "BasicProjectConfigurationLoader") {
		return new BasicProjectConfigurationLoader();
	} else if (configurationLoaderObjectName == "MedeaTwoSunsConfigurationLoader") {
		return new MedeaTwoSunsConfigurationLoader();
	} else if (configurationLoaderObjectName == "MedeaSpecializeConfigurationLoader") {
		return new MedeaSpecializeConfigurationLoader();
	} else if (configurationLoaderObjectName == "ScalePowerConfigurationLoader") {
		return new ScalePowerConfigurationLoader();
	} else if (configurationLoaderObjectName == "ScalePowerSimpleConfigurationLoader") {
		return new ScalePowerSimpleConfigurationLoader();
	} else if (configurationLoaderObjectName == "RiverbedConfigurationLoader") {
		return new RiverbedConfigurationLoader();
	} else if (configurationLoaderObjectName == "MPOConfigurationLoader") {
		return new MPOConfigurationLoader();
	} else if (configurationLoaderObjectName == "HiveMindConfigurationLoader") {
		return new HiveMindConfigurationLoader();
	} else if (configurationLoaderObjectName == "NewscastConfigurationLoader") {
		return new NewscastConfigurationLoader();
	} else if (configurationLoaderObjectName == "ParcoursConfigurationLoader") {
		return new ParcoursConfigurationLoader();
	} else if (configurationLoaderObjectName == "ChangingWorldConfigurationLoader") {
		return new ChangingWorldConfigurationLoader();
	} else if (configurationLoaderObjectName == "ChangeDetectionConfigurationLoader") {
		return new ChangeDetectionConfigurationLoader();
	} else if (configurationLoaderObjectName == "SymbrionConfigurationLoader") {
		return new SymbrionConfigurationLoader();
	}else if(configurationLoaderObjectName == "MONEEConfigurationLoader"){
		return new MONEEConfigurationLoader();
	}else if(configurationLoaderObjectName == "ChangeDetectionLoader"){
		return new ChangeDetectionConfigurationLoader();
	}else if(configurationLoaderObjectName == "DynamicConfigurationLoader"){
		return new DynamicConfigurationLoader();
	} else {
		return NULL;
	}
}
