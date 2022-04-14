#include "Follow.h"
void Follow::Update(float t)
{
	XMFLOAT4 targetPosFloat4 = XMFLOAT4(-2, 0, 0, 0); //target position will be 2 units behind the target car
	XMVECTOR targetXMVec = XMVector3Transform(XMLoadFloat4(&targetPosFloat4), XMLoadFloat4x4(m_chaseTarget->getTransform())); //multiply the target vector by the car's transform to get a position in world space
	XMStoreFloat4(&targetPosFloat4, targetXMVec);
	Vector2D target = Vector2D(targetPosFloat4.x, targetPosFloat4.y);
	Vector2D curPos = m_agent->getPosition();
	MovementManager* movement = m_agent->GetMovementManager();
	float length = (target - curPos).Length();
	if (length > 5) { //5 pixels of error allowed
		movement->SeekWithArrive(target, curPos);
		m_agent->setVehiclePosition(movement->ProcessMovement(t, curPos));
	}
	else {
		movement->ZeroPhysics();
	}
}

void Follow::Cleanup()
{
	m_agent = nullptr;
	m_chaseTarget = nullptr;
}
