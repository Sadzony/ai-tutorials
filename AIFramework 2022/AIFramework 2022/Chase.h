#pragma once
#include "State.h"
class Chase :
	public State
{
	virtual void Update(Vehicle* p_agent, float t) override;
};

