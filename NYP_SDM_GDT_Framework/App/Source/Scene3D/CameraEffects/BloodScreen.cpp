/**
 CBloodScreen
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "BloodScreen.h"

// Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CBloodScreen::CBloodScreen(void)
{
}

/**
 @brief Destructor
 */
CBloodScreen::~CBloodScreen(void)
{
	DeInit();
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CBloodScreen::Init(void)
{
	// Get the handler to the CSettings instance
	pSettings = CSettings::GetInstance();

	// Set up the projection matrix
	projection = glm::ortho(0.0f,
							CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f),
							0.0f,
							CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f),
							-1.0f, 1.0f);

	// Generate the p2DMesh
	p2DMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 1.0f, 1.0f, false);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/CameraEffects_Blood.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CameraEffects_Blood.tga" << endl;
		return false;
	}

	// Init vec4ColourTint to white
	vec4ColourTint = glm::vec4(1.0, 1.0, 1.0, 1.0);

	// Set this Camera Effect to be disabled initially
	bStatus = false;

	return true;
}

/**
 @brief Render
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
void CBloodScreen::Render(void)
{
	// If both the CrossHair and ScopeScreen are enabled, then do not render the CrossHair
	if (bStatus == false)
		return;

	// Re-Set the projection matrix
	projection = glm::ortho(0.0f,
		CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f),
		0.0f,
		CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f),
		-1.0f, 1.0f);

	// Generate the p2DMesh
	float fAspectRatio = CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) /
		CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f);
	delete p2DMesh;
	p2DMesh = CMeshBuilder::GenerateQuadWithPosition(glm::vec4(1, 1, 1, 1),
		0.5f,
		0.5f,
		1.0f, fAspectRatio);

	CCameraEffects::Render();
}
