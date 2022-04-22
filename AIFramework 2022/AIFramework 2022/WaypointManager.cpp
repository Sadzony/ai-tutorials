#include "WaypointManager.h"
#include "Waypoint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <d3d11_1.h>
#include "CollisionHelper.h"
#include <limits>

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

	//calculate all connections between waypoints
	for (unsigned int i = 0; i < m_waypoints.size(); i++) {
		calculateNeighbouringWaypoints(m_waypoints[i]);
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

std::deque<Waypoint*> WaypointManager::GeneratePathQueue(Waypoint* startNode, Waypoint* targetNode)
{	
	//create frontier, came_From and return queue
	std::deque<Waypoint*> frontier;
	std::unordered_map<Waypoint*, Waypoint*> came_from;
	std::deque<Waypoint*> returnQueue;
	//add start node
	frontier.push_back(startNode);
	returnQueue.push_back(startNode);

	bool found = false;
	while (!frontier.empty())
	{
		Waypoint* current = frontier.front();
		frontier.pop_front();
		if (current == targetNode)
		{
			found = true;
			break; //break if goal found
		}
		vecWaypoints* neighbours = current->getNeighbours();
		for (Waypoint* wp : *neighbours)
		{
			if (came_from.find(wp) == came_from.end()) { //if new neighbour doesnt exist in dictionary
				frontier.push_back(wp);
				came_from.insert_or_assign(wp, current);
			}
		}
	}
	if (found) 
	{
		std::stack<Waypoint*> tempStack;
		Waypoint* addedNode = targetNode;
		while (addedNode != startNode) 
		{
			tempStack.push(addedNode);
			addedNode = came_from.at(addedNode);
		}
		while (!tempStack.empty()) {
			Waypoint* stackNode = tempStack.top();
			tempStack.pop();
			returnQueue.push_back(stackNode);
		}
		return returnQueue;
	}
	else
	{
		Debug::LogString("Path to target was not found.");
		return std::deque<Waypoint*>();
	}
}

std::deque<Waypoint*> WaypointManager::GeneratePathQueueDijkstra(Waypoint* startNode, Waypoint* targetNode)
{	
	//create frontier, came_From and return queue
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;
	std::unordered_map<Waypoint*, Waypoint*> came_from;
	std::unordered_map<Waypoint*, double> cost_to_node;
	std::deque<Waypoint*> returnQueue;
	//add start node
	frontier.push(std::make_pair(0, startNode));
	returnQueue.push_back(startNode);

	bool found = false;

	while (!frontier.empty()) 
	{
		Node currentNode = frontier.top();
		frontier.pop();
		if (currentNode.second == targetNode)
		{
			found = true;
			break; //break if goal found
		}
		vecWaypoints* neighbours = currentNode.second->getNeighbours();
		for (Waypoint* neighbour : *neighbours)
		{
			for (Waypoint* neighbour : *neighbours) {
				double costOfEdge = (neighbour->getPosition() - currentNode.second->getPosition()).Length();
				double newCost;
				if (cost_to_node.find(currentNode.second) != cost_to_node.end())
				{
					newCost = cost_to_node.at(currentNode.second) + costOfEdge;
				}
				else
				{
					newCost = costOfEdge;
				}
				double neighbourDictionaryCost;
				if (cost_to_node.find(neighbour) != cost_to_node.end())
				{
					neighbourDictionaryCost = cost_to_node.at(neighbour);
				}
				else
				{
					neighbourDictionaryCost = (std::numeric_limits<double>::max)();
				}

				if (came_from.find(neighbour) == came_from.end() || newCost < neighbourDictionaryCost) //if neighbour doesnt exist in dictionary or new cost is smaller than neighbours cost
				{
					cost_to_node.insert_or_assign(neighbour, newCost);
					came_from.insert_or_assign(neighbour, currentNode.second);
					frontier.push(make_pair(newCost, neighbour));
				}
			}
		}

	}


	if (found)
	{
		std::stack<Waypoint*> tempStack;
		Waypoint* addedNode = targetNode;
		while (addedNode != startNode)
		{
			tempStack.push(addedNode);
			addedNode = came_from.at(addedNode);
		}
		while (!tempStack.empty()) {
			Waypoint* stackNode = tempStack.top();
			tempStack.pop();
			returnQueue.push_back(stackNode);
		}
		return returnQueue;
	}
	else
	{
		Debug::LogString("Path to target was not found.");
		return std::deque<Waypoint*>();
	}
}

std::deque<Waypoint*> WaypointManager::GeneratePathQueueAStar(Waypoint* startNode, Waypoint* targetNode)
{
	//create frontier, came_From and return queue
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;
	std::unordered_map<Waypoint*, Waypoint*> came_from;
	std::unordered_map<Waypoint*, double> cost_to_node;
	std::deque<Waypoint*> returnQueue;
	//add start node
	frontier.push(std::make_pair(0, startNode));
	returnQueue.push_back(startNode);

	bool found = false;

	while (!frontier.empty())
	{
		Node currentNode = frontier.top();
		frontier.pop();
		if (currentNode.second == targetNode)
		{
			found = true;
			break; //break if goal found
		}
		vecWaypoints* neighbours = currentNode.second->getNeighbours();
		for (Waypoint* neighbour : *neighbours)
		{
			for (Waypoint* neighbour : *neighbours) {
				double costOfEdge = (neighbour->getPosition() - currentNode.second->getPosition()).Length();
				double newCost;
				if (cost_to_node.find(currentNode.second) != cost_to_node.end())
				{
					newCost = cost_to_node.at(currentNode.second) + costOfEdge;
				}
				else
				{
					newCost = costOfEdge;
				}
				double neighbourDictionaryCost;
				if (cost_to_node.find(neighbour) != cost_to_node.end()) //if neighbour exists in dictionary
				{
					neighbourDictionaryCost = cost_to_node.at(neighbour);
				}
				else
				{
					neighbourDictionaryCost = (std::numeric_limits<double>::max)();
				}

				if (came_from.find(neighbour) == came_from.end() || newCost < neighbourDictionaryCost) //if neighbour doesnt exist in dictionary or new cost is smaller than neighbours cost
				{
					cost_to_node.insert_or_assign(neighbour, newCost);
					came_from.insert_or_assign(neighbour, currentNode.second);
					double priority = (targetNode->getPosition() - neighbour->getPosition()).ManhattanLength();
					frontier.push(make_pair(priority, neighbour));
				}
			}
		}

	}


	if (found)
	{
		std::stack<Waypoint*> tempStack;
		Waypoint* addedNode = targetNode;
		while (addedNode != startNode)
		{
			tempStack.push(addedNode);
			addedNode = came_from.at(addedNode);
		}
		while (!tempStack.empty()) {
			Waypoint* stackNode = tempStack.top();
			tempStack.pop();
			returnQueue.push_back(stackNode);
		}
		return returnQueue;
	}
	else
	{
		Debug::LogString("Path to target was not found.");
		return std::deque<Waypoint*>();
	}
}

vecWaypoints* WaypointManager::calculateNeighbouringWaypoints(Waypoint* waypoint)
{
	//gets the pointer to the neighbours vector stored in a waypoint. Recalculates the neighbours. returns the pointer
	vecWaypoints* neighbours = waypoint->getNeighbours();
	neighbours->clear();
	for (Waypoint* wp : m_waypoints)
	{
		float d = waypoint->distanceToWaypoint(wp);
		if (d < NEAR_WAYPOINT_DISTANCE)
		{
			// A line between waypoint and a nearby waypoint may cross a building (and so the car would go through the building). This makes sure this doesn't happen. 
			if (doWaypointsCrossBuilding(waypoint, wp) == false)
			{
				neighbours->push_back(wp);
			}
		}
	}

	return neighbours;
}

vecWaypoints* WaypointManager::getNeighbouringWaypoints(Waypoint* waypoint)
{
	vecWaypoints* neighbours = waypoint->getNeighbours();
	return neighbours;
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