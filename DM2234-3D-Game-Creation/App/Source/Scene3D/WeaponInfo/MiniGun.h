/**
 CPistol
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CMiniGun : public CWeaponInfo
{
public:
	// Default Constructor
	CMiniGun(void);
	// Destructor
	virtual ~CMiniGun(void);

	// Initialise this instance to default values
	bool Init(void);

	bool Update(const double dt);

	bool Discharge(glm::vec3 vec3Position,
		glm::vec3 vec3Front,
		CSolidObject* pSource);

private:

	// Default firing rate
	double dBaseFiringRate;
	// Max firing rate
	double dMaxFiringRate;
	// Acceleration
	double dAcceleration;

	//cooldown time
	double dCoolDown;
	double dLastFired;
};

