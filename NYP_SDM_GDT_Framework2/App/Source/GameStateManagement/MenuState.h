#pragma once

/**
 CMenuState
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

struct ButtonData
{
	std::string fileName;
	unsigned textureID;
};

class CMenuState : public CGameStateBase
{
public:
	// Constructor
	CMenuState(void);
	// Destructor
	~CMenuState(void);

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
	// The background image of this game state
	CBackgroundEntity* background;

	// The IMGUI play2D button
	ButtonData play2DButtonData;
	// The IMGUI play3D button
	ButtonData play3DButtonData;
	// The IMGUI exit button
	ButtonData exitButtonData;
};
