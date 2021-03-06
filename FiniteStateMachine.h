#pragma once
#include "DCMotorCommand.h"

class FiniteStateMachine
{

static const int MOTOR_STATE_INIT = 0;
static const int MOTOR_STATE_WINDING_UP = 1;
static const int MOTOR_STATE_RUNNING_PARADIGM = 2;
static const int MOTOR_STATE_SHUTTING_DOWN = 3;

DCMotorCommand *pDCMotorCommand;

public:
	int state;
	char *stateLabel[6];
	char* getStateLabel(void);
	FiniteStateMachine(void);
	~FiniteStateMachine(void);
	int ProceedState(void);
	int Abort(void);
};

