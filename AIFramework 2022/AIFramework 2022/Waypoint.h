#pragma once
#include "DrawableGameObject.h"
class Waypoint :
    public DrawableGameObject
{
public:
	
    virtual HRESULT init(ID3D11Device* pd3dDevice, bool isOnTrack, bool checkPointID);

    bool    isOnTrack()     { return m_isOnTrack; }
    int     getCheckpointID() { return m_checkpointID; }
	float   distanceToWaypoint(Waypoint* waypoint);
    

private:
    bool m_isOnTrack; // is the waypoint a 'track' (path) waypoint. If it isn't, it is used to define the corners of buildings (and shouldn't be used for path finding)
    int  m_checkpointID;
};

