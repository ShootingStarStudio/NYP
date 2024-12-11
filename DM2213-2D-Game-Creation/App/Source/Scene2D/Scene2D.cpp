/**
 CScene2D
 @brief A class which manages the 2D game scene
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene2D.h"
#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

#include "System\filesystem.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene2D::CScene2D(void)
	: cMap2D(NULL)
	, cPlayer2D(NULL)
	, cKeyboardController(NULL)
	, cGUI_Scene2D(NULL)
	, cGameManager(NULL)
	, cSoundController(NULL)
	, cProjectile2D(NULL)
	, eProjectile(NULL)
{
}

/**
 @brief Destructor
 */
CScene2D::~CScene2D(void)
{
	if (cGUI_Scene2D)
	{
		cGUI_Scene2D->Destroy();
		cGUI_Scene2D = NULL;
	}

	if (cKeyboardController)
	{
		// We won't delete this since it was created elsewhere
		cKeyboardController = NULL;
	}

	if (cSoundController)
	{
		// We won't delete this since it was created elsewhere
		cSoundController = NULL;
	}

	if (cPlayer2D)
	{
		cPlayer2D->Destroy();
		cPlayer2D = NULL;
	}

	if (cMap2D)
	{
		cMap2D->Destroy();
		cMap2D = NULL;
	}

	if (cGameManager)
	{
		cGameManager->Destroy();
		cGameManager = NULL;
	}

	if (cProjectile2D)
	{
		cProjectile2D->Destroy();
		cProjectile2D = NULL;
	}

	if (eProjectile)
	{
		eProjectile->Destroy();
		eProjectile = NULL;
	}

	for (int i = 0; i < enemyVector.size(); i++) {
		delete enemyVector[i];
		enemyVector[i] = NULL;
	}
	enemyVector.clear();
}

/**
@brief Init Initialise this instance
*/ 
bool CScene2D::Init(void)
{
	// Include Shader Manager
	CShaderManager::GetInstance()->Use("Shader2D");
	//CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader("Shader2D");
	// Initialise the instance
	if (cMap2D->Init(4, 36, 48) == false)
	{
		cout << "Failed to load CMap2D" << endl;
		return false;
	}
	/*if (cMap2D->LoadMap("Maps/DM2213_Map_Level_01.csv", 0) == false)
	{
		return false;
	}
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_02.csv", 1) == false)
	{
		return false;
	}*/
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_03.csv", 0) == false)
	{
		return false;
	}
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_04.csv", 1) == false)
	{
		return false;
	}
	/*if (cMap2D->LoadMap("Maps/DM2213_Map_TestLevel.csv", 0) == false)
	{
		return false;
	}*/

	// Load Scene2DColour into ShaderManager
	CShaderManager::GetInstance()->Use("Shader2D_Colour");
	//CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);
	// Create and initialise the CPlayer2D
	cPlayer2D = CPlayer2D::GetInstance();
	// Pass shader to cPlayer2D
	cPlayer2D->SetShader("Shader2D_Colour");
	// Initialise the instance
	if (cPlayer2D->Init() == false)
	{
		cout << "Failed to load CPlayer2D" << endl;
		return false;
	}

	cGameManager = CGameManager::GetInstance();
	cGameManager->Init();

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_BG.ogg"), 1, true, true);

	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Slash.ogg"), 3, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Shoot.ogg"), 4, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_ArrowHit.ogg"), 5, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_DoorOpen.ogg"), 6, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_DoorClose.ogg"), 7, true);
	cSoundController->PlaySoundByID(1);

	// Store the cGUI_Scene2D singleton instance here
	cGUI_Scene2D = CGUI_Scene2D::GetInstance();
	cGUI_Scene2D->Init();

	cProjectile2D = CProjectile2D::GetInstance();
	cProjectile2D->SetShader("Shader2D_Colour");
	if (cProjectile2D->Init() == false) {
		cout << "Failed to load CProjectile2D" << endl;
		return false;
	}

	eProjectile = EProjectile2D::GetInstance();
	eProjectile->SetShader("Shader2D_Colour");
	if (eProjectile->Init() == false) {
		cout << "Failed to load EProjectile2D" << endl;
		return false;
	}

	enemyVector.clear();
	while (true) {
		CEnemy2D* cEnemy2D = new CEnemy2D();
		//pass shader to enemy2d
		cEnemy2D->SetShader("Shader2D_Colour");
		if (cEnemy2D->Init() == true) {
			cEnemy2D->SetPlayer2D(cPlayer2D);
			enemyVector.push_back(cEnemy2D);
		}
		else {
			//break when all enemies have been loaded
			break;
		}
	}
	return true;
}

