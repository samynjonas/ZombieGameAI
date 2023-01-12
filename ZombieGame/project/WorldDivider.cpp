#include "stdafx.h"
#include "WorldDivider.h"

WorldDivider::WorldDivider(Elite::Vector2 center, Elite::Vector2 size)
	: m_CurrentGrid{ 0 }
	, m_DestinationQuadrant{ 5 }
	, m_RowCollAmount{ 21 }
{
	m_CurrentGrid = (m_RowCollAmount * m_RowCollAmount) / 2;
	m_DestinationQuadrant = m_CurrentGrid;

	Elite::Vector2 worldPos{ center.x - size.x / 2, center.y - size.y / 2 };

	float gridWidth{ size.x / m_RowCollAmount };
	float gridHeight{ size.y / m_RowCollAmount };

	for (int Windex = 0; Windex < m_RowCollAmount; Windex++)
	{
		for (int Hindex = 0; Hindex < m_RowCollAmount; Hindex++)
		{
			m_VecGrids.push_back(grid{ worldPos.x + Windex * gridWidth, worldPos.y + Hindex * gridHeight, gridWidth, gridHeight });
			m_VecExploredHousesInQuadrant.push_back(std::vector<Elite::Vector2>{});
		}
	}
}

WorldDivider::~WorldDivider()
{

}

void WorldDivider::Render(IExamInterface* pInterface) const
{
	for (const grid& currentGrid : m_VecGrids)
	{
		Elite::Vector2 p0{ currentGrid.x,					  currentGrid.y };
		Elite::Vector2 p1{ currentGrid.x,					  currentGrid.y + currentGrid.height };
		Elite::Vector2 p2{ currentGrid.x + currentGrid.width, currentGrid.y + currentGrid.height };
		Elite::Vector2 p3{ currentGrid.x + currentGrid.width, currentGrid.y };
		
		Elite::Vector3 color{ 0, 0, 0 };

		if (currentGrid.strength > 0)
		{
			color.z = static_cast<float>(currentGrid.strength);
		}
		else if (currentGrid.strength < 0)
		{
			color.x = static_cast<float>(abs(currentGrid.strength));
		}

		Elite::Vector2 points[]{p0, p1, p2, p3};
		pInterface->Draw_SolidPolygon(points, 4, color / 10);
	}
}

void WorldDivider::Update(float deltaT, Elite::Vector2 playerPos)
{
	for (size_t index = 0; index < m_VecGrids.size(); index++)
	{
		if (m_VecGrids[index].IsPointIn(playerPos))
		{
			if (index != m_CurrentGrid)
			{
				//Changed grids
				if (m_VecGrids[m_CurrentGrid].strength <= 0)
				{
					//No houses found
					--m_VecGrids[m_CurrentGrid].strength;
				}
			}
			m_CurrentGrid = index;
			break;
		}
	}
}

int WorldDivider::GetStrength() const
{
	return m_VecGrids[m_CurrentGrid].strength;
}

int WorldDivider::GetCurrentGridIndex() const
{
	return m_CurrentGrid;
}
grid WorldDivider::GetCurrentGrid() const
{
	return m_VecGrids[m_CurrentGrid];
}
std::vector<grid> WorldDivider::GetAllGrids() const
{
	return m_VecGrids;
}

int WorldDivider::GetGridIndex(const grid& grid) const
{
	for (size_t index = 0; index < m_VecGrids.size(); index++)
	{
		if (grid.GetCenter() == m_VecGrids[index].GetCenter())
		{
			return index;
		}
	}

	return 0;
}

int WorldDivider::GetNextGrid()
{	
	const enum class direction { left, right, up, down };
	static direction currentDirection{ direction::left };

	static int visitedGrids{ 0 };
	static int goalGridAmout{ 0 };
	static bool visitedTwice{ false };

	++visitedGrids;
	
	//Go till goal is reached || touched world side || whole world is explored
	if (visitedGrids >= goalGridAmout || isGridIndexOnWorldBorder(m_CurrentGrid))
	{
		visitedGrids = 0;
		++goalGridAmout;


		switch (currentDirection)
		{
		case direction::left:
			currentDirection = direction::down;
			break;
		case direction::right:
			currentDirection = direction::up;
			break;
		case direction::up:
			currentDirection = direction::left;
			break;
		case direction::down:
			currentDirection = direction::right;
			break;
		default:
			break;
		}


	}
	int nextGrid{ m_CurrentGrid };

	switch (currentDirection)
	{
	case direction::left:
		nextGrid -= 1;
		break;
	case direction::right:
		nextGrid += 1;
		break;
	case direction::up:
		nextGrid -= m_RowCollAmount;
		break;
	case direction::down:
		nextGrid += m_RowCollAmount;
		break;
	default:
		break;
	}

	return nextGrid;
}

bool WorldDivider::isGridIndexOnWorldBorder(int index) const
{
	if (index % m_RowCollAmount == 0)
	{
		//Left side
		return true;
	}

	if ((index + 1) % m_RowCollAmount == 0)
	{
		//Right side
		return true;
	}

	if ((index + m_RowCollAmount) >= m_RowCollAmount * m_RowCollAmount)
	{
		//top side
		return true;
	}

	if ((index - m_RowCollAmount) < 0)
	{
		//bottom side
		return true;
	}

	return false;
}

