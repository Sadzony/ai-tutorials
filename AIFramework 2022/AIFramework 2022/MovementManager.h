#pragma once
#include "Vector2D.h"
#include <vector>
class MovementManager
{
public:
	MovementManager(float p_maxSpeed, float p_accelerationPower);

	void setMaxSpeed(const float p_maxSpeed) { m_maxSpeed = p_maxSpeed; }
	float getMaxSpeed() { return m_maxSpeed; }

	//physics get sets
	void SetVelocity(Vector2D pvelocity) { velocity = pvelocity; }
	Vector2D GetVelocity() { return velocity; }

	void SetAcceleration(Vector2D pacceleration) { acceleration = pacceleration; }
	Vector2D GetAcceleration() { return acceleration; }

	void AddForce(Vector2D force) { externalForces.push_back(force); } //add force to external forces list
	Vector2D GetNetForce() { return netForce; }

	void ZeroPhysics();

	//steering behaviours
	void Seek(Vector2D targetPos, Vector2D currentPos);
	void SeekWithArrive(Vector2D targetPos, Vector2D currentPos);
	void Flee(Vector2D chaserPos, Vector2D currentPos);

	Vector2D ReturnPosAfterStep(const float t, Vector2D currentPos);

	//this function will return the new position after the physics update, and update with new values
	Vector2D ProcessMovement(const float t, Vector2D currentPos);

	//same as above but also truncates the move step to stop at target
	Vector2D ProcessMovementWithTrunc(const float t, Vector2D currentPos, float distance);

protected:
	void UpdateAcceleration(); // apply force to update acceleration
	void UpdateNetForce(); //add up each force in the external forces list

protected: //propertiess

	float m_currentSpeed;
	float m_maxSpeed;
	float m_steeringPower;

	//forces
	Vector2D netForce;
	std::vector<Vector2D> externalForces;
	Vector2D velocity;
	Vector2D acceleration;

};

