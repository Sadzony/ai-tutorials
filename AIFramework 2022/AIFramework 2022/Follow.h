#pragma once
#include "State.h"
class Follow :
	public State
{
public:
	Follow(Vehicle* p_agent, Vehicle* p_chaseTarget) { m_agent = p_agent; m_chaseTarget = p_chaseTarget; SetFlag(StateFlag::Follow);}
	virtual void Update(float t) override;
	virtual void Cleanup() override;
	void setChaseTarget(Vehicle* p_chaseTarget) { m_chaseTarget = p_chaseTarget; }
protected:
	Vehicle* m_agent;
	Vehicle* m_chaseTarget;
};

