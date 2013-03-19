#include "Config/ConfigurationLoader.h"
#include <string.h>

//-------------------------
//Include below your loaders in alphabetical order
#include "Config/BasicProjectConfigurationLoader.h"
#include "Config/ChangingWorldConfigurationLoader.h"
#include "Config/ChangeDetectionConfigurationLoader.h"
#include "Config/DynamicConfigurationLoader.h"
#include "Config/MONEEConfigurationLoader.h"
#include "Config/ParcoursConfigurationLoader.h"
#include "Config/SymbrionConfigurationLoader.h"

ConfigurationLoader::ConfigurationLoader() {
	//nothing to do
}

ConfigurationLoader::~ConfigurationLoader() {
	//nothing to do
}

ConfigurationLoader* ConfigurationLoader::make_ConfigurationLoader(std::string configurationLoaderObjectName) {
	if (configurationLoaderObjectName == "BasicProjectConfigurationLoader") {
		return new BasicProjectConfigurationLoader();
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
	}else if(configurationLoaderObjectName == "DynamicConfigurationLoader"){
		return new DynamicConfigurationLoader();
	} else {
		return NULL;
	}
}
