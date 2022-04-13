#include "ArriveToTarget.h"

void ArriveToTarget::Update(Vehicle* p_agent, float t)
{
	Vector2D target = p_agent->getTargetPos();
	Vector2D curPos = p_agent->getPosition();
	MovementManager* movement = p_agent->GetMovementManager();
	float length = (target - curPos).Length();
	if (length > 5) { //5 pixels of error allowed
		movement->SeekWithArrive(p_agent->getTargetPos(), p_agent->getPosition());
	}
	else {
		completed = true;
		movement->ZeroPhysics();
	}
	p_agent->setVehiclePosition(movement->ProcessMovementWithTrunc(t, curPos, length));
}
