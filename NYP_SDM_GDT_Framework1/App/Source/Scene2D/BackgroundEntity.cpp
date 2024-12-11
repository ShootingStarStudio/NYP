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
	if (p2DMesh)
	{
		delete p2DMesh;
		p2DMesh = NULL;
	}
}

/**
  @brief Initialise this instance
  @return true is the initialisation is successful, otherwise false
  */
bool CBackgroundEntity::Init(void)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

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

	// Load a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID(sTextureName.c_str(), true);
	if (iTextureID == 0)
	{
		cout << "Unable to load " << sTextureName.c_str() << endl;
		return false;
	}

	//CS: Create the Quad Mesh using the Mesh builder
	p2DMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 1, 1, false);

	return true;
}

/**
 @brief Render this instance
 */
void CBackgroundEntity::Render(void)
{
	CShaderManager::GetInstance()->Use(sShaderName);

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	CShaderManager::GetInstance()->pActiveShader->setMat4("Model", model);
	CShaderManager::GetInstance()->pActiveShader->setMat4("Projection", projection);
	unsigned int colourLoc = glGetUniformLocation(CShaderManager::GetInstance()->pActiveShader->ID, "ColourTint");
	//vec4ColourTint = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(colourLoc, 1, glm::value_ptr(glm::vec4(1.0, 1.0, 1.0, 1.0)));

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
