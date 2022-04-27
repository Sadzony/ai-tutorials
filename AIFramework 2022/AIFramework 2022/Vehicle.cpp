#include "Vehicle.h"

#define MAX_SPEED 150

#define LOW_FUEL_SPEED 50

#define MAX_STEERING_POWER 400

#define VEHICLE_RADIUS 30.0f;

Vehicle::~Vehicle()
{
	delete m_movementManager;
	m_movementManager = nullptr;

	delete m_obstacleData;
	m_obstacleData = nullptr;
}

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice, carColour colour)
{
	m_scale = XMFLOAT3(30, 20, 1);
	m_targetPos = Vector2D();
	if (colour == carColour::redCar)
	{
		setTextureName(L"Resources\\car_red.dds");
	}
	else if (colour == carColour::blueCar)
	{
		setTextureName(L"Resources\\car_blue.dds");
	}

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	m_lastPosition = Vector2D(0, 0);

	m_movementManager = new MovementManager(MAX_SPEED, MAX_STEERING_POWER);
	m_obstacleData = new ObstacleData();
	m_obstacleData->position = &m_currentPosition;
	m_obstacleData->radius = VEHICLE_RADIUS;

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	if (fuel < 0) 
	{
		m_movementManager->setMaxSpeed(LOW_FUEL_SPEED);
	}
	else {
		m_movementManager->setMaxSpeed(MAX_SPEED);
	}
	// rotate the object based on its last & current position
	Vector2D diff = m_currentPosition - m_lastPosition;
	float distanceTravelledLastFrame = diff.Length();
	if (distanceTravelledLastFrame > 0) { // if zero then don't update rotation
		fuel -= distanceTravelledLastFrame;
		diff.Normalize();
		m_radianRotation = atan2f((float)diff.y, (float)diff.x); // this is used by DrawableGameObject to set the rotation
	}
	m_lastPosition = m_currentPosition;

	// set the current poistion for the drawablegameobject
	setPosition(Vector2D(m_currentPosition));

	DrawableGameObject::update(deltaTime);
}

// set the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	setPosition(position);
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}

void Vehicle::Reset()
{
	fuel = MAX_FUEL;
	m_movementManager->ZeroPhysics();
	setVehiclePosition(Vector2D());
	m_targetPos = Vector2D();
}




