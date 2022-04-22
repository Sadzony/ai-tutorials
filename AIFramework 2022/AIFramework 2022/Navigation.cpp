#include "Navigation.h"
#include "Waypoint.h"

#define WAYPOINTS_MAX_ANGLE 20.0
#define PI 3.14

void Navigation::Update(float t)
{
	Vector2D agentTargetPos = m_agent->getTargetPos();
	if (agentTargetPos != lastTarget) {
		pathFound = false;
	}
	if (pathFound) {
		//do movement
		if (!pathToTravel.empty()) 
		{
			Vector2D target = pathToTravel.front()->getPosition();
			Vector2D curPos = m_agent->getPosition();
			MovementManager* movement = m_agent->GetMovementManager();
			if (pathToTravel.size() == 1) //if travelling to last waypoint then just do arrive
			{
				float length = (target - curPos).Length();
				if (length > 5) { //5 pixels of error allowed
					movement->SeekWithArrive(target, curPos);
					m_agent->setVehiclePosition(movement->ProcessMovementWithTrunc(t, curPos, length));
				}
				else {
					currentWaypoint = pathToTravel.front();
					pathToTravel.pop_front();
					movement->ZeroPhysics();
				}
			}
			else {
				if (CheckIfArrive()) { //decide whether to seek or arrive
					float length = (target - curPos).Length();
					if (length > 15) { //15 pixels of error allowed
						movement->SeekWithArriveAndMinSpeed(target, curPos, 40.0f); //min speed to not slow the car to a stop
						m_agent->setVehiclePosition(movement->ProcessMovementWithTrunc(t, curPos, length));
					}
					else {
						movement->SetAcceleration(Vector2D(0, 0));
						currentWaypoint = pathToTravel.front();
						pathToTravel.pop_front();
					}
				}
				else {
					float length = (target - curPos).Length();
					if (length > 20) { //20 pixels of error allowed
						movement->Seek(target, curPos);
						m_agent->setVehiclePosition(movement->ProcessMovementWithTrunc(t, curPos, length));
					}
					else {
						currentWaypoint = pathToTravel.front();
						pathToTravel.pop_front();
					}
				}
			}
		}

	}
	else {
		lastTarget = agentTargetPos;
		FindPath(m_wpManager->getNearestWaypoint(m_agent->getTargetPos()));
	}
}

void Navigation::Cleanup()
{
	std::deque<Waypoint*>().swap(pathToTravel); //swap pathing queue with empty queue
	m_agent = nullptr;
	m_wpManager = nullptr;
}


void Navigation::FindPath(Waypoint* target)
{
	std::deque<Waypoint*>().swap(pathToTravel); //swap pathing queue with empty queue
	currentWaypoint = m_wpManager->getNearestWaypoint(m_agent->getPosition()); //start node defined by node closest to agent
	pathToTravel = m_wpManager->GeneratePathQueueAStar(currentWaypoint, target);
	if (!pathToTravel.empty()) {
		pathToTravel.pop_front(); //pop the starting waypoint
	}
	pathFound = true;

}

bool Navigation::CheckIfArrive()
{
	//get direction of current waypoint to waypoint at front of queue
	Vector2D dirToNextWaypoint = pathToTravel.front()->getPosition() - currentWaypoint->getPosition();
	//get direction of waypoint at front of queue to waypoint at queue(1)
	Vector2D dirToWaypointAfterNext = pathToTravel.at(1)->getPosition() - pathToTravel.front()->getPosition();

	double angle = dirToNextWaypoint.GetRotation(dirToWaypointAfterNext);

	if (angle < toRadians(WAYPOINTS_MAX_ANGLE)) {
		return false;
	}
	else {
		return true;
	}
}
double Navigation::toRadians(double degrees)
{
	return (degrees * PI) / 180;
}