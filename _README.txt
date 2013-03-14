=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-= ROBOROBO! =-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=- 2008-2010 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
-=-=-=-= nicolas.bredeche(at)gmail.com -=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

latest version update: <launch roborobo! binary or check roborobo.cpp>
How to know about current version and target build? Launch "roborobo -v" in the console.


==== DESCRIPTION ====

Roborobo! is a fast and simple 2D mobile robot simulator written in C++ based on a khepera/epuck model. It is targeted for fast single and multi-robots simulation (primary motivation is Evolutionary Robotics). 

Some random bits of information:
- Robot position and mouvement are real-valued (ie. precise)
- Collision is performed on a pixel-based information for fast speed (ie. fast but not precise)
- Robot-robot collision can be switched off (faster, less accurate)
- both GUI mode and batch mode are available. Note that batch mode is the fastest mode (gBatchMode=true, gVerbose=false).
- only one external dependencies: SDL library (multi-platform fast 2D library).
- most parameters are to be found in the config subdirectory (e.g. default.properties)
- full environment and robot specifications can set up directly with an image editor (check data directories). E.g. Robot proximity sensor positions are set up in the robot image. 
- the very first version of roborobo! can be found here: http://neuronik.free.fr/nemelith/?p=67 (it is quite different as it was first a game sort-of experience, eventhough i already had roborobo! in mind). An included html help file describes the overall idea behind parameterization through image files (some modifications may have occured since then).
- you can specify a properties file as command line option (see "./roborobo -h" for help)
 

==== CREDITS ====

roborobo!: 
 - main developper: nicolas.bredeche(at)lri.fr
 - contributor(s): Jean-Marc Montanier
