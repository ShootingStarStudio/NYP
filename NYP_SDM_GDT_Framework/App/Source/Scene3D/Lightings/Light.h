/**
 CLight
 By: Toh Da Jun
 Date: November 2023
 */
#pragma once

 // Include Entity3D
#include <Primitives/Entity3D.h>

#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <vector>
using namespace std;

class CLight : public CEntity3D
{
public:
	// Types of light
	enum class LIGHTTYPE : unsigned int
	{
		DIRECTIONAL = 0,
		TORCH,
		POINT,
		SPOT,
		NUM_TYPES
	};

	// Constructor
	CLight(void);

	// Destructor
	virtual ~CLight(void);

	// Init
	virtual bool Init(void);

	// DeInitialise this instance
	virtual void DeInit(void);

	// Set methods
	virtual void SetEnabled(const bool bEnabled);
	virtual void SetLightType(const LIGHTTYPE eType);
	virtual void SetRadius(const float fRadius);
	virtual void SetAmbient(glm::vec3 vec3Ambient);
	virtual void SetDiffuse(glm::vec3 vec3Diffuse);
	virtual void SetSpecular(glm::vec3 vec3Specular);
	virtual void SetConstant(const float fConstant);
	virtual void SetLinear(const float fLinear);
	virtual void SetQuadratic(const float fQuadratic);
	virtual void SetCutOff(const float fCutOff);
	virtual void SetOuterCutOff(const float fOuterCutOff);

	// Get methods
	virtual bool GetEnabled(void) const;
	virtual const CLight::LIGHTTYPE GetLightType(void) const;
	virtual float GetRadius(void) const;
	virtual glm::vec3 GetAmbient(void) const;
	virtual glm::vec3 GetDiffuse(void) const;
	virtual glm::vec3 GetSpecular(void) const;
	virtual float GetConstant(void) const;
	virtual float GetLinear(void) const;
	virtual float GetQuadratic(void) const;
	virtual float GetCutOff(void) const;
	virtual float GetOuterCutOff(void) const;

	// Update
	virtual bool Update(const double dElapsedTime);
	
	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// PrintSelf
	virtual void PrintSelf(void);

protected:
	// Boolean variable to indicate if this light is enabled
	bool bEnabled;
	// LIGHTTYPE variable to indicate the type of light
	LIGHTTYPE eLightType;
	// Float variable storing the radius of the light. This is for point and spot lights only
	float fRadius;

	// Ambient
	glm::vec3 vec3Ambient;
	// Diffuse
	glm::vec3 vec3Diffuse;
	// Specular
	glm::vec3 vec3Specular;

	// Variables for SpotLight
	float fConstant;
	float fLinear;
	float fQuadratic;
	float fCutOff;
	float fOuterCutOff;
};
