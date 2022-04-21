#include "Navigation.h"

void Navigation::EnterState()
{
}

void Navigation::Update(float t)
{
	if (m_agent->GetTargetPntr() != lastTarget) {
		lastTarget = m_agent->GetTargetPntr();
		pathFound = false;
	}
	if (pathFound) {
		//do movement
	}
	else {
		FindPath(m_wpManager->getNearestWaypoint(m_agent->getTargetPos()));
	}
}

void Navigation::Cleanup()
{
}

void Navigation::PopToCameFrom(Waypoint* nodeToPop)
{
}

void Navigation::CleanPathfinding()
{
	std::queue<Waypoint*>().swap(frontier); //swap current queue with empty queue
	came_from.clear();

}

void Navigation::FindPath(Waypoint* target)
{
}
