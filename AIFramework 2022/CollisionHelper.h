#pragma once
#include <d3d11_1.h> // this has the appropriate directx structures / objects
#include <DirectXCollision.h> // this is the dx collision class helper
using namespace DirectX; // this means you don't need to put DirectX:: in front of objects like XMVECTOR and so on. 
#include "Vector2D.h"
#include <vector>

class CollisionHelper
{
public:
	static BoundingBox createBoundingBoxFromPoints(Vector2D p1, Vector2D p2, Vector2D p3, Vector2D p4)
	{
		float minx = FLT_MAX; float miny = FLT_MAX;
		float maxx = -FLT_MAX; float maxy = -FLT_MAX;

		std::vector<Vector2D> vecPoints;
		vecPoints.push_back(p1);
		vecPoints.push_back(p2);
		vecPoints.push_back(p3);
		vecPoints.push_back(p4);

		for (Vector2D pt : vecPoints)
		{
			if (minx > (float)pt.x)
				minx = (float)pt.x;
			if (miny > (float)pt.y)
				miny = (float)pt.y;
			if (maxx < (float)pt.x)
				maxx = (float)pt.x;
			if (maxy < (float)pt.y)
				maxy = (float)pt.y;
		}

		XMFLOAT3 min3(minx, miny, -1);
		XMFLOAT3 max3(maxx, maxy, 1);
		XMVECTOR min = XMLoadFloat3(&min3);
		XMVECTOR max = XMLoadFloat3(&max3);
		BoundingBox bb;
		bb.CreateFromPoints(bb, min, max);

		return bb;
	}

	static bool doesLineIntersectBoundingBox(BoundingBox bb, Vector2D lineStart, Vector2D lineEnd)
	{
		XMFLOAT3 o((float)lineStart.x, (float)lineStart.y, 0);
		XMFLOAT3 o2((float)lineEnd.x, (float)lineEnd.y, 0);
		XMFLOAT3 o3((float)lineEnd.x, (float)lineEnd.y, 0.1);

		// triangle
		XMVECTOR v1 = XMLoadFloat3(&o);
		XMVECTOR v2 = XMLoadFloat3(&o2);
		XMVECTOR v3 = XMLoadFloat3(&o3);


		bool collision = false;
		collision = bb.Intersects(v1, v2, v3);
		return collision;
	}
};

