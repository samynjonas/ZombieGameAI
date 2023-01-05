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

	bool SetToFleeBackwards(Elite::Vector2 target);
	bool SetToSeekBackwards(Elite::Vector2 target);
	
	bool SetToTurnAround();
	bool SetToRotateLeft();
	bool SetToForward();

	void SetRunMode(bool set);

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
	SteeringBehaviors* m_pForward;
	SteeringBehaviors* m_pFleeBackwards;
	SteeringBehaviors* m_pSeekBackwards;

	bool m_RunMode;
};

