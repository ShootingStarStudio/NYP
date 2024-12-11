/**
 CCameraShake
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "CameraLeanRight.h"

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
CCameraLeanRight::CCameraLeanRight(void)
	: cCamera(NULL)
	, fRotRate(30.0f)
	, fAngle(15.0f)
	, fCurrentAngle(0.0f)
	, PrepareToStop(false)
{
}

/**
 @brief Destructor
 */
CCameraLeanRight::~CCameraLeanRight(void)
{
	cCamera = NULL;
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCameraLeanRight::Init(void)
{
	// Call the parent's Init()
	CCameraEffects::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Store the camera's instance to this handle
	cCamera = CCamera::GetInstance();

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	return true;
}

/**
 @brief Override the SetStatus() from CEntity3D
 @param bStatus A const bool variable containing the new status
 */
void CCameraLeanRight::SetStatus(const bool bStatus)
{
	if (bStatus == true)
	{
		// If bStatus is true, then just assign to this->bStatus
		this->bStatus = bStatus;
	}
	else
	{
		// If bStatus is true, then just assign to this->bStatus
		PrepareToStop = true;
	}
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CCameraLeanRight::Update(const double dElapsedTime)
{
	if (!PrepareToStop) {						//active
		if (fCurrentAngle < fAngle) {
			fCurrentAngle += fRotRate * dElapsedTime;
		}
		else if (fCurrentAngle >= fAngle) {
			fCurrentAngle = fAngle;
		}
	}
	if (PrepareToStop) {						//stop active
		if (fCurrentAngle > 0.f) {
			fCurrentAngle -= fRotRate * dElapsedTime;
		}
		else if (fCurrentAngle <= 0.f) {
			fCurrentAngle = 0.f;
			PrepareToStop = false;
			bStatus = false;
		}
	}
	cCamera->fRollOffset = -fCurrentAngle;

	//cCamera->vec3Offset = cCamera->vec3Right * fCurrentAngle * 0.01f;
	return true;
}


/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraLeanRight::PreRender(void)
{
	if (!bStatus)
		return;
}

/**
 @brief Render this instance
 */
void CCameraLeanRight::Render(void)
{
	if (!bStatus)
		return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCameraLeanRight::PostRender(void)
{
	if (!bStatus)
		return;
}
