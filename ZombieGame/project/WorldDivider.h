#pragma once
#include <map>

#include "Exam_HelperStructs.h"

#include "IExamInterface.h"

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

struct grid
{
	grid(float X, float Y, float W, float H)
		: x{ X }
		, y{ Y }
		, width{ W }
		, height{ H }
		, strength{ 0 }
	{

	}

	bool IsPointIn(Elite::Vector2 point) const
	{
		if (point.x < x)
		{
			return false;
		}

		if (point.x > x + width)
		{
			return false;
		}

		if (point.y < y)
		{
			return false;
		}

		if (point.y > y + height)
		{
			return false;
		}

		return true;
	}
	Elite::Vector2 GetCenter() const
	{
		return Elite::Vector2{ x + width / 2, y + height / 2 };
	}

	float x;
	float y;
	float width;
	float height;

	int strength; //Will go up when there is something interesting in it
};


class WorldDivider final
{
public:
	WorldDivider(Elite::Vector2 Center, Elite::Vector2 Size);
	~WorldDivider();

	void Update(float deltaT, Elite::Vector2 playerPos);
	void Render(IExamInterface* pInterface) const;
	

	int GetStrength() const;
	int GetCurrentGridIndex() const;
	int GetGridIndex(const grid& grid) const;

	grid GetCurrentGrid() const;
	std::vector<grid> GetAllGrids() const;

	std::vector<grid> GetSurroundingGrids(int currentGridIndex) const;

	int GetDestinationGrid();
	bool constainsHouse(Elite::Vector2 centerPos) const;
	Elite::Vector2 GetCenterOfQuadrant(int index) const;

	int GetNextGrid();
	bool isGridIndexOnWorldBorder(int index) const;
	bool isWorldExplored() const;

	std::vector<Elite::Vector2> GetVisitedHouses() const;
	std::vector<Elite::Vector2> GetAllVisitedHouses() const;

	void AddHouse(HouseInfo house);
	void ResetVec();

private:
	int m_CurrentGrid;
	int m_DestinationQuadrant;
	int m_RowCollAmount;

	std::vector<std::vector<Elite::Vector2>> m_VecExploredHousesInQuadrant;
	std::vector<grid> m_VecGrids;

};

