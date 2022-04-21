#pragma once
#include "State.h"
class Wander :
    public State
{
public:
    Wander(Vehicle* p_agent) { m_agent = p_agent; movement = m_agent->GetMovementManager(); }
    virtual void EnterState() override;
    virtual void Update(float t) override;
    virtual void Cleanup() override;
protected:
    double toRadians(double degrees);
    double angle = 0;
    int steerDirection = 0;
    float timePassed = 0;
    Vehicle* m_agent;
    MovementManager* movement;
};

