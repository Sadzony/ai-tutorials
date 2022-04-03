#pragma once
#include "Vector2D.h"
#include <vector>
class MovementManager
{
public:
	MovementManager(Vector2D startPos, float p_maxSpeed, float p_accelerationPower);

	void setTargetPos(Vector2D position);
	Vector2D getTargetPos() { return targetPos; }

	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setPhysicsPosition(Vector2D position) { m_physicsPosition = position; }
	Vector2D GetPhysicsPosition() { return m_physicsPosition; }

	//physics get sets
	void SetVelocity(Vector2D pvelocity) { velocity = pvelocity; }
	Vector2D GetVelocity() { return velocity; }

	void SetAcceleration(Vector2D pacceleration) { acceleration = pacceleration; }
	Vector2D GetAcceleration() { return acceleration; }

	void SetDestinationReachedBool(bool pReached) { destinationReached = pReached; }
	bool GetDestinationReached() { return destinationReached; }

	void AddForce(Vector2D force) { externalForces.push_back(force); } //add force to external forces list
	Vector2D GetNetForce() { return netForce; }

	void ZeroPhysics();

	void Update(const float t);
	void Update(const float t, Vector2D chaserPos);

protected:
	void UpdateAcceleration(); // apply force to update acceleration
	void UpdateNetForce(); //add up each force in the external forces list
	
	//steering behaviours
	void Seek(Vector2D targetPos);
	void SeekWithArrive(Vector2D targetPos);
	void Flee(Vector2D chaserPos);

	void MoveStep(float t, Vector2D directionVec); //move the position by one step and update velocity

protected: //propertiess
	Vector2D targetPos;

	float m_currentSpeed;
	float m_maxSpeed;
	float m_maxForcePower;
	Vector2D m_physicsPosition;

	//forces
	Vector2D netForce;
	std::vector<Vector2D> externalForces;
	Vector2D velocity;
	Vector2D acceleration;

	//logic
	bool destinationReached = true;
};

