#pragma once
#include "State.h"
#include "WaypointManager.h"
#include <queue>
#include "constants.h"
#include "PickupItem.h"

typedef vector<PickupItem*>* vecPickupsPntr;

class Strategy :
	public State
{
public:
	Strategy(Vehicle* p_agent, WaypointManager* p_wpManager, vecPickupsPntr p_pickups) : m_wpManager(p_wpManager), m_agent(p_agent), m_pickups(p_pickups) { lastTarget = Vector2D(0, 0); SetFlag(StateFlag::Strategy); }
	virtual void Update(float t) override;
	virtual void Cleanup() override;
protected:
	Vehicle* m_agent;
	WaypointManager* m_wpManager;
	std::deque<Waypoint*> pathToTravel;
	Waypoint* currentWaypoint;
	vecPickupsPntr m_pickups;

	//functions
	void FindPath(Waypoint* target);
	bool CheckIfArrive();
	double toRadians(double degrees);

	//logic
	bool pathFound = false;
	bool lastPointReached = false;
	Vector2D lastTarget;
};