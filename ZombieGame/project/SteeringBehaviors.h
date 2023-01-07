#include "Exam_HelperStructs.h"
#include <math.h>

class SteeringBehaviors
{
public:
	SteeringBehaviors() = default;
	virtual ~SteeringBehaviors() = default;

	virtual SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) = 0;

	void SetTarget(Elite::Vector2 target)
	{
		m_Target = target;
	}

	Elite::Vector2 GetTarget() const
	{
		return m_Target;
	}
	void SetRotateSpeed(float speed) { m_RotateSpeed = speed; }

	template<class T, typename std::enable_if<std::is_base_of<SteeringBehaviors, T>::value>::type* = nullptr>
	T* As()
	{
		return static_cast<T*>(this);
	}

protected:
	Elite::Vector2 m_Target;
	float m_RotateSpeed{ 1.f };

};


class Seek : public SteeringBehaviors
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;
};

class SeekAndRotate : public Seek
{
public:
	SeekAndRotate() = default;
	virtual ~SeekAndRotate() = default;

	//Seek Behaviour
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;

private:

};

class SeekBackwards : public SteeringBehaviors
{
public:
	SeekBackwards() = default;
	virtual ~SeekBackwards() = default;

	//Seek Behaviour
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;
};

class Forward : public SteeringBehaviors
{
public:
	Forward() = default;
	virtual ~Forward() = default;

	//Forward Behaviour
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;
};

class Flee : public SteeringBehaviors
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) override;

	void SetFleeRadius(float radius)
	{
		m_FleeRadius = radius;
	}

private:
	float m_FleeRadius{ 10.f };

};

class FleeBackward : public SteeringBehaviors
{
public:
	FleeBackward() = default;
	virtual ~FleeBackward() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) override;
};

class Arrive : public SteeringBehaviors
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) override;
};



class Face : public SteeringBehaviors
{
public:
	Face() = default;
	virtual ~Face() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) override;

};

class turnAround : public Face
{
public:
	turnAround() = default;
	virtual ~turnAround() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) override;
};


class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behaviour
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }

protected:
	float m_OffsetDistance = 6.f;
	float m_Radius = 8.f;
	float m_MaxAngleChange = Elite::ToRadians(180);
	float m_WanderAngle = 0.f;
};


class RotateLeft : public Face
{
public:
	RotateLeft() = default;
	virtual ~RotateLeft() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* agent) override;

};

class RotateAndWalkbackwards : public RotateLeft
{
public:
	RotateAndWalkbackwards() = default;
	virtual ~RotateAndWalkbackwards() = default;

	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;
};


//****************
//BLENDED STEERING
class BlendedSteering final : public SteeringBehaviors
{
public:
	struct WeightedBehavior
	{
		SteeringBehaviors* pBehavior = nullptr;
		float weight = 0.f;

		WeightedBehavior(SteeringBehaviors* pBehavior, float weight) :
			pBehavior(pBehavior),
			weight(weight)
		{};
	};

	BlendedSteering(std::vector<WeightedBehavior> weightedBehaviors);

	void AddBehaviour(WeightedBehavior weightedBehavior) { m_WeightedBehaviors.push_back(weightedBehavior); }
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;

	// returns a reference to the weighted behaviors, can be used to adjust weighting. Is not intended to alter the behaviors themselves.
	std::vector<WeightedBehavior>& GetWeightedBehaviorsRef() { return m_WeightedBehaviors; }

private:
	std::vector<WeightedBehavior> m_WeightedBehaviors = {};

	using SteeringBehaviors::SetTarget; // made private because targets need to be set on the individual behaviors, not the combined behavior
};

//*****************
//PRIORITY STEERING
class PrioritySteering final : public SteeringBehaviors
{
public:
	PrioritySteering(std::vector<SteeringBehaviors*> priorityBehaviors)
		:m_PriorityBehaviors(priorityBehaviors)
	{}

	void AddBehaviour(SteeringBehaviors* pBehavior) { m_PriorityBehaviors.push_back(pBehavior); }
	SteeringPlugin_Output_Expanded calculateSteering(float deltaT, AgentInfo* pAgent) override;

private:
	std::vector<SteeringBehaviors*> m_PriorityBehaviors = {};

	using SteeringBehaviors::SetTarget; // made private because targets need to be set on the individual behaviors, not the combined behavior
};

