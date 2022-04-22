#pragma once
#include "State.h"
#include "WaypointManager.h"
#include <queue>

class Navigation :
	public State
{
public:
	Navigation(Vehicle* p_agent, WaypointManager* p_wpManager) : m_wpManager(p_wpManager), m_agent(p_agent) { lastTarget = Vector2D(0, 0); }
	virtual void Update(float t) override;
	virtual void Cleanup() override;
protected:
	Vehicle* m_agent;
	WaypointManager* m_wpManager;
	std::deque<Waypoint*> pathToTravel;
	Waypoint* currentWaypoint;

	//functions
	void FindPath(Waypoint* target);
	bool CheckIfArrive();
	double toRadians(double degrees);

	//logic
	bool pathFound = false;
	bool lastPointReached = false;
	Vector2D lastTarget;
};