/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

// Include CEnemy3D
#include "Entities\Enemy3D.h"

// Include CPistol
#include "WeaponInfo\Pistol.h"

// Include CRock3D
#include "Entities/Rock3D.h"
// Include CTreeKabak3D
#include "Entities/TreeKabak3D.h"

// Include CStructure3D
#include "Entities\Structure3D.h"
// Include CSpinTower
#include "SceneGraph/SpinTower.h"
// Include CSpinTower
#include "Entities/Hut_Concrete.h"

// Include CWallWithNormalMapping
#include "WallWithNormalMapping\WallWithNormalMapping.h"

// Include CCameraShake
#include "CameraEffects/CameraShake.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: pSettings(NULL)
	, pKeyboardController(NULL)
	, pMouseController(NULL)
	, pFPSCounter(NULL)
	, pGUI_Scene3D(NULL)
	, pSoundController(NULL)
	, pSolidObjectManager(NULL)
	, pPlayer3D(NULL)
	, pCamera(NULL)
	, pSkyBox(NULL)
	, pTerrainManager(NULL)
	, pGrass3D(NULL)
	, pSpatialPartition(NULL)
	, pLightings(NULL)
	, pParticles3D(NULL)
	, pBillboard(NULL)
	, pBillboard2(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	DeInit();
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	pSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Configure the camera
	pCamera = CCamera::GetInstance();
	pCamera->vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
	pCamera->vec3Front = glm::vec3(0.0f, 0.0f, 1.0f);	// Look to the front by default

	// Store the keyboard controller singleton instance here
	pKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	pMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	pFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	pSoundController = CSoundController::GetInstance();
	pSoundController->Init();
	pSoundController->AddSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	pSoundController->AddSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	pSoundController->AddSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	// Load the Environment Entities

	// Initialise CTimeOfTheDay
	pTimeOfTheDay = CTimeOfTheDay::GetInstance();

	// Load the SkyBox
	pSkyBox = CSkyBox::GetInstance();
	// Set a shader to this class instance of CSkyBox
	pSkyBox->SetShader("Shader3D_SkyBox");
	pSkyBox->Init();

	// Add a set of SkyBox textures into Skybox Class
	if (pSkyBox->Add("Image/SkyBox/skybox_right.jpg",
					 "Image/SkyBox/skybox_left.jpg",
					 "Image/SkyBox/skybox_top.jpg",
					 "Image/SkyBox/skybox_bottom.jpg",
					 "Image/SkyBox/skybox_front.jpg",
					 "Image/SkyBox/skybox_back.jpg") == false)
	{
		cout << "Unable to load SkyBox texture files set #1" << endl;
		return false;
	}

	// Add a set of SkyBox textures into Skybox Class
	if (pSkyBox->Add("Image/SkyBox/skybox1_right.jpg",
					"Image/SkyBox/skybox1_left.jpg",
					"Image/SkyBox/skybox1_top.jpg",
					"Image/SkyBox/skybox1_bottom.jpg",
					"Image/SkyBox/skybox1_front.jpg",
					"Image/SkyBox/skybox1_back.jpg") == false)
	{
		cout << "Unable to load SkyBox texture files set #1" << endl;
		return false;
	}

	// Add a set of SkyBox textures into Skybox Class
	if (pSkyBox->Add("Image/SkyBox/skybox2_right.jpg",
					"Image/SkyBox/skybox2_left.jpg",
					"Image/SkyBox/skybox2_top.jpg",
					"Image/SkyBox/skybox2_bottom.jpg",
					"Image/SkyBox/skybox2_front.jpg",
					"Image/SkyBox/skybox2_back.jpg") == false)
	{
		cout << "Unable to load SkyBox texture files set #1" << endl;
		return false;
	}

	// Load the Terrain
	pTerrainManager = CTerrainManager::GetInstance();
	pTerrainManager->Add("Shader3D_Terrain",
						"Image/Terrain/World/terrain.bmp",
						100.0f, 5.0f, 100.0f);
	pTerrainManager->Add("Shader3D_Terrain",
						"Image/Terrain/World/terrain1.bmp",
						100.0f, 5.0f, 100.0f);
	pTerrainManager->Add("Shader3D_Terrain",
						"Image/Terrain/World/terrain2.bmp",
						100.0f, 5.0f, 100.0f);

	// Load the movable Entities
	// Initialise the CSolidObjectManager
	pSolidObjectManager = CSolidObjectManager::GetInstance();
	pSolidObjectManager->Init();

	// Initialise the pPlayer3D
	pPlayer3D = CPlayer3D::GetInstance();
	pPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	pPlayer3D->SetShader("Shader3D");
	pPlayer3D->Init();
	pPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	pPlayer3D->AttachCamera(pCamera);

	// Add the pPlayer3D to the pSolidObjectManager
	pSolidObjectManager->Add(pPlayer3D);

	// Initialise the projectile manager
	pProjectileManager = CProjectileManager::GetInstance();
	pProjectileManager->Init();
	pProjectileManager->SetShader("Shader3D");

	// Assign a pPistol to the pPlayer3D
	CPistol* pPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	pPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	pPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	pPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	// Initialise the instance
	pPistol->Init();
	//pPistol->SetShader("Shader3D_Model");
	pPistol->SetShader("Shader3D_Lights");
	pPlayer3D->SetWeapon(0, pPistol);

	// Initialise the pEnemy3D
	float fCheckHeight = pTerrainManager->GetHeight(0.0f, -10.0f);
	CEnemy3D* pEnemy3D = new CEnemy3D(glm::vec3(0.0f, fCheckHeight, -10.0f));
	pEnemy3D->SetShader("Shader3D");
	pEnemy3D->Init();
	pEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//pEnemy3D->SetScale(glm::vec3(0.5f));

	// Assign a pPistol to the pEnemy3D
	CPistol* pEnemyPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	//pEnemyPistol->SetPosition(glm::vec3(0.05f, -0.075f, 0.5f));
	//pEnemyPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	pEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	// Initialise the instance
	pEnemyPistol->Init();
	//pEnemyPistol->SetShader("Shader3D_Model");
	pEnemyPistol->SetShader("Shader3D_Lights");
	pEnemy3D->SetWeapon(0, pEnemyPistol);

	// Add the pEnemy3D to the pSolidObjectManager
	pSolidObjectManager->Add(pEnemy3D);

	// Initialise a CStructure3D
	fCheckHeight = pTerrainManager->GetHeight(2.0f, -2.0f);
	CStructure3D* pStructure3D = new CStructure3D(glm::vec3(2.0f, fCheckHeight, -2.0f));
	pStructure3D->SetShader("Shader3D_Lights");
	pStructure3D->Init();
	pStructure3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//pStructure3D->SetScale(glm::vec3(0.5f));

	// Add the pStructure3D to the pSolidObjectManager
	pSolidObjectManager->Add(pStructure3D);

	// Initialise a CWallWithNormalMapping
	fCheckHeight = pTerrainManager->GetHeight(2.0f, 2.0f);
	CWallWithNormalMapping* pWallWithNormalMapping = new CWallWithNormalMapping(glm::vec3(2.0f, fCheckHeight, 2.0f));
	//pWallWithNormalMapping->SetShader("Shader3D_Lights");
	pWallWithNormalMapping->SetShader("Shader3D_NormalMapping");
	pWallWithNormalMapping->Init();
	pWallWithNormalMapping->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the pWallWithNormalMapping to the pSolidObjectManager
	pSolidObjectManager->Add(pWallWithNormalMapping);

	// Load the GUI Entities
	// Store the pGUI_Scene3D singleton instance here
	pGUI_Scene3D = CGUI_Scene3D::GetInstance();
	pGUI_Scene3D->Init();

	// Load the non-movable Entities with no collisions
	// Initialise the CEntityManager
	pEntityManager = CEntityManager::GetInstance();
	pEntityManager->Init();

	// Initialise a CSpinTower
	CSpinTower::Create();

	// Initialise the CRock3D
	CRock3D* pRock3D = new CRock3D();
	pRock3D->SetInstancingMode(true);
	if (pRock3D->IsInstancedRendering() == true)
	{
		pRock3D->SetScale(glm::vec3(5.0f));
		pRock3D->SetNumOfInstance(1000);
		pRock3D->SetSpreadDistance(100.0f);

		pRock3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	else
	{
		fCheckHeight = pTerrainManager->GetHeight(2.0f, 2.0f);
		pRock3D->SetPosition(glm::vec3(2.0f, fCheckHeight, 2.0f));
		pRock3D->SetScale(glm::vec3(0.5f));
		pRock3D->SetShader("Shader3D_NoColour");			// FOR NORMAL RENDERING
	}
	if (pRock3D->Init() == true)
	{
		pEntityManager->Add(pRock3D);
	}
	else
	{
		delete pRock3D;
	}

	// Initialise the CTreeKabak3D
	CTreeKabak3D* pTreeKabak3D = new CTreeKabak3D(glm::vec3(0.0f, 0.0f, 0.0f));
	pTreeKabak3D->SetInstancingMode(true);
	if (pTreeKabak3D->IsInstancedRendering() == true)
	{
		pTreeKabak3D->SetScale(glm::vec3(1.0f));
		pTreeKabak3D->SetNumOfInstance(1000);
		pTreeKabak3D->SetSpreadDistance(100.0f);

		pTreeKabak3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	if (pTreeKabak3D->Init() == true)
	{
		pEntityManager->Add(pTreeKabak3D);
	}
	else
	{
		delete pTreeKabak3D;
		pTreeKabak3D = NULL;
	}

	// Grass
	pGrass3D = new CGrass3D(glm::vec3(0.0f, 0.0f, 0.0f));
	pGrass3D->SetShader("Shader3D_Grass");
	pGrass3D->SetMinMaxPos(	glm::vec2(pTerrainManager->GetMinPos().x, pTerrainManager->GetMinPos().z), 
							glm::vec2(pTerrainManager->GetMaxPos().x, pTerrainManager->GetMaxPos().z));
	if (pGrass3D->Init() == false)
	{
		delete pGrass3D;
		pGrass3D = NULL;
	}

	// Initialise a CHut_Concrete
	fCheckHeight = pTerrainManager->GetHeight(-2.0f, 2.0f);
	CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cHut_Concrete->SetShader("Shader3D_Lights");// "Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the cHut_Concrete to the pSolidObjectManager
	pSolidObjectManager->Add(cHut_Concrete);

	// Initialise a Spatial Partition
	pSpatialPartition = CSpatialPartition::GetInstance();
	pSpatialPartition->SetShader("Shader3D_Line");
	pSpatialPartition->SetView(pCamera->GetViewMatrix());
	pSpatialPartition->SetProjection(pCamera->GetPerspectiveMatrix());
	pSpatialPartition->Init(glm::vec3(100.0f, 1.0f, 100.0f), glm::i32vec3(10, 1, 10));

	// Initialise the lightings
	pLightings = CLightings::GetInstance();
	//pLightings->AddDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));
	pLightings->AddDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.2f, 0.2f, 0.2f));
	//pLightings->AddDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.5f, 0.5f, 0.5f));

	// Add point lights
	pLightings->AddPointLight(glm::vec3(3.0f, 3.0f, -2.0f), glm::vec3(0.2f), glm::vec3(0.2f), glm::vec3(0.2f), 1.0f, 0.09f, 0.032f);
	pLightings->AddPointLight(glm::vec3(2.0f, 3.0f, -0.5f), glm::vec3(0.2f), glm::vec3(0.2f), glm::vec3(0.2f), 1.0f, 0.09f, 0.032f);
	pLightings->AddPointLight(glm::vec3(0.8f, 2.0f, -2.2f), glm::vec3(0.2f), glm::vec3(0.2f), glm::vec3(0.2f), 1.0f, 0.09f, 0.032f);
	pLightings->AddPointLight(glm::vec3(2.0f, 3.0f, -3.0f), glm::vec3(0.2f), glm::vec3(0.2f), glm::vec3(0.2f), 1.0f, 0.09f, 0.032f);

	//pLightings->PrintSelf();

	pParticles3D = new CParticles3D();
	pParticles3D->SetShader("Shader3D_Particles");
	fCheckHeight = pTerrainManager->GetHeight(5.0f, 1.0f);
	pParticles3D->SetPosition(glm::vec3(5.0f, fCheckHeight, 1.0f));
	pParticles3D->Init();

	pBillboard = new CBillboard();
	pBillboard->SetShader("Shader3D_Billboard");
	fCheckHeight = pTerrainManager->GetHeight(3.0f, 1.0f);
	pBillboard->SetPosition(glm::vec3(3.0f, fCheckHeight + 0.5f, 1.0f));
	pBillboard->Init();

	pBillboard2 = new CBillboard();
	pBillboard2->SetShader("Shader3D_Billboard");
	fCheckHeight = pTerrainManager->GetHeight(4.0f, 1.0f);
	pBillboard2->SetPosition(glm::vec3(4.0f, fCheckHeight + 0.5f, 1.0f));
	pBillboard2->Init();

	return true;
}

