#pragma once
#include "State.h"
class ArriveToTarget :
    public State
{
public:
    ArriveToTarget(Vehicle* p_agent) { m_agent = p_agent; SetFlag(StateFlag::ArriveToTarget); }
    virtual void Update(float t) override;
    virtual void Cleanup() override;
protected:
    Vehicle* m_agent;
};

