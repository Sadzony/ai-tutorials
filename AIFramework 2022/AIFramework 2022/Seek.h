#pragma once
#include "State.h"
class Seek :
    public State
{
public:
    Seek(Vehicle* p_agent) { m_agent = p_agent; }
    virtual void Update(float t) override;
    virtual void Cleanup() override;
protected:
    Vehicle* m_agent;
};

