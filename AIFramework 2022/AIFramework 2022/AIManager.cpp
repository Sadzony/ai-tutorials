#include "AIManager.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"
#include "main.h"
#include "constants.h"


AIManager::AIManager()
{
	m_pCarBlue = nullptr;
    m_pCarRed = nullptr;
}

AIManager::~AIManager()
{
	release();
}

void AIManager::release()
{
	clearDrawList();

	for (PickupItem* pu : m_pickups)
	{
		delete pu;
	}
	m_pickups.clear();

	delete m_pCarBlue;
    delete m_redCarStateMachine;
    m_redCarStateMachine = nullptr;
	m_pCarBlue = nullptr;

    delete m_pCarRed;
    delete m_blueCarStateMachine;
    m_blueCarStateMachine = nullptr;
    m_pCarRed = nullptr;

    

}

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
    srand(time(NULL)); //generate seed

    // create the vehicle s
    float xPos = -500; // an abtrirary start point
    float yPos = 300;

    m_pCarBlue = new Vehicle();
    HRESULT hr = m_pCarBlue->initMesh(pd3dDevice, carColour::blueCar);
    m_pCarBlue->setVehiclePosition(Vector2D(xPos, yPos));
    m_blueCarStateMachine = new StateManager(m_pCarBlue);
    if (FAILED(hr))
        return hr;

    xPos = 500; // an abtrirary start point
    yPos = 300;
    m_pCarRed = new Vehicle();
    hr = m_pCarRed->initMesh(pd3dDevice, carColour::redCar);
    m_pCarRed->setVehiclePosition(Vector2D(xPos, yPos));
    m_redCarStateMachine = new StateManager(m_pCarRed);
    if (FAILED(hr))
        return hr;

    // setup the waypoints
    m_waypointManager.createWaypoints(pd3dDevice);
    m_pCarBlue->setWaypointManager(&m_waypointManager);
    m_pCarRed->setWaypointManager(&m_waypointManager);

    // create a passenger pickup item
    PickupItem* pPickupPassenger = new PickupItem();
    hr = pPickupPassenger->initMesh(pd3dDevice, pickuptype::Passenger);
    m_pickups.push_back(pPickupPassenger);

    // NOTE!! for fuel and speedboost - you will need to create these here yourself

    // (needs to be done after waypoint setup)
    setRandomPickupPosition(pPickupPassenger);

    return hr;
}


void AIManager::update(const float fDeltaTime)
{
    for (unsigned int i = 0; i < m_waypointManager.getWaypointCount(); i++) {
        m_waypointManager.getWaypoint(i)->update(fDeltaTime);
        //AddItemToDrawList(m_waypointManager.getWaypoint(i)); // if you uncomment this, it will display the waypoints
    }

    for (int i = 0; i < m_waypointManager.getQuadpointCount(); i++)
    {
        Waypoint* qp = m_waypointManager.getQuadpoint(i);
        qp->update(fDeltaTime);
        //AddItemToDrawList(qp); // if you uncomment this, it will display the quad waypoints
    }

    // update and display the pickups
    for (unsigned int i = 0; i < m_pickups.size(); i++) {
        m_pickups[i]->update(fDeltaTime);
        AddItemToDrawList(m_pickups[i]);
    }

	// draw the waypoints nearest to the car
	
    Waypoint* wp = m_waypointManager.getNearestWaypoint(m_pCarBlue->getPosition());
	if (wp != nullptr)
	{
		vecWaypoints* vwps = m_waypointManager.getNeighbouringWaypoints(wp);
		for (Waypoint* wp : *vwps)
		{
			AddItemToDrawList(wp);
		}
	}
    

    // update and draw the car (and check for pickup collisions)
	if (m_pCarBlue != nullptr)
	{
        m_pCarBlue->setFleePos(m_pCarRed->getPosition());
        m_blueCarStateMachine->Update(fDeltaTime);
		m_pCarBlue->update(fDeltaTime);
		checkForCollisions();
		AddItemToDrawList(m_pCarBlue);
	}
    if (m_pCarRed != nullptr)
    {
        //if (m_pCarBlue->GetMovementManager()->GetDestinationReached()) { //if destination reached, generate a new destination
        //    int wpX = rand() % 768 - 384;
        //    int wpY = rand() % 1024 - 512;
        //    Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(wpX, wpY));
        //    m_pCarBlue->setTargetPosition(wp->getPosition());
        //}
        m_redCarStateMachine->Update(fDeltaTime);
        m_pCarRed->update(fDeltaTime);
        AddItemToDrawList(m_pCarRed);
    }
}