/**
@brief Update Update this instance
*/
bool CScene2D::Update(const double dElapsedTime)
{
	// Call the cPlayer2D's update method before Map2D as we want to capture the inputs before map2D update
	cPlayer2D->Update(dElapsedTime);

	if (cPlayer2D->getInteractFrame()) {
		cPlayer2D->setInteractFrame(false);		//activate/deactivate all enemies
		for (int i = 0; i < enemyVector.size(); i++) {
			enemyVector[i]->toggleActive();
		}
	}

	//update projectile physics
	if (cProjectile2D->projectile == true) {		//player projectile
		cProjectile2D->Update(dElapsedTime);
		for (int i = 0; i < enemyVector.size(); i++) {
			if (enemyVector[i]->getPos() == cProjectile2D->vec2Index) {
				cProjectile2D->projectile = false;			//take damage
				enemyVector[i]->takeDamage(10 * enemyVector[i]->getIncomingDamageMultiplier());
			}
		}
	}
	if (eProjectile->projectile == true) {			//entity projectile
		eProjectile->Update(dElapsedTime);
		if (cPlayer2D->vec2Index == eProjectile->vec2Index) {
			eProjectile->projectile = false;			//take damage
			cPlayer2D->takeDamage(10);
		}
	}
	//call enemy update before map
	for (int i = 0; i < enemyVector.size(); i++) {
		enemyVector[i]->Update(dElapsedTime);
	}
	 
	// Call the Map2D's update method
	cMap2D->Update(dElapsedTime);

	// Get keyboard updates
	if (cKeyboardController->IsKeyReleased(GLFW_KEY_F6))
	{
		// Save the current game to a save file
		// Make sure the file is open
		try {
			if (cMap2D->SaveMap("Maps/DM2213_Map_Level_01_SAVEGAMEtest.csv") == false)
			{
				throw runtime_error("Unable to save the current game to a file");
			}
		}
		catch (runtime_error e)
		{
			cout << "Runtime error: " << e.what();
			return false;
		}
	}

	// Call the cGUI_Scene2D's update method
	cGUI_Scene2D->Update(dElapsedTime);

	//check if the game should go to next level
	if (cGameManager->bLevelCompleted == true) {
		cMap2D->SetCurrentLevel(cMap2D->GetCurrentLevel() + 1);
		cGameManager->bLevelCompleted = false;
		//reset player position to new map
		cPlayer2D->Reset();
		//enemy reset
		enemyVector.clear();
		while (true) {
			CEnemy2D* cEnemy2D = new CEnemy2D();
			//pass shader to enemy2d
			cEnemy2D->SetShader("Shader2D_Colour");
			if (cEnemy2D->Init() == true) {
				cEnemy2D->SetPlayer2D(cPlayer2D);
				enemyVector.push_back(cEnemy2D);
			}
			else {
				//break when all enemies have been loaded
				break;
			}
		}
	}
	//check if the game has been won by player
	if (cGameManager->bPlayerWon == true) {
		//end game and go to win screen
	}
	//check if player lost/end game
	else if (cGameManager->bPlayerLost == true) {
		//play lose sound
		return false;
	}
	//play and pause background audio
	if (cKeyboardController->IsKeyReleased(GLFW_KEY_O)) {
		cSoundController->PlaySoundByID(1);
	}
	if (cKeyboardController->IsKeyReleased(GLFW_KEY_P)) {
		cSoundController->StopSoundByID(1);
	}

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene2D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable 2D texture rendering
	glEnable(GL_TEXTURE_2D);
}

/**
 @brief Render Render this instance
 */
void CScene2D::Render(void)
{
	// Call the Map2D's PreRender()
	cMap2D->PreRender();
	// Call the Map2D's Render()
	cMap2D->Render();
	// Call the Map2D's PostRender()
	cMap2D->PostRender();

	for (int i = 0; i < enemyVector.size(); i++) {
		// Call the CEnemy2D's PreRender()
		enemyVector[i]->PreRender();
		// Call the CEnemy2D's Render()
		enemyVector[i]->Render();
		// Call the CEnemy2D's PostRender()
		enemyVector[i]->PostRender();
	}

	// Call the CPlayer2D's PreRender()
	cPlayer2D->PreRender();
	// Call the CPlayer2D's Render()
	cPlayer2D->Render();
	// Call the CPlayer2D's PostRender()
	cPlayer2D->PostRender();

	// Call the cGUI_Scene2D's PreRender()
	cGUI_Scene2D->PreRender();
	// Call the cGUI_Scene2D's Render()
	cGUI_Scene2D->Render();
	// Call the cGUI_Scene2D's PostRender()
	cGUI_Scene2D->PostRender();

	if (cProjectile2D->projectile == true) {
		cProjectile2D->PreRender();
		cProjectile2D->Render();
		cProjectile2D->PostRender();
	}

	if (eProjectile->projectile == true) {
		eProjectile->PreRender();
		eProjectile->Render();
		eProjectile->PostRender();
	}
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene2D::PostRender(void)
{
}