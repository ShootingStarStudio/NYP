// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "PlayGameState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CPlayGameState::CPlayGameState(void)
	: pScene2D(NULL)
{

}

/**
 @brief Destructor
 */
CPlayGameState::~CPlayGameState(void)
{

}

/**
 @brief Init this class instance
 */
bool CPlayGameState::Init(void)
{
	// Initialise the CScene2D instance
	pScene2D = CScene2D::GetInstance();
	if (pScene2D->Init() == false)
	{
		cout << "Failed to load CScene2D" << endl;
		return false;
	}

	// Enable the cursor
	if (CSettings::GetInstance()->cSimpleIniA.GetBoolValue("Mouse", "bUseMousePointerForGameplay", "false") == true)
			glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	return true;
}

/**
 @brief DeInit this class instance
 */
void CPlayGameState::DeInit(void)
{
}

/**
 @brief Reset this class instance
 */
void CPlayGameState::Reset(void)
{
	// Reset the pScene3D instance
	pScene2D->Reset();
}

/**
 @brief Update this class instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CPlayGameState::Update(const double dElapsedTime)
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

		// Load the PauseState
		CGameStateManager::GetInstance()->SetPauseGameState("PauseState");
	}

	// Call the CScene2D's Update method
	if (pScene2D->Update(dElapsedTime) == false)
	{
		// Load the MenuState
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		CGameStateManager::GetInstance()->OffPauseGameState();
		return true;
	}

	return true;
}

/**
 @brief Render this class instance
 */
void CPlayGameState::Render(void)
{
	//cout << "CPlayGameState::Render()\n" << endl;

	// Call the CScene2D's Pre-Render method
	pScene2D->PreRender();

	// Call the CScene2D's Render method
	pScene2D->Render();

	// Call the CScene2D's PostRender method
	pScene2D->PostRender();
}

/**
 @brief Destroy this class instance
 */
void CPlayGameState::Destroy(void)
{
	cout << "CPlayGameState::Destroy()\n" << endl;

	// Disable the cursor
	if (CSettings::GetInstance()->cSimpleIniA.GetBoolValue("Mouse", "bDisableMousePointer", "false") == true)
		glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		// Hide the cursor
		if (CSettings::GetInstance()->cSimpleIniA.GetBoolValue("Mouse", "bShowMousePointer", "true") == false)
			glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Destroy the CScene2D instance
	if (pScene2D)
	{
		pScene2D->Destroy();
		pScene2D = NULL;
	}
}