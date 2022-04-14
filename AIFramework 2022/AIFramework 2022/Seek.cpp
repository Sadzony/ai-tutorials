#include "Seek.h"

void Seek::Update(float t)
{
	Vector2D target = m_agent->getTargetPos();
	Vector2D curPos = m_agent->getPosition();
	MovementManager* movement = m_agent->GetMovementManager();
	movement->Seek(m_agent->getTargetPos(), m_agent->getPosition());
	m_agent->setVehiclePosition(movement->ProcessMovement(t, curPos));
}
void Seek::Cleanup()
{
	m_agent = nullptr;
}
