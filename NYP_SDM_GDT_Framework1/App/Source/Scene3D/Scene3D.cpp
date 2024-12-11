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

// Include CPistol
#include "WeaponInfo\Pistol.h"

// Include CRock3D
#include "Entities/Rock3D.h"
// Include CTreeKabak3D
#include "Entities/TreeKabak3D.h"

// Include CSpinTower
#include "SceneGraph/SpinTower.h"
// Include CSpinTower
#include "Entities/Hut_Concrete.h"

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
	, pSpatialPartition(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the Spatial Partition
	if (pSpatialPartition)
	{
		pSpatialPartition->Destroy();
		pSpatialPartition = NULL;
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
	pCamera->vec3Front = glm::vec3(0.0f, 0.0f, -1.0f);

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

	//// Add a set of SkyBox textures into Skybox Class
	//if (pSkyBox->Add("Image/SkyBox/skybox1_right.jpg",
	//				"Image/SkyBox/skybox1_left.jpg",
	//				"Image/SkyBox/skybox1_top.jpg",
	//				"Image/SkyBox/skybox1_bottom.jpg",
	//				"Image/SkyBox/skybox1_front.jpg",
	//				"Image/SkyBox/skybox1_back.jpg") == false)
	//{
	//	cout << "Unable to load SkyBox texture files set #1" << endl;
	//	return false;
	//}

	//// Add a set of SkyBox textures into Skybox Class
	//if (pSkyBox->Add("Image/SkyBox/skybox2_right.jpg",
	//				"Image/SkyBox/skybox2_left.jpg",
	//				"Image/SkyBox/skybox2_top.jpg",
	//				"Image/SkyBox/skybox2_bottom.jpg",
	//				"Image/SkyBox/skybox2_front.jpg",
	//				"Image/SkyBox/skybox2_back.jpg") == false)
	//{
	//	cout << "Unable to load SkyBox texture files set #1" << endl;
	//	return false;
	//}

	// Load the Terrain
	pTerrainManager = CTerrainManager::GetInstance();
	pTerrainManager->Add("Shader3D_Terrain",
						"Image/Terrain/World/terrain.bmp",
						100.0f, 5.0f, 100.0f);
	//pTerrainManager->Add("Shader3D_Terrain",
	//					"Image/Terrain/World/terrain1.bmp",
	//					100.0f, 5.0f, 100.0f);
	//pTerrainManager->Add("Shader3D_Terrain",
	//					"Image/Terrain/World/terrain2.bmp",
	//					100.0f, 5.0f, 100.0f);

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

	// Assign a cPistol to the pPlayer3D
	CPistol* cPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	cPistol->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
	cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	// Initialise the instance
	cPistol->Init();
	cPistol->SetShader("Shader3D_Model");
	pPlayer3D->SetWeapon(0, cPistol);

	// Initialise the cEnemy3D
	float fCheckHeight = pTerrainManager->GetHeight(0.0f, -10.0f);
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(0.0f, fCheckHeight, -10.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//cEnemy3D->SetScale(glm::vec3(0.5f));

	// Assign a cPistol to the cEnemy3D
	CPistol* cEnemyPistol = new CPistol();
	// Set the position, rotation and scale of this weapon
	//cEnemyPistol->SetPosition(glm::vec3(0.05f, -0.075f, 0.5f));
	//cEnemyPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
	// Initialise the instance
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);

	// Add the cEnemy3D to the pSolidObjectManager
	pSolidObjectManager->Add(cEnemy3D);

	// Initialise a CStructure3D
	fCheckHeight = pTerrainManager->GetHeight(2.0f, -2.0f);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(2.0f, fCheckHeight, -2.0f));
	cStructure3D->SetShader("Shader3D");
	cStructure3D->Init();
	cStructure3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//cStructure3D->SetScale(glm::vec3(0.5f));

	// Add the cStructure3D to the pSolidObjectManager
	pSolidObjectManager->Add(cStructure3D);

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
	CRock3D* cRock3D = new CRock3D();
	cRock3D->SetInstancingMode(true);
	if (cRock3D->IsInstancedRendering() == true)
	{
		cRock3D->SetScale(glm::vec3(5.0f));
		cRock3D->SetNumOfInstance(1000);
		cRock3D->SetSpreadDistance(100.0f);

		cRock3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	else
	{
		fCheckHeight = pTerrainManager->GetHeight(2.0f, 2.0f);
		cRock3D->SetPosition(glm::vec3(2.0f, fCheckHeight, 2.0f));
		cRock3D->SetScale(glm::vec3(0.5f));
		cRock3D->SetShader("Shader3D_NoColour");			// FOR NORMAL RENDERING
	}
	if (cRock3D->Init() == true)
	{
		pEntityManager->Add(cRock3D);
	}
	else
	{
		delete cRock3D;
	}

	// Initialise the CTreeKabak3D
	CTreeKabak3D* cTreeKabak3D = new CTreeKabak3D(glm::vec3(0.0f, 0.0f, 0.0f));
	cTreeKabak3D->SetInstancingMode(true);
	if (cTreeKabak3D->IsInstancedRendering() == true)
	{
		cTreeKabak3D->SetScale(glm::vec3(1.0f));
		cTreeKabak3D->SetNumOfInstance(1000);
		cTreeKabak3D->SetSpreadDistance(100.0f);

		cTreeKabak3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	if (cTreeKabak3D->Init() == true)
	{
		pEntityManager->Add(cTreeKabak3D);
	}
	else
	{
		delete cTreeKabak3D;
	}

	// Initialise a CHut_Concrete
	fCheckHeight = pTerrainManager->GetHeight(-2.0f, 2.0f);
	CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
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

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
*/
bool CScene3D::Update(const double dElapsedTime)
{
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

		// Get mouse updates
		pCamera->ProcessMouseMovement((float)pMouseController->GetMouseDeltaX(), (float)pMouseController->GetMouseDeltaY());
		pCamera->ProcessMouseScroll((float)pMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
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
			pPlayer3D->AttachCamera(pCamera);

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
		pSkyBox->SetIndex(pSkyBox->GetIndex() - 1);
		pTerrainManager->SetIndex(pTerrainManager->GetIndex() - 1);
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_RIGHT_BRACKET))
	{
		pSkyBox->SetIndex(pSkyBox->GetIndex() + 1);
		pTerrainManager->SetIndex(pTerrainManager->GetIndex() + 1);
	}

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

	// Update the Terrain
	pTerrainManager->Update(dElapsedTime);

	// Update the Solid Objects
	pSolidObjectManager->Update(dElapsedTime);

	// Update the projectiles
	pProjectileManager->Update(dElapsedTime);

	// Update the entities
	pEntityManager->Update(dElapsedTime);

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

	//// Render the solid objects
	//pSolidObjectManager->SetView(view);
	//pSolidObjectManager->SetProjection(projection);
	//pSolidObjectManager->Render();

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