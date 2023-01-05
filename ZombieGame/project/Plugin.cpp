#include "stdafx.h"
#include "Plugin.h"
#include "IExamInterface.h"

using namespace std;

//Called only once, during initialization
void Plugin::Initialize(IBaseInterface* pInterface, PluginInfo& info)
{
	//Retrieving the interface
	//This interface gives you access to certain actions the AI_Framework can perform for you
	m_pInterface = static_cast<IExamInterface*>(pInterface);

	//Bit information about the plugin
	//Please fill this in!!
	info.BotName = "MinionExam";
	info.Student_FirstName = "Jonas";
	info.Student_LastName = "Samyn";
	info.Student_Class = "2DAE15";

	m_pInventoryManager = new InventoryManagement();
	m_pSteering = new Steering();
	m_pTimer = new Timer(1000);
	m_pWorldDivider = new WorldDivider(m_pInterface->World_GetInfo().Center, m_pInterface->World_GetInfo().Dimensions);

	m_pInterface->World_GetInfo();

	//Adding blackboard info
	m_pBlackboard = new Elite::Blackboard();

	//Pointers
	m_pBlackboard->AddData("Agent",			&m_pInterface->Agent_GetInfo());
	m_pBlackboard->AddData("Interface",		m_pInterface);
	m_pBlackboard->AddData("Steering",		m_pSteering);
	m_pBlackboard->AddData("Inventory",		m_pInventoryManager);
	m_pBlackboard->AddData("WorldDivider",	m_pWorldDivider);
	m_pBlackboard->AddData("Timer",			m_pTimer);

	//Entity information
	m_pBlackboard->AddData("Item",			static_cast<ItemInfo*>(nullptr));
	m_pBlackboard->AddData("Entity",		static_cast<EntityInfo*>(nullptr));
	m_pBlackboard->AddData("House",			static_cast<HouseInfo*>(nullptr));
	m_pBlackboard->AddData("Target",		Elite::Vector2{});

	//Fov vectors
	m_pBlackboard->AddData("Entities",		&GetEntitiesInFOV());
	m_pBlackboard->AddData("Houses",		&GetHousesInFOV());

	//normal variables
	m_pBlackboard->AddData("EnteringHouse", bool{ false });
	m_pBlackboard->AddData("Looted",		bool{ false });
	m_pBlackboard->AddData("currentLoot",	int{ 0 });
	m_pBlackboard->AddData("attacked",		bool{ false });


	//Behavior tree
	m_pBehaviorTree = new Elite::BehaviorTree(m_pBlackboard, new Elite::BehaviorSelector
	(
		{
			//new Elite::BehaviorAction(BT_Actions::RotateLeft),


			//Check for health or energy
			new Elite::BehaviorSelector
			(
				{
					new Elite::BehaviorSequence
					(
						{
							new Elite::BehaviorConditional(BT_Conditions::LowOnEnergy),
							new Elite::BehaviorAction(BT_Actions::Eat)
						}
					),
					new Elite::BehaviorSequence
					(
						{
							new Elite::BehaviorConditional(BT_Conditions::LowOnHealth),
							new Elite::BehaviorAction(BT_Actions::Heal)
						}
					)
				}
			),

			//PurgeZone Handling
			new Elite::BehaviorSequence
			(
				{
					new Elite::BehaviorConditional(BT_Conditions::SeePurgeZone),
					new Elite::BehaviorAction(BT_Actions::FleeFromPurgeZone)
				}
			),

			//Enemy handling
			new Elite::BehaviorSequence
			(
				{
					new Elite::BehaviorConditional(BT_Conditions::EnemyPresence),
					new Elite::BehaviorSelector
					(
						{
							new Elite::BehaviorSequence
							(
								{
									new Elite::BehaviorConditional(BT_Conditions::HasAmmo),
									new Elite::BehaviorAction(BT_Actions::AttackEnemy),
									new Elite::BehaviorAction(BT_Actions::ShootWeapon)
								}
							),
							new Elite::BehaviorSequence
							(
								{
									new Elite::BehaviorAction(BT_Actions::GoToClosestHouse)
								}
							)
						}
					),
				}
			),

			//Item handling
			new Elite::BehaviorSequence
			(
				{
					new Elite::BehaviorConditional(BT_Conditions::SeeItem),
					new Elite::BehaviorAction(BT_Actions::PickUpItem)
				}
			),

			//Looting handling
			new Elite::BehaviorSequence
			(
				{
					new Elite::BehaviorSelector
					(
						{
							new Elite::BehaviorConditional(BT_Conditions::SeeNewHouse),
							new Elite::BehaviorConditional(BT_Conditions::IsInHouse),
							new Elite::BehaviorAction(BT_Actions::hasLeftHouse)
						}
					),
					new Elite::BehaviorAction(BT_Actions::EnterHouse),
					new Elite::BehaviorAction(BT_Actions::LootHouse),
					new Elite::BehaviorAction(BT_Actions::LeaveHouse)
				}
			),

			//World exploration
			new Elite::BehaviorSequence
			(
				{
					new Elite::BehaviorAction(BT_Actions::GoToNextGrid)
				}
			),


			//Fall back to wander
			new Elite::BehaviorAction(BT_Actions::ChangeToWander)
		}
	));
}

