#pragma once
#include "State.h"
class Flee :
    public State
{
public:
    Flee(Vehicle* p_agent, Vehicle* p_fleeTarget) { m_agent = p_agent; m_fleeTarget = p_fleeTarget; SetFlag(StateFlag::Flee);}
    virtual void Update(float t) override;
    virtual void Cleanup() override;
    void setFleeTarget(Vehicle* p_fleeTarget) { m_fleeTarget = p_fleeTarget; }
protected:
    Vehicle* m_agent;
    Vehicle* m_fleeTarget;
};

