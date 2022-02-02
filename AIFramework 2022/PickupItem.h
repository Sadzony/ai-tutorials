#pragma once
#include "DrawableGameObject.h"
#include "Collidable.h"

#include <Windows.h>

enum class pickuptype
{
    Fuel,
    SpeedBoost,
    Passenger,
};

class PickupItem :
    public DrawableGameObject, public Collidable
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice, pickuptype type);
    void hasCollided() { }
    pickuptype getType() { return m_type; }

protected:

private:
    pickuptype m_type;

};

