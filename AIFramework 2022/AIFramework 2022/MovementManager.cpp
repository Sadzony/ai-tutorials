#include "MovementManager.h"

#define ARRIVAL_RADIUS 120

#define ARRIVAL_MULTIPLIER 1.35

MovementManager::MovementManager(float p_maxSpeed, float p_steeringPower)
{
	m_maxSpeed = p_maxSpeed;
	m_currentSpeed = p_maxSpeed;
	m_steeringPower = p_steeringPower;
}

void MovementManager::ZeroPhysics()
{
	acceleration.Zero();
	velocity.Zero();
	netForce.Zero();
	externalForces.clear();
}
void MovementManager::Seek(Vector2D targetPos, Vector2D currentPos)
{
	m_currentSpeed = m_maxSpeed;
	Vector2D direction = targetPos - currentPos;
	if (direction.isZero()) {
		return;
	}
	Vector2D seekingForce = direction.Normalized() * m_currentSpeed; //find the desired velocity
	seekingForce -= velocity; //subtract current velocity to get closer to desired velocity
	seekingForce.Truncate(m_steeringPower); //truncate to the acceleration power
	AddForce(seekingForce);
}
void MovementManager::SeekWithArrive(Vector2D targetPos, Vector2D currentPos)
{
	Vector2D direction = targetPos - currentPos;
	if (direction.isZero()) {
		return;
	}
	float distance = direction.Length();
	if (distance < ARRIVAL_RADIUS) {
		m_currentSpeed = m_maxSpeed * (distance / (ARRIVAL_RADIUS * ARRIVAL_MULTIPLIER)); //limits the speed depending on how close to the target we are. Arrival multiplier makes the slow stronger
	}
	else {
		m_currentSpeed = m_maxSpeed;
	}
	Vector2D seekingForce = direction.Normalized() * m_currentSpeed; //find the desired velocity
	seekingForce -= velocity; //subtract current velocity to get closer to desired velocity
	seekingForce.Truncate(m_steeringPower); //truncate to the acceleration power
	AddForce(seekingForce);
}
void MovementManager::SeekWithArriveAndMinSpeed(Vector2D targetPos, Vector2D currentPos, float minSpeed)
{
	Vector2D direction = targetPos - currentPos;
	if (direction.isZero()) {
		return;
	}
	float distance = direction.Length();
	if (distance < ARRIVAL_RADIUS) {
		m_currentSpeed = m_maxSpeed * (distance / (ARRIVAL_RADIUS * ARRIVAL_MULTIPLIER)); //limits the speed depending on how close to the target we are. Arrival multiplier makes the slow stronger
		if (m_currentSpeed < minSpeed) {
			m_currentSpeed = minSpeed;
		}
	}
	else {
		m_currentSpeed = m_maxSpeed;
	}
	Vector2D seekingForce = direction.Normalized() * m_currentSpeed; //find the desired velocity
	seekingForce -= velocity; //subtract current velocity to get closer to desired velocity
	seekingForce.Truncate(m_steeringPower); //truncate to the acceleration power
	AddForce(seekingForce);
}
void MovementManager::Flee(Vector2D chaserPos, Vector2D currentPos)
{
	m_currentSpeed = m_maxSpeed;
	Vector2D fleeingForce;
	Vector2D direction = currentPos - chaserPos;
	if (direction.isZero()) {
		return;
	}
	fleeingForce = direction.Normalized() * m_currentSpeed; //find desired velocity
	fleeingForce -= velocity;
	fleeingForce.Truncate(m_steeringPower);
	AddForce(fleeingForce);
}
Vector2D MovementManager::ReturnPosAfterStep(const float t, Vector2D currentPos)
{
	return currentPos + ( velocity * t + (0.5 * acceleration * t * t));
}
Vector2D MovementManager::ProcessMovement(const float t, Vector2D currentPos)
{
	UpdateNetForce();
	UpdateAcceleration();
	Vector2D nextStep = velocity * t + (0.5 * acceleration * t * t);
	velocity += acceleration * t;
	velocity.Truncate(m_currentSpeed); //don't let the velocity be bigger than speed
	return currentPos + nextStep;
	
}
Vector2D MovementManager::ProcessMovementWithTrunc(const float t, Vector2D currentPos, float distance)
{
	UpdateNetForce();
	UpdateAcceleration();
	Vector2D nextStep = velocity * t + (0.5 * acceleration * t * t);
	velocity += acceleration * t;
	velocity.Truncate(m_currentSpeed); //don't let the velocity be bigger than speed
	nextStep.Truncate(distance); 	// if the distance to the end point is less than the car would move, then only move that distance.
	return currentPos + nextStep;
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
