#include "MovementManager.h"

#define ARRIVAL_RADIUS 200

#define FLEE_RADIUS 250

MovementManager::MovementManager(Vector2D startPos, float speed)
{
	m_physicsPosition = startPos;
	m_speed = speed;

}

void MovementManager::setTargetPos(Vector2D position)
{
	destinationReached = false;
	targetPos = position;
}

void MovementManager::ZeroPhysics()
{
	acceleration.Zero();
	velocity.Zero();
	netForce.Zero();
	externalForces.clear();
}

void MovementManager::Update(const float t)
{
	Vector2D vecToTarget = targetPos - m_physicsPosition;
	if (!destinationReached) {
		Seek(targetPos);
		UpdateNetForce();
		UpdateAcceleration();
		MoveStep(t, vecToTarget);
	}
}

void MovementManager::Update(const float t, Vector2D chaserPos)
{
	Vector2D vecToTarget = targetPos - m_physicsPosition;
	if (!destinationReached) {
		Flee(chaserPos);
		Seek(targetPos);
		UpdateNetForce();
		UpdateAcceleration();
		MoveStep(t, vecToTarget);
	}
}

void MovementManager::UpdateAcceleration()
{
	acceleration = netForce;
	netForce.Zero();
}

void MovementManager::UpdateNetForce()
{
	for (int i = 0; i < externalForces.size(); i++)
	{
		netForce += externalForces.at(i);
	}
	externalForces.clear();
}

void MovementManager::Seek(Vector2D targetPos)
{
	Vector2D seekingForce;
	Vector2D direction = targetPos - m_physicsPosition;
	float distance = direction.Length();
	seekingForce = direction.Normalized() * m_speed;
	seekingForce -= velocity;
	if (distance < ARRIVAL_RADIUS) {
		Arrive(seekingForce, distance);
	}
	AddForce(seekingForce);
}

void MovementManager::Arrive(Vector2D seekingVec, float distance)
{
	Vector2D arrivalForce;
	float forcePowerMultiplier = 1 - (distance / ARRIVAL_RADIUS);
	arrivalForce = -1 * seekingVec * forcePowerMultiplier; //force gets smaller with distance
	arrivalForce -= velocity;
	AddForce(arrivalForce);
}

void MovementManager::Flee(Vector2D chaserPos)
{

	float distanceToChaser = (chaserPos - m_physicsPosition).Length();
	Vector2D fleeingForce;
	Vector2D direction = m_physicsPosition - chaserPos;
	fleeingForce = direction.Normalized() * m_speed;
	fleeingForce -= velocity;
	AddForce(fleeingForce);
}

void MovementManager::MoveStep(float t, Vector2D directionVec)
{
	float distanceToTarget = (float)directionVec.Length();

	if (distanceToTarget > 5) { //5 pixels of error allowed
		Vector2D nextStep = velocity * t + (0.5 * acceleration * t * t);
		velocity += acceleration * t;
		velocity.Truncate(m_speed); //don't let the velocity be bigger than max speed
		nextStep.Truncate(distanceToTarget); 	// if the distance to the end point is less than the car would move, then only move that distance. 
		m_physicsPosition += nextStep;
	}
	else {
		destinationReached = true;
	}
}
