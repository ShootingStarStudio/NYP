/**
 Player2D
 @brief A class representing the player object
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Player2D.h"

#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"
#include "Primitives/MeshBuilder.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::CPlayer2D(void)
	: cMap2D(NULL)
	, cKeyboardController(NULL)
	, cSoundController(NULL)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)
	, cGameManager(NULL)
	, cProjectile2D(NULL)
{
	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	vec2Index = glm::vec2(0);

	// Initialise vecNumMicroSteps
	vec2NumMicroSteps = glm::vec2(0);

	// Initialise vec2UVCoordinate
	vec2UVCoordinate = glm::vec2(0.0f);
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::~CPlayer2D(void)
{
	cInventoryManager = NULL;
	cKeyboardController = NULL;
	cSoundController = NULL;
	cMap2D = NULL;
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	cProjectile2D = NULL;

	cGameManager = NULL;
}

/**
  @brief Initialise this instance
  */
bool CPlayer2D::Init(void)
{
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	// Reset all keys since we are starting a new game
	cKeyboardController->Reset();

	// init and load sounds into cSoundController
	cSoundController = CSoundController::GetInstance();

	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();
	// Find the indices for the player in arrMapInfo, and assign it to cPlayer2D
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	if (cMap2D->FindValue(200, uiRow, uiCol) == false)
		return false;	// Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set the start position of the Player to iRow and iCol
	vec2Index = glm::vec2(uiCol, uiRow);
	// By default, microsteps should be zero
	vec2NumMicroSteps = glm::vec2(0, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Load the player texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/PlayerSpriteSheet.png", true);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/PlayerSpriteSheet.png" << endl;
		return false;
	}

	float fGutterSize = 2.0f;

	AnimSprite = CMeshBuilder::GenerateSpriteAnimation(7, 13, cSettings->TILE_WIDTH * 2, cSettings->TILE_HEIGHT * 2);

	AnimSprite->AddAnimation("idleL", 39, 40);
	AnimSprite->AddAnimation("idleR", 45, 46);
	AnimSprite->AddAnimation("walkL", 0, 8);
	AnimSprite->AddAnimation("walkR", 13, 21);
	AnimSprite->AddAnimation("climb", 26, 34);
	AnimSprite->AddAnimation("slashL", 39, 44);
	AnimSprite->AddAnimation("slashR", 45, 50);
	AnimSprite->AddAnimation("shootL", 52, 64);
	AnimSprite->AddAnimation("shootR", 65, 77);
	AnimSprite->AddAnimation("death", 78, 83);
	//CS: Play the "idle" animation as default
	AnimSprite->PlayAnimation("idleR", -1, .75f);

	//CS: Init the color to white
	runtimeColour = glm::vec4(1.0, 1.0, 1.0, 1.0);

	// Set the Physics to fall status by default
	cPhysics2D.Init();
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);

	// Get the handler to the CInventoryManager instance
	cInventoryManager = CInventoryManager::GetInstance();
	cGameManager = CGameManager::GetInstance();

	iJumpCount = 0;

	cInventoryManager = CInventoryManager::GetInstance();
	cInventoryItem = cInventoryManager->Add("Lives", "Image/Scene2D_Lives.tga", 3, 3);
	cInventoryItem->vec2Size = glm::vec2(25, 25);
	ranged = false;

	cInventoryItem = cInventoryManager->Add("Health", "Image/Scene2D_Health.tga", 100, 100);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	cInventoryItem = cInventoryManager->Add("Key", "Image/Scene2D_Key.tga", 3, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

	cProjectile2D = CProjectile2D::GetInstance();

	return true;
}

/**
 @brief Reset this instance
 */
bool CPlayer2D::Reset()
{
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	if (cMap2D->FindValue(200, uiRow, uiCol) == false)
		return false;	// Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set the start position of the Player to iRow and iCol
	vec2Index = glm::vec2(uiCol, uiRow);
	// By default, microsteps should be zero
	vec2NumMicroSteps = glm::vec2(0, 0);

	//Set it to fall upon entering new level
	cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);

	//CS: Reset double jump
	iJumpCount = 0;

	cInventoryManager->GetItem("Health")->Add(100);

	//CS: Play the "idle" animation as default
	AnimSprite->PlayAnimation("idleR", -1, .75f);

	//CS: Init the color to white
	runtimeColour = glm::vec4(1.0, 1.0, 1.0, 1.0);

	return true;
}

