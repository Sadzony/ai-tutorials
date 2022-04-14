#include "Wander.h"

void Wander::Update(float t)
{
	Vector2D target = m_agent->getTargetPos();
	Vector2D curPos = m_agent->getPosition();
	MovementManager* movement = m_agent->GetMovementManager();
}
void Wander::Cleanup()
{
	m_agent = nullptr;
}
