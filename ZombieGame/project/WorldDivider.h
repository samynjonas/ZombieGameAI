#pragma once
#include <map>

#include "Timer.h"
#include "IExamInterface.h"

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


class WorldDivider final
{
public:
	WorldDivider(Elite::Vector2 Center, Elite::Vector2 Size);
	~WorldDivider();

	void Update(float deltaT, Elite::Vector2 playerPos);
	int GetCurrentQuadrant() const;
	int GetDestinationQuadrant();


	void Render() const;

	std::vector<Elite::Vector2> GetVisitedHouses() const;
	std::vector<Elite::Vector2> GetAllVisitedHouses() const;

	void AddHouse(Elite::Vector2 centerPos);
	void ResetVec();

	bool constainsHouse(Elite::Vector2 centerPos) const;

	Elite::Vector2 GetCenterOfQuadrant(int index) const;

private:
	int m_CurrentQuadrant;
	int m_DestinationQuadrant;
	
	std::vector<std::vector<Elite::Vector2>> m_VecExploredHousesInQuadrant;

	std::vector<quadrant> m_VecQuadrants;

	std::vector<quadrant> m_VecGrids;

	Timer* m_pTimer;
	float m_QuadrantMaxTime;


};

