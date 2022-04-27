#pragma once
#include "Vehicle.h"
enum class StateFlag
{
	Empty,
	ArriveAndAvoid,
	ArriveToTarget,
	Flee,
	Follow,
	Navigation,
	Pursue,
	Seek,
	Wander,
	Strategy,
};
class State
{
public:
	State() : flag(StateFlag::Empty) { ; }
	virtual void EnterState() { ; }
	virtual void ExitState() { ; }
	virtual void Cleanup() { ; }
	virtual void Update(float t) { ; }
	StateFlag GetFlag() { return flag; }
	void SetFlag(StateFlag p_flag) { flag = p_flag; }
	bool completed = false;

protected:
	StateFlag flag;
};

