/**
 CCameraShake
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

// Include CCamera
#include "../Camera.h"

class CCameraLeanLeft : public CCameraEffects
{
public:
	// Constructor
	CCameraLeanLeft(void);
	// Destructor
	virtual ~CCameraLeanLeft(void);

	// Initialise this class instance
	bool Init(void);

	// Override the SetStatus() from CEntity3D
	virtual void SetStatus(const bool bStatus);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// The handle to the camera instance
	CCamera* cCamera;

	float fRotRate;

	float fAngle;
	float fCurrentAngle;
	bool PrepareToStop;
};
