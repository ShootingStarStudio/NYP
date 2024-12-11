/**
 CScene2D
 @brief A class which manages the 2D game scene
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"
// Include CPlayer2D
#include "Player2D.h"
// Include enemy2d
#include "Enemy2D.h"

//game manager
#include "GameManager.h"

// Include vector
#include <vector>

// Include Keyboard controller
#include "Inputs\KeyboardController.h"

// include sound controller
#include "..\SoundController\SoundController.h"

// GUI_Scene2D
#include "GUI_Scene2D.h"

#include "Projectile2D.h"
#include "EProjectile2D.h"

#include <vector>

class CScene2D : public CSingletonTemplate<CScene2D>
{
	friend CSingletonTemplate<CScene2D>;
public:
	// Init
	bool Init(void);

	// Update
	bool Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

protected:
	// The handler containing the instance of the 2D Map
	CMap2D* cMap2D;
	// The handler containing the instance of CPlayer2Ds
	CPlayer2D* cPlayer2D;
	//vector containing the instance of enemy2D
	vector<CEnemy2D*> enemyVector;

	CGameManager* cGameManager;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	//handler to CSoundController
	CSoundController* cSoundController;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// GUI_Scene2D
	CGUI_Scene2D* cGUI_Scene2D;

	// Constructor
	CScene2D(void);
	// Destructor
	virtual ~CScene2D(void);

	CProjectile2D* cProjectile2D;
	EProjectile2D* eProjectile;
};

