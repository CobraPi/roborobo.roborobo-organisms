/* 
 * File:   Organism.h
 * Author: bweel
 *
 * Created on April 13, 2011, 6:10 PM
 */

#ifndef ORGANISM_H
#define	ORGANISM_H

#include <vector>
#include <ostream>
#include "RoboroboMain/roborobo.h"

class Organism {
public:
	std::vector<RobotAgentPtr> agents;
    RobotAgentPtr leader;

	Organism();
	virtual ~Organism();
    
    // Called every timestep to update age and do logging
    virtual void update();

	void setId(int _id);
	int getId();

    unsigned int getAge();
	int size();

	void addRobot(RobotAgentPtr robot);
	void removeRobot(RobotAgentPtr robot);
    
	void mergeInto(Organism *other);

	void prepareMove();
	void move();
	void registerOrganism();

	void show();
	void logOrganism();
	
	void getBaryCenter(double &x, double &y);
	
	std::vector<Organism*> checkIntegrity();
	
	bool contains(RobotAgentPtr robot);
	
	void setActive();
	void setInactive();
	bool isActive();

    RobotAgentPtr getLeader();
    void setLeader(RobotAgentPtr);
    
    static bool loadProperties();
	static void remove(Organism *organism);
	static void add(Organism *organism);
	static void reset();
	static void destroy();
	static void showAll();
	static void logAll();
	static void prepareAllMove();
	static void checkAllIntegrity();
	
	static int nbOrganisms;

private:
	void combineVectors(double &angle, double &translation);
	void moveOrganism(double angle, double translation);
	bool detectCollisions();
	void rollbackMove();
	void updateWorldModels();
	void updateSensors();
	int agentsInRiverzone();

	static std::vector<Organism*> *organisms;
	static std::ofstream sizeLog;
	
	int id;
	bool hasMoved;
	bool active;
	
	unsigned int creationIteration;
	unsigned int deathIteration;
    unsigned int age;
};

// Organism related globals
extern bool gUseOrganisms;                      // agents can aggregate into organisms, default := false
                                                // if set to true agents that are part of an organism are moved at the same time
extern std::string gAgentPositiveMaskImageFilename;
extern std::string gAgentNegativeMaskImageFilename;
extern std::string gOrganismSizesLogFilename;
extern SDL_Surface *gAgentPositiveMaskImage;	// mask used for showing that an agent wants to connect
extern SDL_Surface *gAgentNegativeMaskImage;	// mask used for showing that an agent wants to connect
extern int gConnectionGap;                      // the gap allowed between agents in pixels
extern int gOrganismSampleFrequency;            // the frequency of logging samples in iterations. Higher means less frequent
extern bool gUseOrganismLeader;                 // If true, movement of the organism is done by the leader, not summing the individual module vectors
extern bool gOrganismNormaliseSpeed;            // If true, movement is capped at the maximum speed. i.e. organisms cannot move faster than individual modules


#endif	/* ORGANISM_H */

