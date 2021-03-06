#pragma once
#include "State.h"

class StateManager
{
public:
	StateManager(Vehicle* p_agent);
	~StateManager();
	void Update(float t);
	void ChangeState(State* newState);

	StateFlag GetCurrentFlag() { return currentFlag; }

protected:
	State* m_currentState;
	State* m_previousState;
	Vehicle* m_agent;
	StateFlag currentFlag;
};

