#pragma once
#include "State.h"
class Wander :
    public State
{
    virtual void Update(Vehicle* p_agent, float t) override;
};

