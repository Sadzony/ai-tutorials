#pragma once
#include "State.h"
class Wander :
    public State
{
public:
    Wander(Vehicle* p_agent) { m_agent = p_agent; }
    virtual void Update(float t) override;
    virtual void Cleanup() override;
protected:
    Vehicle* m_agent;
};

