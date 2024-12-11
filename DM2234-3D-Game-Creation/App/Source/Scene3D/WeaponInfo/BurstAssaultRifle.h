/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CBurstAssaultRifle : public CWeaponInfo
{
public:
	// Default Constructor
	CBurstAssaultRifle(void);
	// Destructor
	virtual ~CBurstAssaultRifle(void);

	// Initialise this instance to default values
	bool Init(void); 
	
	bool Update(const double dt);

	bool Discharge(glm::vec3 vec3Position,
		glm::vec3 vec3Front,
		CSolidObject* pSource);

private:
	CSolidObject* _pSource;

	// burst count
	int iBurstCount;
	//current burst
	int iShot;
	//is burst boolean
	bool isBurst;
	// burst delay
	double dBurstDelay;
	// cooldown
	double dCooldown;
};

