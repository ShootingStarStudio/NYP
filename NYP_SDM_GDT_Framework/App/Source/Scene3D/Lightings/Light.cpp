/**
 CLight
 By: Toh Da Jun
 Date: November 2023
 */
#include "Light.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CLight::CLight(void)
 : bEnabled(true)
 , eLightType(CLight::LIGHTTYPE::POINT)
 , fRadius(0.0f)
 , vec3Ambient(glm::vec3(0.0f))
 , vec3Diffuse(glm::vec3(0.0f))
 , vec3Specular(glm::vec3(0.0f))
 , fConstant(1.0f)
 , fLinear(0.09f)
 , fQuadratic(0.032f)
 , fCutOff(glm::cos(glm::radians(12.5f)))
 , fOuterCutOff(glm::cos(glm::radians(15.0f)))
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CLight::~CLight(void)
{
	DeInit();
}

/**
@brief Init Initialise this instance
@return true if the initialisation is successful, else false
*/
bool CLight::Init(void)
{
	return true;
}

/**
@brief DeInitialise this instance
*/
void CLight::DeInit(void)
{
	if (p3DMesh)
	{
		delete p3DMesh;
		p3DMesh = NULL;
	}
}

// Set methods
void CLight::SetEnabled(const bool bEnabled)
{
	this->bEnabled = bEnabled;
}

void CLight::SetLightType(const LIGHTTYPE eType)
{
	this->eLightType = eType;

	//this->p3DMesh = CMeshBuilder::GenerateSphere();
}

void CLight::SetRadius(const float fRadius)
{
	this->fRadius = fRadius;
}
void CLight::SetAmbient(glm::vec3 vec3Ambient)
{
	this->vec3Ambient = vec3Ambient;
}
void CLight::SetDiffuse(glm::vec3 vec3Diffuse)
{
	this->vec3Diffuse = vec3Diffuse;
}
void CLight::SetSpecular(glm::vec3 vec3Specular)
{
	this->vec3Specular = vec3Specular;
}
void CLight::SetConstant(const float fConstant)
{
	this->fConstant = fConstant;
}
void CLight::SetLinear(const float fLinear)
{
	this->fLinear = fLinear;
}
void CLight::SetQuadratic(const float fQuadratic)
{
	this->fQuadratic = fQuadratic;
}
void CLight::SetCutOff(const float fCutOff)
{
	this->fCutOff = fCutOff;
}
void CLight::SetOuterCutOff(const float fOuterCutOff)
{
	this->fOuterCutOff = fOuterCutOff;
}

// Get methods
bool CLight::GetEnabled(void) const
{
	return bEnabled;
}

const CLight::LIGHTTYPE CLight::GetLightType(void) const
{
	return eLightType;
}

float CLight::GetRadius(void) const
{
	return fRadius;
}
glm::vec3 CLight::GetAmbient(void) const
{
	return vec3Ambient;
}
glm::vec3 CLight::GetDiffuse(void) const
{
	return vec3Diffuse;
}
glm::vec3 CLight::GetSpecular(void) const
{
	return vec3Specular;
}
float CLight::GetConstant(void) const
{
	return fConstant;
}
float CLight::GetLinear(void) const
{
	return fLinear;
}
float CLight::GetQuadratic(void) const
{
	return fQuadratic;
}
float CLight::GetCutOff(void) const
{
	return fCutOff;
}
float CLight::GetOuterCutOff(void) const
{
	return fOuterCutOff;
}

/**
 @brief Update
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CLight::Update(const double dElapsedTime)
{

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CLight::PreRender(void)
{
	// change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render Render this instance
 */
