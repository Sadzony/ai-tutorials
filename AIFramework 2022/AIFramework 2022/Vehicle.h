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

	void Reset();

protected: // preotected properties

	Vector2D m_targetPos;


	Vector2D m_currentPosition;
	Vector2D m_lastPosition;

	WaypointManager* m_waypointManager;

	MovementManager* m_movementManager;
};

