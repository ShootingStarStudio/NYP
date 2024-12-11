/**
 CGrass
 By: Toh Da Jun
 Date: October 2023
 */
#include "Grass3D.h"
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include CTerrainManager
#include "../Terrain/TerrainManager.h"
// Include CCamera
#include "../Camera.h"

#include <gl/glew.h>

#include <iostream>
using namespace std;

/**b
 @brief Default Constructor
 */
CGrass3D::CGrass3D(void)
	: fTimePassed(0.0f)
	, fSpacing(0.5f)
	, fGrassMinSize(0.4f)
	, bWindStatus(true)
	, fWindStrength(0.02f)
	, pTerrainManager(NULL)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
	vec2WindStrength = glm::vec2(1.0f, 1.0f);
	vMinPos = glm::vec2(-50.0f, -50.0f);
	vMaxPos = glm::vec2(50.0f, 50.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 */
CGrass3D::CGrass3D(	const glm::vec3 vec3Position)
	: fTimePassed(0.0f)
	, fSpacing(0.5f)
	, fGrassMinSize(0.4f)
	, bWindStatus(true)
	, fWindStrength(0.02f)
	, pTerrainManager(NULL)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;
	vec2WindStrength = glm::vec2(1.0f, 1.0f);
	vMinPos = glm::vec2(-50.0f, -50.0f);
	vMaxPos = glm::vec2(50.0f, 50.0f);
}

/**
 @brief Destructor
 */
CGrass3D::~CGrass3D(void)
{
	DeInit();
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CGrass3D::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Store the handler to the pTerrainManager
	pTerrainManager = CTerrainManager::GetInstance();

	// ========================================
	//            CREATE POSITIONS
	// ========================================
	srand((unsigned int)time(NULL));

	float randomisedX, randomisedZ;
	float fPosY;
	for (float x = vMinPos.x; x < vMaxPos.x; x += fSpacing)
		for (float z = vMinPos.y; z < vMaxPos.y; z += fSpacing)
		{
			randomisedX = x + (rand() % 10 - 5.0f) / 2.5f;
			randomisedZ = z + (rand() % 10 - 5.0f) / 2.5f;
			
			if (randomisedX > vMaxPos.x - fSpacing)
				randomisedX = vMaxPos.x - fSpacing;
			if (randomisedX < vMinPos.x + fSpacing)
				randomisedX = vMinPos.x + fSpacing;
			if (randomisedZ > vMaxPos.y - fSpacing)
				randomisedZ = vMaxPos.y - fSpacing;
			if (randomisedZ < vMinPos.y + fSpacing)
				randomisedZ = vMinPos.y + fSpacing;

			if (pTerrainManager)
				fPosY = pTerrainManager->GetHeight(randomisedX, randomisedZ) + fOffsetY;
			else
				fPosY = 0.0f;

			positions.push_back(glm::vec3(randomisedX, fPosY, randomisedZ));
		}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// ========================================
	//            GENERATE TEXTURE
	// ========================================
	iTextureID1 = CImageLoader::GetInstance()->LoadTextureGetID("Image/Terrain/textures/grass_texture.png", true);
	if (iTextureID1 == 0)
	{
		return false;
	}

	iTextureID2 = CImageLoader::GetInstance()->LoadTextureGetID("Image/Terrain/textures/flowmap.png", false);
	if (iTextureID2 == 0)
	{
		return false;
	}

	// Use the shader now as we are sending some values to it
	CShaderManager::GetInstance()->Use(sShaderName);
	CShaderManager::GetInstance()->pActiveShader->setInt("u_texture1", 0);
	CShaderManager::GetInstance()->pActiveShader->setFloat("u_grass_minsize", fGrassMinSize);
	CShaderManager::GetInstance()->pActiveShader->setInt("u_wind", int(bWindStatus));
	CShaderManager::GetInstance()->pActiveShader->setVec2("u_windDirection", vec2WindStrength);
	CShaderManager::GetInstance()->pActiveShader->setFloat("u_windStrength", fWindStrength);

	return true;
}

