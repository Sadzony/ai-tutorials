#pragma once
#include "State.h"
#include <queue>
#include <deque>
#include <unordered_map>
class Navigation :
	public State
{
public:
	Navigation(Vehicle* p_agent, WaypointManager* p_wpManager) : m_wpManager(p_wpManager), m_agent(p_agent) { lastTarget = m_agent->GetTargetPntr(); }
	virtual void EnterState() override;
	virtual void Update(float t) override;
	virtual void Cleanup() override;
private:
	Vehicle* m_agent;
	WaypointManager* m_wpManager;
	std::queue<Waypoint*> pathToTravel;

	//breadth first algorithm variables
	std::queue<Waypoint*> frontier;
	std::unordered_map<Waypoint*, Waypoint*> came_from;

	//breadth first functions
	void PopToCameFrom(Waypoint* nodeToPop);
	//funcctions
	void CleanPathfinding();
	void FindPath(Waypoint* target);

	//logic
	bool pathFound = false;
	bool lastPointReached = false;
	Vector2D* lastTarget;
};