#include "stdafx.h"
#include "WorldDivider.h"

WorldDivider::WorldDivider(Elite::Vector2 center, Elite::Vector2 size)
	: m_CurrentQuadrant{ 0 }
	, m_DestinationQuadrant{ 5 }
	, m_QuadrantMaxTime{ 25 }
{
	m_pTimer = new Timer(m_QuadrantMaxTime);

	Elite::Vector2 worldPos{ center.x - size.x / 2, center.y - size.y / 2 };


	int rowCollumAmount{ 3 };

	float gridWidth{ size.x };
	float gridHeight{ size.y };
	for (size_t Windex = 0; Windex < rowCollumAmount; Windex++)
	{
		for (size_t Hindex = 0; Hindex < rowCollumAmount; Hindex++)
		{
			m_VecGrids.push_back(quadrant{ worldPos.x + Windex * gridWidth, worldPos.y + Hindex * gridHeight, gridWidth, gridHeight });
		}
	}

	int Xamount{ 3 };
	int Yamount{ 3 };	
	
	float width{ size.x / Xamount };
	float height{ size.y / 3 };

	for (size_t Windex = 0; Windex < Xamount; Windex++)
	{
		for (size_t Hindex = 0; Hindex < Yamount; Hindex++)
		{
			m_VecQuadrants.push_back(quadrant{ worldPos.x + Windex * width, worldPos.y + Hindex * height, width, height });
			m_VecExploredHousesInQuadrant.push_back(std::vector<Elite::Vector2>{});
		}
	}
}

WorldDivider::~WorldDivider()
{

}

void WorldDivider::Render() const
{
	/*for (const quadrant& grid : m_VecGrids)
	{
		Elite::Vector2 p0{ grid.x, grid.y };
		Elite::Vector2 p1{ grid.x, grid.y + grid.height };
		Elite::Vector2 p2{ grid.x + grid.width, grid.y + grid.height };
		Elite::Vector2 p3{ grid.x + grid.width, grid.y };

		pInterface->Draw_Segment(p0, p1, Elite::Vector3{ 0, 0, 1 });
		pInterface->Draw_Segment(p1, p2, Elite::Vector3{ 0, 0, 1 });
		pInterface->Draw_Segment(p2, p3, Elite::Vector3{ 0, 0, 1 });
		pInterface->Draw_Segment(p3, p0, Elite::Vector3{ 0, 0, 1 });
	}*/
}

void WorldDivider::Update(float deltaT, Elite::Vector2 playerPos)
{
	m_pTimer->Update(deltaT);

	int index{ -1 };
	for (const quadrant& quad: m_VecQuadrants)
	{
		++index;

		if (playerPos.x < quad.x)
		{
			continue;
		}

		if (playerPos.x > quad.x + quad.width)
		{
			continue;
		}

		if (playerPos.y < quad.y)
		{
			continue;
		}

		if (playerPos.y > quad.y + quad.height)
		{
			continue;
		}

		if (index != m_CurrentQuadrant)
		{
			m_CurrentQuadrant = index;
			m_pTimer->Reset();
			//ResetVec();
		}
		break;
	}
}

int WorldDivider::GetCurrentQuadrant() const
{
	return m_CurrentQuadrant;
}

int WorldDivider::GetDestinationQuadrant()
{
	if (m_pTimer->IsDone() && m_CurrentQuadrant == m_DestinationQuadrant)
	{
		do
		{
			m_DestinationQuadrant = rand() % 3 + 3;
		} while (m_DestinationQuadrant == m_CurrentQuadrant);

		m_pTimer->Reset();
		return m_DestinationQuadrant;
	}
	return m_DestinationQuadrant;
}

Elite::Vector2 WorldDivider::GetCenterOfQuadrant(int index) const
{
	quadrant quadrant = m_VecQuadrants[index];
	Elite::Vector2 center{ quadrant.x + quadrant.width / 2, quadrant.y + quadrant.height / 2 };

	return center;
}

void WorldDivider::AddHouse(Elite::Vector2 centerPos)
{
	for (const Elite::Vector2& houseCenter : m_VecExploredHousesInQuadrant[m_CurrentQuadrant])
	{
		if (houseCenter == centerPos)
		{
			return;
		}
	}

	m_VecExploredHousesInQuadrant[m_CurrentQuadrant].push_back(centerPos);
}

void WorldDivider::ResetVec()
{
	for (std::vector<Elite::Vector2>& houses : m_VecExploredHousesInQuadrant)
	{
		houses.clear();
	}
}

std::vector<Elite::Vector2> WorldDivider::GetVisitedHouses() const
{
	return m_VecExploredHousesInQuadrant[m_CurrentQuadrant];
	//return m_VecExploredHouses;
}

std::vector<Elite::Vector2> WorldDivider::GetAllVisitedHouses() const
{
	std::vector<Elite::Vector2> allHouses;
	for (size_t Quadrantindex = 0; Quadrantindex < m_VecExploredHousesInQuadrant.size(); Quadrantindex++)
	{
		for (size_t index = 0; index < m_VecExploredHousesInQuadrant[Quadrantindex].size(); index++)
		{
			allHouses.push_back(m_VecExploredHousesInQuadrant[Quadrantindex][index]);
		}
	}

	return allHouses;
}

bool WorldDivider::constainsHouse(Elite::Vector2 centerPos) const
{
	for (const Elite::Vector2& houseCenter : m_VecExploredHousesInQuadrant[m_CurrentQuadrant])
	{
		if (houseCenter == centerPos)
		{
			return true;
		}
	}

	return false;
}


