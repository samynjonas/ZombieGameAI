#include "stdafx.h"
#include "SteeringBehaviors.h"

#include <cmath>

SteeringPlugin_Output_Expanded Seek::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};

	steering.AutoOrient = true;

	steering.LinearVelocity = m_Target - agent->Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;

	return steering;
}

SteeringPlugin_Output_Expanded SeekAndRotate::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};
	steering.AutoOrient = false;

	steering.LinearVelocity = m_Target - agent->Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;

	const Elite::Vector2 circleCenter{ agent->Position };

	float direction{ 1 };
	if (agent->AngularVelocity < 0)
	{
		direction *= -1;
	}
	
	static float angle{ 0 };
	angle += Elite::ToRadians(0.5f);

	const Elite::Vector2 newPoint{ circleCenter + Elite::Vector2{ cosf(angle), sinf(angle) } * 10 };

	Elite::Vector2 directionVector = (newPoint - agent->Position);
	directionVector.Normalize();



	const float agentRot{ agent->Orientation + 0.5f * static_cast<float>(M_PI) };
	Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };
		steering.AngularVelocity = (directionVector.Dot(agentDirection)) * (agent->MaxAngularSpeed * m_RotateSpeed);


	return steering;
}

SteeringPlugin_Output_Expanded SeekBackwards::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};
	steering.AutoOrient = false;

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

SteeringPlugin_Output_Expanded Flee::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};

	steering.AutoOrient = true;

	Elite::Vector2 fromTarget = agent->Position - m_Target;
	float distance = fromTarget.Magnitude();

	steering.LinearVelocity = fromTarget;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= agent->MaxLinearSpeed;

	return steering;
}
SteeringPlugin_Output_Expanded FleeBackward::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};

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
SteeringPlugin_Output_Expanded Arrive::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};
	steering.AutoOrient = true;
	steering.RunMode = false;


	steering.AutoOrient = true;

	steering.LinearVelocity = m_Target - agent->Position;
	steering.LinearVelocity.Normalize();

	float distance{ sqrtf(powf((abs(m_Target.x - agent->Position.x)), 2.f) + powf((abs(m_Target.y - agent->Position.y)), 2.f)) };

	steering.LinearVelocity *= agent->MaxLinearSpeed * distance / 5.f;

	return steering;
}
SteeringPlugin_Output_Expanded Face::calculateSteering(float deltaT, AgentInfo* agent)
{
	SteeringPlugin_Output_Expanded steering{};
	steering.AutoOrient = false;
	steering.RunMode = false;

	Elite::Vector2 directionVector = (m_Target - agent->Position);
	directionVector.Normalize();
	
	const float agentRot{ agent->Orientation + 0.5f * static_cast<float>(M_PI) };
	Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };
	
	steering.AngularVelocity = (directionVector.Dot(agentDirection)) * agent->MaxAngularSpeed;
	return steering;
}

SteeringPlugin_Output_Expanded RotateLeft::calculateSteering(float deltaT, AgentInfo* agent)
{
	const Elite::Vector2 circleCenter{ agent->Position };

	static float angle{ 0 };
	angle += Elite::ToRadians(0.5f);

	const Elite::Vector2 newPoint{ circleCenter + Elite::Vector2{ cosf(angle), sinf(angle) } * 10 };

	SteeringPlugin_Output_Expanded steering{};
	steering.AutoOrient = false;

	Elite::Vector2 directionVector = (newPoint - agent->Position);
	directionVector.Normalize();

	const float agentRot{ agent->Orientation + 0.5f * static_cast<float>(M_PI) };
	Elite::Vector2 agentDirection{ std::cosf(agentRot),std::sinf(agentRot) };

	steering.AngularVelocity = (directionVector.Dot(agentDirection)) * agent->MaxAngularSpeed;

	return steering;
}

SteeringPlugin_Output_Expanded RotateAndWalkbackwards::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringPlugin_Output_Expanded steering{ RotateLeft::calculateSteering(deltaT, pAgent) };

	steering.LinearVelocity = pAgent->LinearVelocity * pAgent->MaxLinearSpeed * -1;

	return steering;
}


SteeringPlugin_Output_Expanded turnAround::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	Elite::Vector2 point{ pAgent->Position + (pAgent->LinearVelocity * -1) * 4 };

	m_Target = point;

	SteeringPlugin_Output_Expanded steering{ Face::calculateSteering(deltaT, pAgent)};

	return steering;
}

SteeringPlugin_Output_Expanded Forward::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringPlugin_Output_Expanded steering{};
	steering.AutoOrient = true;
	steering.RunMode = false;


	steering.LinearVelocity = pAgent->LinearVelocity * pAgent->MaxLinearSpeed;
	return steering;
}

SteeringPlugin_Output_Expanded Wander::calculateSteering(float deltaT, AgentInfo* agent)
{
	const Elite::Vector2 circleCenter{ agent->Position + agent->LinearVelocity * m_OffsetDistance };

	static float randAngle = static_cast<float>(rand() % 360 - 180);
	float prevAngle{ randAngle };

	randAngle = prevAngle - static_cast<float>(rand() % 16 - 8);

	m_WanderAngle = Elite::ToRadians(randAngle);

	const Elite::Vector2 newPoint{ circleCenter + Elite::Vector2{ cosf(m_WanderAngle), sinf(m_WanderAngle) } * m_Radius };

	m_Target = newPoint;

	SteeringPlugin_Output_Expanded steering{ Seek::calculateSteering(deltaT, agent) };

	return steering;
}

BlendedSteering::BlendedSteering(std::vector<WeightedBehavior> weightedBehaviors)
	:m_WeightedBehaviors(weightedBehaviors)
{
};

//****************
//BLENDED STEERING
SteeringPlugin_Output_Expanded BlendedSteering::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringPlugin_Output_Expanded blendedSteering = {};
	auto totalWeight = 0.f;

	for (auto weightedBehavior : m_WeightedBehaviors)
	{
		auto steering = weightedBehavior.pBehavior->calculateSteering(deltaT, pAgent);
		blendedSteering.LinearVelocity += weightedBehavior.weight * steering.LinearVelocity;
		blendedSteering.AngularVelocity += weightedBehavior.weight * steering.AngularVelocity;

		totalWeight += weightedBehavior.weight;
	}

	if (totalWeight > 0.f)
	{
		auto scale = 1.f / totalWeight;
		blendedSteering *= scale;
	}

	return blendedSteering;
}

//*****************
//PRIORITY STEERING
SteeringPlugin_Output_Expanded PrioritySteering::calculateSteering(float deltaT, AgentInfo* pAgent)
{
	SteeringPlugin_Output_Expanded steering = {};

	for (auto pBehavior : m_PriorityBehaviors)
	{
		steering = pBehavior->calculateSteering(deltaT, pAgent);

		if (steering.IsValid)
			break;
	}

	//If non of the behavior return a valid output, last behavior is returned
	return steering;
}