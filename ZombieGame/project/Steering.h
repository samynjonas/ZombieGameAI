#pragma once
#include "SteeringBehaviors.h"

class Steering final
{
public:
	Steering();
	~Steering();

	bool SetToWander();
	bool SetToSeek(Elite::Vector2 seekPos);
	bool SetToFlee(Elite::Vector2 fleePos);
	bool SetToFace(Elite::Vector2 target);
	bool SetToTurnAround(Elite::Vector2 target);
	bool SetToRotateLeft();

	SteeringPlugin_Output GetSteering(float deltaT, AgentInfo* agent) const;
	Elite::Vector2 GetTarget() const;

private:
	SteeringBehaviors* m_pCurrentSteering;

	SteeringBehaviors* m_pWander;
	SteeringBehaviors* m_pSeek;
	SteeringBehaviors* m_pFlee;
	SteeringBehaviors* m_pFace;
	SteeringBehaviors* m_pRotateLeft;
	SteeringBehaviors* m_pTurnAround;
};