bool WorldDivider::isWorldExplored() const
{
	for (const grid& grid : m_VecGrids)
	{
		if (grid.strength == 0)
		{
			return false;
		}
	}
	return true;
}

std::vector<grid> WorldDivider::GetSurroundingGrids(int currentGridIndex) const
{
	enum class xPlacement { left, middle, right };
	enum class yPlacement { top,  middle, bottom };

	xPlacement xPlace{ xPlacement::middle };
	yPlacement yPlace{ yPlacement::middle };

	if (currentGridIndex % m_RowCollAmount == 0)
	{
		xPlace = xPlacement::left;
	}
	else if (currentGridIndex + 1 % m_RowCollAmount == 0)
	{
		xPlace = xPlacement::right;
	}

	if (currentGridIndex + m_RowCollAmount > m_RowCollAmount * m_RowCollAmount)
	{
		yPlace = yPlacement::top;
	}
	else if (currentGridIndex - m_RowCollAmount < 0)
	{
		yPlace = yPlacement::bottom;
	}

	std::vector<grid> surroundingGrids;


	switch (xPlace)
	{
	case xPlacement::left:
		//only add right
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid + 1]);
		break;
	case xPlacement::middle:
		//add middle and right
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid - 1]);
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid + 1]);
		break;
	case xPlacement::right:
		//only add left
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid - 1]);
		break;
	default:
		break;
	}

	switch (yPlace)
	{
	case yPlacement::top:
		//only add bottom
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid - m_RowCollAmount]);
		break;
	case yPlacement::middle:
		//add bottom and top
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid - m_RowCollAmount]);
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid + m_RowCollAmount]);
		break;
	case yPlacement::bottom:
		//only add top
		surroundingGrids.push_back(m_VecGrids[m_CurrentGrid + m_RowCollAmount]);
		break;
	default:
		break;
	}

	return surroundingGrids;
}

int WorldDivider::GetDestinationGrid()
{
	return m_DestinationQuadrant;
}

Elite::Vector2 WorldDivider::GetCenterOfQuadrant(int index) const
{
	return m_VecGrids[index].GetCenter();
}

void WorldDivider::AddHouse(HouseInfo house)
{
	for (const Elite::Vector2& houseCenter : m_VecExploredHousesInQuadrant[m_CurrentGrid])
	{
		if (houseCenter == house.Center)
		{
			return;
		}
	}

	Elite::Vector2 houseLeftUnder{ house.Center - house.Size / 2 };

	Elite::Vector2 p0{ house.Center };
	Elite::Vector2 p1{ houseLeftUnder };
	Elite::Vector2 p2{ houseLeftUnder.x,				houseLeftUnder.y + house.Size.y };
	Elite::Vector2 p3{ houseLeftUnder.x + house.Size.x, houseLeftUnder.y + house.Size.y };
	Elite::Vector2 p4{ houseLeftUnder.x + house.Size.x, houseLeftUnder.y };

	for (grid& grid : m_VecGrids)
	{
		if (grid.IsPointIn(p0))
		{
			if (grid.strength < 0)
			{
				grid.strength = 0;
			}
			++grid.strength;

			m_VecExploredHousesInQuadrant[GetGridIndex(grid)].push_back(house.Center);
			continue;
		}
		if (grid.IsPointIn(p1))
		{
			if (grid.strength < 0)
			{
				grid.strength = 0;
			}
			++grid.strength;

			m_VecExploredHousesInQuadrant[GetGridIndex(grid)].push_back(house.Center);
			continue;
		}
		if (grid.IsPointIn(p2))
		{
			if (grid.strength < 0)
			{
				grid.strength = 0;
			}
			++grid.strength;

			m_VecExploredHousesInQuadrant[GetGridIndex(grid)].push_back(house.Center);
			continue;
		}
		if (grid.IsPointIn(p3))
		{
			if (grid.strength < 0)
			{
				grid.strength = 0;
			}
			++grid.strength;

			m_VecExploredHousesInQuadrant[GetGridIndex(grid)].push_back(house.Center);
			continue;
		}
		if (grid.IsPointIn(p4))
		{
			if (grid.strength < 0)
			{
				grid.strength = 0;
			}
			++grid.strength;

			m_VecExploredHousesInQuadrant[GetGridIndex(grid)].push_back(house.Center);
			continue;
		}
	}

	m_VecExploredHousesInQuadrant[m_CurrentGrid].push_back(house.Center);
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
	return m_VecExploredHousesInQuadrant[m_CurrentGrid];
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
	for (size_t index = 0; index < m_VecExploredHousesInQuadrant.size(); index++)
	{
		for (const Elite::Vector2& houseCenter : m_VecExploredHousesInQuadrant[index])
		{
			if (houseCenter == centerPos)
			{
				return true;
			}
		}
	}

	return false;	
}