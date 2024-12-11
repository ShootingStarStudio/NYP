/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CCameraCritical : public CCameraEffects
{
public:
	// Constructor
	CCameraCritical(void);
	// Destructor
	virtual ~CCameraCritical(void);

	// Initialise this class instance
	bool Init(void);
};
