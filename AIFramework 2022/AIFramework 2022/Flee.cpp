#include "Flee.h"
#define FLEE_RADIUS 350
void Flee::Update(float t)
{
	Vector2D curPos = m_agent->getPosition();
	MovementManager* movement = m_agent->GetMovementManager();
	movement->Flee(m_fleeTarget->getPosition(), curPos);
	m_agent->setVehiclePosition(movement->ProcessMovement(t, curPos));
	float length = (m_agent->getPosition() - m_fleeTarget->getPosition()).Length();
	if (length > FLEE_RADIUS) { //check if outside of radius. If true, then inform state manager that task complete
		completed = true;
	}
}

void Flee::Cleanup()
{
	m_agent = nullptr;
	m_fleeTarget = nullptr;
}

