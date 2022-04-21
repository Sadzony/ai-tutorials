#include "Wander.h"
#define MOVE_AMOUNT 100
#define WANDER_RADIUS 200
#define WANDER_ANGLE_CHANGE 60
#define NEW_TARGET_TIME 1.0f
#define PI 3.14
void Wander::EnterState()
{
	timePassed = NEW_TARGET_TIME;
	if (movement->GetVelocity().isZero()) {
		float x = rand() % 3 - 1;
		float y = rand() % 3 - 1;
		movement->SetVelocity(Vector2D(x, y)); //set a random direction if the velocity is 0
	}
	m_agent->setTargetPos(m_agent->getPosition());
}
void Wander::Update(float t)
{
	timePassed += t;
	Vector2D curPos = m_agent->getPosition();
	Vector2D velocityDirection = movement->GetVelocity().Normalized();
	Vector2D target = (velocityDirection * MOVE_AMOUNT) + m_agent->getPosition(); //get a position ahead of current trajectory
	if (timePassed >= NEW_TARGET_TIME) { //every x seconds...
		timePassed = 0;
		int steerDirection = rand() % 2; //find whether to change angle left or right
		if (steerDirection == 0) {
			angle -= WANDER_ANGLE_CHANGE;
			if (angle < -360) {
				angle += 360; //overflow protection
			}
		}
		else {
			angle += WANDER_ANGLE_CHANGE;
			if (angle > 360) {
				angle -= 360;
			}
		}
	}
	target.x = target.x + WANDER_RADIUS * cos(toRadians(angle)); //get a point that lays on a perimeter of a circle in front of the current trajectory
	target.y = target.y + WANDER_RADIUS * sin(toRadians(angle));
	movement->Seek(target, curPos);
	m_agent->setVehiclePosition(movement->ProcessMovement(t, curPos));

}
void Wander::Cleanup()
{
	m_agent = nullptr;
}

double Wander::toRadians(double degrees)
{
	return (degrees * PI) / 180;
}