/**
 @brief Update this instance
 */
void CPlayer2D::Update(const double dElapsedTime)
{
	vec2OldIndex = vec2Index;
	if (cInventoryManager->GetItem("Health")->GetCount() > 0) {
		if (AnimWait > 0) {
			AnimWait -= dElapsedTime;
		}
		if (cKeyboardController->IsKeyDown(GLFW_KEY_A))
		{
			if (vec2Index.x >= 0)
			{
				vec2NumMicroSteps.x -= multiplier_speed;
				if (vec2NumMicroSteps.x < 0)
				{
					vec2NumMicroSteps.x = ((int)cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
					vec2Index.x--;
				}
			}
			direction = 'A';
			Constraint(LEFT);
			if (CheckPosition(LEFT) == false)
			{
				vec2Index = vec2OldIndex;
				vec2NumMicroSteps.x = 0;
			}
			if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE) {
				AnimSprite->PlayAnimation("walkL", -1, .5f);
			}
		}
		else if (cKeyboardController->IsKeyDown(GLFW_KEY_D))
		{
			if (vec2Index.x < (int)cSettings->NUM_TILES_XAXIS)
			{
				vec2NumMicroSteps.x += multiplier_speed;

				if (vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
				{
					vec2NumMicroSteps.x = 0;
					vec2Index.x++;
				}
			}
			direction = 'D';
			Constraint(RIGHT);
			if (CheckPosition(RIGHT) == false)
			{
				vec2NumMicroSteps.x = 0;
			}
			if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE) {
				AnimSprite->PlayAnimation("walkR", -1, .5f);
			}
		}
		else {
			if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE && AnimWait <= 0.f) {
				if (direction == 'A') {
					AnimSprite->PlayAnimation("idleL", -1, .75f);
				}
				else if (direction == 'D') {
					AnimSprite->PlayAnimation("idleR", -1, .75f);
				}
			}
		}
		if (IsMidAir() == true)
		{
			if (cPhysics2D.GetStatus() != CPhysics2D::STATUS::JUMP)
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}
		if (cKeyboardController->IsKeyPressed(GLFW_KEY_SPACE))
		{
			if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::IDLE)
			{
				cPhysics2D.SetStatus(CPhysics2D::STATUS::JUMP);
				cPhysics2D.SetInitialVelocity(glm::vec2(0.0f, multiplier_jump));
				iJumpCount += 1;
				//play jump sound
				cSoundController->PlaySoundByID(2);
			}
		}

		// Update Jump or Fall
		UpdateJumpFall(dElapsedTime);

		// Interact with the Map
		InteractWithMap();
		if (climbable) {
			if (cKeyboardController->IsKeyDown(GLFW_KEY_W))
			{
				if (vec2Index.y < (int)cSettings->NUM_TILES_YAXIS)
				{
					vec2NumMicroSteps.y += multiplier_speed * 0.5f;
					if (vec2NumMicroSteps.y > cSettings->NUM_STEPS_PER_TILE_YAXIS)
					{
						vec2NumMicroSteps.y = 0;
						vec2Index.y++;
					}
				}
				Constraint(UP);
				if (CheckPosition(UP) == false)
				{
					vec2Index = vec2OldIndex;
					vec2NumMicroSteps.y = 10;
				}
				AnimSprite->PlayAnimation("climb", -1, .5f);
			}
			else if (cKeyboardController->IsKeyDown(GLFW_KEY_S))
			{
				if (vec2Index.y >= 0)
				{
					vec2NumMicroSteps.y -= multiplier_speed * 0.5f;
					if (vec2NumMicroSteps.y < 0)
					{
						vec2NumMicroSteps.y = ((int)cSettings->NUM_STEPS_PER_TILE_YAXIS) - 1;
						vec2Index.y--;
					}
				}
				Constraint(DOWN);
				if (CheckPosition(DOWN) == false)
				{
					vec2Index = vec2OldIndex;
					vec2NumMicroSteps.y = 0;
				}
				AnimSprite->PlayAnimation("climb", -1, .25f);
			}
			else {
				if (vec2NumMicroSteps.y != 0 && cMap2D->GetMapInfo(vec2Index.y - 1 , vec2Index.x) <= 100) {
					AnimSprite->Pause();
				}
			}
		}
		int playerLeft = cMap2D->GetMapInfo(vec2Index.y, vec2Index.x - 1);
		int playerRight = cMap2D->GetMapInfo(vec2Index.y, vec2Index.x + 1);
		if (cKeyboardController->IsKeyPressed(GLFW_KEY_C)) {			//melee
			if (direction == 'A') {
				if (playerLeft == 113) {
					cMap2D->SetMapInfo(vec2Index.y, vec2Index.x - 1, 5);
				}
				else if (playerLeft == 114) {
					cMap2D->SetMapInfo(vec2Index.y, vec2Index.x - 1, 6);
				}
				AnimSprite->PlayAnimation("slashL", 0, .5f);
				AnimWait = 0.6f;
			}
			else if (direction == 'D') {
				if (playerRight == 113) {
					cMap2D->SetMapInfo(vec2Index.y, vec2Index.x + 1, 5);
				}
				else if (playerRight == 114) {
					cMap2D->SetMapInfo(vec2Index.y, vec2Index.x + 1, 6);
				}
				AnimSprite->PlayAnimation("slashR", 0, .5f);
				AnimWait = 0.6f;
			}
			cSoundController->PlaySoundByID(3);
		}
		else if (cKeyboardController->IsKeyPressed(GLFW_KEY_V) && !cProjectile2D->projectile && ranged) {		//ranged
			if (direction == 'A' && playerLeft <= 100) {
				if (cInventoryManager->GetItem("Arrow")->GetCount() > 0) {
					cInventoryManager->GetItem("Arrow")->Remove(1);
					cProjectile2D->projectile = true;
					cProjectile2D->SpawnProjectile(true, glm::vec2(vec2Index.x - 1, vec2Index.y));
				}
				AnimSprite->PlayAnimation("shootL", 0, .5f);
				AnimWait = .6f;
				cSoundController->PlaySoundByID(4);
			}
			else if (direction == 'D' && playerRight <= 100) {
				if (cInventoryManager->GetItem("Arrow")->GetCount() > 0) {
					cInventoryManager->GetItem("Arrow")->Remove(1);
					cProjectile2D->projectile = true;
					cProjectile2D->SpawnProjectile(false, glm::vec2(vec2Index.x + 1, vec2Index.y));
				}
				AnimSprite->PlayAnimation("shootR", 0, .5f);
				AnimWait = .6f;
				cSoundController->PlaySoundByID(4);
			}
		}
		if (cKeyboardController->IsKeyPressed(GLFW_KEY_E)) {
			//interact
			if (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x) == 12) {		//turn lever on
				cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 13);
				unsigned int uiRow = -1, uiCol = -1;
				while (cMap2D->FindValue(116, uiRow, uiCol) == true) {		//unlock locked doors
					cMap2D->SetMapInfo(uiRow, uiCol, 14);
					cSoundController->PlaySoundByID(6);
				}
				interactframe = true;
			}
			else if (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x) == 13) {		//turn lever off
				cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 12);
				unsigned int uiRow = -1, uiCol = -1;
				while (cMap2D->FindValue(14, uiRow, uiCol) == true) {		//lock unlocked doors
					cMap2D->SetMapInfo(uiRow, uiCol, 116);
					cSoundController->PlaySoundByID(7);
				}
				interactframe = true;
			}
		}
		if (cKeyboardController->IsKeyPressed(GLFW_KEY_B)) {
			//build
			for (int i = 0; i < 3; i++) {
				if (cMap2D->GetMapInfo(vec2Index.y + i, vec2Index.x) == 0) {
					if (cInventoryManager->GetItem("Stick")->GetCount() > 0) {
						cInventoryManager->GetItem("Stick")->Remove(1);
						cMap2D->SetMapInfo(vec2Index.y + i, vec2Index.x, 96);
						break;
					}
					else if (cMap2D->GetMapInfo(vec2Index.y + i, vec2Index.x) != 0 &&
						cMap2D->GetMapInfo(vec2Index.y + i, vec2Index.x) != 96) {
						break;
					}
				}
			}
		}
	}
	else {
		if (direction == 'A') {
			AnimSprite->PlayAnimation("deathL", 0, 1.f);
		}
		else if (direction == 'D') {
			AnimSprite->PlayAnimation("deathR", 0, 1.f);
		}
		deathElapsed += dElapsedTime;
		if (deathElapsed >= respawnTimer) {
			deathElapsed = 0;
			if (cInventoryManager->GetItem("Lives")->GetCount() > 0) {
				cInventoryManager->GetItem("Lives")->Remove(1);
				cMap2D->SetCurrentLevel(cMap2D->GetCurrentLevel());				//doesnt work
				Reset();
			}
			else {
				cGameManager->bPlayerLost = true;
			}
			
		}
	}

	AnimSprite->Update(dElapsedTime);

	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, vec2Index.x, false, vec2NumMicroSteps.x*cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, vec2Index.y, false, vec2NumMicroSteps.y*cSettings->MICRO_STEP_YAXIS);
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CPlayer2D::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CPlayer2D::Render(void)
{
	glBindVertexArray(VAO);
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	unsigned int colorLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "runtimeColour");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(
		vec2UVCoordinate.x,
		vec2UVCoordinate.y + (cSettings->TILE_HEIGHT / 2),
		0.0f));
	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(colorLoc, 1, glm::value_ptr(runtimeColour));

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);
		glBindVertexArray(VAO);
		AnimSprite->Render();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

