/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// Behaviors.h: Implementation of certain reusable behaviors for the BT version of the Agario Game
/*=============================================================================*/
#ifndef ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
#define ELITE_APPLICATION_BEHAVIOR_TREE_BEHAVIORS
//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "EliteMath/EMath.h"
#include "EBehaviorTree.h"
#include "IExamInterface.h"

#include "Steering.h"
#include "InventoryManagement.h"
#include "WorldDivider.h"
#include "Timer.h"

//-----------------------------------------------------------------
// Behaviors -- Actions
//-----------------------------------------------------------------
namespace BT_Actions
{
	Elite::BehaviorState ChangeToWander(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		Steering* pSteering;
		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToWander();
		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState ChangeToSeek(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		Elite::Vector2 targetPos;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Target", targetPos) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToSeek(targetPos);
		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState ChangeToFlee(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		Elite::Vector2 targetPos;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Target", targetPos) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToFlee(targetPos);
		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState ChangeToFace(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		Elite::Vector2 targetPos;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Target", targetPos) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToFace(targetPos);
		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState ChangeToForward(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToForward();
		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState ChangeToFleeBackwards(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		Elite::Vector2 targetPos;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Target", targetPos) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToFleeBackwards(targetPos);
		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState ChangeToSeekBackwards(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		Elite::Vector2 targetPos;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Target", targetPos) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToFleeBackwards(targetPos);
		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState RotateLeft(Elite::Blackboard* pBlackboard)
	{
		//Rotate agent
		Steering* pSteering;
		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(true);
		pSteering->SetToRotateLeft();

		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState TurnAround(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToTurnAround();

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState ResetTimer(Elite::Blackboard* pBlackboard)
	{
		Timer* pTimer;
		if (pBlackboard->GetData("Timer", pTimer) == false || pTimer == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pTimer->Reset();

		return Elite::BehaviorState::Success;
	}


	//----------------------
	//World exploration
	//----------------------
	Elite::BehaviorState GoToNextGrid(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		Steering* pSteering;
		IExamInterface* pInterface;
		WorldDivider* pWorldDivider;
		InventoryManagement* pInventory;

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}		

		static int destIndex{ pWorldDivider->GetDestinationGrid() };		

		Elite::Vector2 gridPos{ pWorldDivider->GetAllGrids()[destIndex].GetCenter() };
		
		if (pAgent->Position.DistanceSquared(gridPos) <= 25)
		{
			std::vector<grid> positiveGrids;
			std::vector<grid> normalGrids;
			std::vector<grid> negativeGrids;
			std::vector<grid> positiveNormalGrids;

			for (const grid& currentgrid : pWorldDivider->GetSurroundingGrids(pWorldDivider->GetCurrentGridIndex()))
			{
				if (currentgrid.strength > 0)
				{
					positiveGrids.push_back(currentgrid);
					positiveNormalGrids.push_back(currentgrid);
				}
				else if (currentgrid.strength == 0)
				{
					normalGrids.push_back(currentgrid);
					positiveNormalGrids.push_back(currentgrid);
				}
				else if (currentgrid.strength < 0)
				{
					negativeGrids.push_back(currentgrid);
				}
			}

			bool canNormal{ false };
			bool canPositive{ false };

			//Explore
			if (normalGrids.empty() == false)
			{
				canNormal = true;
			}

			if (positiveGrids.empty() == false)
			{
				if (pAgent->Health < 5)
				{
					if (pInventory->GetHealth(pInterface) <= 0)
					{
						//Has no healing but is low --- prefer known grid with houses
						canPositive = true;
					}
				}

				if (pAgent->Energy < 5)
				{
					if (pInventory->GetEnergy(pInterface) <= 0)
					{
						//Has no food but is low --- prefer known grid with houses
						canPositive = true;
					}
				}
			}

			if (canNormal == false && canPositive == false)
			{
				//pick negative
				int random = rand() % static_cast<int>(negativeGrids.size());
				destIndex = pWorldDivider->GetGridIndex(negativeGrids[random]);
			}
			else if (canNormal == true && canPositive == false)
			{
				//pick normal
				int random = rand() % static_cast<int>(normalGrids.size());
				destIndex = pWorldDivider->GetGridIndex(normalGrids[random]);
			}
			else if (canNormal == true && canPositive == true)
			{
				//pick 2-3 positive normal
				int random = rand() % static_cast<int>(positiveNormalGrids.size());
				destIndex = pWorldDivider->GetGridIndex(positiveNormalGrids[random]);
			}
			else if (canNormal == false && canPositive == true)
			{
				//pick positive
				int random = rand() % static_cast<int>(positiveGrids.size());
				destIndex = pWorldDivider->GetGridIndex(positiveGrids[random]);
			}
		}

		Elite::Vector2 goToPoint = pInterface->NavMesh_GetClosestPathPoint(gridPos);
		
		pSteering->SetRunMode(false);
		pSteering->SetToSeek(goToPoint);
		return Elite::BehaviorState::Running;
	}



	Elite::BehaviorState ExploreQuadrant(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetRunMode(false);
		pSteering->SetToWander();
		return Elite::BehaviorState::Success;
	}



	//----------------------
	//Item Handling
	//----------------------
	Elite::BehaviorState ShootWeapon(Elite::Blackboard* pBlackboard)
	{
		//Get weapon slot and shoot
		InventoryManagement* pInventory;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pInventory->Shoot(pInterface);
		pBlackboard->ChangeData("attacked", false);

		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState Heal(Elite::Blackboard* pBlackboard)
	{
		//Get weapon slot and shoot
		InventoryManagement* pInventory;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pInventory->Heal(pInterface) == true)
		{
			return Elite::BehaviorState::Success;
		}
		return Elite::BehaviorState::Failure;
	}
	Elite::BehaviorState Eat(Elite::Blackboard* pBlackboard)
	{
		//Get weapon slot and shoot
		InventoryManagement* pInventory;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pInventory->EatFood(pInterface) == true)
		{
			return Elite::BehaviorState::Success;
		}
		return Elite::BehaviorState::Failure;
	}
	

	//----------------------
	//Enemy Handling
	//----------------------
	Elite::BehaviorState RunFromEnemy(Elite::Blackboard* pBlackboard)
	{
		//Variables
		bool seesEnemy{ false };
		Elite::Vector2 target{};

		//retrieving valid data from blackboard
		AgentInfo* pAgent;
		std::vector<EntityInfo>* pEntitiesVec;
		Steering* pSteering;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pEntitiesVec->empty() == false)
		{
			//Putting al visible enemies in vector
			std::vector<Elite::Vector2> enemies;
			for (auto& Entity : *pEntitiesVec)
			{
				if (Entity.Type == eEntityType::ENEMY)
				{
					enemies.push_back(Entity.Location);
				}
			}

			if (enemies.empty() == false)
			{
				seesEnemy = true;
			}


			if (seesEnemy == true)
			{
				float distance{ enemies[0].DistanceSquared(pAgent->Position) };
				size_t closestIndex{ 0 };

				for (size_t index{ 1 }; index < enemies.size(); ++index)
				{
					float checkDistance{ enemies[index].DistanceSquared(pAgent->Position) };

					if (distance > checkDistance)
					{
						distance = checkDistance;
						closestIndex = index;
					}
				}

				target = enemies[closestIndex];

				pSteering->SetRunMode(true);
				pSteering->SetToFlee(target);
				
				return Elite::BehaviorState::Running;
			}			
		}

		pSteering->SetRunMode(true);
		pSteering->SetToForward();
		return Elite::BehaviorState::Running;
	}
	Elite::BehaviorState AttackEnemy(Elite::Blackboard* pBlackboard)
	{
		//Variables
		bool seesEnemy{ false };

		//retrieving valid data from blackboard
		AgentInfo* pAgent;
		std::vector<EntityInfo>* pEntitiesVec;
		Steering* pSteering;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pEntitiesVec->empty() == false)
		{
			//Putting al visible enemies in vector
			std::vector<Elite::Vector2> enemies;
			for (auto& Entity : *pEntitiesVec)
			{
				if (Entity.Type == eEntityType::ENEMY)
				{
					enemies.push_back(Entity.Location);
				}
			}

			if (enemies.empty() == false)
			{
				seesEnemy = true;
			}

			if (enemies.empty() == true)
			{
				seesEnemy = false;
			}


			if (seesEnemy == true)
			{
				float distance{ enemies[0].DistanceSquared(pAgent->Position) };
				size_t closestIndex{ 0 };

				for (size_t index{ 1 }; index < enemies.size(); ++index)
				{
					float checkDistance{ enemies[index].DistanceSquared(pAgent->Position) };

					if (distance > checkDistance)
					{
						distance = checkDistance;
						closestIndex = index;
					}
				}
				
				Elite::Vector2 enemyPos = enemies[closestIndex];

				float widthToEnemy{ enemyPos.x - pAgent->Position.x };
				float heightToEnemy{ enemyPos.y - pAgent->Position.y };

				float angleToEnemy{ atan2f(heightToEnemy, widthToEnemy) };

				float angleToEnemyDeg{ Elite::ToDegrees(angleToEnemy) };
				float playerAngleDeg{ Elite::ToDegrees(pAgent->Orientation) };

				float angleOffset{ 5.f };

				if (angleToEnemyDeg > playerAngleDeg - angleOffset && angleToEnemyDeg < playerAngleDeg + angleOffset)
				{
					//Is in sight
					return Elite::BehaviorState::Success;
				}

				pSteering->SetRunMode(false);
				pSteering->SetToFleeBackwards(enemyPos);
				return Elite::BehaviorState::Running;
			}
		}

		pSteering->SetRunMode(true);
		pSteering->SetToForward();
		pSteering->SetToRotateLeft();
		return Elite::BehaviorState::Running;
	}


	//----------------------
	//House Handling
	//----------------------
	Elite::BehaviorState EnterHouse(Elite::Blackboard* pBlackboard)
	{
		bool wasBitten;
		if (pBlackboard->GetData("attacked", wasBitten) == false)
		{
			return Elite::BehaviorState::Failure;
		}
		if (wasBitten == true)
		{
			return Elite::BehaviorState::Failure;
		}

		AgentInfo* pAgent;
		HouseInfo* pHouse;
		Steering* pSteering;
		IExamInterface* pInterface;
		WorldDivider* pWorldDivider;
		std::vector<HouseInfo>* pHouseVec;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("House", pHouse) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Houses", pHouseVec) == false || pHouseVec == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		
		if (pAgent->IsInHouse)
		{
			return Elite::BehaviorState::Success;
		}

		if (pHouse == nullptr)
		{
			//Get all not visited houses
			std::vector<HouseInfo> NotVisitedHouse;
			for (const HouseInfo currentHouse : (*pHouseVec))
			{
				if (pWorldDivider->constainsHouse(currentHouse.Center) == false)
				{
					NotVisitedHouse.push_back(currentHouse);
				}
			}
			if (NotVisitedHouse.empty())
			{
				return Elite::BehaviorState::Failure;
			}

			//Getting closest house
			float distance{ NotVisitedHouse[0].Center.DistanceSquared(pAgent->Position) };
			size_t closestIndex{ 0 };

			for (size_t index{ 1 }; index < NotVisitedHouse.size(); ++index)
			{
				float checkDistance{ NotVisitedHouse[index].Center.DistanceSquared(pAgent->Position) };

				if (distance > checkDistance)
				{
					distance = checkDistance;
					closestIndex = index;
				}
			}

			pHouse = new HouseInfo(NotVisitedHouse[closestIndex]);
			pBlackboard->ChangeData("House", pHouse);		
		}
		
		Elite::Vector2 point = pInterface->NavMesh_GetClosestPathPoint(pHouse->Center);	

		pSteering->SetToSeek(point);
		return Elite::BehaviorState::Running;
	}
	Elite::BehaviorState LeaveHouse(Elite::Blackboard* pBlackboard)
	{
		HouseInfo* pHouse;
		AgentInfo* pAgent;
		Steering* pSteering;
		IExamInterface* pInterface;
		WorldDivider* pWorldDivider;
		
		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("House", pHouse) == false || pHouse == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		Elite::Vector2 outsideHouse{ pHouse->Center.x + pHouse->Size.x + 2, pHouse->Center.y };
		Elite::Vector2 goToPoint = pInterface->NavMesh_GetClosestPathPoint(outsideHouse);
	
		if (pAgent->IsInHouse == false)
		{
			pWorldDivider->AddHouse(*pHouse);
			pBlackboard->ChangeData("House", static_cast<HouseInfo*>(nullptr));
			pBlackboard->ChangeData("Looted", false);
			return Elite::BehaviorState::Success;
		}

		pSteering->SetToSeek(goToPoint);
		return Elite::BehaviorState::Running;
	}

	Elite::BehaviorState LootHouse(Elite::Blackboard* pBlackboard)
	{
		bool wasBitten;
		if (pBlackboard->GetData("attacked", wasBitten) == false)
		{
			return Elite::BehaviorState::Failure;
		}
		if (wasBitten == true)
		{
			return Elite::BehaviorState::Failure;
		}

		bool hasLooted;
		HouseInfo* pHouse;
		AgentInfo* pAgent;
		Steering* pSteering;
		IExamInterface* pInterface;
		WorldDivider* pWorldDivider;

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("House", pHouse) == false || pHouse == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Looted", hasLooted) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		if (hasLooted == true)
		{
			return Elite::BehaviorState::Success;
		}


		static int currentStage{ 0 };
		float wallOffset{ 5.f };

		float halfSizeX{ pHouse->Size.x / 2 };
		float halfSizeY{ pHouse->Size.y / 2};
		
		static std::vector<Elite::Vector2> lootingStages
		{
			{	pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y + halfSizeY - wallOffset },
			{	pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y + halfSizeY - wallOffset },
			{	pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y - halfSizeY + wallOffset },
			{	pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y - halfSizeY + wallOffset }
		};

		//updating points
		lootingStages[0] = { pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y + halfSizeY - wallOffset };
		lootingStages[1] = { pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y + halfSizeY - wallOffset };
		lootingStages[2] = { pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y - halfSizeY + wallOffset };
		lootingStages[3] = { pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y - halfSizeY + wallOffset };

		float range{ 10 };

		if (pAgent->Position.DistanceSquared(lootingStages[currentStage]) <= range)
		{
			++currentStage;
		}

		if (currentStage >= lootingStages.size())
		{
			pWorldDivider->AddHouse(*pHouse);

			currentStage = 0;

			pBlackboard->ChangeData("Looted", true);
			return Elite::BehaviorState::Success;
		}

		Elite::Vector2 goToPoint = pInterface->NavMesh_GetClosestPathPoint(lootingStages[currentStage]);

		pSteering->SetToSeek(goToPoint);

		return Elite::BehaviorState::Running;
	}	
	Elite::BehaviorState hasLeftHouse(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		HouseInfo* pHouse;
		WorldDivider* pWorldDivider;
		bool hasLooted;

		if (pBlackboard->GetData("Looted", hasLooted) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("House", pHouse) == false || pHouse == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (hasLooted == false)
		{
			return Elite::BehaviorState::Success;
		}


		if (pAgent->IsInHouse == false)
		{
			pWorldDivider->AddHouse(*pHouse);
			pBlackboard->ChangeData("House", static_cast<HouseInfo*>(nullptr));
			pBlackboard->ChangeData("Looted", false);
			return Elite::BehaviorState::Failure;
		}
	}


	Elite::BehaviorState GoToClosestHouse(Elite::Blackboard* pBlackboard)
	{
		//Retrieve valid blackboard data
		AgentInfo* pAgent;
		IExamInterface* pInterface;
		Steering* pSteering;
		WorldDivider* pWorldDivider;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pWorldDivider->GetVisitedHouses().empty())
		{
			return Elite::BehaviorState::Failure;
		}

		Elite::Vector2 playerPos{ pAgent->Position };

		//Get closest house
		Elite::Vector2 closestHouse{ pWorldDivider->GetAllVisitedHouses()[0] };
		float distance{ closestHouse.DistanceSquared(playerPos) };
		for (const Elite::Vector2& house : pWorldDivider->GetVisitedHouses())
		{
			float newDistance{ house.DistanceSquared(playerPos) };
			if (newDistance < distance)
			{
				closestHouse = house;
				distance = newDistance;
			}
		}

		//Get navmesh point to point and update blackboard
		Elite::Vector2 point = pInterface->NavMesh_GetClosestPathPoint(closestHouse);
		pSteering->SetRunMode(true);
		pSteering->SetToSeek(point);

		return Elite::BehaviorState::Running;
	}
	
	//----------------------
	//Loot Handling
	//----------------------
	Elite::BehaviorState PickUpItem(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		Steering* pSteering;
		IExamInterface* pInterface;
		InventoryManagement* pInventory;
		std::vector<EntityInfo>* pEntitiesVec;

		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pEntitiesVec->empty())
		{
			return Elite::BehaviorState::Failure;
		}

		//Checking if the entity is an item
		std::vector<Elite::Vector2> Items;
		std::vector<size_t> entityIndex;
		for (size_t index = 0; index < pEntitiesVec->size(); index++)
		{
			if ((*pEntitiesVec)[index].Type == eEntityType::ITEM)
			{
				Items.push_back((*pEntitiesVec)[index].Location);
				entityIndex.push_back(index);
			}
		}
		if (Items.empty())
		{
			return Elite::BehaviorState::Failure;
		}

		//Getting closest item
		float distance{ Items[0].DistanceSquared(pAgent->Position) };
		size_t closestIndex{ 0 };
		for (size_t index{ 1 }; index < Items.size(); ++index)
		{
			float checkDistance{ Items[index].DistanceSquared(pAgent->Position) };

			if (distance > checkDistance)
			{
				distance = checkDistance;
				closestIndex = index;
			}
		}

		//Convert to itemInfo
		EntityInfo closestEntity{ (*pEntitiesVec)[closestIndex] };
		ItemInfo item;
		if (pInterface->Item_GetInfo(closestEntity, item) == false)
		{
			return Elite::BehaviorState::Failure;
		}

		if (distance >= pAgent->GrabRange)
		{
			pSteering->SetRunMode(false);
			pSteering->SetToSeek(Items[closestIndex]);
			return Elite::BehaviorState::Running;
		}

		if (pInterface->Item_Grab(closestEntity, item))
		{
			pInventory->AddItem(pInterface, item);
			return Elite::BehaviorState::Success;
		}

		return Elite::BehaviorState::Failure;
	}



	//----------------------
	//Other
	//----------------------
	Elite::BehaviorState FleeFromPurgeZone(Elite::Blackboard* pBlackboard)
	{
		//Retrieving valid data from blackboard
		AgentInfo* pAgent;
		Steering* pSteering;
		IExamInterface* pInterface;
		std::vector<EntityInfo>* pEntitiesVec;

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}
		if (pEntitiesVec->empty())
		{
			return Elite::BehaviorState::Failure;
		}

		//Getting al purge zones
		std::vector<Elite::Vector2> purges;
		for (auto& Entity : *pEntitiesVec)
		{
			if (Entity.Type == eEntityType::PURGEZONE)
			{
				purges.push_back(Entity.Location);
			}
		}

		if (purges.empty())
		{
			return Elite::BehaviorState::Failure;
		}

		float distance{ purges[0].DistanceSquared(pAgent->Position) };
		size_t closestIndex{ 0 };

		for (size_t index{ 1 }; index < purges.size(); ++index)
		{
			float checkDistance{ purges[index].DistanceSquared(pAgent->Position) };

			if (distance > checkDistance)
			{
				distance = checkDistance;
				closestIndex = index;
			}
		}


		Elite::Vector2 purgeCenter{ purges[closestIndex] };
		Elite::Vector2 fromTarget = (pAgent->Position - purgeCenter) * 5;

		Elite::Vector2 goToPoint{ pInterface->NavMesh_GetClosestPathPoint(fromTarget) };

		pSteering->SetRunMode(true);
		pSteering->SetToSeek(goToPoint);
		return Elite::BehaviorState::Running;
	}

}

//-----------------------------------------------------------------
// Behaviors -- Condition
//-----------------------------------------------------------------
namespace BT_Conditions
{
	//------------------
	//House looting
	//------------------
	bool SeeNewHouse(Elite::Blackboard* pBlackboard)
	{
		//retrieving valid data from blackboard
		AgentInfo* pAgent;
		bool enteringHouse;
		IExamInterface* pInterface;
		WorldDivider* pWorldDivider;
		std::vector<HouseInfo>* pHouseVec;

		if (pBlackboard->GetData("EnteringHouse", enteringHouse) == false)
		{
			return false;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return false;
		}

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		if (pBlackboard->GetData("Houses", pHouseVec) == false || pHouseVec == nullptr)
		{
			return false;
		}		

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return false;
		}


		if (pAgent->IsInHouse)
		{
			return false;
		}

		//Check if house is visited yet
		std::vector<HouseInfo> NotVisitedHouse;
		for (const HouseInfo currentHouse : (*pHouseVec))
		{
			if (pWorldDivider->constainsHouse(currentHouse.Center) == false)
			{
				NotVisitedHouse.push_back(currentHouse);
			}
		}

		if (NotVisitedHouse.empty())
		{
			return false;
		}

		return true;
	}
	bool IsInHouse(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		return pAgent->IsInHouse;
	}

	//------------------
	//Loot Handling
	//------------------
	bool SeeItem(Elite::Blackboard* pBlackboard)
	{
		//Retrieving valid data from blackboard
		AgentInfo* pAgent;
		std::vector<EntityInfo>* pEntitiesVec;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return false;
		}
		if (pEntitiesVec->empty())
		{
			return false;
		}

		//Checking if the entity is an item
		std::vector<Elite::Vector2> Items;
		std::vector<size_t> entityIndex;
		for (size_t index = 0; index < pEntitiesVec->size(); index++)
		{
			if ((*pEntitiesVec)[index].Type == eEntityType::ITEM)
			{
				Items.push_back((*pEntitiesVec)[index].Location);
				entityIndex.push_back(index);
			}
		}

		if (Items.empty())
		{
			return false;
		}

		return true;
	}	

	//------------------
	//Enemy handling
	//------------------
	bool EnemyPresence(Elite::Blackboard* pBlackboard)
	{
		//retrieving valid data from blackboard
		AgentInfo* pAgent;
		std::vector<EntityInfo>* pEntitiesVec;
		bool wasAttacked;
		if (pBlackboard->GetData("attacked", wasAttacked) == false)
		{
			return false;
		}
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return false;
		}

		if (wasAttacked)
		{
			return true;
		}
		
		if (pAgent->WasBitten)
		{
			return true;
		}

		if (pEntitiesVec->empty())
		{
			return false;
		}
		
		//Putting al visible enemies in vector
		std::vector<Elite::Vector2> enemies;
		for (auto& Entity : *pEntitiesVec)
		{
			if (Entity.Type == eEntityType::ENEMY)
			{
				enemies.push_back(Entity.Location);
			}
		}

		//Checking if there are any enemies
		if (enemies.empty() == false)
		{
			return true;
		}

		return false;
	}
	
	bool IsInSight(Elite::Blackboard* pBlackboard)
	{	
		Elite::Vector2 enemyPos;
		if (pBlackboard->GetData("Target", enemyPos) == false)
		{
			return false;
		}

		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		float widthToEnemy{	 enemyPos.x - pAgent->Position.x };
		float heightToEnemy{ enemyPos.y - pAgent->Position.y };

		float angleToEnemy{ atan2f(heightToEnemy, widthToEnemy ) };

		float angleToEnemyDeg{	Elite::ToDegrees(angleToEnemy) };
		float playerAngleDeg{	Elite::ToDegrees(pAgent->Orientation) };

		float angleOffset{ 5.f };

		if (angleToEnemyDeg > playerAngleDeg - angleOffset && angleToEnemyDeg < playerAngleDeg + angleOffset)
		{
			return true;
		}

		return false;
	}
	bool HasAmmo(Elite::Blackboard* pBlackboard)
	{
		//Get weapon slot and shoot
		InventoryManagement* pInventory;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return false;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return false;
		}


		if (pInventory->GetAmmo(pInterface) > 0)
		{
			return true;
		}
		return false;
	}


