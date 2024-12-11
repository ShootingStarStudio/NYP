#pragma once

/**
 CPauseState
 @brief This class is derived from CGameState. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"

#include "Primitives/Mesh.h"
#include "../Scene2D/BackgroundEntity.h"

#include <string>

 // Include IMGUI
 // Important: GLEW and GLFW must be included before IMGUI
#ifndef IMGUI_ACTIVE
#include "GUI\imgui.h"
#include "GUI\backends\imgui_impl_glfw.h"
#include "GUI\backends\imgui_impl_opengl3.h"
#define IMGUI_ACTIVE
#endif

class CPauseState : public CGameStateBase
{
public:
	// Constructor
	CPauseState(void);
	// Destructor
	~CPauseState(void);

	// Init this class instance
	virtual bool Init(void);
	// DeInit this class instance
	virtual void DeInit();
	// Reset this class instance
	virtual void Reset();
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

protected:
	// Structure to store button data
	struct ButtonData
	{
		std::string fileName;
		unsigned textureID;
	};

	// The IMGUI volume increase button
	ButtonData VolumeIncreaseButtonData;
	// The IMGUI volume decrease button
	ButtonData VolumeDecreaseButtonData;
};
