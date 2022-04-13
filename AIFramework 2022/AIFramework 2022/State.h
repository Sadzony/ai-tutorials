#pragma once
#include "Vehicle.h"
class State
{
public:
	State() { ; }
	virtual void EnterState() { ; }
	virtual void ExitState() { ; }
	virtual void Update(Vehicle* p_agent, float t) { ; }
	bool completed = false;
};

