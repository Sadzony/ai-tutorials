#include "WaypointManager.h"
#include "Waypoint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <d3d11_1.h>
#include "CollisionHelper.h"

#define NEAR_WAYPOINT_DISTANCE 200.0f

struct waypointCoord {
	float x, y;
	bool quad;
};

WaypointManager::~WaypointManager()
{
	destroyWaypoints();
	printf("done");
}

HRESULT WaypointManager::createWaypoints(ID3D11Device* pd3dDevice)
{
	std::vector<waypointCoord> wpVec;

	HRESULT hr = S_OK;
	m_waypoints.clear();
	string filename = "Resources\\waypoints.txt"; 
	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cout << "Could not open file: " << filename << endl;
		return 1;
	}

	
	string str;
	while (getline(fin, str)) {
		// Output the text from the file
		stringstream ss;
		ss << str;
		waypointCoord wc;
		ss >> wc.x;
		ss >> wc.y;
		ss >> wc.quad;
		wpVec.push_back(wc);
	}

	fin.close();

	int checkpointID = 0;
	for (unsigned int i = 0; i < wpVec.size(); i++) {
		Waypoint* wp = new Waypoint();
			
		hr = wp->init(pd3dDevice, !wpVec[i].quad, checkpointID++);

		wp->setPosition(Vector2D(wpVec[i].x, wpVec[i].y));

		if (wp->isOnTrack())
		{
			m_waypoints.push_back(wp);
		}
		else
		{
			m_quadpoints.push_back(wp);
		}
	}

	// pathfinding
	// create the bounding boxes
	if (m_quadpoints.size() % 4 != 0)
	{
		OutputDebugStringA("Something has gone wrong with the quadpoints...");
	}

	for (unsigned int i=0; i< m_quadpoints.size(); i+= 4)
	{
		BoundingBox bb = CollisionHelper::createBoundingBoxFromPoints(m_quadpoints[i]->getPosition(), 
			m_quadpoints[i+1]->getPosition(), 
			m_quadpoints[i+2]->getPosition(), 
			m_quadpoints[i+3]->getPosition());

		m_boundingBoxes.push_back(bb);
	}


	return hr;
}

void WaypointManager::destroyWaypoints()
{
	for (Waypoint* wp : m_waypoints)
	{
		delete wp;
	}

	m_waypoints.clear();

	
	for (Waypoint* wp : m_quadpoints)
	{
		delete wp;
	}

	m_quadpoints.clear();
	
}


Waypoint* WaypointManager::getWaypoint(const unsigned int index)
{
	if (index >= m_waypoints.size())
		return nullptr;

	return m_waypoints[index];
}

Waypoint* WaypointManager::getQuadpoint(const unsigned int index)
{
	if (index >= m_quadpoints.size())
		return nullptr;

	return m_quadpoints[index];
}

Waypoint* WaypointManager::getNearestWaypoint(Vector2D position)
{
	float shortestDistance = FLT_MAX;
	Waypoint* nearestWP = nullptr;

	for (Waypoint* wp : m_waypoints)
	{
		Vector2D d = wp->getPosition() - position;
		float l = (float)d.LengthSq();
		if (l < shortestDistance)
		{
			shortestDistance = l;
			nearestWP = wp;
		}
	}

	return nearestWP;
}

vecWaypoints WaypointManager::getNeighbouringWaypoints(Waypoint* waypoint)
{
	// not very efficient, should ideally be pre-cached. 
	// should also return a pointer to a vector, not a copy
	vecWaypoints nearest;
	for (Waypoint* wp : m_waypoints)
	{
		float d = waypoint->distanceToWaypoint(wp);
		if (d < NEAR_WAYPOINT_DISTANCE)
		{
			// A line between waypoint and a nearby waypoint may cross a building (and so the car would go through the building). This makes sure this doesn't happen. 
			if (doWaypointsCrossBuilding(waypoint, wp) == false)
			{
				nearest.push_back(wp);
			}
		}
	}

	return nearest;
}

bool WaypointManager::doWaypointsCrossBuilding(Waypoint* wp1, Waypoint* wp2)
{
	if (wp1 == wp2)
		return false; 

	bool collision = false;
	for (const BoundingBox& bb : m_boundingBoxes)
	{
		collision = CollisionHelper::doesLineIntersectBoundingBox(bb, wp1->getPosition(), wp2->getPosition());
		if (collision == true)
		{
			return true;
		}
	}

	return collision;
}