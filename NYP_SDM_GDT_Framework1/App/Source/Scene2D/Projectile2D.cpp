/**
 CProjectile2D
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "Projectile2D.h"

#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CProjectile2D::CProjectile2D(void)
	: dLifetime(0.0f)
	, fSpeed(1.0f)
	, pSource(NULL)
	, vec2Direction(glm::vec2(0.0f, 0.0f))
	, vec4Colour(glm::vec4(0.0f))
	, vec2PreviousPosition(glm::vec2(0.0f, 0.0f))
	, vec2Scale(glm::vec2(0.0f, 0.0f))
	, bVisible(false)
	, pMap2D(NULL)
{
}

/**
@brief Destructor
*/
CProjectile2D::~CProjectile2D(void)
{
	// We won't delete this since it was created elsewhere
	pMap2D = NULL;

	// We set it to NULL only since it was declared somewhere else
	pSource = NULL;
}

/** 
 @brief Set the position and direction of the projectile
 @param vec2Position A const glm::vec2 variable which contains the position of the projectile
 @param vec2Direction A const glm::vec2 variable which contains the up direction of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CProjectile2D::Init(	glm::vec2 vec2Position,
						glm::vec2 vec2Direction,
						const double dLifetime,
						const float fSpeed)
{
	// Call the parent's Init()
	CEntity2D::Init();

	// Set the attributes for this projectile
	this->vec2Position = vec2Position;
	this->vec2Direction = vec2Direction;
	this->dLifetime = dLifetime;
	this->fSpeed = fSpeed;
	vec4Colour = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate the p2DMesh
	p2DMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 1, 1);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene2D_Projectile.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene2D_Projectile.tga" << endl;
		return false;
	}

	// Set this projectile to be visible
	bVisible = true;

	// Get the handler to the CMap2D instance
	pMap2D = CMap2D::GetInstance();

	return true;
}


/** 
 @brief Activate the projectile. true == active, false == inactive
 @param bStatus A const bool variable which is true if this projectile is active, otherwise false.
 */
void CProjectile2D::SetStatus(const bool bStatus)
{
	if (bStatus == false)
		dLifetime = 0.0f;
	this->bStatus = bStatus;
}

/** 
 @brief Set the direction of the projectile
 @param vec2Direction A const glm::vec2 variable which contains the up direction of the projectile
 */
void CProjectile2D::SetDirection(glm::vec2 vec2Direction)
{
	this->vec2Direction = vec2Direction;
}

/** 
 @brief Set the lifetime of the projectile
 @param dLifetime A const float variable which contains the lifetime for this projectile
 */
void CProjectile2D::SetLifetime(const double dLifetime)
{
	this->dLifetime = dLifetime;
}

/** 
 @brief Set the speed of the projectile
 @param fSpeed A const float variable which contains the speed of the projectile
 */
void CProjectile2D::SetSpeed(const float fSpeed)
{
	this->fSpeed = fSpeed;
}

/** 
 @brief Set the source of the projectile
 @param pSource A CEntity2D* variable which is the CEntity2D who created this projectile
 */
void CProjectile2D::SetSource(CEntity2D* pSource)
{
	this->pSource = pSource;
}

/** 
 @brief Get the direction of the projectile
 @return A const glm::vec2 variable which contains the up direction of the projectile
 */
glm::vec2 CProjectile2D::GetDirection(void)
{
	return vec2Direction;
}

/** 
 @brief Get the lifetime of the projectile
 @return A const double variable which contains the lifetime for this projectile
 */
double CProjectile2D::GetLifetime(void) const
{
	return dLifetime;
}

/** 
 @brief Get the speed of the projectile
 @return A const float variable which contains the speed of the projectile
 */
float CProjectile2D::GetSpeed(void) const
{
	return fSpeed;
}

/** 
 @brief Get the source of the projectile
 @return A CEntity2D* variable which is the CEntity2D who created this projectile
 */
CEntity2D* CProjectile2D::GetSource(void) const
{
	return pSource;
}

/** 
 @brief Update this projectile
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
*/
bool CProjectile2D::Update(const double dElapsedTime)
{
	if (bStatus == false)
		return false;

	// Update TimeLife of projectile. Set to inactive if too long
	dLifetime -= dElapsedTime;
	if (dLifetime < 0.0f)
	{
		SetStatus(false);
	}

	// Store current position
	vec2PreviousPosition = vec2Position;
	// Update Position
	vec2Position = vec2Position + vec2Direction * (float)dElapsedTime * fSpeed;
	// For calculating the collision point's x-coordinate
	float fCollisionCoordX = 0;
	// For calculating the collision point's y-coordinate
	float fCollisionCoordY = 0;
	// Check if the Projectile collided with solid objects in the Map2D
	if (pMap2D->CheckHorizontalCollision(vec2PreviousPosition, vec2HalfSize, vec2Position, fCollisionCoordX) == CSettings::RESULTS::POSITIVE)
	{
		vec2Position.x = (float)fCollisionCoordX;
		SetStatus(false);
	}
	if (pMap2D->CheckVerticalCollision(vec2PreviousPosition, vec2HalfSize, vec2Position, fCollisionCoordY) == CSettings::RESULTS::POSITIVE)
	{
		vec2Position.y = (float)fCollisionCoordY;
		SetStatus(false);
	}

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CProjectile2D::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/** 
 @brief Render this projectile
 */
void CProjectile2D::Render(void)
{
	if (bStatus == false)
		return;

	if (dLifetime < 0.0f)
		return;
	
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(vec2Position, 0.0f));
	model = glm::scale(model, glm::vec3(25.0f, 25.0f, 1.0f));

	// Upload the model to OpenGL shader
	CShaderManager::GetInstance()->pActiveShader->setMat4("Model", model);
	// Upload the Projection to OpenGL shader
	CShaderManager::GetInstance()->pActiveShader->setMat4("Projection", projection);
	unsigned int colourLoc = glGetUniformLocation(CShaderManager::GetInstance()->pActiveShader->ID, "ColourTint");
	glUniform4fv(colourLoc, 1, glm::value_ptr(vec4ColourTint));

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);
		//CS: Render the animated sprite
		glBindVertexArray(VAO);
			p2DMesh->Render();
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CProjectile2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

/**
@brief PrintSelf
*/
void CProjectile2D::PrintSelf(void)
{
	// Display the specifications of this projectile
	cout << "Position: [" << vec2Position.x << ", " << vec2Position.y << ", " << "], ";
	cout << "Direction: [" << vec2Direction.x << ", " << vec2Direction.y << ", " << "], ";
	cout << "LifeTime: " << dLifetime << ", Speed: " << fSpeed << ", Status: ";
	if (bStatus == true)
		cout << "Active" << endl;
	else
		cout << "Inactive" << endl;
}
