#pragma once
#include "State.h"
class Flee :
    public State
{
    virtual void Update(Vehicle* p_agent, float t) override;
};

