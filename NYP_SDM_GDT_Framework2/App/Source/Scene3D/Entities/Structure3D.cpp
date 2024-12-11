/**
 CStructure3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Structure3D.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include CLightings
#include "..\Lightings\Lightings.h"

// Include Camera
#include "../Camera.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CStructure3D::CStructure3D(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, fHeightOffset, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CStructure3D::CStructure3D(	const glm::vec3 vec3Position,
							const glm::vec3 vec3Front)
{
	this->vec3Position = glm::vec3(vec3Position.x, vec3Position.y + fHeightOffset, vec3Position.z);
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CStructure3D::~CStructure3D(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CStructure3D::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::STRUCTURE);

    p3DMesh = CMeshBuilder::GenerateBox2(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Structure_01.tga", true);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Structure_01.tga" << endl;
		return false;
	}
	specularMap = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Structure_01_Specular.tga", true);
	if (specularMap == 0)
	{
		cout << "Unable to load Image/Scene3D_Structure_01_Specular.tga" << endl;
		return false;
	}

	return true;
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CStructure3D::Update(const double dElapsedTime)
{
	CSolidObject::Update(dElapsedTime);

    model = glm::mat4(1.0f);
    model = glm::translate(model, vec3Position);

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CStructure3D::PreRender(void)
{
	//CSolidObject::PreRender();

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
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CStructure3D::Render(void)
{
	//CSolidObject::Render();

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, iTextureID);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // Configure the shader
    CShaderManager::GetInstance()->pActiveShader->setMat4("model", model);

    // Render the 3D mesh
    p3DMesh->Render();

    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CStructure3D::PostRender(void)
{
	CSolidObject::PostRender();
}
