#ifndef SYMBRIONCONTROLARCHITECTURE_H
#define SYMBRIONCONTROLARCHITECTURE_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "newscast/PopulationControlArchitecture.h"
#include "newscast/HiveMindInstance.h"

#include "neuralnetworks/ESN.h"

#include "WorldModels/RobotAgentWorldModel.h"

class SymbrionControlArchitecture : public BehaviorControlArchitecture { //: public DifferentialControlArchitecture {
public:
	SymbrionControlArchitecture( RobotAgentWorldModel* _wm );
	virtual ~SymbrionControlArchitecture();


    virtual void reset();
	virtual void step();

private:
    void followAngle(double angle, double &left, double &right);
    
    vector<double> getSensorValues();
	
	vector<double> weights;
	Neural::ESN* neuralNet;
    int id;
    double desiredAngle;
};

#endif

