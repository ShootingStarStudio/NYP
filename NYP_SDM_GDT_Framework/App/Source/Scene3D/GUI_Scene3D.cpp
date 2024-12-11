/**
 CGUI_Scene3D
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "GUI_Scene3D.h"

// Include CBloodScreen
#include "CameraEffects/BloodScreen.h"
// Include CScopeScreen
#include "CameraEffects/ScopeScreen.h"
// Include CCrossHair
#include "CameraEffects/CrossHair.h"
// Include CCameraShake
#include "CameraEffects/CameraShake.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene3D::CGUI_Scene3D(void)
	: pSettings(NULL)
	, fProgressBar(0.0f)
	, cCameraEffectsManager(NULL)
	, pMinimap(NULL)
	, pInventoryManager(NULL)
	, pInventoryItem(NULL)
	, pPlayer3D(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene3D::~CGUI_Scene3D(void)
{
	DeInit();
}

/**
  @brief Reset this instance
  */
void CGUI_Scene3D::Reset(void)
{
	if (pMinimap)
	{
		// DeInit the CMinimap instance
		pMinimap->DeInit();

		// Init the CMinimap instance
		pMinimap->Init();
	}

	if (cCameraEffectsManager)
	{
		CBloodScreen* cBloodScreen = (CBloodScreen*)cCameraEffectsManager->Get("BloodScreen");
		if (cBloodScreen)
		{
			cBloodScreen->DeInit();
			cBloodScreen->Init();
		}

		CScopeScreen* cScopeScreen = (CScopeScreen*)cCameraEffectsManager->Get("ScopeScreen");
		if (cScopeScreen)
		{
			cScopeScreen->DeInit();
			cScopeScreen->Init();
		}

		CCrossHair* cCrossHair = (CCrossHair*)cCameraEffectsManager->Get("CrossHair");
		if (cCrossHair)
		{
			cCrossHair->DeInit();
			cCrossHair->Init();
		}
	}
}

/**
 @brief Initialise this instance
 */
