#include "FiniteStateMachine.h"


FiniteStateMachine::FiniteStateMachine(void)
{
	state = MOTOR_STATE_INIT;
	stateLabel[0] = "MOTOR_STATE_INIT";
	stateLabel[1] = "MOTOR_STATE_WINDING_UP";
	stateLabel[2] = "MOTOR_STATE_RUNNING_PARADIGM";
	stateLabel[3] = "MOTOR_STATE_SHUTTING_DOWN";
}


FiniteStateMachine::~FiniteStateMachine(void)
{
}


char* FiniteStateMachine::getStateLabel(void)
{
	return stateLabel[state];
}

int FiniteStateMachine::ProceedState(void)
{
    switch(state)
    {
    case MOTOR_STATE_INIT:
        //EnableMotors(&gEnableHandle);
		state = MOTOR_STATE_WINDING_UP;  
        break;
    case MOTOR_STATE_WINDING_UP:
        state = MOTOR_STATE_RUNNING_PARADIGM;
        break;
	case MOTOR_STATE_RUNNING_PARADIGM:
        //DisableMotors(&gEnableHandle);
		state = MOTOR_STATE_SHUTTING_DOWN;
        break;
    case MOTOR_STATE_SHUTTING_DOWN:
        state = MOTOR_STATE_SHUTTING_DOWN;
        break;
    default:
        state = MOTOR_STATE_INIT;
    }
    return 0;
}

int FiniteStateMachine::Abort(void)
{
	state = 3;
	
	return 0;
}

