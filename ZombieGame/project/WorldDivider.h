#pragma once
#include "Timer.h"

struct quadrant
{
	quadrant(float X, float Y, float W, float H)
		: x{ X }
		, y{ Y }
		, width{ W }
		, height{ H }
	{

	}

	float x;
	float y;
	float width;
	float height;
};


class WorldDivider
{
public:
	WorldDivider(Elite::Vector2 Center, Elite::Vector2 Size);
	~WorldDivider();

	void Update(float deltaT, Elite::Vector2 playerPos);
	int GetCurrentQuadrant() const;
	int GetDestinationQuadrant();


	Elite::Vector2 GetCenterOfQuadrant(int index) const;

private:
	int m_CurrentQuadrant;
	int m_DestinationQuadrant;

	std::vector<quadrant> m_VecQuadrants;

	Timer* m_pTimer;
	float m_QuadrantMaxTime;


};