/**
 @brief DeInit this class instance. 
*/
void CScene3D::DeInit(void)
{
	// Destroy the pBillboard
	if (pBillboard2)
	{
		delete pBillboard2;
		pBillboard2 = NULL;
	}

	// Destroy the pBillboard
	if (pBillboard)
	{
		delete pBillboard;
		pBillboard = NULL;
	}

	// Destroy the pParticles3D
	if (pParticles3D)
	{
		delete pParticles3D;
		pParticles3D = NULL;
	}

	// Destroy the pTimeOfTheDay
	if (pTimeOfTheDay)
	{
		pTimeOfTheDay->Destroy();
		pTimeOfTheDay = NULL;
	}

	// Destroy the CLightings
	if (pLightings)
	{
		pLightings->Destroy();
		pLightings = NULL;
	}

	// Destroy the Spatial Partition
	if (pSpatialPartition)
	{
		pSpatialPartition->Destroy();
		pSpatialPartition = NULL;
	}

	// Destroy the pGrass3D
	if (pGrass3D)
	{
		delete pGrass3D;
		pGrass3D = NULL;
	}

	// Destroy the pTerrainManager
	if (pTerrainManager)
	{
		pTerrainManager->Destroy();
		pTerrainManager = NULL;
	}

	// Destroy the pSkyBox
	if (pSkyBox)
	{
		pSkyBox->Destroy();
		pSkyBox = NULL;
	}

	// Destroy the camera
	if (pCamera)
	{
		pCamera->Destroy();
		pCamera = NULL;
	}

	// Destroy the pEntityManager
	if (pEntityManager)
	{
		pEntityManager->Destroy();
		pEntityManager = NULL;
	}

	// Destroy the pProjectileManager
	if (pProjectileManager)
	{
		pProjectileManager->Destroy();
		pProjectileManager = NULL;
	}

	// Destroy the pPlayer3D
	if (pPlayer3D)
	{
		pSolidObjectManager->Erase(pPlayer3D);
		pPlayer3D->Destroy();
		pPlayer3D = NULL;
	}

	// Destroy the pSolidObjectManager
	if (pSolidObjectManager)
	{
		pSolidObjectManager->Destroy();
		pSolidObjectManager = NULL;
	}
	// Destroy the pSoundController
	if (pSoundController)
	{
		// We won't delete this since it was created elsewhere
		pSoundController = NULL;
	}

	// Destroy the pSoundController
	if (pGUI_Scene3D)
	{
		pGUI_Scene3D->Destroy();
		pGUI_Scene3D = NULL;
	}

	// We won't delete this since it was created elsewhere
	pFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	pMouseController = NULL;

	// We won't delete this since it was created elsewhere
	pKeyboardController = NULL;

	// We won't delete this since it was created elsewhere
	pSettings = NULL;
}

