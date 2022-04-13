#include "Flee.h"

void Flee::Update(Vehicle* p_agent, float t)
{
	p_agent->GetMovementManager()->Flee(p_agent->getFleePos(), p_agent->getPosition());
}
