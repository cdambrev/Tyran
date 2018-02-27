#pragma once

UENUM() 
enum class EInventorySlot : uint8 {
	/* Arme équipée, en main */ 
	Hands,
	/* Autre arme sur le dos */ 
	Primary, 
	/* Emplacement pour petits items comme lampe de poche */ 
	Secondary 
};
