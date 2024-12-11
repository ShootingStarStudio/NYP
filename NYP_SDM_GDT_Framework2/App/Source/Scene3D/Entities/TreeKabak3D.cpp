/**
 CTreeKabak3D
 By: Toh Da Jun
 Date: Sept 2021
 */
#include "TreeKabak3D.h"
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include CTerrainManager
#include "../Terrain/TerrainManager.h"

// Include CLightings
#include "..\Lightings\Lightings.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CTreeKabak3D::CTreeKabak3D(void)
	: bInstancedRendering(false)
	, iNumOfInstance(10)
	, fSpreadDistance(5.0f)
	, iIndicesSize(0)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CTreeKabak3D::CTreeKabak3D(	const glm::vec3 vec3Position,
							const glm::vec3 vec3Front,
							const float fYaw,
							const float fPitch)
	: bInstancedRendering(false)
	, iNumOfInstance(10)
	, fSpreadDistance(5.0f)
	, iIndicesSize(0)
{
	// Set the default position to the origin
	this->vec3Position = vec3Position;
}

/**
 @brief Destructor
 */
CTreeKabak3D::~CTreeKabak3D(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CTreeKabak3D::Init(void)
{
	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Load the OBJ into p3DMesh
	std::string file_path = "Models/Tree_Kabak/Kabak-1.obj";
	CLoadOBJ theCLoadOBJ;
	p3DMesh = theCLoadOBJ.CreateMeshForInstancing(file_path.c_str(), true);
	p3DMesh->NumOfInstance = iNumOfInstance;

	// Load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/Tree_Kabak/Kabak-1.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/Tree_Kabak/Kabak-1.tga" << endl;
		return false;
	}

	// Load and create a texture for specular map
	specularMap = CImageLoader::GetInstance()->LoadTextureGetID("Models/Tree_Kabak/Kabak_Bark_and_Leaves_v1_Specular.png", false);
	if (specularMap == 0)
	{
		cout << "Unable to load Models/Tree_Kabak/Kabak_Bark_and_Leaves_v1_Specular.png" << endl;
		return false;
	}

	if (bInstancedRendering)
	{
		// Generate the list of transformation matrices which 
		// indicates where each instance will be at, and its orientation
		glm::mat4* modelMatrices = new glm::mat4[iNumOfInstance];

		// Initialize random seed
		srand((unsigned int)glfwGetTime());

		for (unsigned int i = 0; i < iNumOfInstance; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. Use Translation to randomly disperse the trees in a rectangular area
			float x = vec3Position.x + (rand() % 100) * fSpreadDistance * 0.01f - fSpreadDistance * 0.5f;
			float z = vec3Position.z + (rand() % 100) * fSpreadDistance * 0.01f - fSpreadDistance * 0.5f;
			float y = CTerrainManager::GetInstance()->GetHeight(x, z);
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. Use scaling to create trees of various sizes
			float scale = (rand() % 100) * 0.0005f + 0.01f;
			model = glm::scale(model, glm::vec3(vec3Scale.x * scale,
				vec3Scale.y * scale,
				vec3Scale.z * scale));

			// 3. Randomly rotate the rock around y-axis vector
			float rotAngle = float(rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));

			// 4. Add this transformation matrix to list of matrices
			modelMatrices[i] = model;
		}

		// configure instanced array
		// -------------------------
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, iNumOfInstance * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		// Delete the array of modelMatrices since we had already uploaded it to the graphics card
		delete[] modelMatrices;

		// Set transformation matrices as an instance vertex attribute (with divisor 1)
		// We simplify our codes by loading the VAO as well
		// Use the loop for multiple VAOs for this rendering pass
		for (unsigned int i = 0; i < 1; i++)
		{
			glBindVertexArray(p3DMesh->VAO);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindVertexArray(0);
		}
	}

	return true;
}

/**
@brief DeInitialise this instance
*/
void CTreeKabak3D::DeInit(void)
{
}

/**
@brief Set the instancing mode
@param bInstancedRendering A const bool containing the status of the instancing mode
*/
void CTreeKabak3D::SetInstancingMode(const bool bInstancedRendering)
{
	this->bInstancedRendering = bInstancedRendering;
}

/**
 @brief Set the number of instances
 @param iNumOfInstance A const unsigned int containing the number of instances to render for this entity
 */
void CTreeKabak3D::SetNumOfInstance(const unsigned int iNumOfInstance)
{
	this->iNumOfInstance = iNumOfInstance;
}

/**
 @brief Set the spread distance
 @param fSpreadDistance A const float containing the spread distance when randomly generating positions for the instances
 */
void CTreeKabak3D::SetSpreadDistance(const float fSpreadDistance)
{
	this->fSpreadDistance = fSpreadDistance;
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CTreeKabak3D::Update(const double dElapsedTime)
{
	if (!bInstancedRendering)
	{
		// create transformations
		model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//model = glm::rotate(model, (float)glfwGetTime()/10.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, vec3Position);
		model = glm::rotate(model, -1.5708f, glm::vec3(1, 0, 0));
		model = glm::scale(model, vec3Scale);
	}

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CTreeKabak3D::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//// Enable alphas test
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.1f);

	// Disable cull face
	glDisable(GL_CULL_FACE);

	// Configure the shader
	CShaderManager::GetInstance()->Use(sShaderName);
	CShaderManager::GetInstance()->pActiveShader->setVec3("viewPos", CCamera::GetInstance()->vec3Position);
	CShaderManager::GetInstance()->pActiveShader->setInt("material.diffuse", 0);
	CShaderManager::GetInstance()->pActiveShader->setInt("material.specular", 1);
	CShaderManager::GetInstance()->pActiveShader->setFloat("material.shininess", 32.0f);

	// view/projection transformations
	CShaderManager::GetInstance()->pActiveShader->setMat4("projection", projection);
	CShaderManager::GetInstance()->pActiveShader->setMat4("view", view);
	// world transformation
	CShaderManager::GetInstance()->pActiveShader->setMat4("model", model);

	// Setup the lightings for the current shader
	CLightings::GetInstance()->PreRender();
}

/**
@brief Render Render this instance
*/
void CTreeKabak3D::Render(void)
{
	if (bInstancedRendering)
	{
		// draw a tree
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iTextureID); // note: we also made the textures_loaded vector public (instead of private) from the model class.
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
				p3DMesh->Render();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		// note: currently we set the projection matrix each frame, but since the projection 
		// matrix rarely changes it's often best practice to set it outside the main loop only once.
		CShaderManager::GetInstance()->pActiveShader->setMat4("projection", projection);
		CShaderManager::GetInstance()->pActiveShader->setMat4("view", view);
		CShaderManager::GetInstance()->pActiveShader->setMat4("model", model);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, iTextureID);
			// Render the OBJ
			glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CTreeKabak3D::PostRender(void)
{
	// Enable cull face
	glEnable(GL_CULL_FACE);

	// Disable blending
	glDisable(GL_BLEND);

	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Print Self
 */
void CTreeKabak3D::PrintSelf(void)
{
	cout << "CTreeKabak3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}