/**
 @brief Constraint the player's position within a boundary
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void CPlayer2D::Constraint(DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		if (vec2Index.x < 0)
		{
			vec2Index.x = 0;
			vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == RIGHT)
	{
		if (vec2Index.x >= (int)cSettings->NUM_TILES_XAXIS - 1)
		{
			vec2Index.x = ((int)cSettings->NUM_TILES_XAXIS) - 1;
			vec2NumMicroSteps.x = 0;
		}
	}
	else if (eDirection == UP)
	{
		if (vec2Index.y >= (int)cSettings->NUM_TILES_YAXIS - 1)
		{
			vec2Index.y = ((int)cSettings->NUM_TILES_YAXIS) - 1;
			vec2NumMicroSteps.y = 0;
		}
	}
	else if (eDirection == DOWN)
	{
		if (vec2Index.y < 0)
		{
			vec2Index.y = 0;
			vec2NumMicroSteps.y = 0;
		}
	}
	else
	{
		cout << "CPlayer2D::Constraint: Unknown direction." << endl;
	}
}

/**
 @brief Check if a position is possible to move into
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
bool CPlayer2D::CheckPosition(DIRECTION eDirection)
{
	if (eDirection == LEFT)
	{
		// If the new position is fully within a row, then check this row only
		if (vec2NumMicroSteps.y == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 rows, then check both rows as well
		else if (vec2NumMicroSteps.y != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(vec2Index.y, vec2Index.x) >= 100) ||
				(cMap2D->GetMapInfo(vec2Index.y + 1, vec2Index.x) >= 100))
			{
				return false;
			}
		}
	}
	else if (eDirection == RIGHT)
	{
		// If the new position is at the top row, then return true
		if (vec2Index.x >= cSettings->NUM_TILES_XAXIS - 1)
		{
			vec2NumMicroSteps.x = 0;
			return true;
		}

		// If the new position is fully within a row, then check this row only
		if (vec2NumMicroSteps.y == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x + 1) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 rows, then check both rows as well
		else if (vec2NumMicroSteps.y != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(vec2Index.y, vec2Index.x + 1) >= 100) ||
				(cMap2D->GetMapInfo(vec2Index.y + 1, vec2Index.x + 1) >= 100))
			{
				return false;
			}
		}

	}
	else if (eDirection == UP)
	{
		// If the new position is at the top row, then return true
		if (vec2Index.y >= cSettings->NUM_TILES_YAXIS - 1)
		{
			vec2NumMicroSteps.y = 0;
			return true;
		}

		// If the new position is fully within a column, then check this column only
		if (vec2NumMicroSteps.x == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(vec2Index.y + 1, vec2Index.x) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 columns, then check both columns as well
		else if (vec2NumMicroSteps.x != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(vec2Index.y + 1, vec2Index.x) >= 100) ||
				(cMap2D->GetMapInfo(vec2Index.y + 1, vec2Index.x + 1) >= 100))
			{
				return false;
			}
		}
	}
	else if (eDirection == DOWN)
	{
		// If the new position is fully within a column, then check this column only
		if (vec2NumMicroSteps.x == 0)
		{
			// If the grid is not accessible, then return false
			if (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x) >= 100)
			{
				return false;
			}
		}
		// If the new position is between 2 columns, then check both columns as well
		else if (vec2NumMicroSteps.x != 0)
		{
			// If the 2 grids are not accessible, then return false
			if ((cMap2D->GetMapInfo(vec2Index.y, vec2Index.x) >= 100) ||
				(cMap2D->GetMapInfo(vec2Index.y, vec2Index.x + 1) >= 100))
			{
				return false;
			}
		}
	}
	else
	{
		cout << "CPlayer2D::CheckPosition: Unknown direction." << endl;
	}

	return true;
}

// Check if the player is in mid-air
bool CPlayer2D::IsMidAir(void)
{
	if (vec2Index.y == 0) return false;
	else if ((vec2NumMicroSteps.x == 0 && ((cMap2D->GetMapInfo(vec2Index.y-1, vec2Index.x) < 100 ||  cMap2D->GetMapInfo(vec2Index.y - 1, vec2Index.x) > 199))) 
			|| vec2NumMicroSteps.y != 0)
	{
		return true;
	}

	return false;
}

// Update Jump or Fall
void CPlayer2D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics2D.SetTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics2D.Update();
		// Get the displacement from the physics engine
		glm::vec2 v2Displacement = cPhysics2D.GetDisplacement();

		// Store the current vec2Index.y
		int iIndex_YAxis_OLD = vec2Index.y;

		int iDisplacement_MicroSteps = (int)(v2Displacement.y / cSettings->MICRO_STEP_YAXIS); //Displacement divide by distance for 1 microstep
		if (vec2Index.y < (int)cSettings->NUM_TILES_YAXIS)
		{
			vec2NumMicroSteps.y += iDisplacement_MicroSteps;
			if (vec2NumMicroSteps.y > cSettings->NUM_STEPS_PER_TILE_YAXIS)
			{
				vec2NumMicroSteps.y -= cSettings->NUM_STEPS_PER_TILE_YAXIS;
				if (vec2NumMicroSteps.y < 0)
					vec2NumMicroSteps.y = 0;
				vec2Index.y++;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(UP);

		// Iterate through all rows until the proposed row
		// Check if the player will hit a tile; stop jump if so.
		int iIndex_YAxis_Proposed = vec2Index.y;
		for (int i = iIndex_YAxis_OLD; i <= iIndex_YAxis_Proposed; i++)
		{
			// Change the player's index to the current i value
			vec2Index.y = i;
			// If the new position is not feasible, then revert to old position
			if (CheckPosition(UP) == false)
			{
				// Align with the row
				vec2NumMicroSteps.y = 0;
				// Set the Physics to fall status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
				break;
			}
		}

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics2D.GetStatus() == CPhysics2D::STATUS::JUMP) && (cPhysics2D.GetDisplacement().y <= 0.0f))
		{
			// Set status to fall
			cPhysics2D.SetStatus(CPhysics2D::STATUS::FALL);
		}
	}
	else if (cPhysics2D.GetStatus() == CPhysics2D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics2D.SetTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics2D.Update();
		// Get the displacement from the physics engine
		glm::vec2 v2Displacement = cPhysics2D.GetDisplacement();

		// Store the current vec2Index.y
		int iIndex_YAxis_OLD = vec2Index.y;

		// Translate the displacement from pixels to indices
		int iDisplacement_MicroSteps = (int)(v2Displacement.y / cSettings->MICRO_STEP_YAXIS);

		if (vec2Index.y >= 0)
		{
			vec2NumMicroSteps.y -= fabs(iDisplacement_MicroSteps);
			if (vec2NumMicroSteps.y < 0)
			{
				vec2NumMicroSteps.y = ((int)cSettings->NUM_STEPS_PER_TILE_YAXIS) - 1;
				vec2Index.y--;
			}
		}

		// Constraint the player's position within the screen boundary
		Constraint(DOWN);

		// Iterate through all rows until the proposed row
		// Check if the player will hit a tile; stop fall if so.
		int iIndex_YAxis_Proposed = vec2Index.y;
		for (int i = iIndex_YAxis_OLD; i >= iIndex_YAxis_Proposed; i--)
		{
			// Change the player's index to the current i value
			vec2Index.y = i;
			// If the new position is not feasible, then revert to old position
			if (CheckPosition(DOWN) == false)
			{
				// Revert to the previous position
				if (i != iIndex_YAxis_OLD)
					vec2Index.y = i + 1;
				// Set the Physics to idle status
				cPhysics2D.SetStatus(CPhysics2D::STATUS::IDLE);
				iJumpCount = 0;
				vec2NumMicroSteps.y = 0;
				break;
			}
		}
	}
}

/**
 @brief Let player interact with the map. You can add collectibles such as powerups and health here.
 */
