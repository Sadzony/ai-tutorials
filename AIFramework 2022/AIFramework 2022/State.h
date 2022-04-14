#pragma once
#include "Vehicle.h"
class State
{
public:
	State() { ; }
	virtual void EnterState() { ; }
	virtual void ExitState() { ; }
	virtual void Cleanup() { ; }
	virtual void Update(float t) { ; }
	bool completed = false;
};

