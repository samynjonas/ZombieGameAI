#include "Exam_HelperStructs.h"
#include <math.h>

class SteeringBehaviors
{
public:
	SteeringBehaviors() = default;
	virtual ~SteeringBehaviors() = default;

	virtual SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) = 0;

	void SetTarget(Elite::Vector2 target)
	{
		m_Target = target;
	}
	Elite::Vector2 GetTarget() const
	{
		return m_Target;
	}

protected:
	Elite::Vector2 m_Target;

};


class Seek : public SteeringBehaviors
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* pAgent) override;
};

class SeekBackwards : public SteeringBehaviors
{
public:
	SeekBackwards() = default;
	virtual ~SeekBackwards() = default;

	//Seek Behaviour
	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* pAgent) override;
};

class Forward : public SteeringBehaviors
{
public:
	Forward() = default;
	virtual ~Forward() = default;

	//Forward Behaviour
	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* pAgent) override;
};

class Flee : public SteeringBehaviors
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) override;

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

	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) override;
};

class Arrive : public SteeringBehaviors
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) override;
};


class Face : public SteeringBehaviors
{
public:
	Face() = default;
	virtual ~Face() = default;

	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) override;

};

class turnAround : public Face
{
public:
	turnAround() = default;
	virtual ~turnAround() = default;

	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) override;
};


class Wander : public Seek
{
public:
	Wander() = default;
	virtual ~Wander() = default;

	//Wander Behaviour
	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* pAgent) override;

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

	SteeringPlugin_Output calculateSteering(float deltaT, AgentInfo* agent) override;

};

