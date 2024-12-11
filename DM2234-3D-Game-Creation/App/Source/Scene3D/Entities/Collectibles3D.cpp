/**
 CCollectible3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Collectibles3D.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;
/**
 @brief Default Constructor
 */
CCollectible3D::CCollectible3D(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, +fHeightOffset, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CCollectible3D::CCollectible3D(	const glm::vec3 vec3Position)
{
	this->vec3Position = vec3Position;
	this->vec3Position.y += +fHeightOffset;
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CCollectible3D::~CCollectible3D(void)
{

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}

	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCollectible3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::COLLECTIBLES);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1), 0.15, 0.15, 0.15);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Crate.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Crate.tga" << endl;
		return false;
	}

	ID = 0;
	value = 0;

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CCollectible3D::Update(const double dElapsedTime)
{
	// Don't update if this entity is not active
	if (bStatus == false || iHealth<=0)
	{
		bStatus = false;
		return false;
	}

	UpdateJumpFall(dElapsedTime);

	// Store the enemy's current position, if rollback is needed.
	StorePositionForRollback();

	// Update the model
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, vec3Position);
	model = glm::scale(model, vec3Scale);

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CCollectible3D::PreRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CCollectible3D::Render(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CCollectible3D::PostRender(void)
{
	// If this entity is not active, then skip this
	if (bStatus == false)
	{
		return;
	}

	CSolidObject::PostRender();
}

/**
 @brief Constraint the player's position
 */
void CCollectible3D::Constraint(void)
{
	// Get the new height
	float fNewYValue = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	// Smooth out the change in height
	vec3Position.y += (fNewYValue - vec3Position.y) * 0.05f;
}

int CCollectible3D::getID(void) {
	return ID;
}

float CCollectible3D::getValue(void) {
	return value;
}