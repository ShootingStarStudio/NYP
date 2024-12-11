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

#include <iostream>

#include "WeaponInfo/Pistol.h"
#include "WeaponInfo/BurstAssaultRifle.h"
#include "WeaponInfo/MiniGun.h"
#include "WeaponInfo/Sniper.h"
#include "WeaponInfo/SubMachineGun.h"
//#include "WeaponInfo/RPG.h"
//#include "WeaponInfo/MountedTurret.h"				//sets the current weapon to this weapon bahavior

#include "Entities/Collectibles3D_Ammo.h"
#include "Entities/Collectibles3D_Health.h"

#include "Entities/Hut_Concrete.h"
#include "Entities/TreeKabak3D.h"
#include "Terrain/Plane.h"

using namespace std;

float RandomFloat(float Min, float Max) {
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cGUI_Scene3D(NULL)
	, cSoundController(NULL)
	, cSolidObjectManager(NULL)
	, cEntityManager(NULL)
	, cPlayer3D(NULL)
	, cProjectileManager(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cTerrain(NULL)
	, cMinimap(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cSoundController
	if (cSoundController)
	{
		// We won't delete this since it was created elsewhere
		cSoundController = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	// We won't delete this since it was created elsewhere
	cSettings = NULL;

	// Destroy the cSkyBox
	if (cSkyBox) {
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}
	// Destroy the cTerrain
	if (cTerrain) {
		cTerrain->Destroy();
		cTerrain = NULL;
	}

	if (cPlayer3D) {
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	if (cSolidObjectManager) {
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}

	if (cEntityManager) {
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	// Destroy the cProjectileManager
	if (cProjectileManager) {
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
	}

	if (cGUI_Scene3D) {
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
	}
	if (cMinimap) {
		cMinimap->Destroy();
		cMinimap = NULL;
	}

}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	// Load the Environment Entities
	// Load the SkyBox
	cSkyBox = CSkyBox::GetInstance();
	// Set a shader to this class instance of SkyBox
	cSkyBox->SetShader("Shader3D_SkyBox");
	cSkyBox->Init();

	// Load the Ground
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init();
	// Set the size of the Terrain
	cTerrain->SetRenderSize(1000.f, 50.f, 1000.f);
	//load texture
	if (cTerrain->LoadHeightMapFromImage("Image/Terrain/World/FractalTerrain1.ppm") == false)
		return false;

	// Load the movabl Entities
	// Initialise the CSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	cPlayer3D->AttachCamera(cCamera);

	cSolidObjectManager->Add(cPlayer3D);
	cSolidObjectManager->SetPlayerPtr(cPlayer3D);

	float StructureX = 10.f, StructureY = 10.f;
	float fCheckHeight = cTerrain->GetHeight(StructureX, StructureY);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(StructureX, fCheckHeight, StructureY));
	cStructure3D->SetScale(glm::vec3(5, 5, 5));
	cStructure3D->Init();
	cStructure3D->InitCollider("Shader3D_Line", glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
		glm::vec3(-cStructure3D->GetScale().x * 0.5f, -cStructure3D->GetScale().y * 0.5f, -cStructure3D->GetScale().z * 0.5f),
		glm::vec3(cStructure3D->GetScale().x * 0.5f, cStructure3D->GetScale().y * 0.5f, cStructure3D->GetScale().z * 0.5f));
	cStructure3D->SetShader("Shader3D");

	cSolidObjectManager->Add(cStructure3D);
	
	// Initialise the projectile manager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");
	
	//primary
	{
		// Assign a burst rifle to the cPlayer3D
		CBurstAssaultRifle* cBurstAssaultRifle = new CBurstAssaultRifle();
		//Set the postion, rotation and scale of this weapon
		cBurstAssaultRifle->SetPosition(glm::vec3(0.1f, -0.05f, -0.4f));
		cBurstAssaultRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
		cBurstAssaultRifle->SetRotation(-3.14159f / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		cBurstAssaultRifle->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
		//initialise the instance
		cBurstAssaultRifle->Init();
		cBurstAssaultRifle->SetShader("Shader3D_Model");
		cBurstAssaultRifle->pSource = cPlayer3D;
		cPlayer3D->SetWeapon(0, cBurstAssaultRifle);
	}

	//secondary
	{
		// Assign a minigun to the cPlayer3D
		CMiniGun* cMiniGun = new CMiniGun();
		//Set the postion, rotation and scale of this weapon
		cMiniGun->SetPosition(glm::vec3(0.05f, -0.075f, -0.3f));
		cMiniGun->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
		cMiniGun->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
		//initialise the instance
		cMiniGun->Init();
		cMiniGun->SetShader("Shader3D_Model");
		cMiniGun->pSource = cPlayer3D;
		cPlayer3D->SetWeapon(1, cMiniGun);
	}

	//load the GUI entities
	// store the cGUI_Scene3D singleton instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();

	// Load the Minimap
	cMinimap = CMinimap::GetInstance();
	// Set a shader to this class instance of Minimap
	cMinimap->SetShader("Shader3D_RenderToTextureShader");
	cMinimap->Init();

	//LOD test
	fCheckHeight = cTerrain->GetHeight(2.f, 2.f);
	CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-2.f, fCheckHeight, 20.f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 1.f, 1.f));

	cSolidObjectManager->Add(cHut_Concrete);

	//instancing test
	CTreeKabak3D* cTreeKabak3D = new CTreeKabak3D(glm::vec3(0.f, 0.f, 0.f));
	cTreeKabak3D->SetInstancingMode(true);
	if (cTreeKabak3D->IsInstancedRendering() == true) {
		cTreeKabak3D->SetScale(glm::vec3(5.f));
		cTreeKabak3D->SetNumOfInstance(1000);
		cTreeKabak3D->SetSpreadDistance(1000.f);

		cTreeKabak3D->SetShader("Shader3D_Instancing");
	}
	if (cTreeKabak3D->Init() == true) {
		cEntityManager->Add(cTreeKabak3D);
	}
	else {
		delete cTreeKabak3D;
	}

	CPlane* cPlane = new CPlane();
	cPlane->SetShader("Shader3D_Model");
	cPlane->Init();
	cEntityManager->Add(cPlane);
	

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	/*if (cPlayer3D->getHealth() <= 0) {
		return false;
	}*/

	//camera, GUI, effect updates
	{
		//attach and detach camera from player
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_0)) {
			if (cPlayer3D->IsCameraAttached()) {
				cPlayer3D->AttachCamera();
				//clear all effects with function call

			}
			else
				cPlayer3D->AttachCamera(cCamera);
			CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
		}

		// Get keyboard and mouse updates for camera
		if (!cPlayer3D->IsCameraAttached())
		{
			if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
				cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
			if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
				cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
			if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
				cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
			if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
				cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);
			// Get mouse updates
			cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
			//cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
		}
		else {
			//cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
			//cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
			if (cPlayer3D->scoped) {
				cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX() * 0.25, (float)cMouseController->GetMouseDeltaY() * 0.25);
			}
			else {
				cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
			}
		}
	}

	//terrain updates - generates new terrain with height map
	//skybox updates - chooses the next skybox in the list
	//level update - creates new level enemies
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_O)) {

			//cTerrain->Generate();
			//cTerrain->Increment();
			CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_O);
			//generate new level
			GenerateLevel();
		}
		cTerrain->Update(dElapsedTime);
		cSkyBox->Update(dElapsedTime);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_F)) {
		SpawnLoot();
	}
	
	//general functions
	{
		cMouseController->PostUpdate();

		cSolidObjectManager->Update(dElapsedTime);

		cEntityManager->Update(dElapsedTime);

		// update the projectiles
		cProjectileManager->Update(dElapsedTime);

		cGUI_Scene3D->Update(dElapsedTime);
	}

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
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	float playerheight = cPlayer3D->GetPosition().y;
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, playerheight + 20.f, storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);
	// Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(glm::radians(45.0f),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	CMinimap::GetInstance()->Activate();
	// Setup the rendering environment
	CMinimap::GetInstance()->PreRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render the Terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();
	
	// Render the entities for the minimap
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->Render();

	cEntityManager->SetView(playerView);
	cEntityManager->SetProjection(playerProjection);
	cEntityManager->Render();
	

	// Deactivate the cMinimap so that we can render as per normal
	CMinimap::GetInstance()->Deactivate();

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(CCamera::GetInstance()->fZoom),
		(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
		0.1f, 1000.0f);

	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	// Render the Terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the solid objects
	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->Render();

	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	//render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cGUI_Scene3D->SetProjection(projection);
	cGUI_Scene3D->PreRender();
	cGUI_Scene3D->Render();
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}

