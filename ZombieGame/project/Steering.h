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
	bool SetToArrive(Elite::Vector2 target);

	bool SetToFleeBackwards(Elite::Vector2 target);
	bool SetToSeekBackwards(Elite::Vector2 target);
	
	bool SetToTurnAround();
	bool SetToForward();

	bool SetToRotateLeft();
	bool SetToRotateAndBackwards();

	bool SetToSeekRotating(Elite::Vector2 seekPos, float rotateSpeed = 1.f);

	enum class SteeringBehaviorsTypes
	{
		Wander,
		Seek,
		Flee,
		Face,
		RotateLeft,
		Forward
	};
	struct weightSteeringBehavior
	{
		SteeringBehaviorsTypes behavior = SteeringBehaviorsTypes::Wander;
		float weight = 0.f;

		weightSteeringBehavior(SteeringBehaviorsTypes behavior, float weight) :
			behavior(behavior),
			weight(weight)
		{};
	};


	bool SetToBlended(std::vector<weightSteeringBehavior> vecSteeringBehaviors, Elite::Vector2 target);
	bool SetToPriority(std::vector<SteeringBehaviorsTypes> vecSteeringBehaviors, Elite::Vector2 target);

	void SetRunMode(bool set);
	bool IsRotating() const;


	SteeringPlugin_Output GetSteering(float deltaT, AgentInfo* agent) const;
	Elite::Vector2 GetTarget() const;

private:
	SteeringBehaviors* m_pCurrentSteering;

	SteeringBehaviors* m_pWander;
	SteeringBehaviors* m_pSeek;
	SteeringBehaviors* m_pSeekAndTurn;
	SteeringBehaviors* m_pFlee;
	SteeringBehaviors* m_pFace;
	SteeringBehaviors* m_pRotateLeft;
	SteeringBehaviors* m_pTurnAround;
	SteeringBehaviors* m_pForward;
	SteeringBehaviors* m_pFleeBackwards;
	SteeringBehaviors* m_pSeekBackwards;
	SteeringBehaviors* m_pRotateAndBackwards;
	SteeringBehaviors* m_pArrive;

	bool m_RunMode;
};

