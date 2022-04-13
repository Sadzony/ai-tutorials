#include "Chase.h"

void Chase::Update(Vehicle* p_agent, float t)
{
	p_agent->GetMovementManager()->Seek(p_agent->getTargetPos(), p_agent->getPosition());
}
