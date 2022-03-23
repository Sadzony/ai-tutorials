#include "Vehicle.h"

#define NORMAL_MAX_SPEED 300

Vehicle::~Vehicle()
{
	delete m_movementManager;
}

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice, carColour colour, carBehaviour pbehaviour)
{
	behaviour = pbehaviour;
	m_scale = XMFLOAT3(30, 20, 1);
	m_chaserPos = Vector2D();
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

	m_movementManager = new MovementManager(Vector2D(0,0), NORMAL_MAX_SPEED);

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	if (!fleeToggle) {
		m_movementManager->Update(deltaTime);
	}
	else {
		m_movementManager->Update(deltaTime, m_chaserPos);
	}
	m_currentPosition = m_movementManager->GetPhysicsPosition();

	// rotate the object based on its last & current position
	Vector2D diff = m_currentPosition - m_lastPosition;
	if (diff.Length() > 0) { // if zero then don't update rotation
		diff.Normalize();
		m_radianRotation = atan2f((float)diff.y, (float)diff.x); // this is used by DrawableGameObject to set the rotation
	}
	m_lastPosition = m_currentPosition;

	// set the current poistion for the drawablegameobject
	setPosition(Vector2D(m_currentPosition));

	DrawableGameObject::update(deltaTime);
}

// set a position to move to
void Vehicle::setTargetPosition(Vector2D position)
{
	m_movementManager->setTargetPos(position);

}

// set the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_movementManager->setPhysicsPosition(position);
	m_currentPosition = position;
	setPosition(position);
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}




