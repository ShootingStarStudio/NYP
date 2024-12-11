/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CCameraSmoke : public CCameraEffects
{
public:
	// Constructor
	CCameraSmoke(void);
	// Destructor
	virtual ~CCameraSmoke(void);

	// Initialise this class instance
	bool Init(void);
};
