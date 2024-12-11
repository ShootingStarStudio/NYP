/**
 BackgroundEntity
 @brief A class which stores a background entity for the game states
 By: Lim Chian Song
 Date: May 2020
 */
#include "BackgroundEntity.h"

 // Include Shader Manager
#include "RenderControl\ShaderManager.h"

 // Include shader
#include "RenderControl\shader.h"

// For allowing creating of Quad Mesh 
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
#include <vector>

using namespace std;

/**
 @brief Constructor
 @param name A std::string variable containing the name of the texture
 */
CBackgroundEntity::CBackgroundEntity(std::string name)
{
	p2DMesh = NULL;

	sTextureName = name;
}

/**
 @brief Destructor
 */
CBackgroundEntity::~CBackgroundEntity(void)
{
	DeInit();
}

/**
  @brief Initialise this instance
  @return true is the initialisation is successful, otherwise false
  */
bool CBackgroundEntity::Init(void)
{
	// Set up the projection matrix
	projection = glm::ortho(0.0f,
							CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f),
							0.0f,
							CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f),
							-1.0f, 1.0f);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowWidth", 800.0f), 
										CSettings::GetInstance()->cSimpleIniA.GetFloatValue("Size", "iWindowHeight", 600.0f), 
										1.0f));

	//CS: Create the Quad Mesh using the Mesh builder
	p2DMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 1, 1, false);

	// Load a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID(sTextureName.c_str(), true);
	if (iTextureID == 0)
	{
		cout << "Unable to load " << sTextureName.c_str() << endl;
		return false;
	}

	return true;
}

/**
 @brief DeInitialise this instance
 */
void CBackgroundEntity::DeInit(void)
{
	if (p2DMesh)
	{
		delete p2DMesh;
		p2DMesh = NULL;
	}
}

/**
 @brief Render this instance
 */
void CBackgroundEntity::Render(void)
{
	CShaderManager::GetInstance()->Use(sShaderName);

	// Call the parent's Render()
	CEntity2D::Render();
}