//Called only once
void Plugin::DllInit()
{
	//Called when the plugin is loaded
}

//Called only once
void Plugin::DllShutdown()
{
	//Called wheb the plugin gets unloaded
}

//Called only once, during initialization
void Plugin::InitGameDebugParams(GameDebugParams& params)
{
	params.AutoFollowCam = true; //Automatically follow the AI? (Default = true)
	params.RenderUI = true; //Render the IMGUI Panel? (Default = true)
	params.SpawnEnemies = true; //Do you want to spawn enemies? (Default = true)
	params.EnemyCount = 20; //How many enemies? (Default = 20)
	params.GodMode = false; //GodMode > You can't die, can be useful to inspect certain behaviors (Default = false)
	params.LevelFile = "GameLevel.gppl";
	params.AutoGrabClosestItem = true; //A call to Item_Grab(...) returns the closest item that can be grabbed. (EntityInfo argument is ignored)
	params.StartingDifficultyStage = 1;
	params.InfiniteStamina = false;
	params.SpawnDebugPistol = true;
	params.SpawnDebugShotgun = true;
	params.SpawnPurgeZonesOnMiddleClick = true;
	params.PrintDebugMessages = true;
	params.ShowDebugItemNames = true;
	params.SpawnZombieOnRightClick = true;
	params.Seed = 36;
}

//Only Active in DEBUG Mode
//(=Use only for Debug Purposes)
void Plugin::Update(float dt)
{
	//Demo Event Code
	//In the end your AI should be able to walk around without external input
	if (m_pInterface->Input_IsMouseButtonUp(Elite::InputMouseButton::eLeft))
	{
		//Update target based on input
		Elite::MouseData mouseData = m_pInterface->Input_GetMouseData(Elite::InputType::eMouseButton, Elite::InputMouseButton::eLeft);
		const Elite::Vector2 pos = Elite::Vector2(static_cast<float>(mouseData.X), static_cast<float>(mouseData.Y));
		m_Target = m_pInterface->Debug_ConvertScreenToWorld(pos);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Space))
	{
		m_CanRun = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Left))
	{
		m_AngSpeed -= Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Right))
	{
		m_AngSpeed += Elite::ToRadians(10);
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_G))
	{
		m_GrabItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_U))
	{
		m_UseItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_R))
	{
		m_RemoveItem = true;
	}
	else if (m_pInterface->Input_IsKeyboardKeyUp(Elite::eScancode_Space))
	{
		m_CanRun = false;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Delete))
	{
		m_pInterface->RequestShutdown();
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Minus))
	{
		if (m_InventorySlot > 0)
			--m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_KP_Plus))
	{
		if (m_InventorySlot < 4)
			++m_InventorySlot;
	}
	else if (m_pInterface->Input_IsKeyboardKeyDown(Elite::eScancode_Q))
	{
		ItemInfo info = {};
		m_pInterface->Inventory_GetItem(m_InventorySlot, info);
		std::cout << (int)info.Type << std::endl;
	}
}

