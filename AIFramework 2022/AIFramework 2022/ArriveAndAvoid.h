#pragma once
#include "State.h"
class ArriveAndAvoid :
    public State
{
public:
    ArriveAndAvoid(Vehicle* p_agent) { m_agent = p_agent; movement = m_agent->GetMovementManager(); SetFlag(StateFlag::ArriveAndAvoid); }
    virtual void Update(float t) override;
    virtual void Cleanup() override;
    void AddObstacle(ObstacleData* p_obstacle);
protected:
    vector<ObstacleData*> obstacles;
    Vehicle* m_agent;
    MovementManager* movement;

    bool intersects(ObstacleData* obstacle, float width, float length);

};

