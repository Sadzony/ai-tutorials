#pragma once
using namespace std;
//using namespace DirectX;

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
// DONT ADD TO THIS!!! CREATE ANOTHER HEADER, THIS IS DIRECTX DEPENDENT AND MAY CAUSE COMPILER ERRORS

#include <vector>
class DrawableGameObject;
class Waypoint;
class PickupItem;

using namespace std;

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};


struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexCoord;
};