	//------------------
	//Purge zone
	//------------------
	bool SeePurgeZone(Elite::Blackboard* pBlackboard)
	{
		//Retrieving valid data from blackboard
		AgentInfo* pAgent;
		std::vector<EntityInfo>* pEntitiesVec;

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}
		if (pBlackboard->GetData("Entities", pEntitiesVec) == false || pEntitiesVec == nullptr)
		{
			return false;
		}
		if (pEntitiesVec->empty())
		{
			return false;
		}

		//Getting al purge zones
		std::vector<Elite::Vector2> purges;
		for (auto& Entity : *pEntitiesVec)
		{
			if (Entity.Type == eEntityType::PURGEZONE)
			{
				purges.push_back(Entity.Location);
			}
		}

		if (purges.empty())
		{
			return false;
		}
		return true;
	}


	//------------------
	//Other
	//------------------	
	bool LowOnHealth(Elite::Blackboard* pBlackboard)
	{
		//Retrieving valid data from blackboard
		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		//If health is lower then 5
		if (pAgent->Health <= 5)
		{
			return true;
		}

		return false;
	}
	bool LowOnEnergy(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		if (pAgent->Energy <= 5)
		{
			return true;
		}

		return false;
	}


	bool IsTimerDone(Elite::Blackboard* pBlackboard)
	{
		Timer* pTimer;
		if (pBlackboard->GetData("Timer", pTimer) == false || pTimer == nullptr)
		{
			return false;
		}

		return pTimer->IsDone();
	}



}
#endif