//Update
//This function calculates the new SteeringOutput, called once per frame
//SteeringPlugin_Output Plugin::UpdateSteering(float dt)
//{
//	auto steering = SteeringPlugin_Output();
//	
//	//Use the Interface (IAssignmentInterface) to 'interface' with the AI_Framework
//	auto agentInfo = m_pInterface->Agent_GetInfo();
//
//
//	//Use the navmesh to calculate the next navmesh point
//	//auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(checkpointLocation);
//
//	//OR, Use the mouse target
//	auto nextTargetPos = m_pInterface->NavMesh_GetClosestPathPoint(m_Target); //Uncomment this to use mouse position as guidance
//
//	auto vHousesInFOV = GetHousesInFOV();		//uses m_pInterface->Fov_GetHouseByIndex(...)
//	auto vEntitiesInFOV = GetEntitiesInFOV();	//uses m_pInterface->Fov_GetEntityByIndex(...)
//
//	for (auto& e : vEntitiesInFOV)
//	{
//		if (e.Type == eEntityType::PURGEZONE)
//		{
//			PurgeZoneInfo zoneInfo;
//			m_pInterface->PurgeZone_GetInfo(e, zoneInfo);
//			//std::cout << "Purge Zone in FOV:" << e.Location.x << ", "<< e.Location.y << "---Radius: "<< zoneInfo.Radius << std::endl;
//		}
//	}
//
//	//INVENTORY USAGE DEMO
//	//********************
//
//	if (m_GrabItem)
//	{
//		ItemInfo item;
//		//Item_Grab > When DebugParams.AutoGrabClosestItem is TRUE, the Item_Grab function returns the closest item in range
//		//Keep in mind that DebugParams are only used for debugging purposes, by default this flag is FALSE
//		//Otherwise, use GetEntitiesInFOV() to retrieve a vector of all entities in the FOV (EntityInfo)
//		//Item_Grab gives you the ItemInfo back, based on the passed EntityHash (retrieved by GetEntitiesInFOV)
//		if (m_pInterface->Item_Grab({}, item))
//		{
//			//Once grabbed, you can add it to a specific inventory slot
//			//Slot must be empty
//			m_pInterface->Inventory_AddItem(m_InventorySlot, item);
//		}
//	}
//
//	if (m_UseItem)
//	{
//		//Use an item (make sure there is an item at the given inventory slot)
//		m_pInterface->Inventory_UseItem(m_InventorySlot);
//	}
//
//	if (m_RemoveItem)
//	{
//		//Remove an item from a inventory slot
//		m_pInterface->Inventory_RemoveItem(m_InventorySlot);
//	}
//
//	//Simple Seek Behaviour (towards Target)
//	steering.LinearVelocity = nextTargetPos - agentInfo.Position; //Desired Velocity
//	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
//	steering.LinearVelocity *= agentInfo.MaxLinearSpeed; //Rescale to Max Speed
//
//	if (Distance(nextTargetPos, agentInfo.Position) < 2.f)
//	{
//		steering.LinearVelocity = Elite::ZeroVector2;
//	}
//
//	//steering.AngularVelocity = m_AngSpeed; //Rotate your character to inspect the world while walking
//	steering.AutoOrient = true; //Setting AutoOrient to TRue overrides the AngularVelocity
//
//	steering.RunMode = m_CanRun; //If RunMode is True > MaxLinSpd is increased for a limited time (till your stamina runs out)
//
//	//SteeringPlugin_Output is works the exact same way a SteeringBehaviour output
//
////@End (Demo Purposes)
//	m_GrabItem = false; //Reset State
//	m_UseItem = false;
//	m_RemoveItem = false;
//
//	return steering;
//}

SteeringPlugin_Output Plugin::UpdateSteering(float dt)
{
	auto steering = SteeringPlugin_Output();

	auto agentInfo = m_pInterface->Agent_GetInfo();

	bool wasAttacked{};
	m_pBlackboard->GetData("attacked", wasAttacked);

	if (agentInfo.WasBitten == true && wasAttacked == false)
	{
		m_pBlackboard->ChangeData("attacked", true);
		m_pTimer->Reset();
	}
	
	m_pTimer->Update(dt);
	if (m_pTimer->IsDone() && wasAttacked == true)
	{
		m_pBlackboard->ChangeData("attacked", false);
	}

	auto vHousesInFOV = GetHousesInFOV();
	auto vEntitiesInFOV = GetEntitiesInFOV();

	m_pWorldDivider->Update(dt, agentInfo.Position);
	
	m_pInventoryManager->Manage(m_pInterface);
	m_pBlackboard->ChangeData("Agent", &agentInfo);
	m_pBlackboard->ChangeData("Houses", &vHousesInFOV);
	m_pBlackboard->ChangeData("Entities", &vEntitiesInFOV);

	m_pBehaviorTree->Update(dt);

	steering = m_pSteering->GetSteering(dt, &agentInfo);

	return steering;
}

//This function should only be used for rendering debug elements
void Plugin::Render(float dt) const
{
	m_pWorldDivider->Render(m_pInterface);

	//This Render function should only contain calls to Interface->Draw_... functions
	m_pInterface->Draw_SolidCircle(m_Target, .7f, { 0,0 }, { 1, 0, 0 });

	Elite::Vector2 target;
	m_pBlackboard->GetData("Target", target);
	m_pInterface->Draw_SolidCircle(target, .7f, { 0,0 }, { 1, 0, 0 });

	m_pInterface->Draw_SolidCircle(m_pSteering->GetTarget(), .7f, {0,0}, { 0, 1, 0 });
}

vector<HouseInfo> Plugin::GetHousesInFOV() const
{
	vector<HouseInfo> vHousesInFOV = {};

	HouseInfo hi = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetHouseByIndex(i, hi))
		{
			vHousesInFOV.push_back(hi);
			continue;
		}

		break;
	}

	return vHousesInFOV;
}
vector<EntityInfo> Plugin::GetEntitiesInFOV() const
{
	vector<EntityInfo> vEntitiesInFOV = {};

	EntityInfo ei = {};
	for (int i = 0;; ++i)
	{
		if (m_pInterface->Fov_GetEntityByIndex(i, ei))
		{
			vEntitiesInFOV.push_back(ei);
			continue;
		}

		break;
	}

	return vEntitiesInFOV;
}
