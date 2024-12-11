/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CSubMachineGun : public CWeaponInfo
{
public:
	// Default Constructor
	CSubMachineGun(void);
	// Destructor
	virtual ~CSubMachineGun(void);

	// Initialise this instance to default values
	bool Init(void);
};

