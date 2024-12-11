// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "Play3DGameState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CPlay3DGameState::CPlay3DGameState(void)
	: pScene3D(NULL)
{

}

/**
 @brief Destructor
 */
CPlay3DGameState::~CPlay3DGameState(void)
{

}

/**
 @brief Init this class instance
 */
bool CPlay3DGameState::Init(void)
{
	// Initialise the CScene3D instance
	pScene3D = CScene3D::GetInstance();
	if (pScene3D->Init() == false)
	{
		cout << "Failed to load CScene3D" << endl;
		return false;
	}

	return true;
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CPlay3DGameState::Update(const double dElapsedTime)
{
	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the MenuState
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		CGameStateManager::GetInstance()->OffPauseGameState();
		return true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_F10))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		CGameStateManager::GetInstance()->SetPauseGameState("PauseState");
	}

	// Call the CScene3D's Update method
	pScene3D->Update(dElapsedTime);

	return true;
}

/**
 @brief Render this class instance
 */
void CPlay3DGameState::Render(void)
{
	//cout << "CPlay3DGameState::Render()\n" << endl;

	// Call the CScene3D's Pre-Render method
	pScene3D->PreRender();

	// Call the CScene3D's Render method
	pScene3D->Render();

	// Call the CScene3D's PostRender method
	pScene3D->PostRender();
}

/**
 @brief Destroy this class instance
 */
void CPlay3DGameState::Destroy(void)
{
	cout << "CPlay3DGameState::Destroy()\n" << endl;

	// Destroy the CScene3D instance
	if (pScene3D)
	{
		pScene3D->Destroy();
		pScene3D = NULL;
	}
}