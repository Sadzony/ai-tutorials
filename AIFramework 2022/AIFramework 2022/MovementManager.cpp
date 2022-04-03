#include "MovementManager.h"

#define ARRIVAL_RADIUS 150

#define FLEE_RADIUS 250

#define ARRIVAL_MULTIPLIER 1.0

MovementManager::MovementManager(Vector2D startPos, float p_maxSpeed, float p_maxForcePower)
{
	m_physicsPosition = startPos;
	m_maxSpeed = p_maxSpeed;
	m_currentSpeed = p_maxSpeed;
	m_maxForcePower = p_maxForcePower;
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
		SeekWithArrive(targetPos);
		UpdateNetForce();
		UpdateAcceleration();
		MoveStep(t, vecToTarget);
	}
}

void MovementManager::Update(const float t, Vector2D chaserPos)
{
	Vector2D vecToTarget = targetPos - m_physicsPosition;
	if (!destinationReached) {
		float distanceToChaser = (chaserPos - m_physicsPosition).Length();
		if (distanceToChaser < FLEE_RADIUS) {
			m_currentSpeed = m_maxSpeed;
			Flee(chaserPos);
		}
		else {
			SeekWithArrive(targetPos);
		}
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

	Vector2D direction = targetPos - m_physicsPosition;
	Vector2D seekingForce = direction.Normalized() * m_currentSpeed; //find the desired velocity
	seekingForce -= velocity; //subtract current velocity to get closer to desired velocity
	seekingForce.Truncate(m_maxForcePower); //truncate to the acceleration power
	AddForce(seekingForce);
}

void MovementManager::SeekWithArrive(Vector2D targetPos)
{
	Vector2D direction = targetPos - m_physicsPosition;
	float distance = direction.Length();
	if (distance < ARRIVAL_RADIUS) {
		m_currentSpeed = m_maxSpeed * (distance / (ARRIVAL_RADIUS*ARRIVAL_MULTIPLIER));
	}
	else {
		m_currentSpeed = m_maxSpeed;
	}
	Vector2D seekingForce = direction.Normalized() * m_currentSpeed; //find the desired velocity
	seekingForce -= velocity; //subtract current velocity to get closer to desired velocity
	seekingForce.Truncate(m_maxForcePower); //truncate to the acceleration power
	AddForce(seekingForce);
}

void MovementManager::Flee(Vector2D chaserPos)
{
	Vector2D fleeingForce;
	Vector2D direction = m_physicsPosition - chaserPos;
	fleeingForce = direction.Normalized() * m_currentSpeed; //find desired velocity
	fleeingForce -= velocity;
	fleeingForce.Truncate(m_maxForcePower);
	AddForce(fleeingForce);
}

void MovementManager::MoveStep(float t, Vector2D directionVec)
{
	float distanceToTarget = (float)directionVec.Length();

	if (distanceToTarget > 5) { //5 pixels of error allowed
		Vector2D nextStep = velocity * t + (0.5 * acceleration * t * t);
		velocity += acceleration * t;
		velocity.Truncate(m_currentSpeed); //don't let the velocity be bigger than speed
		nextStep.Truncate(distanceToTarget); 	// if the distance to the end point is less than the car would move, then only move that distance. 
		m_physicsPosition += nextStep;
	}
	else {
		destinationReached = true;
		ZeroPhysics();
	}
}