/**
 @brief Reset
*/
void CScene3D::Reset(void)
{
	pGUI_Scene3D->Reset();
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
*/
bool CScene3D::Update(const double dElapsedTime)
{
	pTimeOfTheDay->Update(dElapsedTime);

	// Store the current position, if rollback is needed.
	pPlayer3D->StorePositionForRollback();
	
	// Get keyboard updates for player3D
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		pPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		pPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		pPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		pPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F))
		pLightings->GetTorchLight(pPlayer3D->iTorchLightID)->SetEnabled(!pLightings->GetTorchLight(pPlayer3D->iTorchLightID)->GetEnabled());
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
		pPlayer3D->SetToJump();
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		pPlayer3D->TogglePosture();

	// Get keyboard updates for camera
	if (!pPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			pCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			pCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			pCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			pCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);

		// Change camera view
		if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F1))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::FRONTVIEW, pPlayer3D->GetPosition(), pPlayer3D->GetFront());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F2))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::REARVIEW, pPlayer3D->GetPosition(), pPlayer3D->GetFront());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F3))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::LEFTVIEW, pPlayer3D->GetPosition(), pPlayer3D->GetFront());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F4))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::RIGHTVIEW, pPlayer3D->GetPosition(), pPlayer3D->GetFront());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F5))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::TOPVIEW, pPlayer3D->GetPosition());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F6))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::BOTTOMVIEW, pPlayer3D->GetPosition());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F7))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::ISOVIEW, pPlayer3D->GetPosition(), pPlayer3D->GetFront());
		}
		else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F8))
		{
			pCamera->SetCameraView(CCamera::CAMERAVIEW::THIRDPERSONVIEW, pPlayer3D->GetPosition(), pPlayer3D->GetFront());
		}
		else
		{
			// Get mouse updates
			pCamera->ProcessMouseMovement((float)pMouseController->GetMouseDeltaX(), (float)pMouseController->GetMouseDeltaY());
			pCamera->ProcessMouseScroll((float)pMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
		}
	}
	else
	{
		// Get mouse updates
		pPlayer3D->ProcessRotate((float)pMouseController->GetMouseDeltaX(), (float)pMouseController->GetMouseDeltaY());
		pCamera->ProcessMouseScroll((float)pMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (pPlayer3D->IsCameraAttached())
		{
			pPlayer3D->AttachCamera();
			pCamera->fZoom = 45.0f;
		}
		else
		{
			pPlayer3D->AttachCamera(pCamera);
			pCamera->SetCameraView(CCamera::CAMERAVIEW::FIRSTPERSONVIEW, pPlayer3D->GetPosition());
		}

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		pPlayer3D->SetCurrentWeapon(0);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		pPlayer3D->SetCurrentWeapon(1);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_8))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CameraShake")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_8);
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CrossHair")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
	{
		pPlayer3D->GetWeapon()->Reload();
	}

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_LEFT_BRACKET))
	{
		//pSkyBox->SetIndex(pSkyBox->GetIndex() - 1);
		pTerrainManager->SetIndex(pTerrainManager->GetIndex() - 1);
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_RIGHT_BRACKET))
	{
		//pSkyBox->SetIndex(pSkyBox->GetIndex() + 1);
		pTerrainManager->SetIndex(pTerrainManager->GetIndex() + 1);
	}

	// Update the Skybox
	pSkyBox->Update(dElapsedTime);
	// Update the Skybox
	pLightings->Update(dElapsedTime);

	// Get mouse button updates
	if (pMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::LMB))
	{
		pPlayer3D->DischargeWeapon();
	}
	if (pMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::RMB))
	{
		// Switch on Scope mode and zoom in
		pCamera->fZoom = 1.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
	}
	else if (pMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
	{
		// Switch off Scope mode and zoom out
		pCamera->fZoom = 45.0f;
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
	}

	// Update the Grass3D
	pGrass3D->Update(dElapsedTime);

	// Update the Solid Objects
	pSolidObjectManager->Update(dElapsedTime);

	// Update the projectiles
	pProjectileManager->Update(dElapsedTime);

	// Update the entities
	pEntityManager->Update(dElapsedTime);

	pParticles3D->Update(dElapsedTime);

	pBillboard->Update(dElapsedTime);
	pBillboard2->Update(dElapsedTime);

	// Reset the Spatial Partition and 
	// Add the CSolidObjects back into the Spatial Partition
	pSpatialPartition->Reset(pSolidObjectManager);

	// Update the Spatial Partition
	pSpatialPartition->Update(dElapsedTime);

	// Check for collisions among Entities and also with Projectiles
	// Check for collisions between Entities and Projectiles
	pSolidObjectManager->CheckForCollision();

	// Call the pGUI_Scene3D's update method
	pGUI_Scene3D->Update(dElapsedTime);

	// Post Update the mouse controller
	pMouseController->PostUpdate();

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	// Part 1: Render for the minimap by binding to framebuffer and render to colour texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = pPlayer3D->GetPosition();
	float storeCameraYaw = pCamera->fYaw;
	float storeCameraPitch = pCamera->fPitch;
	glm::vec3 storeCameraPosition = pCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	pCamera->fYaw += 180.0f;
	pCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	pCamera->vec3Position = glm::vec3(storePlayerPosition.x, 10.0f, storePlayerPosition.z);
	pCamera->vec3Position = glm::vec3(storePlayerPosition.x, 10.0f, storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	pCamera->ProcessMouseMovement(0, 0, false);

	// Generate the view and projection
	glm::mat4 playerView = pCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(	glm::radians(45.0f),
													CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) /
														CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f),
													0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	pCamera->fYaw = storeCameraYaw;
	pCamera->fPitch = storeCameraPitch;
	pCamera->vec3Position = storeCameraPosition;
	pCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	CMinimap::GetInstance()->Activate();
	// Setup the rendering environment
	CMinimap::GetInstance()->PreRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render the Terrain
	pTerrainManager->SetView(playerView);
	pTerrainManager->SetProjection(playerProjection);
	pTerrainManager->PreRender();
	pTerrainManager->Render();
	pTerrainManager->PostRender();

	// Render the entities
	pEntityManager->SetView(playerView);
	pEntityManager->SetProjection(playerProjection);
	pEntityManager->Render();

	// Render the entities for the minimap
	pSolidObjectManager->SetView(playerView);
	pSolidObjectManager->SetProjection(playerProjection);
	pSolidObjectManager->Render();

	// Deactivate the pMinimap so that we can render as per normal
	CMinimap::GetInstance()->Deactivate();

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();
	glm::mat4 projection = CCamera::GetInstance()->GetPerspectiveMatrix();
	//glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
	//											(float)pSettings->iWindowWidth / (float)pSettings->iWindowHeight,
	//											0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the SkyBox
	pSkyBox->SetView(view);
	pSkyBox->SetProjection(projection);
	pSkyBox->PreRender();
	pSkyBox->Render();
	pSkyBox->PostRender();

	// Render the Terrain
	pTerrainManager->SetView(view);
	pTerrainManager->SetProjection(projection);
	pTerrainManager->PreRender();
	pTerrainManager->Render();
	pTerrainManager->PostRender();

	// Render the Grass
	pGrass3D->SetPosition(pPlayer3D->GetPosition());
	pGrass3D->SetView(view);
	pGrass3D->SetProjection(projection);
	pGrass3D->PreRender();
	pGrass3D->Render();
	pGrass3D->PostRender();

	// Render the solid objects
	pSolidObjectManager->SetView(view);
	pSolidObjectManager->SetProjection(projection);
	pSolidObjectManager->Render();

	// Render the Spatial Partition
	//pSpatialPartition->frustumCullingView = pPlayer3D->GetViewMatrix();
	pSpatialPartition->SetView(view);
	pSpatialPartition->SetProjection(projection);
	pSpatialPartition->PreRender();
	pSpatialPartition->Render();
	pSpatialPartition->PostRender();

	// Render the projectiles
	pProjectileManager->SetView(view);
	pProjectileManager->SetProjection(projection);
	pProjectileManager->PreRender();
	pProjectileManager->Render();
	pProjectileManager->PostRender();

	// Render the entities
	pEntityManager->SetView(view);
	pEntityManager->SetProjection(projection);
	pEntityManager->Render();

	// Render the pBillboard
	pBillboard->SetView(view);
	pBillboard->SetProjection(projection);
	pBillboard->PreRender();
	pBillboard->Render();
	pBillboard->PostRender();

	// Render the pBillboard
	pBillboard2->SetView(view);
	pBillboard2->SetProjection(projection);
	pBillboard2->PreRender();
	pBillboard2->Render();
	pBillboard2->PostRender();

	// Render the pParticles3D
	// This should be rendered just before GUI is rendered as the blending will not work
	// properly if another object with blending is rendered after it
	pParticles3D->SetView(view);
	pParticles3D->SetProjection(projection);
	pParticles3D->PreRender();
	pParticles3D->Render();
	pParticles3D->PostRender();

	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	pGUI_Scene3D->SetProjection(projection);
	// Call the pGUI_Scene3D's PreRender()
	pGUI_Scene3D->PreRender();
	// Call the pGUI_Scene3D's Render()
	pGUI_Scene3D->Render();
	// Call the pGUI_Scene3D's PostRender()
	pGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}