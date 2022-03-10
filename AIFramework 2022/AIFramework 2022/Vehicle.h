#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Vector2D.h"
#include "Collidable.h"

enum class carColour
{
	redCar,
	blueCar,
};
enum class carBehaviour {
	wander,
	arriveToMouse,
	seekVehicle
};

class Vehicle : public DrawableGameObject, public Collidable
{
public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour, carBehaviour behaviour);
	virtual void update(const float deltaTime);

	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void setTargetPosition(Vector2D positionTo); // a position to move to
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	void setChaserPosition(Vector2D position);
	void setWaypointManager(WaypointManager* wpm);
	void hasCollided() {}

	//force physics
	void SetVelocity(Vector2D pvelocity); //velocity get set
	Vector2D GetVelocity();

	Vector2D GetAcceleration();
	void SetAcceleration(Vector2D pacceleration); //acceleration get set

	void AddForce(Vector2D force); //add force to external forces list
	Vector2D GetNetForce(); //get the net force vector

	void ZeroPhysics();

	//logic variables
	bool destinationReached = true;
	bool fleeToggle = false;
	carBehaviour behaviour;
	
protected: // protected methods
	void UpdateAcceleration(); // apply force to update acceleration
	void UpdateNetForce(); //add up each force in the external forces list
	void Move(float t, Vector2D direction); //move the position and update velocity.
	void Seek();
	void Arrive();
	void Flee();

protected: // preotected properties

	float m_maxSpeed;
	
	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_targetPos;
	Vector2D m_ChaserPosition;
	Vector2D m_lastPosition;
	WaypointManager* m_waypointManager;

	//force physics
	Vector2D netForce;
	std::vector<Vector2D> externalForces;
	Vector2D velocity;
	Vector2D acceleration;

};

