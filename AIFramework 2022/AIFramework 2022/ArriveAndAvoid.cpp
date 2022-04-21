#include "ArriveAndAvoid.h"
#define LOOK_AHEAD_MULTIPLIER 1.0f
#define AVOID_FORCE_POWER 100;
void ArriveAndAvoid::Update(float t)
{
	Vector2D currentPos = m_agent->getPosition();
	Vector2D targetPos = m_agent->getTargetPos();
	float detectionLength = movement->GetVelocity().Length() * LOOK_AHEAD_MULTIPLIER;
    for (int i = 0; i < obstacles.size(); i++)
    {
        if (intersects(obstacles.at(i), m_agent->GetObstacleData()->radius, detectionLength))  //if the obstacle is detected in front of the agent
        {
            Vector2D obstacleDirectionVec = *(obstacles.at(i)->position) - currentPos;
            int forceDirection = movement->GetVelocity().Sign(obstacleDirectionVec);
            Vector2D avoidanceForce = movement->GetVelocity().Perp().Normalized() * AVOID_FORCE_POWER; //find the vector perpendicular to current trajectory. Scale it by power

            movement->AddForce(avoidanceForce * forceDirection);
        }
    }
    float length = (targetPos - currentPos).Length();
    if (length > 5) { //5 pixels of error allowed
        movement->SeekWithArrive(targetPos, currentPos);
        m_agent->setVehiclePosition(movement->ProcessMovementWithTrunc(t, currentPos, length));
    }
    else {
        movement->ZeroPhysics();
    }
}

void ArriveAndAvoid::Cleanup()
{
	obstacles.clear();
	movement = nullptr;
	m_agent = nullptr;
}

void ArriveAndAvoid::AddObstacle(ObstacleData* p_obstacle)
{
    if (std::find(obstacles.begin(), obstacles.end(), p_obstacle) == obstacles.end()) //only add a new element if it's not contained in the obstacles vector
    {
        obstacles.push_back(p_obstacle);
    }
}

bool ArriveAndAvoid::intersects(ObstacleData* obstacle, float width, float length)
{
    // get the position and rotation of the car and store in dx friendly xmvectors
    XMVECTOR dummy;
    XMVECTOR boxPos;
    XMVECTOR agentRotation;
    XMMatrixDecompose(
        &dummy,
        &agentRotation,
        &boxPos,
        XMLoadFloat4x4(m_agent->getTransform())
    );
    // create a bounding oriented box for the car
    XMFLOAT3 offset = XMFLOAT3(length, 0, 0);
    XMVECTOR offsetVector = XMVector3Rotate(XMLoadFloat3(&offset), agentRotation); //rotate the offset
    boxPos = XMVectorAdd(boxPos, offsetVector); //find the position of the box where it starts at the centre of the agent
    BoundingOrientedBox agentBoundingBox;
    XMStoreFloat3(&agentBoundingBox.Center, boxPos);
    XMStoreFloat4(&agentBoundingBox.Orientation, agentRotation);
    agentBoundingBox.Extents = XMFLOAT3(length, width, 1.0f);


    // create a bounding sphere for the obstacle
    XMFLOAT3 obstaclePosFloat3 = XMFLOAT3(obstacle->position->x, obstacle->position->y, 0.0f);
    BoundingSphere boundingSphereObstacle;
    boundingSphereObstacle.Center = obstaclePosFloat3;
    boundingSphereObstacle.Radius = obstacle->radius;

    return agentBoundingBox.Intersects(boundingSphereObstacle);
}
