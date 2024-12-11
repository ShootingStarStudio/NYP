/**
 CCameraEffects
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "CameraEffects.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CCameraEffects::CCameraEffects(void)
	: fCountdownTimer(2.0f)
{
}

/**
 @brief Destructor
 */
CCameraEffects::~CCameraEffects(void)
{
	DeInit();
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCameraEffects::Init(void)
{
	// Call the parent's Init()
	CEntity2D::Init();

	// Generate the p2DMesh
	p2DMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 1, 1, false);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/CameraEffects_Default.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CameraEffects_Default.tga" << endl;
		return false;
	}

	bStatus = false;

	return true;
}

/**
@brief DeInitialise this instance
*/
void CCameraEffects::DeInit(void)
{
	glDeleteBuffers(1, &iTextureID);
	if (p2DMesh)
	{
		delete p2DMesh;
		p2DMesh = NULL;
	}
}

/** 
 @brief Set Countdown Timer
 @param fCountdownTimer A const float variable containing the new countdown timer
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CCameraEffects::SetTimer(const float fCountdownTimer)
{
	this->fCountdownTimer = fCountdownTimer;
	return true;
}

/** 
 @brief Get Countdown Timer
 @return A float variable
 */
float CCameraEffects::GetTimer(void) const
{
	return fCountdownTimer;
}

/**
 @brief Update
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CCameraEffects::Update(const double dElapsedTime)
{
	if (bStatus == false)
		return false;

	fCountdownTimer -= (float)dElapsedTime;
	if (fCountdownTimer <= 0.0f)
	{
		// Set this Camera Effects' status to false
		bStatus = false;
		// Reset the countdown timer to the default value
		fCountdownTimer = 2.0f;
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCameraEffects::PreRender(void)
{
	if (!bStatus)
		return;

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CCameraEffects::Render(void)
{
	if (!bStatus)
		return;

	// Update the model
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(vec2Position, 0.0f));

	//model = glm::translate(model, glm::vec3(0.5f * pSettings->TILE_WIDTH, 0.5f * pSettings->TILE_HEIGHT, 0.0f));
	//model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(-0.5f * pSettings->TILE_WIDTH, -0.5f * pSettings->TILE_HEIGHT, 0.0f));
	//model = glm::scale(model, glm::vec3(800.0f, 600.0f, 1.0f));

	model = glm::scale(model, 
					   glm::vec3(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f),
								 CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f),
								 1.0f));	

	// Call the parent's Render()
	CEntity2D::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCameraEffects::PostRender(void)
{
	if (!bStatus)
		return;

	// Disable blending
	glDisable(GL_BLEND);
}

/**
 @brief Print Self
 */
void CCameraEffects::PrintSelf(void)
{
	cout << "CCameraEffects::PrintSelf()" << endl;
	cout << "===========================" << endl;

	cout << "The default CCameraEffects is meant to be a template class only." << endl;
}