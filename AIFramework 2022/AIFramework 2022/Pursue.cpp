#include "Pursue.h"

void Pursue::Update(float t)
{
	Vector2D target = m_chaseTarget->getPosition();
	Vector2D curPos = m_agent->getPosition();
	MovementManager* movement = m_agent->GetMovementManager();
	float speedCombined = (movement->GetVelocity() + m_chaseTarget->GetMovementManager()->GetVelocity()).Length();
	float length = (target - curPos).Length();
	float lookAheadValue = length / speedCombined; //a time value proportional to distance and speed of both cars
	target = m_chaseTarget->GetMovementManager()->ReturnPosAfterStep(lookAheadValue, target); //if target is moving fast or is far away, seek to a position in front of it. otherwise (slow target/distance is small) seek directly to the car
	movement->Seek(target, curPos);
	m_agent->setVehiclePosition(movement->ProcessMovement(t, curPos));
}

void Pursue::Cleanup()
{
	m_agent = nullptr;
	m_chaseTarget = nullptr;
}
