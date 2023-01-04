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
#include "Timer.h"
#include "WorldDivider.h"

//-----------------------------------------------------------------
// Behaviors -- Actions
//-----------------------------------------------------------------
namespace BT_Actions
{
	Elite::BehaviorState ChangeToWander(Elite::Blackboard* pBlackboard)
	{
		Steering* pSteering;
		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

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

		pSteering->SetToFace(targetPos);
		return Elite::BehaviorState::Success;
	}

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

	Elite::BehaviorState LookAround(Elite::Blackboard* pBlackboard)
	{
		//Rotate agent
		Steering* pSteering;
		if (pBlackboard->GetData("Steering", pSteering) == false || pSteering == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		pSteering->SetToRotateLeft();

		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState TurnAround(Elite::Blackboard* pBlackboard)
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

		pSteering->SetToTurnAround(targetPos);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState PickUpItem(Elite::Blackboard* pBlackboard)
	{
		ItemInfo* pItem;
		EntityInfo* pEnity;
		IExamInterface* pInterface;
		InventoryManagement* pInventory;

		if (pBlackboard->GetData("Inventory", pInventory) == false || pInventory == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Item", pItem) == false || pItem == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("Entity", pEnity) == false || pEnity == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pInterface->Item_Grab(*pEnity, *pItem))
		{
			pInventory->AddItem(pInterface, *pItem);
			return Elite::BehaviorState::Success;
		}

		return Elite::BehaviorState::Failure;
	}

	Elite::BehaviorState EnterHouse(Elite::Blackboard* pBlackboard)
	{
		HouseInfo* pHouse;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("House", pHouse) == false || pHouse == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		Elite::Vector2 point = pInterface->NavMesh_GetClosestPathPoint(pHouse->Center);

		pBlackboard->ChangeData("House", pHouse);
		pBlackboard->ChangeData("Target", point);
		pBlackboard->ChangeData("EnteringHouse", true);

		return Elite::BehaviorState::Success;
	}
	Elite::BehaviorState LeaveHouse(Elite::Blackboard* pBlackboard)
	{
		HouseInfo* pHouse;
		IExamInterface* pInterface;
		
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

		pBlackboard->ChangeData("Target", goToPoint);
		pBlackboard->ChangeData("EnteringHouse", false);

		return Elite::BehaviorState::Success;
	}

	Elite::BehaviorState LootHouse(Elite::Blackboard* pBlackboard)
	{
		HouseInfo* pHouse;
		AgentInfo* pAgent;

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("House", pHouse) == false || pHouse == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		static int currentStage{ 0 };
		float wallOffset{ 5.f };

		float halfSizeX{ pHouse->Size.x / 2 };
		float halfSizeY{ pHouse->Size.y / 2};
		
		static std::vector<Elite::Vector2> lootingStages
		{
			{	pHouse->Center },
			{	pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y + halfSizeY - wallOffset },
			{	pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y + halfSizeY - wallOffset },
			{	pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y - halfSizeY + wallOffset },
			{	pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y - halfSizeY + wallOffset }
		};


		lootingStages[0] = { pHouse->Center };
		lootingStages[1] = { pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y + halfSizeY - wallOffset };
		lootingStages[2] = { pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y + halfSizeY - wallOffset };
		lootingStages[3] = { pHouse->Center.x + halfSizeX - wallOffset, pHouse->Center.y - halfSizeY + wallOffset };
		lootingStages[4] = { pHouse->Center.x - halfSizeX + wallOffset, pHouse->Center.y - halfSizeY + wallOffset };



		pBlackboard->ChangeData("Target", lootingStages[currentStage]);

		float range{ 2.f };

		if (pAgent->Position.DistanceSquared(lootingStages[currentStage]) <= range)
		{
			++currentStage;
		}

		if (currentStage >= 5)
		{
			pBlackboard->ChangeData("Looting", false);
			currentStage = 0;
			return Elite::BehaviorState::Failure;
		}

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

	Elite::BehaviorState GoToDestinationQuadrant(Elite::Blackboard* pBlackboard)
	{
		WorldDivider* pWorldDivider;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return Elite::BehaviorState::Failure;
		}

		Elite::Vector2 point = pInterface->NavMesh_GetClosestPathPoint(pWorldDivider->GetCenterOfQuadrant(pWorldDivider->GetDestinationQuadrant()));
		pBlackboard->ChangeData("Target", point);

		return Elite::BehaviorState::Success;
	}
}

//-----------------------------------------------------------------
// Behaviors -- Condition
//-----------------------------------------------------------------
namespace BT_Conditions
{
	bool SeeHouse(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		std::vector<HouseInfo>* pHouseVec;
		IExamInterface* pInterface;
		bool enteringHouse;

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

		HouseInfo house;
		pInterface->Fov_GetHouseByIndex(0, house);

		if (pHouseVec->empty() == false )
		{
			HouseInfo* pHouse = new HouseInfo(house);
			
			if (pAgent->IsInHouse == false)
			{
				pBlackboard->ChangeData("Looting", true);
			}

			pBlackboard->ChangeData("House", pHouse);
			return true;
		}
		return false;
	}
	
	bool HasLootedHouse(Elite::Blackboard* pBlackboard)
	{
		bool isLooting;
		if (pBlackboard->GetData("Looting", isLooting) == false)
		{
			return false;
		}

		if (isLooting == true)
		{
			return false;
		}


		//TODO better check
		HouseInfo* pHouse;
		std::vector<HouseInfo>* pHouseVec;
		IExamInterface* pInterface;

		if (pBlackboard->GetData("Houses", pHouseVec) == false || pHouseVec == nullptr)
		{
			return false;
		}

		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return false;
		}

		if (pHouseVec->empty())
		{
			return false;
		}

		HouseInfo* pPrevHouse;
		if (pBlackboard->GetData("House", pPrevHouse) == false || pPrevHouse == nullptr)
		{
			return false;
		}

		if ((*pHouseVec)[0].Center.x == pPrevHouse->Center.x)
		{
			return true;
		}
		return false;
	}

	bool SeeEnemy(Elite::Blackboard* pBlackboard)
	{
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

		std::vector<Elite::Vector2> enemies;

		for (auto& Entity : *pEntitiesVec)
		{
			if (Entity.Type == eEntityType::ENEMY)
			{
				enemies.push_back(Entity.Location);
			}
		}

		if (enemies.empty())
		{
			return false;
		}

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

		pBlackboard->ChangeData("Target", enemies[closestIndex]);
		return true;
	}
	
	bool SeeItem(Elite::Blackboard* pBlackboard)
	{
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

		IExamInterface* pInterface;
		if (pBlackboard->GetData("Interface", pInterface) == false || pInterface == nullptr)
		{
			return false;
		}

		//Convert to itemInfo
		EntityInfo closestEntity{ (*pEntitiesVec)[closestIndex] };
		ItemInfo item;
		if (pInterface->Item_GetInfo(closestEntity, item) == false)
		{
			return false;
		}

		pBlackboard->ChangeData("Entity",	&closestEntity);
		pBlackboard->ChangeData("Item",		&item);
		pBlackboard->ChangeData("Target",	Items[closestIndex]);
		return true;
	}

	bool SeePurgeZone(Elite::Blackboard* pBlackboard)
	{
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

		pBlackboard->ChangeData("Target", purges[closestIndex]);
		return true;
	}

	bool LowOnHealth(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

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

	bool WasBitten(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		return pAgent->WasBitten;
	}

	bool CanGrabItem(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		ItemInfo* pItem;

		if (pBlackboard->GetData("Item", pItem) == false || pItem == nullptr)
		{
			return false;
		}

		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		float distanceToItem{ pAgent->Position.DistanceSquared(pItem->Location) };

		if (pAgent->GrabRange >= distanceToItem)
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

		return true;
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

	//TODO add inverter for conditions
	bool IsInHouse(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}
		return pAgent->IsInHouse;
	}
	bool NotInHouse(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		return !pAgent->IsInHouse;
	}

	bool IsEnteringHouse(Elite::Blackboard* pBlackboard)
	{
		AgentInfo* pAgent;
		if (pBlackboard->GetData("Agent", pAgent) == false || pAgent == nullptr)
		{
			return false;
		}

		bool enteringHouse;
		if (pBlackboard->GetData("EnteringHouse", enteringHouse) == false)
		{
			return false;
		}

		if (enteringHouse == true && pAgent->IsInHouse == false)
		{
			pBlackboard->ChangeData("Looting", enteringHouse);
		}
		return enteringHouse;
	}

	bool IsLooting(Elite::Blackboard* pBlackboard)
	{
		bool isLooting;
		if (pBlackboard->GetData("Looting", isLooting) == false)
		{
			return false;
		}

		return isLooting;
	}

	bool IsAiming(Elite::Blackboard* pBlackboard)
	{
		bool isAiming;
		if (pBlackboard->GetData("IsAiming", isAiming) == false)
		{
			return false;
		}

		return isAiming;
	}


	bool IsDestinationNotEqualToCurrentQuadrant(Elite::Blackboard* pBlackboard)
	{
		WorldDivider* pWorldDivider;
		if (pBlackboard->GetData("WorldDivider", pWorldDivider) == false || pWorldDivider == nullptr)
		{
			return false;
		}

		if (pWorldDivider->GetCurrentQuadrant() != pWorldDivider->GetDestinationQuadrant())
		{
			return true;
		}

		return false;
	}
}
#endif