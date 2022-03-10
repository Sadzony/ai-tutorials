#include "Vehicle.h"

#define NORMAL_MAX_SPEED 300

#define ARRIVAL_RADIUS 200

#define FLEE_RADIUS 250

HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice, carColour colour, carBehaviour pbehaviour)
{
	behaviour = pbehaviour;
	m_scale = XMFLOAT3(30, 20, 1);
	m_ChaserPosition = Vector2D();
	if (colour == carColour::redCar)
	{
		setTextureName(L"Resources\\car_red.dds");
	}
	else if (colour == carColour::blueCar)
	{
		setTextureName(L"Resources\\car_blue.dds");
	}

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	m_maxSpeed = NORMAL_MAX_SPEED;
	setVehiclePosition(Vector2D(0, 0));

	m_lastPosition = Vector2D(0, 0);

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	Vector2D vecToTarget = m_targetPos - m_currentPosition;
	float distance = vecToTarget.Length();
	float distanceToChaser = (m_ChaserPosition - m_currentPosition).Length();
	if (fleeToggle && distanceToChaser < FLEE_RADIUS) {
		Flee();
	}
	if (!destinationReached) {
		if (distance < ARRIVAL_RADIUS) {
			Arrive();
		}
		else {
			Seek();
		}
		UpdateNetForce();
		UpdateAcceleration();
		Move(deltaTime, vecToTarget);
	}

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
	destinationReached = false;
	m_startPosition = m_currentPosition;
	m_targetPos = position;
}

// set the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	m_targetPos = position;
	m_startPosition = position;
	setPosition(position);
}

void Vehicle::setChaserPosition(Vector2D position)
{
	m_ChaserPosition = position;
}

void Vehicle::setWaypointManager(WaypointManager* wpm)
{
	m_waypointManager = wpm;
}

void Vehicle::SetVelocity(Vector2D pvelocity)
{
	velocity = pvelocity;
}

Vector2D Vehicle::GetVelocity()
{
	return velocity;
}

Vector2D Vehicle::GetAcceleration()
{
	return acceleration;
}

void Vehicle::SetAcceleration(Vector2D pacceleration)
{
	acceleration = pacceleration;
}

void Vehicle::AddForce(Vector2D force)
{
	externalForces.push_back(force);
}

Vector2D Vehicle::GetNetForce()
{
	return netForce;
}

void Vehicle::ZeroPhysics()
{
	acceleration.Zero();
	velocity.Zero();
	netForce.Zero();
	externalForces.clear();
}

void Vehicle::UpdateAcceleration()
{
	acceleration = netForce;
	netForce.Zero();
}

void Vehicle::UpdateNetForce()
{
	for (int i = 0; i < externalForces.size(); i++)
	{
		netForce += externalForces.at(i);
	}
	externalForces.clear();
}

void Vehicle::Move(float t, Vector2D direction)
{
	float distanceToTarget = (float)direction.Length();

	if (distanceToTarget > 5) { //5 pixels of error allowed
		Vector2D nextStep = velocity * t + (0.5 * acceleration * t * t);
		velocity += acceleration * t;
		velocity.Truncate(m_maxSpeed);
		nextStep.Truncate(distanceToTarget); 	// if the distance to the end point is less than the car would move, then only move that distance. 
		m_currentPosition += nextStep;
	}
	else if (!destinationReached) {
		destinationReached = true;
		//ZeroPhysics();
	}
}

void Vehicle::Seek()
{
	Vector2D seekingForce;
	Vector2D direction = m_targetPos - m_currentPosition;
	seekingForce = direction.Normalized() * m_maxSpeed;
	seekingForce -= velocity;
	AddForce(seekingForce);
}

void Vehicle::Arrive()
{
	Vector2D arrivalForce;
	Vector2D direction = m_targetPos - m_currentPosition;
	float distance = direction.Length();
	arrivalForce = direction.Normalized() * m_maxSpeed * (distance/ARRIVAL_RADIUS); //force gets smaller with distance
	arrivalForce -= velocity;
	AddForce(arrivalForce);
}

void Vehicle::Flee()
{
	Vector2D fleeingForce;
	Vector2D direction = m_currentPosition - m_ChaserPosition;
	fleeingForce = direction.Normalized() * m_maxSpeed;
	fleeingForce -= velocity;
	AddForce(fleeingForce);
}


