#pragma once
#include "State.h"
class ArriveToTarget :
    public State
{
    virtual void Update(Vehicle* p_agent, float t) override;
};

