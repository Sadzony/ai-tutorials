#include "Waypoint.h"

HRESULT	Waypoint::init(ID3D11Device* pd3dDevice, bool isOnTrack, bool checkPointID)
{
	m_checkpointID = checkPointID;
	m_isOnTrack = isOnTrack;

	float s = 10;
	m_scale = XMFLOAT3(s, s, s);

	if (m_isOnTrack)
	{
		setTextureName(L"Resources\\blue.dds");
	}
	else {
		setTextureName(L"Resources\\red.dds");
	}

	// create the visible mesh to draw
	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	return hr;
}

float Waypoint::distanceToWaypoint(Waypoint* waypoint)
{
	if (waypoint == nullptr)
		return FLT_MAX;

	return (float)waypoint->getPosition().Distance(getPosition());
}