#include "stdafx.h"
#include "Steering.h"

Steering::Steering()
{
	m_pCurrentSteering = new Wander();

	m_pSeek = new Seek();
	m_pFlee = new Flee();
	m_pFace = new Face();
	m_pWander = new Wander();
	m_pForward = new Forward();
	m_pRotateLeft = new RotateLeft();
	m_pTurnAround = new turnAround();
	m_pFleeBackwards = new FleeBackward();
	m_pSeekBackwards = new SeekBackwards();
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

SteeringPlugin_Output Steering::GetSteering(float deltaT, AgentInfo* agent) const
{
	return m_pCurrentSteering->calculateSteering(deltaT, agent);
}

Elite::Vector2 Steering::GetTarget() const
{
	return m_pCurrentSteering->GetTarget();
}
