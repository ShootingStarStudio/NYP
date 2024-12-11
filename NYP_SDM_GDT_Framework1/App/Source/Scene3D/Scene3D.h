/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include CKeyboardController
#include "Inputs\KeyboardController.h"
// Include CMouseController
#include "Inputs\MouseController.h"

// Include CFPSCounter
#include "TimeControl\FPSCounter.h"

// Include CSoundController
#include "..\SoundController\SoundController.h"

// Include CSolidObjectManager
#include "Entities\SolidObjectManager.h"
// Include CPlayer3D
#include "Entities\Player3D.h"
// Include CEnemy3D
#include "Entities\Enemy3D.h"
// Include CStructure3D
#include "Entities\Structure3D.h"
// Include CProjectileManager
#include "WeaponInfo/ProjectileManager.h"

// Include CCamera
#include "Camera.h"

// Include CSkyBox
#include "SkyBox/SkyBox.h"
// Include CTerrain
#include "Terrain/Terrain.h"
// Include CTerrainManager
#include "Terrain/TerrainManager.h"

// Include CGUI_Scene3D
#include "GUI_Scene3D.h"

// Include CEntityManager
#include "Entities\EntityManager.h"

// Include CSpatialPartition
#include "SpatialPartition\SpatialPartition.h"

class CSettings;

class CScene3D : public CSingletonTemplate<CScene3D>
{
	friend CSingletonTemplate<CScene3D>;
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
	// Constructor
	CScene3D(void);
	// Destructor
	virtual ~CScene3D(void);

	// The handler to the CSettings
	CSettings* pSettings;

	// Keyboard Controller singleton instance
	CKeyboardController* pKeyboardController;

	// Mouse Controller singleton instance
	CMouseController* pMouseController;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// FPS Control
	CFPSCounter* pFPSCounter;

	// Handler to the CSoundController
	CSoundController* pSoundController;

	// Handler to the CSolidObjectManager class
	CSolidObjectManager* pSolidObjectManager;
	// Handler to the CProjectileManager instance
	CProjectileManager* pProjectileManager;

	// Handler to the Player3D class
	CPlayer3D* pPlayer3D;
	// Handler to the camera class
	CCamera* pCamera;

	// Handler to the CSkyBox
	CSkyBox* pSkyBox;
	// Handler to the CTerrainManager
	CTerrainManager* pTerrainManager;

	// GUI_Scene3D
	CGUI_Scene3D* pGUI_Scene3D;

	// Handler to the EntityManager class
	CEntityManager* pEntityManager;

	// Handler to the CSpatialPartition instance
	CSpatialPartition* pSpatialPartition;
};