void CScene3D::GenerateLevel(void) {
	//delete all entities
	cSolidObjectManager->ResetLevel();

	//generate enemies
	for (int i = 0; i < numEnemies; i++) {
		float EnemyX = RandomFloat(-50.f, 50.f), EnemyY = -10.0f + RandomFloat(-10.f, 10.f);
		float fCheckHeight = cTerrain->GetHeight(EnemyX, EnemyY) + 50.f;
		CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(EnemyX, fCheckHeight, EnemyY));
		cEnemy3D->SetShader("Shader3D");
		cEnemy3D->Init();
		cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		cEnemy3D->SetToFall();

		//enemy weapon
		//3 classes
		int WeaponClass = rand() % 3 + 1;
		if (WeaponClass == 1) {
			// Assign a cSMG to the cEnemy3D
			CSubMachineGun* cEnemySMG = new CSubMachineGun();
			cEnemySMG->SetScale(glm::vec3(.025f, .025f, .025f));
			cEnemySMG->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
			//initialise the instance
			cEnemySMG->Init();
			cEnemySMG->SetShader("Shader3D_Model");
			cEnemySMG->pSource = cEnemy3D;
			cEnemy3D->SetWeapon(0, cEnemySMG);
			cEnemy3D->setDetectionDistance(10);

		}
		else if (WeaponClass == 2) {
			// Assign a cBurstAssaultRifle to the cEnemy3D
			CBurstAssaultRifle* cEnemyRifle = new CBurstAssaultRifle();
			cEnemyRifle->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
			cEnemyRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
			cEnemyRifle->SetRotation(-3.14159f / 2, glm::vec3(1.0f, 0.0f, 0.0f));
			//initialise the instance
			cEnemyRifle->Init();
			cEnemyRifle->SetShader("Shader3D_Model");
			cEnemyRifle->pSource = cEnemy3D;
			cEnemy3D->SetWeapon(0, cEnemyRifle);
			cEnemy3D->setDetectionDistance(20);
		}
		else {		//WeaponClass == 3
			// Assign a cSniper to the cEnemy3D
			CSniper* cEnemySniper = new CSniper();
			cEnemySniper->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
			//initialise the instance
			cEnemySniper->Init();
			cEnemySniper->SetShader("Shader3D_Model");
			cEnemySniper->pSource = cEnemy3D;
			cEnemy3D->SetWeapon(0, cEnemySniper);
			cEnemy3D->setDetectionDistance(40);
		}

		//secondary
		// Assign a cPistol to the cEnemy3D
		//CPistol* cEnemyPistol = new CPistol();
		//cEnemyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
		////initialise the instance
		//cEnemyPistol->Init();
		//cEnemyPistol->SetShader("Shader3D_Model");
		//cEnemyPistol->pSource = cEnemy3D;
		//cEnemy3D->SetWeapon(1, cEnemyPistol);

		cSolidObjectManager->Add(cEnemy3D);

	}

	//generate allies
	for (int i = 0; i < numAllies; i++) {
		float AllyX = RandomFloat(-50.f, 50.f), AllyY = 10.0f + RandomFloat(-10.f, 10.f);
		float fCheckHeight = cTerrain->GetHeight(AllyX, AllyY) + 50.f;
		CAlly3D* cAlly3D = new CAlly3D(glm::vec3(AllyX, fCheckHeight, AllyY));
		cAlly3D->SetShader("Shader3D");
		cAlly3D->Init();
		cAlly3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		cAlly3D->SetToFall();

		//Ally weapon
		//3 classes
		int WeaponClass = rand() % 3 + 1;
		if (WeaponClass == 1) {
			// Assign a cSMG to the cAlly3D
			CSubMachineGun* cAllySMG = new CSubMachineGun();
			cAllySMG->SetScale(glm::vec3(.025f, .025f, .025f));
			//initialise the instance
			cAllySMG->Init();
			cAllySMG->SetShader("Shader3D_Model");
			cAllySMG->pSource = cAlly3D;
			cAlly3D->SetWeapon(0, cAllySMG);
			cAlly3D->setDetectionDistance(10);
		}
		else if (WeaponClass == 2) {
			// Assign a cBurstAssaultRifle to the cAlly3D
			CBurstAssaultRifle* cAllyRifle = new CBurstAssaultRifle();
			cAllyRifle->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
			cAllyRifle->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
			cAllyRifle->SetRotation(-3.14159f / 2, glm::vec3(1.0f, 0.0f, 0.0f));
			//initialise the instance
			cAllyRifle->Init();
			cAllyRifle->SetShader("Shader3D_Model");
			cAllyRifle->pSource = cAlly3D;
			cAlly3D->SetWeapon(0, cAllyRifle);
			cAlly3D->setDetectionDistance(20);
		}
		else {		//WeaponClass == 3
			// Assign a cSniper to the cAlly3D
			CSniper* cAllySniper = new CSniper();
			cAllySniper->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
			//initialise the instance
			cAllySniper->Init();
			cAllySniper->SetShader("Shader3D_Model");
			cAllySniper->pSource = cAlly3D;
			cAlly3D->SetWeapon(0, cAllySniper);
			cAlly3D->setDetectionDistance(40);
		}

		//secondary
		// Assign a cPistol to the cAlly3D
		//CPistol* cAllyPistol = new CPistol();
		//cAllyPistol->SetScale(glm::vec3(1.75f, 1.75f, 1.75f));
		////initialise the instance
		//cAllyPistol->Init();
		//cAllyPistol->SetShader("Shader3D_Model");
		//cAllyPistol->pSource = cAlly3D;
		//cAlly3D->SetWeapon(1, cAllyPistol);

		cSolidObjectManager->Add(cAlly3D);
	}

	//generate structures
	for (int i = 0; i < numStructures; i++) {

		// Initialise a cStructure3D
		float StructureX = RandomFloat(-250.f, 250.f), StructureY = RandomFloat(-250.f, 250.f);
		float fCheckHeight = cTerrain->GetHeight(StructureX, StructureY);
		CStructure3D* cStructure3D = new CStructure3D(glm::vec3(StructureX, fCheckHeight, StructureY));
		cStructure3D->SetScale(glm::vec3(5, 5, 5));
		cStructure3D->Init();
		cStructure3D->InitCollider("Shader3D_Line", glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
			glm::vec3(-cStructure3D->GetScale().x * 0.5f, -cStructure3D->GetScale().y * 0.5f, -cStructure3D->GetScale().z * 0.5f),
			glm::vec3(cStructure3D->GetScale().x * 0.5f, cStructure3D->GetScale().y * 0.5f, cStructure3D->GetScale().z * 0.5f));
		cStructure3D->SetShader("Shader3D");

		cSolidObjectManager->Add(cStructure3D);
	}
}

