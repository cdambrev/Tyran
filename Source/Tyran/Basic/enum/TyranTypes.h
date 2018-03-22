#pragma once

UENUM() 
enum class EInventorySlot : uint8 {
	/* Arme �quip�e, en main */ 
	Hands,
	/* Autre arme sur le dos */ 
	Primary, 
	/* Emplacement pour pistolets */ 
	Secondary 
};

enum class EAmmoType : uint8 {
	AssaultRifle,
	Shotgun,
	Pistol,
	SniperRifle
};

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	None,
	AssaultRifle,
	Shotgun,
	Pistol,
	SniperRifle
};