included contributions:
 - the Properties class from Evert Haasdijk (VU Amsterdam)
 - box-muller algorithm from Everett F. Carter Jr. (1994) - ref: http://www.taygeta.com/random/gaussian.html
 - other contribs are mentionned in the code (check: core/Misc.h and contrib/*)
 - past contributor(s): Pierre Delarboulas

=====================
=====================
==== QUICK START ====
=====================
=====================

================================
==== INTALLER'S QUICK START ====
================================

**General Comments**
- The only (not-really-)tricky thing is to link with SDL (http://www.libsdl.org/ -- available for all platforms). Otw the code mostly follow a classic C/C++ sub-directory structure.
- SDL include templates may vary from one target to another (MACosX and Linux targets imply different include scheme). Hence, the target OS is set in "include/core/RoboroboMain/common.h". This should be set automatically.

**MacOS X**
- install SDL and extensions (example of location: http://www.libsdl.org/ and http://www.libsdl.org/projects/)
	- SDL main library (e.g.: SDL-1.2.13.dmg for MacosX)
	- SDL Image (e.g.: SDL_image-1.2.7.dmg for MacosX)
	- SDL Mixer  (e.g.: SDL_mixer-1.2.8.dmg for MacosX)
	- SDL ttf  (e.g.: SDL_ttf-2.0.9.dmg for MacosX)
- create symbolic links for config/, data/ and logs/ in the directory where the binary is created. (e.g.: <roborobo_root_directory>/build/Debug for MacosX with Xcode)
- use Xcode project
- compile and go.
NOTE: later version of SDL may cause problems -- if you experience strange problem such as error wrt. sensor numbering, this may be caused by a too recent SDL.

**Linux**
- install SDL and extensions (using apt-get, synaptic, or any package installation manager), example with apt-get:
	sudo apt-get install libsdl1.2	(or libsdl1.2debian-all)
	Ubuntu : sudo apt-get install libsdl1.2debian-alsa
	sudo apt-get install libsdl1.2-dev
	sudo apt-get install libsdl-image1.2
	sudo apt-get install libsdl-image1.2-dev
	sudo apt-get install libsdl-ttf2.0-0
	sudo apt-get install libsdl-ttf2.0-dev
	sudo apt-get install libsdl-mixer1.2
	sudo apt-get install libsdl-mixer1.2-dev
- compile and link: type "make" (a new binary is created at the roborobo root called "roborobo.exe")
- launch: "./roborobo"
- note: if it fails to compile, check the Makefile file for up-to-date OBJS list.

**Wilndows**
- install SDL and extensions (DLL may also be put directly in the same directory that of the binary file)
- use MinGW or Cygwin to compile using linux GNU compiler/linker.
	- not tested, but probably work: type "make".
- launch: double-click in explorer, or launch with command line.


=================================
==== DEVELOPER'S QUICK START ====
=================================

**Quick Overview**

The philosophy is that in the standard case, the designer should be able to code the agent behavior (the ''Behavior Control Architecture'') and to access both the world and agents states in a seperate fashion (''World'' and ''Agent'' observers). The world and agents observers are run before the actual agent behavioral update, meaning that the observers do observe a ''stable'' snapshot of the environment. Then, agent behavioral update are performed in turn-based fashion, to deal with possible collision.


The only useful things to know in order to write your own evolutionary algorithms are:

Concepts and hints:
- WorldObserver: object dealing with world level, used for accessing state of the world, including all agents (methods: reset and step).
- AgentObserver: object dealing with agent level, used for accessing state of agent. All agent observers are called before behavior update (methods: reset and step)
	=> note that both observers object are really useful for monitoring, logging, computing fitness updates, managing off-line evolutionary optimization, etc.
- BehaviorControlArchitecture: object dealing with behavior at the robot level (useful methods: init and reset)
- RobotAgentWorldInterface:    implements the influence of robot dynamics (hardware and context-dependant), 
	- ie. map control architecture desired command to actual robot's internal update values (e.g. sensor/motor noise/defect, terrain slippery/ruggedness, etc.)
	- hints: this is where you should write your sensor/motor noise model
	- access robot WorldModel, static object.
	- by default: simply return unmodified control architecture output values (ie. no noise, standard environment)
- If you write new classes derived from the above:
	- references: check out roborobo.h to update the object references (ie. dont need to modify anything else).
	- includes  : check additional includes in RobotAgent.h (and possibly elsewhere)
- *WorldModel: object where the (robot) world model is stored. You may want to derive and extend it.
- From the *ControlArchitecture objects, you can access the world and robot(s) world-models. All the rest is simulator-specific implementation.
- of course, you may want to create a new properties file in the config sub-directory
- Lastly, dont forget to edit the "include/core/Config/config.h" file to ''parameterize'' your internal active class structure. You may also precise which properties file to load with command line option (see command line help with "-h" option).

About the simulation update method scheme:
	- AgentObserver is called N times per iterations (N = nb of agents)
	- WorldObserver is called once per iterations
	- Update method: turn-based, synchroneous and shuffle-ordered update method.
		- update method call sequence: WorldObserver => N_a * agentObservers => (N_e * energyPoints update) => N_a * agent steps => N_a agent moves
			- worldObserver is called *before* agentObservers, agentObservers are all called *before* agent.stepBehavior
			- environment physics and actual moving of agents are performed in a row *after* all agent.stepBehavior has been called
			- Agent position movements is performed using synchroneous position updates. Hence, solving collisions is robust to agent index ordering
		- the update ordering is shuffled for each new world iterations, ie. agent indexes are shuffled (to avoid update ordering nasty effects accross time)
			- a typical problem if shuffling is not done is that agents with low indexes always act and move first, hence a big survival advantage.
			- however, agentObservers and agent.stepBehavior are called in the same order (ie. no re-shuffling)
	- WorldModel: contains all information on on agent, ie. its representation of the outside world, including its own status (e.g. energy level).
		- in practical, neither controlarchitecture or observers should contain general agent-related information. All should be stored in the worldmodel.

The idea behing the ''Observers'':
	- WorldObserver 
		- WorldObserver is called once per world update, before anything else;
		- WorldObserver is typically used to prepare data for the world iterations
			- e.g.: updating energy points, global data, global logging, setting up current experimental setup, etc.
	- AgentObserver
		- AgentObserver is called once per agents, before updating controller and position
		- AgentObserver is typically used to manage various agent-related sub-routines such as radio broadcast, learning/adaptation, but also data logging
	- WorldObserver vs. AgentObserver:
		- The conceptual idea behing the two types of observers is:
			1 -	if:
					it is about the whole population and/or environment [e.g. switch between experimental setups or any ''god'' level actions]
					it is concerned with experimental constraints [e.g. set active/inactive agent depending on energy level]
				then:
					do it in WorldObserver
			2 - if:
					if it is about the agent
				then:
					do it in AgentObserver
			3 - HOWEVER, there may be ambiguous situations, especially if an agent-centered routines would highly benefits from factorization within the WorldObserver.
				- A typical example is broadcasting: N * broadcasting for each agent is N^2 ( - N), while only N*(N+1)/2 (- N) if it is centralized (as symmetry is naturaly handled)
				=> Now then, let's take into account two considerations:
					(a) the code written in roborobo is not intended to be directly transferred into another system (e.g. to a robot)
					(b) if the benefit of optimization is *truly* significant (e.g. order of magnitude), it should be of highest priority
				=> Now, the guideline:
					- if (b) is true and significant, then code should be written in WorldObserver (and a comment added to AgentObserver)
	- AgentObserver vs. ControlArchitecture
		- ControlArchitecture deals with acting in the environment (setting up motor control, updating the neural nets, etc.)
		- AgentObserver deals with the internal agent state (updating learning, performing optimization, etc.)
	

**Quick start**

In a nutshell: Let's say you want to quickly start a project, what do you need? Here is a short step by step tutorial.

0. setup your project space
  a. create a new folder with your project name in the ''prj/'' folder. Create two sub-directories named include and src. 
	=> A good way to start is to simply clone and rename the "BasicProject" folder or any other project folder close enough to what you intend to do.
  b. create your ConfigurationLoader class in the ext/ roborobo directories (include/ext and src/ext)
    => Follow the BasicProject example
  c. add reference to your ConfigurationLoader class in the general ConfigurationLoader class in the src/core/ roborobo directory.
    => Follow the BasicProject example
  d. copy/clone one of the properties file in the config folder
    => Follow the BasicProject example, it is used by default-mono.properties (single agent) and default-mas.properties (100 agents).
  e. at this point, you just have to compile and launch ("./roborobo -l config/myconfigfile.properties")
  
1. which files should I create/write?  The simplest way to start is to write you own controller, everything else can be left with default classes. 
  a. Create your own behavior class -- simply clone the most basic one to start with
  b. Create a new ConfigurationLoader. The purpose of this class is to declare the classes to be used.  You can start by simply clone the DefaultConfigurationLoader.
  c. src/ext/YourConfigurationLoader.cpp
  	If you followed the previous steps, you should just have a new behavior class. Hence, you only have to update make_BehaviorControlArchitecture(RobotAgentWorldModel* wm) function
		However, let's take a closer at the items in the DefaultConfigurationLoader (src/ext/DefaultConfigurationLoader.cpp):
			make_WorldObserver(World* wm)	: [default is ok] access all of the world state, called inbetween two world updates.
			make_RobotAgentWorldModel() : [default is ok] - Sensori-motor (and more) agent interface with the world (e.g. getSensor and setMotor methods, etc.)
			make_AgentObserver(RobotAgentWorldModel* wm): [default is ok] - access an agent state, all agent observers are called once *before* behavior update.
			make_BehaviorControlArchitecture(RobotAgentWorldModel *wm) : [default is wall-avoidance] - "[!!] ENTRY POINT [!!]" - this is where you write your actual behavior code
			make_RobotAgentWorldInterface() : [default is ok] - implementation of robot dynamics (max. acceleration, etc.)
  d. use YourConfigurationLoader
		src/core/ConfigurationFile : modify it in order to be able to use your new configurationLoader. 
			include your configurationLoader header file
			add a condition to be able to load YourConfigurationLoader
		src/core/roborobo.cpp : take a look at which properties file is loaded (gPropertiesFileName)
		properies file : modify ConfigurationLoaderObjectName value to YourConfigurationLoader 

2. how can I modify simulation parameters? chech the file you created in step 0 in the "config" sub-directory. Check roborobo command line option to launch roborobo with the correct properties file ("roborobo -l config/myConfigFile.propertie").

3. how can I compile/launch roborobo? compile with "Xcode" (mac) or use "make clean ; make" and type "./roborobo" (linux)

4. Where do i go from here? Check and learn from the other projects in the project directory.


** Methodology issues **

- Any new project should start, *and be limited to*, a specific sub-directory in prj/, with both an include/ and src/ sub-directories. E.g. ./prj/MyProject/[include&src].
- Observers should be understood as "deals with everything that happens inbetween two lifetimes" while Behavior should be understood as "everything that happens during lifetime", disregarding if it is agent or expert viewpoint, ie. behavior class may also include some external environmental constraints (such as specific locomotion mode in some given zones that are not dependent of the robot motor decision). In this sense, the WorldModel classes are also more general than what is usually intended as it is mainly a container for all robot states, including those the actual robot controller may not access.
- In case you implement new functionalities, the ext sub-directory should be the place to write them. For user-specific code, i would suggest to add a new directory with you name (e.g. nicolas/, evert/, ...) to keep core/ext/contrib as simple as possible (ie. evolutionary robotics may not be the only future application of roborobo!) - Thus, avoid modification into the core of roborobo, so that it can be straightforwadly re-use in other contexts.
- When developping a new "fork" of roborobo, all configuration parameters should limited to the .properties file and the include/core/Config/config.h file. These two files along with your own instance if the class described just above should be enough to create new setups. It should be easy to switch from one experiment to another just by commenting blocks in the include/core/Config/config.h file (see config.h file for example).
- A last word: roborobo! is not exactly the paragon of Clean Coding philosophy as it was originaly, and still is, a single-coder-in-a-very-big-hurry short term project. While i tried to keep the code clean, there are several do-as-i-say-not-as-i-do features (lack of accessing methods, etc.). Hence, roborobo! may not be the best starting point for developping your homebrew robot simulator. However, it can still be pretty useful if you just intend to extend it for a particular application (e.g. a specific swarm mobile robotics environment and simulation).

- Minor comments: 
  - indentation assumes only tab character spacing, display is assumed to be 4 empty characters equivalent.


** Contents of directories **

- prj    : project locations, each project is indenpendant, and may be seen as a special instance of roborobo - parameterized through ConfigurationLoader and .properties file.
- core   : core roborobo! code. Contains the elementary elements for running roborobo.
- ext    : derived class and extensions for roborobo! code, which may be of global interest. It contains addition to roborobo core, including all code that can be factorised from projects (ie. re-usable code, such as e.g. neural nets, logger), as well as project specific code.
- contrib: imported code with useful features.
- log    : log files.
- data   : contains image files.
- config : contains properties files.


** Images description **

gAgentMaskFilename				: the agent as how it is displayed and used for collision.
gAgentSpecsFilename				: the agent specification regarding proximity sensor (location and orientation).
gForegroundImageFilename 		: the environment decorative image, ie. only used for displaying -- should more or less match environment image.
gEnvironmentImageFilename  		: the environment description image. 
									R component is unused
									G component is used to determine agent #G starting position (if more than one pixel with same G value, choose the most upper-left), 
									B component is used to determine object type.
									obstacle (and agent) Id ("type") are returned by proximity sensors. 0..255: #obstacle, 1000..1256..+++: #agent
									R=255 means "no obstacle", G=255 means nothing, ie.: pure white means "no obstacle"
									(!) use png to avoid pixel exact color loss
									note that the sensor beam lenght parameter is set in the config file.
gBackgroundImageFilename  		: this image is purely decorative. Change parralax factor for faster scrolling (if screen size is smaller that environment/foreground size).
gZoneImageFilename        		: this image specifies the "zone". It has two purposes:
									(a) The red component for each pixel determines the zone Id, which is sensed through the agent floor sensor
									(b) if caption display mode is on, a caption image may be displayed if exists (see below gZoneCaption*) -- value 00 is ignored.
gZoneCaptionPrefixFilename 		: image name prefix for caption (optional) - in display caption mode, corresponding image is displayed on screen whenever a robot is in a given zone.

** default images **

Some image files are available by default in the data subdirectory. Here's a quick summary (but you may add anything you like):
- agent*: a 32x32 robot image, with 8 sensors (khepera-like)
- miniagent*: a 5x5 robot image, with 8 sensors (sensor belt)

** EnergyPoint usage **
Description of a standard energy point is given in src/core/EnergyPoint.cpp. It is used in SwarmOnlineAgentObserver.cpp and EnergyWorldObserver.cpp.

Basically an energyPoint can be of two type : fixed or not. A fixed energy point will always re-appear at the same place on the map, while an unfixed energy point will always re-appear at a new random place on the map.
The differenciation is done at the creation of the energy point. If an id is given to the energy point, this one will be fixed. This id can be used to set the position of the fixed point. If one want to give the position of a fixed energy point, this position has to be given in the properties file :
energy[id].x = XX;
energy[id].y = XX;
If the position of a fixed point isn't described in the properties file, the initial position of this point will be choosen randomly. Still the energy point will always re-appear at its initial position.

Energy points could be displayed on the screen. In order to do this define the VisibleEnergyPoint property (in the propoerties file) to true. Beware, displaying energy points will slow down the simulator.

EnergyPoints can be inactivated (ie: no nore reachable by a robot). The function setInactivate() has just to be called. Inactivated EnergyPoints can be reactivated by calling the function setActivate(). If a respawn lag has been defined (time between the descativation and reactivation of an EnergyPoint), one can just call the step() function at each time step (this call will idealy done in a WorldObserver. This step function will take care to reactivate the EnergyPoint once the respawn lag is passed.

** Some conventions **

Other (important) conventions
- if agent_i proximity sensor meets agent_j, the object ID return value will be agent_j.id+1000. Values <1000 are used for object Id. (see also gEnvironmentImageFilename, above)


**Dev notes**
//Some notes on internal implementations - (i try to keep this list up-to-date, but you should double-check by browing though the code if anything seems "weird")//

- motor control: control architecture should set desired translation absolute speed (e.g. 2 pixels/steps) + desired rotational velocity (e.g. -13deg/steps)
- motor control: check values un Parameter file for maximum speed and maximum displacement/steps values (e.g. maximum update per steps for translation speed)
- sensor/motor noise model: should be implemented in RobotAgentWorldInterface*.cpp/h


==================
==== MISC ====
==================

** History **

- 20100527: 
		refactor, clean up.
		post-PPSN submission, online evolution.
- rc2fork2:
		started 20090319 
		online-evolution
- rc2fork1: 
		20090200
		evolutionary robotics experiment (1+1-es setup, deceiving landscape, cf. CEC2009Tolman)
		CORRECTED: range sensor return distance problem (castRaySensor)
- rc2:
		20090123.1848
		Heavy refactoring. basi entry points for starting developping evolution.
- rc1: 
		20090113.1533
		first stable release of the simulator. batch and ui mode.



** Benchmarking **

// display mode 2, with/without collision//
// test 1 : default.conf: one big robot//
//    test a : collision-free//
//    test b : collision//
// test 2 : default-mas.conf: 106 small robots//
//    test a : collision-free, no communication network//
//    test b : collision, no communication network//
//    test c : collision, with communication network//

[20091119] PC Labo (dualcore2 3ghz, 4go, Nvidia GeForce 280 GTX) 
  - default.conf:     19230it/s   19230it/s    -
  - default-mas.conf: 1250it/s    1200it/s     1020it/s

[20091120] Mac nicolas (dualcore2 2.9ghz, 4go, Nvidia 9400M or 9600M GT (no diff.))
  - default.conf:     13900it/s   13900it/s    -
  - default-mas.conf: 930it/s     915it/s      515it/s

Eléments de comparaison:
 robot unique:
   ksk: >>72h jours observe -- dit 48h dans l'article. Donc env. ~1234fps. (??? sur site, dit 1000000 steps en 1.5 sec ??? -- probab. faux)
   simbad pur: 15000 fps sur PC labo 2007. (note: chiffre donné sur le site différent)
 multi-robots:
   Mason: env.1000it/s avec 106 agents (pc labo 2009).


=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 


**List of future implementations**


Priority 1 (do it now!):
- (none)

Priority 2 (should do it, someday):
- sensor (range and orientation) and motor noise. (rate should be set by user in properties file)
- makefile ability to compile only part of ext/prj files (projects independant wrt. compilation) [current: all projects are registered in ConfigurationLoader]

Priority 3 (maybe?)
-?convert into mm or cm. (well, it's a matter of conversion, e.g. 1 pix = 1 cm? depends on the ration envt size vs. robot size)
-?implement millisecond precision for creating the random seed (critical or not?) -- random seed is based on second precision, ie. two runs starting at the same second with identical properties/parameters are identical (may be relevant when using clusters, but also, maybe unlikely to happen).
- check fullscreen.
- add controller library (MLP, ESN, ...)

=-=-=

Archives of modifications
- 2010.09.15:
	-.add milliseconds in automatic file numbering.
	-.max translational speed should be limited wrt. agent size (add a safe-check) [done - display warning]
	-.user-controlled robot provokes segmentation fault if collision [done]
	-.proximity sensor should be displayed optionaly in render mode (hot-key for visualization?) [done]
	-.inspector mode for user-control: agent can be very difficult to spot. show flashing circle. [done]

=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 


**BUG TRACKER**

- 2010.03.??: solve the verbosing on the creation of robots in the selfRep mode
- 2010.04.??: placement of energy points away from any obstacles
-.2010.06.23: collision bug when user-control agent bump into other agent.
-.2010.06.17: sub-pixel translation value is ignored [solved]


=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 


=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

-----------------------------------
-----------------------------------
===================================
====== DEVELOPPER'S NOTEBOOK ======
===================================
-----------------------------------
-----------------------------------

//>> This is nicolas personal developper's notes
//>> ''personal notes'' as in ''not supposed to be readable'' (e.g. mix of french/english)
//>> PLEASE DO NOT MODIFY ANYTHING IN THIS SECTION


=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 
= ** TRACKER : CURRENT CHANGES ** =
=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 
=  "="  means open entry          =
=  "-." means closed entry        =
=  "-~" means controversial entry =
=  "-?" means optional entry      =
=  "-#" means comment             =
=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 

[20100922]
-.[core] EnergyPoint: add local management method for energy value and respawn lag:
		bool getEnergyPointValueIsLocal();
		void setEnergyPointValueIsLocal( bool __value );
		int getEnergyPointValue();
		void setEnergyPointValue( int __value );
		void setRespawnLagMaxValue( int __lagMaxValue );
		int getRespawnLagMaxValue();		
		void setRespawnLagMethodIsLocal( bool __value );
		bool getRespawnLagMethodIsLocal();
	By default: all flags are false (to ensure same behavior as before)
	also modified config files

[20100915]
-#roborobo is moved to evorob/dev
-.added new project 'medea-specialize' (clone of twosuns) -- objective: specialization/speciation in envt with more than one resource.

[20100914]
-.robot light for user monitoring (e.g. different species)
  -.3x3 square, setLightStatus(on/off), setLightColor(color)
  -.comments: by default is false - while it may be used for communication, the LED was initialy used to 'tag' the robot for user-purpose.
-.inspector mode for user-control, agent can be very difficult to spot. 
  -.solution: Show flashing square around user-controller robot
-.possible conflict in naming files when launching many run in parallel: resolution at the second timescale is not enough (same names occur)
  -.solution: added ms resolution in file numbering (note that random seed is still second-based)
  -.note: added include <sys/time.h> for using gettimeofday under linux (no need in macosx)
-.max translational speed should be limited wrt. agent size (add a safe-check)
  -.display a warning on console if gMaxTranslationalSpeed is larger than agent image width and/or height. (ie. considered not critical)
-.user-controlled robot provokes segmentation fault if collision
  -.cause: desired translation value was not re-initialized if collision (ie. desired value always far beyond reach, provokes infinite recursive call)
  -.note: problem occurs if max speed >= 2.0 is allowed (e.g. medea-twosuns autonomous has no problem while user-controlled has)
  -.implem: added '_wm->_desiredTranslationalValue = 0;' in RobotAgent::move(...)

[20100627]
-.added saveScreenshot(...) global method : take a screenshot and save BMP in log directory. Automatic numbering.
  -.automatic numbering
  -.optional parameter for user comment (saveScreenshot("blabla"))
  -.added hotkey related ("o")

[20100625]
-.medea-twosuns: monitoring distance et orientation. Nord en haut.

[20100622]
-.added hotkey ("j") and properties variable name ("gDisplaySensors") for sensor raycasting display on/off

[20100618]
- medea: l'image background et foreground ne correspondent pas
- check PPSN-ni-submitted.properties (deprecated)
-.RobotAgent: minor bug detected (by Pierre) [corrected]
  -.in applyDynamics(): abs is used instead of fabs, implying truncating subpixel translation values
  -.correction reveals two bugs (infinite recursive calls of move())
    -.in move(...): if ( isCollision() ) block content is not consistent and was shadowed by prior truncation. This was not a problem before, but becomes a major bug [corrected]
    -.in move(...): if statement for recursive call deals only with positive translation. Minor bug as not recursive solving of collision was replaced by abrupt solving (ie. if collision, cancel move). [corrected]

[20100603]
-?une option restartActif pour medea (qui lors d'un restart met un genome au hasard et passe en actif)
- EnergyPoints: rendre indépendant de World
-.creation de MedeaSharedData
-.creation du projet Medea-TwoSuns
- tutorial PRJ: 
  - changer le configurationLoader dans le properties
- affichage quel configurationLoader lors du lancement.
- bug lors du remote control en collision (dans medea*)
-?actuellement, l'appel hide() des energypoints doit être fait par l'utilisateur. Ce n'est pas forcément clair. Clarifier? (mais comment?) (utile lorsqu'on change la position)
  - dans la fonction setPosition, forcer un hide+show sous condition? ==> bonne idée. todo.
- difference entre background x-ray et affichage (medea*)
- l'agent remote control est difficile a voir
- option pour supprimmer l'affichage des IR
- les energy points ne sont pas centrée, les coordonnées donne le haut-gauche -- c'est grave? en general non, mais genant a l'utilisation.
- corriger la generation des positions aléatoires des agents pour un retirage si obstacle.
- verifier/corriger specification explicite des coodonnées des points d'energie.

[20100525->27]
-.ajout d'une cible ''snapshot'' dans le Makefile qui permet de generer une archive tgz rapidement, sans les .svn et les fichiers datalog_* et properties_*
-.mise a jour du code, historique des modifications:
  -.Refactoring Medea:
    -.renommage PPSN* -> Medea*
    -.deplacement dans PRJ, regroupement avec battles et replay
    -.deplacement des variables d'état de l'agent depuis Controller vers WorldModel (+accessing)
    -.PPSNWorldObserver:
      -.Ajout PPSNWorldObserver
      -.refactoring repartition du code entre les observers (lourd)
      -.deplacement des données dans WorldModel
	-~externaliser MLP / controleur => cas par cas? a voir...
  - modifications diverses
    -.suppression de EnergyWorldObserver (code inutile)
    -.multiple définition de euclidianDistance(...) => une seule methode, dans Misc.
    -.déplacement de Point2d depuis common.h (qui sert aux includes communs a roborobo) vers Misc.h (utilitaires divers)
    -.déplacement de modulus (et renommage en applyModulo) dans Misc.h
	-.gRespawnLag => gEnergyPointRespawnLag (dans le code, et dans les fichiers .properties)
    -.gStory* renommé en gZone* avec quelques variations. fichiers properties mis a jour.
    -.WorldModel et classes ''friend'': mécanismes supprimés et remplacés soit par des get/set, soit par un passage en public (pour RobotAgentWorldModel)
    	- a faire éventuellement: passage get/set pour RobotAgentWorldModel (prévoir un loooong week-end)
    -.mise a jour du _readme.txt
    -.renommage OnePlusOne...Observer et StrongCausality...Observer
    -.LockedEnergyPoint devient EnergyPoint. Refactoring léger (renommage surtout).
    	-#Libre a l'utilisateur d'utiliser ou non la clé (l'interprétation n'est pas faite dans EnergyPoint).
    	- pas setLocation pour l'instant
    -.gEnergyPointSize: renommer en gEnergyPointRadius (+ *.properties mise a jour)
    -.make_EnergyPoints...?() -- a ecrire? => non, on considere que c'est une particularité du code, donc a gerer entierement dans un worldobserver
	-.repertoire logs, fichier de dump du properties file courant créé au lancement: entete explicite dans le fichier, nommage en fonction de l'heure de lancement
		-#exemple de nommage: ''properties_20100526-15h10m11s.txt''
	-.repertoire logs, idem fichier log.txt (lorsque gLogFilename n'est pas donné dans le fichier .properties)
		-#si le parametre gLogFilename n'existe pas, alors le nom de fichier est créé selon la même approche. Ex.: ''datalog_20100526-15h10m11s.txt''
		-#macosx: signifie que certains scripts sont a modifier (ie. fait un renommage de log.txt en fonction de l'exp courante)
	-.legere retouche sur le format des infos affichées (entre autre correction d'un bug de synchro sur l'affichage du compile time lors de la demande de version)
	-.repertoire tools:
	  -.deplacement des scripts de la racine dans python-scripts/BattlesAndReplay
	  -.mise a jour de la structure du repertoire
    -.ancien PPSNAgentObserver::step->broadcast: usage de current[Observer|genome|sigma] pour des sens differents => resolu, y compris dans Battles et Replay.
    -.Makefile unix mis a jour.
  - repertoire PRJ
    -.deplacement de fichiers venant de ext/ vers prj/
    -.mise a jour du projet Xcode
    - mise a jour du Makefile "TODO"
    -.mise a jour des chemins dans les sources
    -.regroupement:
      -.medea: Battle, Replay et mEDEA: un seul projet? non factorisable (donc pas dans ext/)
      -.nommage: suivant l'algorithme et la version (pas de nom de conf, sauf a posteriori eventuellement - redondant avec archivage de toutes facons) 
    -.tutorial nouveau projet
      -.mettre a jour quick start
    - probleme séparation des projets
      -#info: ConfigurationLoader doit quand même être initialisé... mmh. probleme.
      -#configurationLoader doit être toujours accessible, configurationLoader contient une ref vers tous les projets
      -#il peut y avoir plus d'un configurationLoader par projet, donc un configurationLoader global est nécessaire
        -#option: forcer un projet = un ConfigurationLoader ?
      -#en l'état:
        -#le point d'include doit être à partir de PRJ/ (e.g. ''nomdeprojet/include/blabla.h'')
        -#tout doit compiler (pas d'indépendance de développement)
    -.ajout de BasicProject dans le repertoire prj/ (braitenberg evitement d'obstacle, le point d'entree du tutorial)

[20100402.1459]
- PPSN
  - I/O
    -.closest point. cf. ctlarc.searchClosestEnergyPoint
    -.detecteur de zone -floorsensor
    - rdn
  - zone recharge
    - implem monde
    - implem fct
  - scenarisation
    - extraire 3 fonctions dans le step observer
  - MLP
  -.virer key
  -.refill implique pas de controleur
    -.ajout d'un flag dans l'agent (controleur? WM?)
    -.pas de broadcast du genome
    -.actionneur a zero dans ctl.step
    -.energy inchangée (ctl.step) tant que pas actif == ah? non.
  -.modif: deltaEnergy = 0 apres un revive (sinon peut transmettre n'importe quoi).
  -.bug: broadcast meme si pas d'energie (jusqu'a la prochaine synchro) [corrected]
  - refactor: PPSNAgentObserver::step -> broadcast: usage de current[Observer|genome|sigma] pour des sens differents -> a renommer.
  -.choix: lors d'une interv. humain, on redonne de l'energie et on choisis un genome dans la liste des importes si possible, sinon, reste immobile en attente d'insemination.
    -.combien d'energie? (moins qu'une vie) -> avant: 2*gEvaluationTime*0.1 - maintenant: gEvaluationTime*0.1 / 2 (moitie d'une vie)
    - eventuellement: seuil max sur Energy. (une vie?)
  - plusieurs changements dans les valeurs de parametre (energy--, energypoint.internlag = respawnlag, etc.) pour comprendre ce qui se passe.
- gStoryZonesImage to rename
-.distance: changer pour que distance relle dans le fichier properties, et distance^2 dans le code (conversion). [done]
-.PPSNPerceptronControlArchitecture:
  -.random walk se base du wm->_age, qui n'est jamais incremente...
  -.solution: commente cette partie (pas forcement utile de toutes facons.
- integration sesam dans branche principale roborobo (vient d'un fork de jm)
  - EnergyPoint vs. LockedEnergyPoint
  - bug: gRespawningEnergyPoints not declare in header file - never used.
	- todo: relevance to evaluate
  -.roborobo.h/cpp:
	-.orthographe gMoveStepWidth (inversion deux dernieres lettres)
		=> propagation partout.
	-.note: gMaxIt doit etre explicitement defini dans le fichier config, alors qu'il existe une valeur par défaut dans le code...?
		=> ajout commentaire, valeur = 0, conseil -1.
- refactor: dans PPSN.AgentObserver: variables globales
-.refactor: nommage de EnergyPoint
  -.initLock, iterationMax (???), size (?!!), etc.
  -.faire la part entre ce qui peut etre global et ce qui correspond a un point particulier.
  -.le mecanisme de cle... dans energypoint? ou dans un derive (qui devrait etre dans ext)
- refactor: dans roborobo.h, nommage de qqs var globales (gEnergyPoint vs. gEnergyrespawning..etc.)
- refactor: raisonner nommmage PPSN/SESAM/etc.
- implementation energyPoints
 -.actuellement: placement random
 -.prevision: 
    - cas 1 : placement explicite + random
      - la présence d'obstacles ne permet pas de prévoir un placement garanti correct.
      => non.
    - cas 2 : placement explicite
      - maitrise, mais potentiellement fastidieux.
      => ok.
 	- cas 3 : placement random avec prevision obstacle
      - pas de maitrise, cas impossible non gere.
      => non.
    - cas 4 : placement explicite ou random, avec optim random
      - hyp: 
        - le centre d'un point d'energie doit etre place sur une partie definie vide de l'envt. (cf. background)
        - 2 pts peuvent partager le meme centre.
        - a charge de l'utilisateur de bien definir les parties vides dans l'image background.
      - implementations possibles
        - a la volee #1
          - choisi un point au hasard, test si possible, recommence sinon.
          - probleme: la densite de l'environnement impact le nombre de retirage.
          - param nb_tirage: mettre une borne sur max d'essai -- ca peut etre regle automatiquement.
            - calcul densite
            - Q: quelle est la valeur de la proba de succes de tirage aleatoire d'une position tel qu'il est plus interessant de passe en methode "a la volee #2" ? 
              - si P < 1 / ( dx*dy - #position_possible )
              - ie.: sur un grand nombre de tirages, on regarde le nombre de lecture de l'image.
        - a la volee #2
          - calcul le nombre de points possibles, fait un random a chaque fois, puis recalcul tant que
          - probleme: couteux a chaque essai.
        - stockage memoire (mais 2*dx*dy -> 1go pour du 1000x1000!)
        - optimisation PCX n&b
        - approximation: stockage memoire carre de N*N (1000x1000 -> 100x100 -> 10mo au lieu de 1go)
          - problematique pour des labyrinthes denses + sensible a l'alignement du pavage
        ==> solution retenue: combiner a la volee #1 et #2 en fonction de nb_tirage.
 - plan d'implementaiton:
   - suppression gMaxEnergyPoints
   - chargement p/r properties
   - raisonner EneryPoints vs. LockedEnergyPoints (factoring nommage)
   - deplacer dans ext
   - appel dans XXXWorldObserver
     - strategie: derivation de WorldObserver liee a projet --> enlever de World.cpp
     - init dans XXXWorldObserver.reset()
     - eventuellement, un heritage intermediaire EnergyWorldObserver (facilite adaptation du code existant)
       - note: ceci justifie une distinction ext/ (ext ordre general, toolbox) et prj/ (prj-oriente)
     - y compris gerer affichage (.step())
   - parametre: x, y, radius (impact calcul distance), actif/inactif (pour affichage)
   - gestion agent
    - actuellement: calcul si agent prend energy dans (SelfRep)PerceptrionControlArchitecture.step()
      - distance euclidienne brutale dans le step
      - todo: externaliser.
        - ne doit pas etre fait dans le ctlarc, 
        	- opt.1: agent.step (mais generalement non derive)
        	- opt.2: agentObserver (todo: l'info point d'energie trouvee doit etre connu de l'agent)
				- agentObserver accede au WM de l'agent
				  -> dans WM, ajouter un attribut pour cette info? meme si la plupart du temps pas utilisee.
- TODO: rajouter dans extendedproperties la possibilite de definir un texte libre qui sera affichee
-.EnergyPoint step [corrected]
  -.void EnergyWorldObserver::step(): appel energypoint.step() que si actif
    -.probleme 1: le test d'actif ou non doit etre fait dans le enerypoint.step, puisque celui ci gere l'affichage.
    -.probleme 2: la gestion des energypoints doit etre faite dans le world, pas dans un observer.
- EnergyWorldObserver ne sert a rien. a effacer. (et remplacer les appels par WorldObserver)
- gEnergyPointValue dans roborobo.cpp semble etre obligatoire... pourquoi?
-.probleme de coherence d'affichage des energypoints lors d'un harvest + overlap.
  -.diag: l'overlapped apparait partiellement mange (pas d'impact sur la precision)
  -.solution: re-affichage systematique
    -.probleme: cout du reaffichage
    =>.reaffichage que si displayMode=0 ET gEnergyPoints_alwaysRender existe (et valeur ''true'')


[20091124.0002]
- basic setup: environment, robot, random behavior
-.debug danger code (and 1+1, strongcausality code) => problem diagnosis:
  - personal note: C++ class hierarchy does not imply that most derived class shadow hierarchy! If cast at level N, method at level N will be called (!= java).
- RobotAgentWorldModel: sensor count with fixed size to translate with std::vector -- currently: 8 sensors max.
- python interface (later) => cf. Boost.python (http://www.boost.org/doc/libs/1_41_0/libs/python/doc/index.html)
-.refactoring
 -.analyse relevance of worldAgent vs. robotAgent / revise genetics. => simply rename genetics? GeneticsControlArchitecture => Observers
-.upgrading options
 -.write config#0 (default obstacle avoidance)
 -.write basic start-up reference object => already written (wander behavior)
 -.check manual control agent signalling
 -.inspector should work on solo agent (check with coordinates)
 - radio signalling (array that monitor who's who ranging table) 
   -.implement scanning in World.cpp, line95
     -.use std::vector for 2D radiocom array
     -.implement loading from properties
   - note: sending and receiving cone may differ (receiving cone is larger)
   -.how to access neighbors from agent? => RobotAgent.getNeighbors() returns a list of reachable neighbors.

[before 200911]
-.Locomotion mode
 -.danger zone locomotion
  -.move danger zone locomotion parameters into properties file
  -.derive RobotAgentWorldInterface
  -.implement danger zone
  -.robot counter / neighbor influence
    -.(1) prepare proximity (N! calls) => fastest. but: implementation not easy within current structural choice (trade-off btw clarity and efficiency)
     -.implies: (a) pre-processing of proximity prior to turn (b) RobotAgentWorldInterface based on pre-processed information (c) memory structure for pre-process information
 -.velocity impacted dep. on counter
 -.locomotion specific dynamics - internal proprioceptive-perceived modification vs. envt external modification 
  -.RobotAgentWorldInterface::applyInternalDynamics - external method updates trans/rot wrt. world dynamics model.
   -.assume: (a) desired* *should not* be modified. (b) actual* *may* (dep. if the robot actually "sense" back modification, ie. proprioception), (c) internal absolute coord and orientation *are* updated
  -.Question: impact user-control mode? no. (assume: user control is for positionning, not for robot experience)
  -.rename updateDeltaPosition en applyDynamics
 -.(_wm->_maxTranslationalDeltaValue = gMaxTranslationalSpeed) => favorise bang-bang... à changer, mettre dans properties
  -.en pratique: non, favorise pas bang-bang (note: dans evo1+1, le robot n.0 est le best-so-far)
= collision mode: always on (avec un message d'avertissement pour le cote sous optimale, uniquement pour inspector)
= noise interface (a = __NoiseModel__.applyNoise(value,range=0,type=0))
= symbrion scenario
 - danger zone et influence moteur.
  -.zone? un index particulier dans storyzones (donc: floor sensor utilise)
  -.implementation dans ...? R: le moins invasif: dans la classe derivee de BehaviorControlArchitecture
    - cloner la classe PerceptronBehaviorControlArchitecture (prise en compte danger zone est un cas particulier)
  - prise en compte proximite?
    - chaque robot parcours tous les voisins? (max. N*(N-1) tests si aucun contact) => ... pour 200 robots, 200*199=~40K appels - optimisable? certainement, mais bon.
    - balayage zone? (couteux, incertains, probablement tres eleve selon diametre, prise en compte rayon) => non
 - aspects techniques
  - genome migration: detecter autre robot
  - batterie
  - calcul fitness (??? ben non - il n'y en a pas)
 - controleur
  - formalisme (recurrent ou non, jordan ou esn?)
  - in/out (en particulier: moteur re-entrant? niveau batterie? capteur de zone?)
 - algorithme evoswarm+bootstrap
-.motor control (2) - desired vs. actual values
 -.note: rotation value is an angular velocity ; translation is a desired absolute velocity value (ie. feedback ctl)
 -.check if actual [Rotational|Translational] values are correctly updated
 -.refactoring (semantic-oriented). Details:
    _translationalDesiredValue=>_desiredTranslationalValue
    _rotationalDeltaValue=>_desiredRotationalVelocity
    _desiredRotationalVelocitySensor=>_actualRotationalVelocity
    _translationalActualValue=>_actualTranslationalValue
-.motor control is assumed through delta value, ie. the output of the controller gives a desired delta, not an absolute value.
 -.revise? oui. Car dans l'etat actuel, sans rebouclage, aucun moyen de savoir la vitesse, donc la strategie la plus simple est saturation.
 -.revision: 
   -.asservissement a une vitesse de translation (translationalDeltaValueSensor)
   -.pour l'angle: asservissement delta (ie. vitesse angulaire) -- comme avant, ne change pas!
-.floorsensor: repositionnement au milieu (xReal,yReal sont *deja* au milieu)
-.command line: 
 -.properties file -l <filename> ; h(elp) ; v(ersion) - batch mode is specified in the properties file.
-.inspector mode: display gps coordinate
-.debug
 -.probleme affichage sur les castRay et god mode...? 400/800x400/800 ok mais le reste, non. (400x400, 200x200, et les autres)
 -.SDL_Rect gCamera et gCameraParallax dimensionne en dur ==> resole
 -.gScreenWidth pas pris en compte
-.god mode is too fast in fast render mode => might behave faster/slower depending on the load.
-.Parameterization
 -.convention couleur dans les images
  -.agentXstart devient inspectorAgentXstart
  -.emplacement de depart des robots
  -.probleme du robot 0 vs. pas d'obstacle. (pb: reserve pour "rien") --> pas un probleme puisque register avec Id+1000
  -.numerotation robot+100 -- alors qu'objet jusqu'a 255? (e verifier) --> corrige: +1000.
  -.visibilite sur storyzones...? --> blue component
 -."SDL_Surface *specsImage = load_image( "data/agent-specs.png" );" dans RobotAgent.cpp
 -.#define pour les noms des objets
 -.Properties java-like implementation. (cf. Evert's code)
-.evolution (off-line, 1+1ES) => cf. previous fork.
 -.Perceptron (load param, step)
 -.Eval
 -.fitness: needs actual velocity. (velocity/rotation is updated with real values)
 -.note: not a problem, but bug NN parameters may result in NaN output values... (e.g. if sigma update never gets smaller with 1+1ES)
 	- solution: bound sigma?
 - todo: sigma: minimal bound. (e.g. 10e-12)
 -.fitness is integer... ==> PROBLEM WITH RANGE SENSOR (CORRECTED 20090125.0953 -- VERSION FORK1)
 - agent.updateFitness => friend with World
-.structure pour evolution
	-.controller.step distinct
	-.calcul de fitness (renvoi un objet?)   ==> depend si on/off-line evolution
	-.point d'entree (cf. launchBatchmode)   ==> depend si on/off-line evolution
-.refactoring
	-.OrientedAgent: RobotAgent
	-.NonOrientedAgent: InspectorAgent
	-.fichiers graphique, roborobo, World
-.interface
	-.deplacement sur robot ou global
-.multi-bot 
	-.switch entre agents, initialisation, etc.
	-.analyser la positionCamera dans le cas plusieurs robots...
	-?point de depart
	- collision entre agent
		-?sampling suffisamment eleve pour pas de probleme, ie. longueur mouvement << taille robot
		- collision entre agents
			- externaliser le test de collision (evite de le faire N*(N-1))
				- sol1: detecter collision, backtracker [TROP LOURD]
					(+) rapide si pas de collision
					(-) backtracking peut etre couteux si dependance en cascade
					(--) backtracking recursif sans arret possible (dependance en cercle) (petite probabilite)
				- sol2: pas de simulation (robot) et pas de resolution (monde)
					(+) rapide possible
					(-) pas de solution pour la collision avec 1 pixel de commun => implique blocage
					(-) si collision, alors robot definitivement immobile (mort)
					(=) code: tour de tous, test collision apres coup, desactive les morts en une passe.
					(=) contrainte: pas de simulation doit etre multiple entier de pas de resolution ; resol <= simu
				- sol3: collision implique arret immediat [IRREALISTE]
					(+) rapide dans tous les cas
					(+) (facile a faire - sauve ancienne position, valide apres que tous les agents aient bouges)
					(-) pas de contact, position sauvee depend de la vitesse instantannee avant choc
					(--) moins realiste 
				- (!) solution retenue: 
					- sol2
					- ajout: resolution collision
						etat mort tant que collision superposee
						routine de-collision: 
							apres tous les agents.move
							avant les tests de collision robot-robot
							tente une execution vecteur arriere si libre (nombre max d'essai?)
							enleve *eventuellement* le flag mort
				- TODO: **DEPRECATED 0111.1208**
					-.ajouter senseursRobot (idrobot et distance)
					- senseurs mis a jour avec proximite autre robot
						- quelle methode?
					-.etat mort/vivant
					- detection de collision, etat mort si collision.
					- memorisation vecteur inverse, pas de 1 pixel.
					- verification si nouvel localisation sans conflit (???)
	ETAT ACTUEL [BP20090111.1208]:
		- td: collision entre robot, lance de rayon, pas d'etat mort ou vivant car collision resolue (biais synchrone permet pas superposition).
		- todo 1: implanter la reconnaissance du no_id de l'autre robot (deux images? la 2nde etant un z-buffer?) 
			-.ajouter senseur type objet -> pourquoi quand rien on a 1.67772e+07?
			-.include zones (sur storyzones)
			-.ajouter senseur zone
		- todo 2: optimisation de la vitesse. faire une seconde methode basee sur un seul calcul, (gerer probleme des senseurs)
			-Xoption 1: pour chaque agent, parcours boite englobante, si inclus dans zone perceptible, alors test precision
				(+) pas de lecture/ecriture image
				(-) nombre d'iterations de lancer de rayon conditionne test
			-.option 2: pour chaque agent, parcours boite englobante, enregistre les agents proches dans l'image.
				(+) simple a mettre en place
				(-) pire cas: tout doit etre ecrit (cas actuel)
		-?todo 3: seconde passe optimisation
			-?reecrire la partie interne pour ne stocker qu'un seul octet...? 
				(?) optim? On ne stocke de toutes facons qu'on int, meme pour rgb. le seul gain serait l'extraction des composants couleurs...
-.off-line on-line
	-.lie a gCamera et gScreen
	-.enlever CastSensorRay de la methode "show" -- a ceci pret, c'est tout je crois.
	-.batch mode
-.compteur fps.
-.un "bot" evitement obstacle (eventuellement, refactorer la classe dot et faire un heritage)
-.lancer de rayon - renvoyer la valeur
-.charger depuis le fichier agent-specs les positions et orientation de senseurs (constructeur).
-.affichage orientation -- deriver agent::show(), afficher un point
-.dynamique orientation -- asservissement a un angle, ensuite le code fait du PID.
-.l'integrateur de controleur (on donne un asservissement, il le gere)


=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 


**MEMO**

- In C++, class hierarchy does not imply that most derived class shadow hierarchy! If cast at level N, method at level N will be called, not N_max (!= java).
- dont forget semi-colon at the end of "class bla {...};"
  => typical if error is: ''multiple types in one declaration''
- "class  ExtendedProperties : public Properties {...};" - public kword, otw public field from parent class become protected
- use extern keyword in include files. and dont forget to *actually* declare the variable (ie. extern gives prototype).
- abstract defini par: virtual void move() = 0;
- SVN IGNORE: 
 - créer ".svn/svnignore" dans le repertoire concerne + "svn propset svn:ignore -F .svn/svnignore ."
 - "svn status --no-ignore" pour verifier

- conventions:
 - "_gDataName" : static class variable (ie. global scope)
 - une variable de classe statique doit etre déclarée dans le .C : int MyClass::_gDataName;

- declare 2D array with std::vector
	std::vector<std::vector<int> > my2DArray
	my2DArray.resize(maxLength);
	for ( int i = 0 ; i != maxWidth ; i++)
	  (my2DArray.at(i)).reserve(gAgentCounter);

- inheritance and shadowing methods: C++ class hierarchy does not imply that most derived class shadow hierarchy! If cast at level N, method at level N will be called (!= java).

- inheritance and default constructor:
	by default, the empty constructor of the super class is called. To declare that a non empyt super-class constructor should be called, follow this example:
		MyNewClass::MyNewClass( MyParam *myParam ) : MySuperClass( *myParam )
		{
			blabla;
		}

- static vs. dynamic casts
	static_cast => C-style typecasting in C++
	dynamic_cast => to cast objects across the inheritance hierarchies
	check: http://www.cplusplus.com/doc/tutorial/typecasting/

=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 
	

**HOWTO**

Installation of a specific path in Xcode: 
 Select 'Edit active target 'x' from the Project menu.
 Choose the Build tab.
 Choose "All Configurations" from the Configuration popup button.
 Choose "Search Paths" from the Collection popup button. 
 Double click on the entry marked 'Header Search Paths'
 Add the path to the top level include folder. You should not select 'recursive'.

=-=-=-=-=-= =-=-=-=-=-= =-=-=-=-=-= 
