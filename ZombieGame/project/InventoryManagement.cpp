#include "stdafx.h"

#include "InventoryManagement.h"

InventoryManagement::InventoryManagement()
{

}

InventoryManagement::~InventoryManagement()
{

}


//Struggles when item slot isnt empty and with pistol
bool InventoryManagement::AddItem(IExamInterface* pInterface, ItemInfo& groundItem)
{
	ItemInfo equipedItem;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Garbage), equipedItem) == true)
	{
		//Throw away equiped item
		pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Garbage));
	}

	switch (groundItem.Type)
	{
	case eItemType::PISTOL:
		if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Pistol), equipedItem) == true)
		{
			if (pInterface->Weapon_GetAmmo(groundItem) > pInterface->Weapon_GetAmmo(equipedItem))
			{
				pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Pistol));
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Pistol), groundItem);
			}
			else
			{
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Garbage), groundItem);
			}
		}
		else
		{
			pInterface->Inventory_AddItem(static_cast<UINT>(slot::Pistol), groundItem);
		}
		break;
	case eItemType::SHOTGUN:
		if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Shotgun), equipedItem) == true)
		{
			if (pInterface->Weapon_GetAmmo(groundItem) > pInterface->Weapon_GetAmmo(equipedItem))
			{
				pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Shotgun));
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Shotgun), groundItem);
			}
			else
			{
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Garbage), groundItem);
			}
		}
		else
		{
			pInterface->Inventory_AddItem(static_cast<UINT>(slot::Shotgun), groundItem);
		}
		break;
	case eItemType::MEDKIT:
		if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Healing), equipedItem) == true)
		{
			if (pInterface->Medkit_GetHealth(groundItem) > pInterface->Medkit_GetHealth(equipedItem))
			{
				//Switch to ground item
				pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Healing));
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Healing), groundItem);
			}
			else
			{
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Garbage), groundItem);
			}
		}
		else
		{
			pInterface->Inventory_AddItem(static_cast<UINT>(slot::Healing), groundItem);
		}
		
		break;
	case eItemType::FOOD:
		if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Food), equipedItem) == true)
		{
			if (pInterface->Food_GetEnergy(groundItem) > pInterface->Food_GetEnergy(equipedItem))
			{
				//Switch to ground item
				pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Food));
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Food), groundItem);
			}
			else
			{
				pInterface->Inventory_AddItem(static_cast<UINT>(slot::Garbage), groundItem);
			}
		}
		else
		{
			pInterface->Inventory_AddItem(static_cast<UINT>(slot::Food), groundItem);
		}
		
		break;
	case eItemType::GARBAGE:
	default:
		if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Garbage), equipedItem) == true)
		{
			//Throw away equiped item
			pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Garbage));
		}
		//Throw away ground item
		pInterface->Inventory_AddItem(static_cast<UINT>(slot::Garbage),groundItem);
		pInterface->Inventory_RemoveItem(static_cast<UINT>(slot::Garbage));
		break;
	}
	return true;
}

void InventoryManagement::Manage(IExamInterface* pInterface)
{
	
}

bool InventoryManagement::EatFood(IExamInterface* pInterface)
{
	ItemInfo food;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Food), food) == false)
	{
		return false;
	}

	if (pInterface->Food_GetEnergy(food) == 0)
	{
		return false;
	}

	return pInterface->Inventory_UseItem(static_cast<UINT>(slot::Food));
}
bool InventoryManagement::Heal(IExamInterface* pInterface)
{
	ItemInfo medkit;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Healing), medkit) == false)
	{
		return false;
	}

	if (pInterface->Medkit_GetHealth(medkit) == 0)
	{
		return false;
	}
	return pInterface->Inventory_UseItem(static_cast<UINT>(slot::Healing));
}
bool InventoryManagement::Shoot(IExamInterface* pInterface)
{
	ItemInfo pistol;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Pistol), pistol) == false || pInterface->Weapon_GetAmmo(pistol) == 0)
	{
		ItemInfo shotgun;
		if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Shotgun), shotgun) == false)
		{
			return false;
		}
		return pInterface->Inventory_UseItem(static_cast<UINT>(slot::Shotgun));

	}
	return pInterface->Inventory_UseItem(static_cast<UINT>(slot::Pistol));
}

int InventoryManagement::GetAmmo(IExamInterface* pInterface) const
{
	int totalAmmo{ 0 };

	ItemInfo pistol;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Pistol), pistol) == true)
	{
		totalAmmo += pInterface->Weapon_GetAmmo(pistol);
	}

	ItemInfo shotgun;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Shotgun), shotgun) == true)
	{
		totalAmmo += pInterface->Weapon_GetAmmo(shotgun);
	}

	return totalAmmo;
}
int InventoryManagement::GetHealth(IExamInterface* pInterface) const
{
	ItemInfo healing;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Healing), healing) == true)
	{
		return pInterface->Medkit_GetHealth(healing);
	}

	return 0;
}
int InventoryManagement::GetEnergy(IExamInterface* pInterface) const
{
	ItemInfo food;
	if (pInterface->Inventory_GetItem(static_cast<UINT>(slot::Food), food) == true)
	{
		return pInterface->Food_GetEnergy(food);
	}

	return 0;
}


ItemInfo InventoryManagement::GetPistol(IExamInterface* pInterface)
{
	ItemInfo weapon;
	pInterface->Inventory_GetItem(static_cast<UINT>(slot::Pistol), weapon);
	return weapon;
}
ItemInfo InventoryManagement::GetShotgun(IExamInterface* pInterface)
{
	ItemInfo weapon;
	pInterface->Inventory_GetItem(static_cast<UINT>(slot::Shotgun), weapon);
	return weapon;
}
ItemInfo InventoryManagement::GetHealing(IExamInterface* pInterface)
{
	ItemInfo healing;
	pInterface->Inventory_GetItem(static_cast<UINT>(slot::Healing), healing);
	return healing;
}
ItemInfo InventoryManagement::GetFood(IExamInterface* pInterface)
{
	ItemInfo food;
	pInterface->Inventory_GetItem(static_cast<UINT>(slot::Food), food);
	return food;
}