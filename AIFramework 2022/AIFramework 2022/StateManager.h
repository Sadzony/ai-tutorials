#pragma once
#include "State.h"

class StateManager
{
public:
	StateManager(Vehicle* p_agent);
	~StateManager();
	void Update(float t);
	void ChangeState(State* newState);

protected:
	State* m_currentState;
	State* m_previousState;
	Vehicle* m_agent;
};