void CScene3D::SpawnLoot(void) {
	float ItemX = RandomFloat(-200.f, 200.f), ItemY = RandomFloat(-200.f, 200.f);
	float fCheckHeight = cTerrain->GetHeight(ItemX, ItemX) + 50.f;
	int rand_int = rand() % 2;					//assumes there are 2 types of collectibles now
	if (rand_int == 0) {
		CCollectible3D_Ammo* cCollectible = new CCollectible3D_Ammo(glm::vec3(ItemX, fCheckHeight, ItemY));
		cCollectible->Init();
		cCollectible->InitCollider("Shader3D_Line", glm::vec4(0.2f, 0.2f, 1.f, 1.f),
			glm::vec3(-cCollectible->GetScale().x * 0.5f, -cCollectible->GetScale().y * 0.5f, -cCollectible->GetScale().z * 0.5f),
			glm::vec3(cCollectible->GetScale().x * 0.5f, cCollectible->GetScale().y * 0.5f, cCollectible->GetScale().z * 0.5f));
		cCollectible->SetToFall();
		cCollectible->SetShader("Shader3D");
		cSolidObjectManager->Add(cCollectible);
	}
	else if (rand_int == 1) {
		CCollectible3D_Health* cCollectible = new CCollectible3D_Health(glm::vec3(ItemX, fCheckHeight, ItemY));
		cCollectible->Init();
		cCollectible->InitCollider("Shader3D_Line", glm::vec4(0.5f, 0.5f, 0.5f, 1.f),
			glm::vec3(-cCollectible->GetScale().x * 0.5f, -cCollectible->GetScale().y * 0.5f, -cCollectible->GetScale().z * 0.5f),
			glm::vec3(cCollectible->GetScale().x * 0.5f, cCollectible->GetScale().y * 0.5f, cCollectible->GetScale().z * 0.5f));
		cCollectible->SetToFall();
		cCollectible->SetShader("Shader3D");
		cSolidObjectManager->Add(cCollectible);
	}

	
}