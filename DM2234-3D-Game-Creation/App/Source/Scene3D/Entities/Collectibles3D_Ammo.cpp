/**
 CCollectible3D_Ammo
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Collectibles3D_Ammo.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

// Set this to true if printing the debug info
#define _DEBUG_FSM false

/**
 @brief Default Constructor
 */
CCollectible3D_Ammo::CCollectible3D_Ammo(void)
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
CCollectible3D_Ammo::CCollectible3D_Ammo(	const glm::vec3 vec3Position)
{
	this->vec3Position = vec3Position;
	this->vec3Position.y += +fHeightOffset;
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CCollectible3D_Ammo::~CCollectible3D_Ammo(void)
{
	// Delete the rendering objects in the graphics card
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CCollectible3D_Ammo::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::COLLECTIBLES);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	mesh = CMeshBuilder::GenerateBox(glm::vec4(1, 1, 1, 1), 0.5, 0.5, 0.5);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Ammo.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Ammo.tga" << endl;
		return false;
	}

	ID = 2;
	value = 1;		//refills 1 mag

	return true;
}