void AIManager::mouseUp(int x, int y)
{
	// get a waypoint near the mouse click, then set the car to move to the this waypoint
	Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
	if (wp == nullptr)
		return;

    m_pCarBlue->setTargetPos(wp->getPosition());
    m_blueCarStateMachine->ChangeState(new ArriveToTarget());
}

void AIManager::keyUp(WPARAM param)
{
    const WPARAM key_a = 65;
    switch (param)
    {
        case key_a:
        {
            OutputDebugStringA("a Up \n");
            break;
        }
    }
}

void AIManager::keyDown(WPARAM param)
{
	// hint 65-90 are a-z
	const WPARAM key_a = 65;
	const WPARAM key_s = 83;
    const WPARAM key_t = 84;

    switch (param)
    {
        case VK_NUMPAD0:
        {
            OutputDebugStringA("0 pressed \n");
            break;
        }
        case VK_NUMPAD1:
        {
            OutputDebugStringA("1 pressed \n");
            break;
        }
        case VK_NUMPAD2:
        {
            OutputDebugStringA("2 pressed \n");
            break;
        }
        case key_a:
        {

            break;
        }
		case key_s:
		{

			break;
		}
        case 0x46: //f key

            break;
        case key_t:
		{
            break;
        }
        case VK_SPACE:

            break;
        // etc
        default:
            break;
    }
}

void AIManager::setRandomPickupPosition(PickupItem* pickup)
{
    if (pickup == nullptr)
        return;

    int x = (rand() % SCREEN_WIDTH) - (SCREEN_WIDTH / 2);
    int y = (rand() % SCREEN_HEIGHT) - (SCREEN_HEIGHT / 2);

    Waypoint* wp = m_waypointManager.getNearestWaypoint(Vector2D(x, y));
    if (wp) {
        pickup->setPosition(wp->getPosition());
    }
}

/*
// IMPORTANT
// hello. This is hopefully the only time you are exposed to directx code 
// you shouldn't need to edit this, but marked in the code below is a section where you may need to add code to handle pickup collisions (speed boost / fuel)
// the code below does the following:
// gets the *first* pickup item "m_pickups[0]"
// does a collision test with it and the car
// creates a new random pickup position for that pickup

// the relevant #includes are already in place, but if you create your own collision class (or use this code anywhere else) 
// make sure you have the following:
#include <d3d11_1.h> // this has the appropriate directx structures / objects
#include <DirectXCollision.h> // this is the dx collision class helper
using namespace DirectX; // this means you don't need to put DirectX:: in front of objects like XMVECTOR and so on. 
*/

bool AIManager::checkForCollisions()
{
    if (m_pickups.size() == 0)
        return false;

    XMVECTOR dummy;

    // get the position and scale of the car and store in dx friendly xmvectors
    XMVECTOR carPos;
    XMVECTOR carScale;
    XMMatrixDecompose(
        &carScale,
        &dummy,
        &carPos,
        XMLoadFloat4x4(m_pCarBlue->getTransform())
    );

    // create a bounding sphere for the car
    XMFLOAT3 scale;
    XMStoreFloat3(&scale, carScale);
    BoundingSphere boundingSphereCar;
    XMStoreFloat3(&boundingSphereCar.Center, carPos);
    boundingSphereCar.Radius = scale.x;

    // do the same for a pickup item
    // a pickup - !! NOTE it is only referring the first one in the list !!
    // to get the passenger, fuel or speedboost specifically you will need to iterate the pickups and test their type (getType()) - see the pickup class
    XMVECTOR puPos;
    XMVECTOR puScale;
    XMMatrixDecompose(
        &puScale,
        &dummy,
        &puPos,
        XMLoadFloat4x4(m_pickups[0]->getTransform())
    );

    // bounding sphere for pickup item
    XMStoreFloat3(&scale, puScale);
    BoundingSphere boundingSpherePU;
    XMStoreFloat3(&boundingSpherePU.Center, puPos);
    boundingSpherePU.Radius = scale.x;

	// THIS IS generally where you enter code to test each type of pickup
    // does the car bounding sphere collide with the pickup bounding sphere?
    if (boundingSphereCar.Intersects(boundingSpherePU))
    {
        OutputDebugStringA("A collision with pick up has occurred!\n");
        m_pickups[0]->hasCollided();
        setRandomPickupPosition(m_pickups[0]);

        // you will need to test the type of the pickup to decide on the behaviour
        // m_pCarRed->dosomething(); ...

        return true;
    }

    return false;
}





