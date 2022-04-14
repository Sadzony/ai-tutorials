#include "ArriveToTarget.h"

void ArriveToTarget::Update(float t)
{
	Vector2D target = m_agent->getTargetPos();
	Vector2D curPos = m_agent->getPosition();
	MovementManager* movement = m_agent->GetMovementManager();
	float length = (target - curPos).Length();
	if (length > 5) { //5 pixels of error allowed
		movement->SeekWithArrive(m_agent->getTargetPos(), m_agent->getPosition());
		m_agent->setVehiclePosition(movement->ProcessMovementWithTrunc(t, curPos, length));
	}
	else {
		movement->ZeroPhysics();
	}
		
}
void ArriveToTarget::Cleanup()
{
	m_agent = nullptr;
}