#pragma once

#include "WaypointManager.h"
#include "Vehicle.h"
#include "StateManager.h"
#include "StateList.h"

using namespace std;

class PickupItem;
typedef vector<PickupItem*> vecPickups;

class AIManager
{
public:
	AIManager();
	virtual  ~AIManager();
	void	release();
	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyDown(WPARAM param);
	void	keyUp(WPARAM param);

protected:
	bool	checkForCollisions();
	void	setRandomPickupPosition(PickupItem* pickup);

private:
	vecPickups              m_pickups;
	Vehicle*				m_pCarBlue = nullptr;
	StateManager*			m_redCarStateMachine;
	StateManager*			m_blueCarStateMachine;
	Vehicle*				m_pCarRed = nullptr;
	WaypointManager			m_waypointManager;

};

