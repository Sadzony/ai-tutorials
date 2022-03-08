#include "PickupItem.h"

HRESULT	PickupItem::initMesh(ID3D11Device* pd3dDevice, pickuptype type)
{
	m_type = type;

	m_scale = XMFLOAT3(20, 20, 1);

	if(m_type == pickuptype::Fuel)
		setTextureName(L"Resources\\pickup_fuel.dds");
	else if (m_type == pickuptype::Passenger)
		setTextureName(L"Resources\\pickup_passenger.dds");
	else
		setTextureName(L"Resources\\pickup_speedboost.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	setPosition(Vector2D(0, 0));

	return hr;
}