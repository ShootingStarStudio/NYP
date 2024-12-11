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
	: pMap2D(NULL)
	, pPlayer2D(NULL)
	, pKeyboardController(NULL)
	, pMouseController(NULL)
	, pProjectileManager2D(NULL)
	, pGUI_Scene2D(NULL)
	, pGameManager(NULL)
	, pSoundController(NULL)
{
}

/**
 @brief Destructor
 */
CScene2D::~CScene2D(void)
{
	if (pSoundController)
	{
		// We won't delete this since it was created elsewhere
		pSoundController = NULL;
	}

	if (pGameManager)
	{
		pGameManager->Destroy();
		pGameManager = NULL;
	}

	if (pGUI_Scene2D)
	{
		pGUI_Scene2D->Destroy();
		pGUI_Scene2D = NULL;
	}

	// Destroy the pProjectileManager2D
	if (pProjectileManager2D)
	{
		pProjectileManager2D->Destroy();
		pProjectileManager2D = NULL;
	}

	if (pMouseController)
	{
		// We won't delete this since it was created elsewhere
		pMouseController = NULL;
	}

	if (pKeyboardController)
	{
		// We won't delete this since it was created elsewhere
		pKeyboardController = NULL;
	}

	// Destroy the pEnemy2DManager
	if (pEnemy2DManager)
	{
		pEnemy2DManager->Destroy();
		pEnemy2DManager = NULL;
	}

	if (pPlayer2D)
	{
		pPlayer2D->Destroy();
		pPlayer2D = NULL;
	}

	if (pMap2D)
	{
		pMap2D->Destroy();
		pMap2D = NULL;
	}

	// Clear out all the shaders
	//CShaderManager::GetInstance()->Destroy();
}

/**
@brief Init Initialise this instance
*/ 
bool CScene2D::Init(void)
{
	// Create and initialise the Map 2D
	pMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	pMap2D->SetShader("Shader2D");
	// Load Scene2DColour into ShaderManager
	CShaderManager::GetInstance()->Use("Shader2D");

	// Initialise the instance
	if (pMap2D->Init(2, 24, 32) == false)
	{
		cout << "Failed to load CMap2D" << endl;
		return false;
	}
	// Load the map into an array
	if (pMap2D->LoadMap("Maps/DM2213_Map_Level_01.csv") == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}
	// Load the map into an array
	if (pMap2D->LoadMap("Maps/DM2213_Map_Level_02.csv", 1) == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}

	// Activate diagonal movement
	pMap2D->SetDiagonalMovement(false);

	// Create and initialise the CPlayer2D
	pPlayer2D = CPlayer2D::GetInstance();
	// Pass shader to pPlayer2D
	pPlayer2D->SetShader("Shader2D");
	// Initialise the instance
	if (pPlayer2D->Init() == false)
	{
		cout << "Failed to load CPlayer2D" << endl;
		return false;
	}

	// Initialise the CEnemy2DManager
	pEnemy2DManager = CEnemy2DManager::GetInstance();
	pEnemy2DManager->Init();
	pEnemy2DManager->SetShader("Shader2D");

	// Create and initialise the CEnemy2D
	while (true)
	{
		// Find the indices for the enemies in arrMapInfo, and assign it to cEnemy2D
		unsigned int uiRow = -1;
		unsigned int uiCol = -1;
		if (pMap2D->FindValue(300, uiRow, uiCol) == false)
			break;	// Stop this loop since there are no more enemies in this map

		// Erase the value of the player in the arrMapInfo
		pMap2D->SetMapInfo(uiRow, uiCol, 0);

		int uiIndex = -1;
		if (pEnemy2DManager->Activate(glm::vec2(uiCol * pMap2D->GetTileSize().x + pMap2D->GetTileHalfSize().x,
												uiRow * pMap2D->GetTileSize().y + pMap2D->GetTileHalfSize().y), uiIndex) == false)
		{
			cout << "Unable to activate an Enemy2D at [" << uiRow << ", " << uiCol << "]" << endl;
		}
	}

	// Store the keyboard controller singleton instance here
	pKeyboardController = CKeyboardController::GetInstance();
	// Store the mouse controller singleton instance here
	pMouseController = CMouseController::GetInstance();

	// Store the pProjectileManager2D singleton instance here
	pProjectileManager2D = CProjectileManager2D::GetInstance();
	pProjectileManager2D->SetShader("Shader2D");
	pProjectileManager2D->Init();

	// Store the pGUI_Scene2D singleton instance here
	pGUI_Scene2D = CGUI_Scene2D::GetInstance();
	pGUI_Scene2D->Init();

	// Game Manager
	pGameManager = CGameManager::GetInstance();
	pGameManager->Init();

	// Load the sounds into CSoundController
	pSoundController = CSoundController::GetInstance();
	pSoundController->AddSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	pSoundController->AddSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	pSoundController->AddSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CScene2D::Update(const double dElapsedTime)
{
	// Call the pPlayer2D's update method before Map2D as we want to capture the inputs before map2D update
	pPlayer2D->Update(dElapsedTime);

	// Call all the cEnemy2D's update method before Map2D 
	// as we want to capture the updates before map2D update
	pEnemy2DManager->Update(dElapsedTime);

	// Call the pProjectileManager2D's update method
	pProjectileManager2D->Update(dElapsedTime);

	// Call the Map2D's update method
	pMap2D->Update(dElapsedTime);

	// Get keyboard updates
	if (pKeyboardController->IsKeyReleased(GLFW_KEY_F6))
	{
		// Save the current game to a save file
		// Make sure the file is open
		try {
			if (pMap2D->SaveMap("Maps/DM2213_Map_Level_01_SAVEGAMEtest.csv") == false)
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

	// Call the pGUI_Scene2D's update method
	pGUI_Scene2D->Update(dElapsedTime);

	// Check if the game should go to the next level
	if (pGameManager->bLevelCompleted == true)
	{
		pMap2D->SetCurrentLevel(pMap2D->GetCurrentLevel()+1);
		pPlayer2D->Reset();
		pGameManager->bLevelCompleted = false;
	}

	// Check if the game has been won by the player
	if (pGameManager->bPlayerWon == true)
	{
		// End the game and switch to Win screen
	}
	// Check if the game should be ended
	else if (pGameManager->bPlayerLost == true)
	{
		pSoundController->PlaySoundByID(2);
		pPlayer2D->SetStatus(false);
		return false;
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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
	pMap2D->PreRender();
	// Call the Map2D's Render()
	pMap2D->Render();
	// Call the Map2D's PostRender()
	pMap2D->PostRender();

	pEnemy2DManager->PreRender();
	pEnemy2DManager->Render();
	pEnemy2DManager->PostRender();

	// Call the CPlayer2D's PreRender()
	pPlayer2D->PreRender();
	// Call the CPlayer2D's Render()
	pPlayer2D->Render();
	// Call the CPlayer2D's PostRender()
	pPlayer2D->PostRender();

	// Call the pProjectileManager2D's PreRender()
	pProjectileManager2D->PreRender();
	// Call the pProjectileManager2D's Render()
	pProjectileManager2D->Render();
	// Call the pProjectileManager2D's PostRender()
	pProjectileManager2D->PostRender();

	// Call the pGUI_Scene2D's PreRender()
	pGUI_Scene2D->PreRender();
	// Call the pGUI_Scene2D's Render()
	pGUI_Scene2D->Render();
	// Call the pGUI_Scene2D's PostRender()
	pGUI_Scene2D->PostRender();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene2D::PostRender(void)
{
}