#pragma once
#include "Exam_HelperStructs.h"

#include "IExamInterface.h"

class InventoryManagement final
{
public:
	const enum class slot
	{
		Pistol	= 0,
		Shotgun = 1,
		Healing = 2,
		Food	= 3,
		Garbage = 4
	};

	InventoryManagement();
	~InventoryManagement();

	void Manage(IExamInterface* pInterface);
	
	bool AddItem(IExamInterface* pInterface, ItemInfo& groundItem);

	bool EatFood(IExamInterface* pInterface);
	bool Heal(IExamInterface* pInterface);
	bool Shoot(IExamInterface* pInterface);

	int GetAmmo(IExamInterface* pInterface) const;
	int GetHealth(IExamInterface* pInterface) const;
	int GetEnergy(IExamInterface* pInterface) const;


	ItemInfo& GetPistol(IExamInterface* pInterface);
	ItemInfo& GetShotgun(IExamInterface* pInterface);
	ItemInfo& GetHealing(IExamInterface* pInterface);
	ItemInfo& GetFood(IExamInterface* pInterface);

private:


};

