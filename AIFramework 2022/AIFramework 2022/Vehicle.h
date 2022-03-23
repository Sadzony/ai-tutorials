#pragma once

#include "DrawableGameObject.h"
#include "WaypointManager.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "MovementManager.h"

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
	~Vehicle();
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour, carBehaviour behaviour);
	virtual void update(const float deltaTime);


	void setTargetPosition(Vector2D position);
	void setChaserPosition(Vector2D position) { m_chaserPos = position; }
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	void setWaypointManager(WaypointManager* wpm);
	void hasCollided() {}

	//logic variables
	bool destinationReached = true;
	bool fleeToggle = false;
	carBehaviour behaviour;

protected: // preotected properties
	
	Vector2D m_chaserPos;

	Vector2D m_currentPosition;
	Vector2D m_lastPosition;
	WaypointManager* m_waypointManager;

	MovementManager* m_movementManager;

};

