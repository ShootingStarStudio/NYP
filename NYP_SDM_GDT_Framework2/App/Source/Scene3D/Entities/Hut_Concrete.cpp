/**
 CHut_Concrete
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Hut_Concrete.h"

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include CPlayer3D
#include "../Entities/Player3D.h"

// Include CLightings
#include "..\Lightings\Lightings.h"

// Include Camera
#include "../Camera.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CHut_Concrete::CHut_Concrete(void)
{
	// Set the default position to the origin
	vec3Position = glm::vec3(0.0f, fHeightOffset, 0.0f);
}

/**
 @brief Constructor with vectors
 @param vec3Position A const glm::vec3 variable which contains the position of the camera
 @param vec3Front A const glm::vec3 variable which contains the up direction of the camera
 */
CHut_Concrete::CHut_Concrete(	const glm::vec3 vec3Position,
								const glm::vec3 vec3Front)
{
	this->vec3Position = glm::vec3(vec3Position.x, vec3Position.y + fHeightOffset, vec3Position.z);
	this->vec3Front = vec3Front;
}

/**
 @brief Destructor
 */
CHut_Concrete::~CHut_Concrete(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CHut_Concrete::Init(void)
{
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::STRUCTURE);

	if (bLODStatus == false)
	{
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_H.obj", 
								"Models/Hut_Concrete/Hut_Concrete_H.tga",
								iTextureID) == false)
		{
			cout << "Unable to load model and texture" << endl;
		}
	}
	else
	{
		// Set the current detail to HIGH_DETAILS before loading the model and texture
		eDetailLevel = CLevelOfDetails::DETAIL_LEVEL::HIGH_DETAILS;
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_H.obj",
								"Models/Hut_Concrete/Hut_Concrete_H.tga",
								arriTextureID[0]) == false)
		{
			cout << "Unable to load high LOD model and texture" << endl;
		}
		// Set the current detail to MID_DETAILS before loading the model and texture
		eDetailLevel = CLevelOfDetails::DETAIL_LEVEL::MID_DETAILS;
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_M.obj",
								"Models/Hut_Concrete/Hut_Concrete_M.tga",
								arriTextureID[1]) == false)
		{
			cout << "Unable to load mid LOD model and texture" << endl;
		}
		// Set the current detail to LOW_DETAILS before loading the model and texture
		eDetailLevel = CLevelOfDetails::DETAIL_LEVEL::LOW_DETAILS;
		if (LoadModelAndTexture("Models/Hut_Concrete/Hut_Concrete_L.obj",
								"Models/Hut_Concrete/Hut_Concrete_L.tga",
								arriTextureID[2]) == false)
		{
			cout << "Unable to load low LOD model and texture" << endl;
		}
	}

	// Load the specular map. 
	// We only load one as we don't need to show the difference in specular effects due to LOD
	specularMap = CImageLoader::GetInstance()->LoadTextureGetID("Models/Hut_Concrete/Hut_Concrete_H_Specular.tga", true);
	if (specularMap == 0)
	{
		cout << "Unable to load Models/Hut_Concrete/Hut_Concrete_H_Specular.tga" << endl;
		return false;
	}

	return true;
}

/**
 @brief Load a model and texture
 @param filenameModel A const char* variable containing the filename for the model
 @param filenameTexture A const char* variable containing the filename for the texture
 @param iTextureID A GLuint& variable containing the iTextureID to be returned
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CHut_Concrete::LoadModelAndTexture(const char* filenameModel,
										const char* filenameTexture,
										GLuint& iTextureID)
{
	std::string file_path = filenameModel;
	CLoadOBJ theCLoadOBJ;

	if (bLODStatus == false)
	{
		p3DMesh = theCLoadOBJ.CreateMesh(file_path.c_str(), false, true);
	}
	else
	{
		p3DMeshArray[eDetailLevel] = theCLoadOBJ.CreateMesh(file_path.c_str(), true, true);
	}

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID(filenameTexture, false);
	if (iTextureID == 0)
	{
		cout << "Unable to load " << filenameTexture << endl;
		return false;
	}

	return true;
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CHut_Concrete::Update(const double dElapsedTime)
{
	CSolidObject::Update(dElapsedTime);

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CHut_Concrete::PreRender(void)
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
 */
void CHut_Concrete::Render(void)
{
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
		CSolidObject::Render();
	// unbind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CHut_Concrete::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Print Self
 */
void CHut_Concrete::PrintSelf(void)
{
	cout << "CHut_Concrete::PrintSelf()" << endl;
	cout << "========================" << endl;
}
