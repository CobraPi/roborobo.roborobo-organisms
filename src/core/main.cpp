/*
 *  main.cpp
 *  roborobo
 *
 *  Created by Nicolas on 16/01/09.
 *
 *  See roborbo.cpp for description
 *
 */

#include "RoboroboMain/main.h"

void displayGeneralInformation()
{
	std::cout << std::endl;
	std::cout << " =-= Roborobo! " << std::endl;
	std::cout << " =-= nicolas.bredeche(at)gmail.com" << std::endl;
	std::cout << " =-= Official version tag    : " << gVersion << std::endl;
	std::cout << " =-= Current build name      : " << gCurrentBuildInfo << std::endl;
	std::cout << " =-= Compilation version tag : " << gCompileDate << " - " << gCompileTime << std::endl;
	std::cout << std::endl;
}



/** default demo mode with visualization. If at least one argument, launch demo in batch mode (ie. no display, fast pace). */
int main( int argc, char* args[] )
{
	if ( argc > 1 ) // command line parameters?
	{
		// extract parameter
		std::string s = args[1];

		if ( args[1][0] != '-' )
		{
			std::cerr << "unknown parameter." << std::endl;
			exit(-1);
		}

		if ( args[1][1] == 'h')
		{
			std::cout << std::endl << "Roborobo! Command line help:" << std::endl << std::endl;
			std::cout << "  usage  : " << args[0] << " [-h] [-v] [-l <propertiesfilename>]" << std::endl << std::endl;
			std::cout << "  options:" << std::endl;
			std::cout << "    h: print this help message." << std::endl;
			std::cout << "    v: print current version number." << std::endl;
			std::cout << "    l: specify location and name of the properties file." << std::endl;
			std::cout << "         next parameter should be the filename (e.g. \""<< args[0] <<" -l config/MyConfigFile.properties\")" << std::endl;
			std::cout << "         default properties file is \"" << gPropertiesFilename << "\"" << std::endl;
			std::cout << std::endl;
			exit(-1);
		}

		if ( args[1][1] == 'v')
		{
			std::cout << std::endl;
			std::cout << " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
			std::cout << " =-=-=-=-=-=-= Roborobo! =-=-=-=-=-=-=" << std::endl;
			std::cout << " =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << std::endl;
			std::cout << std::endl;
			std::cout << "  description     : A lightweight robot/agent swarm simulator written in C++/SDL." << std::endl;
			std::cout << "  author          : nicolas.bredeche(at)gmail.com" << std::endl;
			std::cout << "  started         : december 2008" << std::endl;
			std::cout << "  version tag     : " << gVersion << std::endl;
			std::cout << "  current build   : " << gCurrentBuildInfo << std::endl << std::endl;
			std::cout << "  compilation tag : " << gCompileDate << " - " << gCompileTime << std::endl;
			exit(-1);
		}

		if ( args[1][1] == 'l' )
		{
			displayGeneralInformation();
		
			if ( argc < 3 )
			{
				std::cout << "[ERROR] Missing properties filename (command line parameter \"-l\" should be followed by a filename)." << std::endl;
				std::cout << "Check help (\"" << args[0] << " -h\")." << std::endl;
				exit(-1);
			}
			std::string s = args[2];
			//s.erase(0,2);
			gPropertiesFilename = s;
			std::cout << "[info] Properties will be loaded from file \"" << gPropertiesFilename << "\"" << std::endl;
		}
		else
		{
			std::cerr << "Unknown command line parameter. Check help (\"" << args[0] << " -h\")." << std::endl;
			exit(-1);
		}
	}
	else // no command line parameter
	{
		displayGeneralInformation();
	}

	std::cout << std::endl << std::endl;

	int returnValue = 0;
	returnValue = launchRoborobo();
	
    return returnValue;    
}

