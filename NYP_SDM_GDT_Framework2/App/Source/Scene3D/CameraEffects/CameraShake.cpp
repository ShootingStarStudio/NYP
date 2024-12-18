/**
 CCameraShake
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "CameraShake.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include math.h
#include <math.h>

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCameraShake::CCameraShake(void)
	: pCamera(NULL)
	, fAngle(0.0f)
	, vec2Magnitude(glm::vec2(0.0f))
	, bToBeUpdated(false)
	, bPrepareToStop(false)
{
}

/**
 @brief Destructor
 */
CCameraShake::~CCameraShake(void)
{
	pCamera = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCameraShake::Init(void)
{
	// Call the parent's Init()
	CCameraEffects::Init();

	// Set the type
	//SetType(CEntity3D::TYPE::OTHERS);

	// Store the camera's instance to this handle
	pCamera = CCamera::GetInstance();

	// Magnitude of the camera shake
	vec2Magnitude = glm::vec2(0.10f, 0.05f);
	fAngle = 1.570797f;

	return true;
}

/**
 @brief Override the SetStatus() from CEntity3D
 @param bStatus A const bool variable containing the new status
 */
void CCameraShake::SetStatus(const bool bStatus)
{
	if (bStatus == true)
	{
		// If bStatus is true, then just assign to this->bStatus
		this->bStatus = bStatus;
	}
	else
	{
		// If bStatus is true, then just assign to this->bStatus
		bPrepareToStop = true;
	}
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CCameraShake::Update(const double dElapsedTime)
{
	if ((!bStatus) || (!bToBeUpdated))
		return false;

	// Update fAngle
	fAngle += 0.0174533f * 5.0f;	// 0.0.0174533f radians == 1 degree
	if (fAngle > 6.28319f)			// If fAngle is > 360 degrees
		fAngle = fAngle - 6.28319f;	// Then deduct the value by 360 degrees

	// Update the camera's position on the x-z axes
	pCamera->vec3Offset = pCamera->vec3Right * (vec2Magnitude.x * cos(fAngle));
	// Update the camera's position on the y axis
	pCamera->vec3Offset.y = vec2Magnitude.y * sin(2 * fAngle) * 0.5f;

	// Set to false since we have already updated camera shake
	bToBeUpdated = false;

	// If bPrepareToStop == true, then stop the camera shake when fAngle 
	// is around 1.570797f. This will prevent sudden camera reset
	if ((bPrepareToStop) && ((fAngle > 1.570f) && (fAngle < 1.571f)))
	{
		bStatus = false;
		bPrepareToStop = false;
	}

	return true;
}


/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraShake::PreRender(void)
{
	if (!bStatus)
		return;
}

/**
 @brief Render this instance
 */
void CCameraShake::Render(void)
{
	if (!bStatus)
		return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCameraShake::PostRender(void)
{
	if (!bStatus)
		return;
}