/**
@brief DeInitialise this instance
*/
void CGrass3D::DeInit(void)
{
	if (pTerrainManager)
	{
		// We set it to NULL only since it was declared somewhere else
		pTerrainManager = NULL;
	}

	// ========================================
	//              CLEAN UP
	// ========================================
	glDisableVertexAttribArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &iTextureID1);
	glDeleteTextures(1, &iTextureID2);
}

/**
  @brief Update the elapsed time
  @param dElapsedTime A const double variable containing the elapsed time since the last frame
  @return A bool variable to indicate this method successfully completed its tasks
  */
bool CGrass3D::Update(const double dElapsedTime)
{
	fTimePassed += (float)dElapsedTime;

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CGrass3D::PreRender(void)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glPointSize(5.0f); // comment this line to reset points to normal size

	CShaderManager::GetInstance()->Use(sShaderName);
	// projection matrix
	CShaderManager::GetInstance()->pActiveShader->setMat4("u_projection", projection);

	// view transformation
	CShaderManager::GetInstance()->pActiveShader->setMat4("u_view", view);

	// bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, iTextureID2);
}

/**
 @brief Render Render this instance
 */
void CGrass3D::Render(void)
{
	// update view
	CShaderManager::GetInstance()->pActiveShader->setMat4("u_view", view);
	CShaderManager::GetInstance()->pActiveShader->setVec3("u_cameraPosition", vec3Position);
	CShaderManager::GetInstance()->pActiveShader->setFloat("u_time", fTimePassed);

	// draw
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, positions.size());
	glBindVertexArray(0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGrass3D::PostRender(void)
{
	glPointSize(1.0f); // comment this line to reset points to normal size
	glDisable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
}

/**
 @brief GetWindStatus Get the wind status.
 @return bool Returns true if the wind status is switched on, otherwise false
 */
bool CGrass3D::GetWindStatus(void) const
{
	return bWindStatus;
}
/**
 @brief GetMinPos Get the vMinPos.
 @return glm::vec2 Returns the minimum position of the grass spawn area
 */
glm::vec2 CGrass3D::GetMinPos(void) const
{
	return vMinPos;
}
/**
 @brief GetMaxPos Get the vMaxPos
 @return glm::vec2 Returns the maximum position of the grass spawn area
 */
glm::vec2 CGrass3D::GetMaxPos(void) const
{
	return vMaxPos;
}

/**
 @brief GetSize Get the Grass Min Size
 @return float Returns the minimum size of each grass 
 */
float CGrass3D::GetGrassMinSize(void) const
{
	return fGrassMinSize;
}

/**
 @brief GetSpacing Get the fSpacing
 */
float CGrass3D::GetSpacing(void) const
{
	return fSpacing;
}

/**
 @brief GetMaxPos Get the vMaxPos
 @param bWindStatus A const bool variable which determines if the wind status is switched on
 */
void CGrass3D::SetWindStatus(const bool bWindStatus)
{
	this->bWindStatus = bWindStatus;
}

/**
 @brief GetMaxPos Get the vMaxPos
 @param vMinPos A glm::vec2 variable which is the minimum position for the grass to spawn within
 @param vMaxPos A glm::vec2 variable which is the maximum position for the grass to spawn within
 */
void CGrass3D::SetMinMaxPos(glm::vec2 vMinPos, glm::vec2 vMaxPos)
{
	this->vMinPos = vMinPos;
	this->vMaxPos = vMaxPos;
}

/**
 @brief SetSize Set the Grass Min Size
 @param fGrassMinSize A const float variable which is the minimum size for each grass
 */
void CGrass3D::SetGrassMinSize(const float fGrassMinSize)
{
	this->fGrassMinSize = fGrassMinSize;
}

/**
@brief SetSpacing Set the fSpacing
 @param fSpacing A const float variable which is the spacing between grass spawns
 */
void CGrass3D::SetSpacing(const float fSpacing)
{
	this->fSpacing = fSpacing;
}

/**
 @brief Print Self
 */
void CGrass3D::PrintSelf(void)
{
	cout << "CGrass3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}
