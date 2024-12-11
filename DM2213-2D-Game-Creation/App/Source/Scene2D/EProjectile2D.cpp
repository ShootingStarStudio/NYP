/**
 Player2D
 @brief A class representing the player object
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "EProjectile2D.h"

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
EProjectile2D::EProjectile2D(void)
	: cMap2D(NULL)
	, cGameManager(NULL)
	, cSoundController(NULL)
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
EProjectile2D::~EProjectile2D(void)
{
	// We won't delete this since it was created elsewhere
	cMap2D = NULL;
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);

	cGameManager = NULL;

	if (cSoundController)
	{
		// We won't delete this since it was created elsewhere
		cSoundController = NULL;
	}
}

/**
  @brief Initialise this instance
  */
bool EProjectile2D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();
	// By default, microsteps should be zero
	vec2NumMicroSteps = glm::vec2(0, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	Left = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_EnemyBoltLeft.tga", true);
	Right = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_EnemyBoltRight.tga", true);
	if (Left == 0 || Right == 0)
	{
		cout << "Unable to load Arrow.tga" << endl;
		return false;
	}
	
	//CS: Create the animated sprite and setup the animation 
	animatedSprites = CMeshBuilder::GenerateSpriteAnimation(1, 1, cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);
	animatedSprites->AddAnimation("flight", 0, 0);
	animatedSprites->PlayAnimation("flight", -1, .25f);

	//CS: Init the color to white
	runtimeColour = glm::vec4(1.0, 1.0, 1.0, 1.0);

	cGameManager = CGameManager::GetInstance();

	cSoundController = CSoundController::GetInstance();

	return true;
}

/**
 @brief Update this instance
 */
void EProjectile2D::Update(const double dElapsedTime)
{
	vec2OldIndex = vec2Index;
	//movement here
	if (isLeft == true) {
		if (vec2Index.x >= 0)
		{
			vec2NumMicroSteps.x -= speed;
			if (vec2NumMicroSteps.x < 0)
			{
				vec2NumMicroSteps.x = ((int)cSettings->NUM_STEPS_PER_TILE_XAXIS) - 1;
				vec2Index.x--;
			}
		}
		else {
			projectile = false;
		}
	}
	else if (isLeft == false) {
		if (vec2Index.x < (int)cSettings->NUM_TILES_XAXIS)
		{
			vec2NumMicroSteps.x += speed;
			if (vec2NumMicroSteps.x >= cSettings->NUM_STEPS_PER_TILE_XAXIS)
			{
				vec2NumMicroSteps.x = 0;
				vec2Index.x++;
			}
		}
		else {
			projectile = false;
		}
	}

	if (projectile) {
		InteractWithMap();
	}
	
	animatedSprites->Update(dElapsedTime);

	vec2UVCoordinate.x = cSettings->ConvertIndexToUVSpace(cSettings->x, vec2Index.x, false, vec2NumMicroSteps.x*cSettings->MICRO_STEP_XAXIS);
	vec2UVCoordinate.y = cSettings->ConvertIndexToUVSpace(cSettings->y, vec2Index.y, false, vec2NumMicroSteps.y*cSettings->MICRO_STEP_YAXIS);
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void EProjectile2D::PreRender(void)
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
void EProjectile2D::Render(void)
{
	glBindVertexArray(VAO);
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	unsigned int colorLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "runtimeColour");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	transform = glm::translate(transform, glm::vec3(vec2UVCoordinate.x,
													vec2UVCoordinate.y,
													0.0f));
	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(colorLoc, 1, glm::value_ptr(runtimeColour));

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	// Get the texture to be rendered

	if (isLeft == true) {
		iTextureID = Left;
	}
	else if (isLeft == false) {
		iTextureID = Right;
	}
	
	glBindTexture(GL_TEXTURE_2D, iTextureID);
		//CS: Render the animated sprite
		glBindVertexArray(VAO);
		animatedSprites->Render();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void EProjectile2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

/**
 @brief Constraint the player's position within a boundary
 @param eDirection A DIRECTION enumerated data type which indicates the direction to check
 */
void EProjectile2D::Constraint(DIRECTION eDirection)
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
bool EProjectile2D::CheckPosition(DIRECTION eDirection)
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
		cout << "Projectile::CheckPosition: Unknown direction." << endl;
	}

	return true;
}

/**
 @brief Let player interact with the map. You can add collectibles such as powerups and health here.
 */
void EProjectile2D::InteractWithMap(void)
{
	unsigned int uiRow = -1, uiCol = -1;
	switch (cMap2D->GetMapInfo(vec2Index.y, vec2Index.x))
	{
	case 115:		//target block
		projectile = false;
		while (cMap2D->FindValue(116, uiRow, uiCol) == true) {		//open all doors
			cMap2D->SetMapInfo(uiRow, uiCol, 14);
		}
		cSoundController->PlaySoundByID(5);
		break;
	case 98:		//swamp
		projectile = false;
		cSoundController->PlaySoundByID(5);
		break;
	case 100:		//ground
		projectile = false;
		cSoundController->PlaySoundByID(5);
		break;
	case 101:		//Brick
		projectile = false;
		cSoundController->PlaySoundByID(5);
		break;
	case 110:		//mushroom
		projectile = false;
		cSoundController->PlaySoundByID(5);
		break;
	case 111:		//trunk
		projectile = false;
		cSoundController->PlaySoundByID(5);
		break;
	case 112:		//leaves
		projectile = false;
		cSoundController->PlaySoundByID(5);
		break;
	case 113:		//mango leaves
		projectile = false;
		cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 5);
		cSoundController->PlaySoundByID(5);
		break;
	case 114:		//shrub
		projectile = false;
		cMap2D->SetMapInfo(vec2Index.y, vec2Index.x, 6);
		cSoundController->PlaySoundByID(5);
		break;
	default:		//0
		projectile = true;
		break;
	}
}

void EProjectile2D::SpawnProjectile(bool DirectionL, glm::vec2 position) {
	isLeft = DirectionL;
	vec2Index = position;
}
