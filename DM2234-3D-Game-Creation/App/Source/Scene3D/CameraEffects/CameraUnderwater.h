/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CCameraUnderwater : public CCameraEffects
{
public:
	// Constructor
	CCameraUnderwater(void);
	// Destructor
	virtual ~CCameraUnderwater(void);

	// Initialise this class instance
	bool Init(void);
};
