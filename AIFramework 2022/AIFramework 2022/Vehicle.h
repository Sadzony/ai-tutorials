#pragma once

#include "DrawableGameObject.h"
#include "Vector2D.h"
#include "Collidable.h"
#include "MovementManager.h"
#include "constants.h"

class WaypointManager;

enum class carColour
{
	redCar,
	blueCar,
};
struct ObstacleData {
	Vector2D* position;
	float radius;
};

class Vehicle : public DrawableGameObject, public Collidable
{
public:
	~Vehicle();
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice, carColour colour);
	virtual void update(const float deltaTime);

	MovementManager* GetMovementManager() { return m_movementManager; }
	WaypointManager* GetWaypointManager() { return m_waypointManager; }

	Vector2D getTargetPos() { return m_targetPos; }
	void setTargetPos(Vector2D p_targetPos) { m_targetPos = p_targetPos; }

	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	void setWaypointManager(WaypointManager* wpm);
	void hasCollided() {}

	ObstacleData* GetObstacleData() { return m_obstacleData; }

	Vector2D* GetTargetPntr() { return &m_targetPos; }

	void RefillFuel() { fuel = MAX_FUEL; }
	double GetCurrentFuel() { return fuel; }

	void Reset();

protected: // preotected properties

	Vector2D m_targetPos;


	Vector2D m_currentPosition;
	Vector2D m_lastPosition;

	WaypointManager* m_waypointManager;

	MovementManager* m_movementManager;

	ObstacleData* m_obstacleData;

	double fuel = MAX_FUEL;
};