void CPlayer2D::InteractWithMap(void)
{
	switch (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x))
	{
	case 99:	//campfire
		if (cInventoryManager->GetItem("Stick")->GetCount() == cInventoryManager->GetItem("Stick")->GetMaxCount() &&
			cInventoryManager->GetItem("Mango")->GetCount() == cInventoryManager->GetItem("Mango")->GetMaxCount()) {
			CGameManager::GetInstance()->bLevelCompleted = true;
			cInventoryManager->GetItem("Stick")->Remove(10);
			cInventoryManager->GetItem("Mango")->Remove(10);
		}
		break;
	case 14:	//open door
		if (cMap2D->GetCurrentLevel() == 2) {			//hard code for ending game
			cGameManager->bPlayerWon = true;
		}
		else {
			CGameManager::GetInstance()->bLevelCompleted = true;
			cInventoryManager->GetItem("Health")->Add(100);
		}
		break;
	case 15:		//key
		if (cInventoryManager->GetItem("Key")->GetCount() < cInventoryManager->GetItem("Key")->GetMaxCount()) {
			cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 0);
			cInventoryManager->GetItem("Key")->Add(1);
			interactframe = true;
		}
		break;
	case 16:		//chest
		if (cInventoryManager->GetItem("Key")->GetCount() > 0) {
			cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 17);
			cInventoryManager->GetItem("Key")->Remove(1);
			interactframe = true;
		}
		break;
	case 17:
		//give loot
		break;
	case 2:		//herb
		cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 0);
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Add(20);
		break;
	case 20:	//spike
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(1);
		break;
	case 98:	//swamp
		multiplier_speed = 0.5f;
		break;
	case 96:	//scaffold
		climbable = true;
		cPhysics2D.SetStatus(CPhysics2D::STATUS::IDLE);
		break;
	case 97:	//vine
		climbable = true;
		cPhysics2D.SetStatus(CPhysics2D::STATUS::IDLE);
		break;
	case 3:		//bow
		cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 0);
		ranged = true;
		break;
	case 4:		//quiver
		cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 0);
		cInventoryItem = cInventoryManager->GetItem("Arrow");
		cInventoryItem->Add(10);
		break;
	case 5:		//Mango
		if (cInventoryManager->GetItem("Mango")->GetCount() < cInventoryManager->GetItem("Mango")->GetMaxCount()) {
			cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 0);
			cInventoryManager->GetItem("Mango")->Add(1);
		}
		break;
	case 6:		//Stick
		if (cInventoryManager->GetItem("Stick")->GetCount() < cInventoryManager->GetItem("Stick")->GetMaxCount()) {
			cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 0);
			cInventoryManager->GetItem("Stick")->Add(1);
		}
		break;
	case 12:	//lever off
		//give prompt to press E
		break;
	case 13:	//lever on
		//give prompt to press E
		break;

	//enemy collisions
	case 300:
		cInventoryItem = cInventoryManager->GetItem("Health");
		cInventoryItem->Remove(1);
		break;
	default:		//0
		multiplier_speed = 1.5f;
		multiplier_jump = 2.25f;
		if (cMap2D->GetMapInfo(vec2Index.y - 1, vec2Index.x) == 110) {		//mushroom
			multiplier_jump = 4.f;
		}
		climbable = false;
		break;
	}
}

void CPlayer2D::takeDamage(int damage) {
	cInventoryItem = cInventoryManager->GetItem("Health");
	cInventoryItem->Remove(damage);
}