bool CGUI_Scene3D::Init(void)
{
	// Get the handler to the CSettings instance
	pSettings = CSettings::GetInstance();

	// Store the CFPSCounter singleton instance here
	pFPSCounter = CFPSCounter::GetInstance();

	// Load the CCameraEffectsManager
	cCameraEffectsManager = CCameraEffectsManager::GetInstance();
	cCameraEffectsManager->Init();

	// Add the CameraEffects
	// Add BloodScreen
	CBloodScreen* cBloodScreen = new CBloodScreen();
	cBloodScreen->Init();
	cBloodScreen->SetShader("Shader2D");
	cCameraEffectsManager->Add("BloodScreen", cBloodScreen);
	// Add ScopeScreen
	CScopeScreen* cScopeScreen = new CScopeScreen();
	cScopeScreen->Init();
	cScopeScreen->SetShader("Shader2D");
	cCameraEffectsManager->Add("ScopeScreen", cScopeScreen);
	// Add CrossHair
	CCrossHair* cCrossHair = new CCrossHair();
	cCrossHair->Init();
	cCrossHair->SetShader("Shader2D");
	cCameraEffectsManager->Add("CrossHair", cCrossHair);
	// Add CameraShake
	CCameraShake* cCameraShake = new CCameraShake();
	cCameraShake->Init();
	cCameraEffectsManager->Add("CameraShake", cCameraShake);

	// Load the Minimap
	pMinimap = CMinimap::GetInstance();
	// Set a shader to this class instance of Minimap
	pMinimap->SetShader("Shader3D_RenderToTexture");
	pMinimap->Init();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(CSettings::GetInstance()->pWindow, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Define the window flags
	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// Get the handler to the CInventoryManager instance
	pInventoryManager = CInventoryManager::GetInstance();
	// Add a Lives icon as one of the inventory items
	pInventoryItem = pInventoryManager->Add("Lives", "Image/Scene2D_Lives.tga", 3, 0);
	pInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a Health icon as one of the inventory items
	pInventoryItem = pInventoryManager->Add("Health", "Image/Scene2D_Health.tga", 100, 100);
	pInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a pistol as one of the inventory items
	pInventoryItem = pInventoryManager->Add("Pistol", "Image/Scene2D_TreeTile.tga", 5, 0);
	pInventoryItem->vec2Size = glm::vec2(25, 25);

	// Get the handler to the CPlayer3D instance
	pPlayer3D = CPlayer3D::GetInstance();

	return true;
}

/**
 @brief DeInitialise this instance
 */
void CGUI_Scene3D::DeInit(void)
{
	// Set the pPlayer3D to NULL since it was initialised elsewhere
	pPlayer3D = NULL;

	// Destroy the pInventoryManager and set it to NULL
	if (pInventoryManager)
	{
		pInventoryManager->Destroy();
		pInventoryManager = NULL;
	}

	// Cleanup IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Destroy the pMinimap and set it to NULL
	if (pMinimap)
	{
		pMinimap->Destroy();
		pMinimap = NULL;
	}

	// Destroy the cCameraEffects and set it to NULL
	if (cCameraEffectsManager)
	{
		cCameraEffectsManager->Destroy();
		cCameraEffectsManager = NULL;
	}

	// Set the pSettings to NULL since it was initialised elsewhere
	pSettings = NULL;
}


/**
 @brief Update this instance
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CGUI_Scene3D::Update(const double dElapsedTime)
{
	// Calculate the relative scale to our default windows width
	const float relativeScale_x = CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) / 800.0f;
	const float relativeScale_y = CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f) / 600.0f;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f),
								CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f)));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);

	// Display the FPS
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS: %d", pFPSCounter->GetFrameRate());

	// If the camera is not attached to the player then don't update the GUI
	if (!pPlayer3D->IsCameraAttached())
	{
		ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background colour
		ImGui::Begin("PlayerPos", NULL, inventoryWindowFlags);
		ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) * 0.55f, 
									CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f) * 0.9f));
		ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
		ImGui::SetWindowFontScale(1.0f * relativeScale_y);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Player: %3.2f, %3.2f, %3.2f / %3.2f, %3.2f, %3.2f",
			pPlayer3D->GetPosition().x,
			pPlayer3D->GetPosition().y,
			pPlayer3D->GetPosition().z,
			pPlayer3D->GetFront().x,
			pPlayer3D->GetFront().y,
			pPlayer3D->GetFront().z);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Cam: %3.2f, %3.2f, %3.2f / %3.2f, %3.2f, %3.2f",
			CCamera::GetInstance()->vec3Position.x,
			CCamera::GetInstance()->vec3Position.y,
			CCamera::GetInstance()->vec3Position.z,
			CCamera::GetInstance()->vec3Front.x,
			CCamera::GetInstance()->vec3Front.y,
			CCamera::GetInstance()->vec3Front.z);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Camera view: %s", (CCamera::GetInstance()->GetCameraViewName()).c_str());
		ImGui::End();
		ImGui::PopStyleColor();

		ImGui::End();

		return false;
	}

	// Render the Health
	ImGuiWindowFlags healthWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Health", NULL, healthWindowFlags);
	ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) * 0.03f,
								CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f) * 0.03f));
	ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	pInventoryItem = pInventoryManager->GetItem("Health");
	ImGui::Image((void*)(intptr_t)pInventoryItem->GetTextureID(),
		ImVec2(pInventoryItem->vec2Size.x * relativeScale_x, pInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::ProgressBar(pInventoryItem->GetCount() /
		(float)pInventoryItem->GetMaxCount(), ImVec2(100.0f * relativeScale_x, 20.0f * relativeScale_y));
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::End();

	// Render the Lives
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background colour
	ImGuiWindowFlags livesWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Lives", NULL, livesWindowFlags);
	ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) * 0.03f, 
								CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f) * 0.09f));
	ImGui::SetWindowSize(ImVec2(100.0f * relativeScale_x, 25.0f * relativeScale_y));
	pInventoryItem = pInventoryManager->GetItem("Lives");
	ImGui::Image((void*)(intptr_t)pInventoryItem->GetTextureID(),
		ImVec2(pInventoryItem->vec2Size.x * relativeScale_x, pInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d / %d",
		pInventoryItem->GetCount(), pInventoryItem->GetMaxCount());
	ImGui::End();
	ImGui::PopStyleColor();

	// Render the inventory items
	pInventoryItem = pInventoryManager->GetItem("Pistol");
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background colour
	ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Image", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) * 0.03f,
								CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f) * 0.9f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::Image((void*)(intptr_t)pPlayer3D->GetWeapon()->GetTextureID(),
		ImVec2(pInventoryItem->vec2Size.x * relativeScale_x, pInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Ammo : %d / %d",
		pPlayer3D->GetWeapon()->GetMagRound(), pPlayer3D->GetWeapon()->GetMaxMagRound());
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Total Ammo : %d / %d",
		pPlayer3D->GetWeapon()->GetTotalRound(), pPlayer3D->GetWeapon()->GetMaxTotalRound());
	ImGui::End();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));  // Set a background colour
	ImGui::Begin("PlayerPos", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f) * 0.55f, 
								CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f) * 0.9f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::SetWindowFontScale(1.0f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Player: %3.2f, %3.2f, %3.2f / %3.2f, %3.2f, %3.2f",
		pPlayer3D->GetPosition().x,
		pPlayer3D->GetPosition().y,
		pPlayer3D->GetPosition().z,
		pPlayer3D->GetFront().x,
		pPlayer3D->GetFront().y,
		pPlayer3D->GetFront().z);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Cam: %3.2f, %3.2f, %3.2f / %3.2f, %3.2f, %3.2f",
		CCamera::GetInstance()->vec3Position.x,
		CCamera::GetInstance()->vec3Position.y,
		CCamera::GetInstance()->vec3Position.z,
		CCamera::GetInstance()->vec3Front.x,
		CCamera::GetInstance()->vec3Front.y,
		CCamera::GetInstance()->vec3Front.z);
		ImGui::End();
	ImGui::PopStyleColor();

	ImGui::End();


	// Update camera effects
	cCameraEffectsManager->Update(dElapsedTime);

	return true;
}


/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI_Scene3D::PreRender(void)
{
}

/**
 @brief Render this instance
 */
void CGUI_Scene3D::Render(void)
{
	// If the camera is not attached to the player then don't render the GUI
	if (!pPlayer3D->IsCameraAttached())
	{
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		return;
	}

	// Render the camera effects
	cCameraEffectsManager->Render();

	// Render the minimap
	pMinimap->PreRender();
	pMinimap->Render();
	pMinimap->PostRender();

	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		pPlayer3D->GetWeapon()->SetProjection(projection);
		pPlayer3D->GetWeapon()->PreRender();
		pPlayer3D->GetWeapon()->Render();
		pPlayer3D->GetWeapon()->PostRender();
	//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI_Scene3D::PostRender(void)
{
	glDisable(GL_DEPTH_TEST);
}
