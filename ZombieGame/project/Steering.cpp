#include "stdafx.h"
#include "Steering.h"

Steering::Steering()
	: m_RunMode{ false }
{
	m_pCurrentSteering = new Wander();

	m_pSeek = new Seek();
	m_pFlee = new Flee();
	m_pFace = new Face();
	m_pWander = new Wander();
	m_pArrive = new Arrive();
	m_pForward = new Forward();
	m_pRotateLeft = new RotateLeft();
	m_pTurnAround = new turnAround();
	m_pFleeBackwards = new FleeBackward();
	m_pSeekBackwards = new SeekBackwards();

	m_pSeekAndTurn = new SeekAndRotate();
	m_pRotateAndBackwards = new RotateAndWalkbackwards();
}

Steering::~Steering()
{
	SAFE_DELETE(m_pCurrentSteering);
	SAFE_DELETE(m_pWander);
	SAFE_DELETE(m_pSeek);
	SAFE_DELETE(m_pFlee);
	SAFE_DELETE(m_pFace);
	SAFE_DELETE(m_pRotateLeft);
	SAFE_DELETE(m_pTurnAround);
	SAFE_DELETE(m_pForward);
	SAFE_DELETE(m_pFleeBackwards);
	SAFE_DELETE(m_pSeekBackwards);
	SAFE_DELETE(m_pRotateAndBackwards);
	SAFE_DELETE(m_pArrive);
}

bool Steering::SetToWander()
{
	m_pCurrentSteering = m_pWander;
	return true;
}

bool Steering::SetToSeek(Elite::Vector2 seekPos)
{
	m_pCurrentSteering = m_pSeek;
	m_pCurrentSteering->SetTarget(seekPos);
	return true;
}

bool Steering::SetToArrive(Elite::Vector2 arrivePoint)
{
	m_pCurrentSteering = m_pArrive;
	m_pCurrentSteering->SetTarget(arrivePoint);
	return true;
}

bool Steering::SetToSeekRotating(Elite::Vector2 seekPos, float rotateSpeed)
{
	m_pCurrentSteering = m_pSeekAndTurn;
	m_pCurrentSteering->SetTarget(seekPos);
	m_pCurrentSteering->SetRotateSpeed(rotateSpeed);

	return true;
}


bool Steering::SetToFlee(Elite::Vector2 fleePos)
{
	m_pCurrentSteering = m_pFlee;
	m_pCurrentSteering->SetTarget(fleePos);
	return true;
}

bool Steering::SetToFace(Elite::Vector2 target)
{
	m_pCurrentSteering = m_pFace;
	m_pCurrentSteering->SetTarget(target);
	return true;
}

bool Steering::SetToRotateLeft()
{
	m_pCurrentSteering = m_pRotateLeft;
	return true;
}

bool Steering::SetToTurnAround()
{
	m_pCurrentSteering = m_pTurnAround;
	return true;
}

bool Steering::SetToForward()
{
	m_pCurrentSteering = m_pForward;
	return true;
}

bool Steering::SetToFleeBackwards(Elite::Vector2 target)
{
	m_pCurrentSteering = m_pFleeBackwards;
	m_pCurrentSteering->SetTarget(target);
	return true;
}

bool Steering::SetToSeekBackwards(Elite::Vector2 target)
{
	m_pCurrentSteering = m_pSeekBackwards;
	m_pCurrentSteering->SetTarget(target);
	return true;
}

bool Steering::SetToRotateAndBackwards()
{
	m_pCurrentSteering = m_pRotateAndBackwards;
	return true;
}

void Steering::SetRunMode(bool set)
{
	m_RunMode = set;
}

bool Steering::SetToBlended(std::vector<weightSteeringBehavior> vecSteeringBehaviors, Elite::Vector2 target)
{
	std::vector<BlendedSteering::WeightedBehavior> blendBehaviors;

	for (const weightSteeringBehavior& steeringBehaviorType : vecSteeringBehaviors)
	{
		switch (steeringBehaviorType.behavior)
		{
		case SteeringBehaviorsTypes::Wander:
			blendBehaviors.push_back({ m_pWander, steeringBehaviorType.weight });
			break;
		case SteeringBehaviorsTypes::Seek:
			blendBehaviors.push_back({ m_pSeek, steeringBehaviorType.weight });
			break;
		case SteeringBehaviorsTypes::Flee:
			blendBehaviors.push_back({ m_pSeek, steeringBehaviorType.weight });
			break;
		case SteeringBehaviorsTypes::Face:
			blendBehaviors.push_back({ m_pFace, steeringBehaviorType.weight });
			break;
		case SteeringBehaviorsTypes::RotateLeft:
			blendBehaviors.push_back({ m_pRotateLeft, steeringBehaviorType.weight });
			break;
		case SteeringBehaviorsTypes::Forward:
			blendBehaviors.push_back({ m_pForward, steeringBehaviorType.weight });
			break;
		}
	}

	BlendedSteering* pBlend = new BlendedSteering{ blendBehaviors };
	m_pCurrentSteering = pBlend;

	return true;
}

bool Steering::SetToPriority(std::vector<SteeringBehaviorsTypes> vecSteeringBehaviors, Elite::Vector2 target)
{
	std::vector<SteeringBehaviors*> priorityBehaviors;
	for (const auto& steeringBehaviorType : vecSteeringBehaviors)
	{
		switch (steeringBehaviorType)
		{
		case SteeringBehaviorsTypes::Wander:
			priorityBehaviors.push_back({ m_pWander });
			break;
		case SteeringBehaviorsTypes::Seek:
			m_pSeek->SetTarget(target);
			priorityBehaviors.push_back({ m_pSeek });
			break;
		case SteeringBehaviorsTypes::Flee:
			m_pFlee->SetTarget(target);
			priorityBehaviors.push_back({ m_pFlee });
			break;
		case SteeringBehaviorsTypes::Face:
			m_pFace->SetTarget(target);
			priorityBehaviors.push_back({ m_pFace });
			break;
		case SteeringBehaviorsTypes::RotateLeft:
			priorityBehaviors.push_back({ m_pRotateLeft });
			break;
		case SteeringBehaviorsTypes::Forward:
			priorityBehaviors.push_back({ m_pForward });
			break;
		}
	}

	PrioritySteering* pPriority = new PrioritySteering{ priorityBehaviors };

	m_pCurrentSteering = pPriority;
	return true;
}



SteeringPlugin_Output Steering::GetSteering(float deltaT, AgentInfo* agent) const
{
	SteeringPlugin_Output_Expanded Expandedsteering{ m_pCurrentSteering->calculateSteering(deltaT, agent) };

	//Convert to Ai can use
	SteeringPlugin_Output steering{};
	steering.AngularVelocity = Expandedsteering.AngularVelocity;
	steering.LinearVelocity	 = Expandedsteering.LinearVelocity;
	steering.AutoOrient		 = Expandedsteering.AutoOrient;
	steering.RunMode		 = m_RunMode;

	return steering;
}

bool Steering::IsRotating() const
{
	if (m_pCurrentSteering == m_pRotateLeft)
	{
		return true;
	}
	return false;
}

Elite::Vector2 Steering::GetTarget() const
{
	return m_pCurrentSteering->GetTarget();
}