void CLight::Render(void)
{
	if (!bVisible)
		return;

	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	CShaderManager::GetInstance()->pActiveShader->setMat4("projection", projection);
	CShaderManager::GetInstance()->pActiveShader->setMat4("view", view);
	CShaderManager::GetInstance()->pActiveShader->setMat4("model", model);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// Render the p3DMesh
	glBindVertexArray(VAO);
	if (p3DMesh)
		p3DMesh->Render();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CLight::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief PrintSelf
 */
void CLight::PrintSelf(void)
{
	cout << "CLight::PrintSelf()" << endl;
	cout << "===================" << endl;
	cout << "ID\t\t: " << ID << endl;

	if (eLightType == CLight::LIGHTTYPE::DIRECTIONAL)
	{
		cout << "A. Directional Light(s)" << endl;
		cout << "Direction\t: " << vec3Front.x << ", " << vec3Front.y << ", " << vec3Front.z << endl;
		cout << "Ambient\t\t: " << vec3Ambient.x << ", " << vec3Ambient.y << ", " << vec3Ambient.z << endl;
		cout << "Diffuse\t\t: " << vec3Diffuse.x << ", " << vec3Diffuse.y << ", " << vec3Diffuse.z << endl;
		cout << "Specular\t: " << vec3Specular.x << ", " << vec3Specular.y << ", " << vec3Specular.z << endl;
		return;
	}
	else if (eLightType == CLight::LIGHTTYPE::TORCH)
	{
		cout << "B. Torch Light(s)" << endl;
		cout << "Position\t: " << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << endl;
		cout << "Direction\t: " << vec3Front.x << ", " << vec3Front.y << ", " << vec3Front.z << endl;
		cout << "Colour\t: " << vec4Colour.x << ", " << vec4Colour.y << ", " << vec4Colour.z << ", " << vec4Colour.a << endl;
		cout << "Ambient\t\t: " << vec3Ambient.x << ", " << vec3Ambient.y << ", " << vec3Ambient.z << endl;
		cout << "Diffuse\t\t: " << vec3Diffuse.x << ", " << vec3Diffuse.y << ", " << vec3Diffuse.z << endl;
		cout << "Specular\t: " << vec3Specular.x << ", " << vec3Specular.y << ", " << vec3Specular.z << endl;
		cout << "fConstant\t: " << fConstant << endl;
		cout << "fLinear\t: " << fLinear << endl;
		cout << "fQuadratic\t: " << fQuadratic << endl;
		cout << "fCutOff\t: " << fCutOff << endl;
		cout << "fOuterCutOff\t: " << fOuterCutOff << endl;
	}
	else if (eLightType == CLight::LIGHTTYPE::POINT)
	{
		cout << "C. Point Light(s)" << endl;
		cout << "Position\t: " << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << endl;
		cout << "Direction\t: " << vec3Front.x << ", " << vec3Front.y << ", " << vec3Front.z << endl;
		cout << "Colour\t: " << vec4Colour.x << ", " << vec4Colour.y << ", " << vec4Colour.z << ", " << vec4Colour.a << endl;
		cout << "Radius\t: " << fRadius << endl;
	}
	else if (eLightType == CLight::LIGHTTYPE::SPOT)
	{
		cout << "D. Spot Light(s)" << endl;
		cout << "Position\t: " << vec3Position.x << ", " << vec3Position.y << ", " << vec3Position.z << endl;
		cout << "Direction\t: " << vec3Front.x << ", " << vec3Front.y << ", " << vec3Front.z << endl;
		cout << "Colour\t: " << vec4Colour.x << ", " << vec4Colour.y << ", " << vec4Colour.z << ", " << vec4Colour.a << endl;
		cout << "Ambient\t\t: " << vec3Ambient.x << ", " << vec3Ambient.y << ", " << vec3Ambient.z << endl;
		cout << "Diffuse\t\t: " << vec3Diffuse.x << ", " << vec3Diffuse.y << ", " << vec3Diffuse.z << endl;
		cout << "Specular\t: " << vec3Specular.x << ", " << vec3Specular.y << ", " << vec3Specular.z << endl;
		cout << "fConstant\t: " << fConstant << endl;
		cout << "fLinear\t: " << fLinear << endl;
		cout << "fQuadratic\t: " << fQuadratic << endl;
		cout << "fCutOff\t: " << fCutOff << endl;
		cout << "fOuterCutOff\t: " << fOuterCutOff << endl;
	}
}
