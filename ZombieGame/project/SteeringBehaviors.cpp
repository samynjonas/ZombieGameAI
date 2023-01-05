#include "stdafx.h"
#include "SteeringBehaviors.h"

#include <cmath>

SteeringPlugin_Output Seek::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output steering{};

	steering.AutoOrient = true;
	steering.RunMode = false;

	steering.LinearVelocity = m_Target - agent->Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;

	return steering;
}

SteeringPlugin_Output SeekBackwards::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output steering{};
	steering.AutoOrient = false;
	steering.RunMode = true;

	steering.LinearVelocity = m_Target - agent->Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;

	Elite::Vector2 directionVector = (agent->LinearVelocity - agent->Position);
	directionVector.Normalize();

	const float agentRot{ agent->Orientation + 0.5f * static_cast<float>(M_PI) };
	Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };

	steering.AngularVelocity = (directionVector.Dot(agentDirection)) * agent->MaxAngularSpeed;

	return steering;
}

SteeringPlugin_Output Flee::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output steering{};

	steering.AutoOrient = true;
	steering.RunMode = true;

	Elite::Vector2 fromTarget = agent->Position - m_Target;
	float distance = fromTarget.Magnitude();

	/*if (distance > m_FleeRadius)
	{
		return steering;
	}*/

	steering.LinearVelocity = fromTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;

	return steering;
}

SteeringPlugin_Output FleeBackward::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output steering{};

	steering.AutoOrient = false;
	steering.RunMode = false;


	Elite::Vector2 fromTarget = agent->Position - m_Target;
	float distance = fromTarget.Magnitude();

	steering.LinearVelocity = fromTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;


	Elite::Vector2 directionVector = (m_Target - agent->Position);
	directionVector.Normalize();

	const float agentRot{ agent->Orientation + 0.5f * static_cast<float>(M_PI) };
	Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };

	steering.AngularVelocity = (directionVector.Dot(agentDirection)) * agent->MaxAngularSpeed;

	return steering;
}

SteeringPlugin_Output Arrive::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output steering{};

	steering.AutoOrient = true;
	steering.RunMode = false;


	steering.LinearVelocity = agent->Position - m_Target;
	steering.LinearVelocity.Normalize();

	float distance{ sqrtf(powf((abs(m_Target.x - agent->Position.x)), 2.f) + powf((abs(m_Target.y - agent->Position.y)), 2.f)) };

	steering.LinearVelocity *= agent->MaxLinearSpeed * distance / 10.f;

	return steering;
}

SteeringPlugin_Output Face::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output steering{};
	steering.AutoOrient = false;
	steering.RunMode = false;


	Elite::Vector2 directionVector = (m_Target - agent->Position);
	directionVector.Normalize();
	
	const float agentRot{ agent->Orientation + 0.5f * static_cast<float>(M_PI) };
	Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };
	
	steering.AngularVelocity = (directionVector.Dot(agentDirection)) * agent->MaxAngularSpeed;
	return steering;
}

SteeringPlugin_Output RotateLeft::calculateSteering(float deltaT, AgentInfo* agent)
{
	const Elite::Vector2 circleCenter{ agent->Position };

	static float angle{ 0 };
	angle += Elite::ToRadians(0.5f);

	//const float randAngle{ (static_cast<float>(rand() % 314)) / 10.f };

	const Elite::Vector2 newPoint{ circleCenter + Elite::Vector2{ cosf(angle), sinf(angle) } * 10 };

	m_Target = newPoint;

	SteeringPlugin_Output steering{ Face::calculateSteering(deltaT, agent) };

	return steering;
}

SteeringPlugin_Output turnAround::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	Elite::Vector2 point{ pAgent->Position + (pAgent->LinearVelocity * -1) * 4 };

	m_Target = point;

	SteeringPlugin_Output steering{ Face::calculateSteering(deltaT, pAgent)};

	return steering;
}

SteeringPlugin_Output Forward::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringPlugin_Output steering{};
	steering.AutoOrient = true;
	steering.RunMode = false;


	steering.LinearVelocity = pAgent->LinearVelocity * pAgent->MaxLinearSpeed;
	return steering;
}

SteeringPlugin_Output Wander::calculateSteering(float deltaT, AgentInfo* agent)
{
	const Elite::Vector2 circleCenter{ agent->Position + agent->LinearVelocity * m_OffsetDistance };

	static float randAngle = static_cast<float>(rand() % 360 - 180);
	float prevAngle{ randAngle };

	randAngle = prevAngle - static_cast<float>(rand() % 16 - 8);

	m_WanderAngle = Elite::ToRadians(randAngle);

	const Elite::Vector2 newPoint{ circleCenter + Elite::Vector2{ cosf(m_WanderAngle), sinf(m_WanderAngle) } * m_Radius };

	m_Target = newPoint;

	SteeringPlugin_Output steering{ Seek::calculateSteering(deltaT, agent) };

	return steering;
}