#include "stdafx.h"
#include "WorldDivider.h"

WorldDivider::WorldDivider(Elite::Vector2 Center, Elite::Vector2 Size)
	: m_CurrentQuadrant{ 0 }
	, m_DestinationQuadrant{ 0 }
	, m_QuadrantMaxTime{ 10 }
{
	m_pTimer = new Timer(m_QuadrantMaxTime);

	Elite::Vector2 worldPos{ Center.x - Size.x / 2, Center.y - Size.y / 2 };


	int Xamount{ 3 };
	int Yamount{ 3 };	
	
	float width{ Size.x / Xamount };
	float height{ Size.y / 3 };

	for (size_t Windex = 0; Windex < Xamount; Windex++)
	{
		for (size_t Hindex = 0; Hindex < Yamount; Hindex++)
		{
			m_VecQuadrants.push_back(quadrant{ worldPos.x + Windex * width, worldPos.y + Hindex * height, width, height });
		}
	}
}

WorldDivider::~WorldDivider()
{

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
			m_DestinationQuadrant = rand() % 9;
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



