/**
 CLightings
 By: Toh Da Jun
 Date: November 2023
 */
#pragma once

 // Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include "Light.h"

// Include TimeOfTheDay.h
#include "TimeControl/TimeOfTheDay.h"

#include <map>
using namespace std;

class CLightings : public CSingletonTemplate<CLightings>
{
	friend class CSingletonTemplate<CLightings>;
public:
	// Constructor
	CLightings(void);

	// Destructor
	virtual ~CLightings(void);

	// Init
	virtual bool Init(void);

	// Add methods
	virtual int AddDirectionalLight(glm::vec3 vec3Direction, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular, CTimeOfTheDay::PARTSOFTHEDAY partOfTheDay = CTimeOfTheDay::PARTSOFTHEDAY::NOON);
	virtual int AddTorchLight(glm::vec3 vec3Position, glm::vec3 vec3Direction, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular,
								float fConstant = 1.0f, float fLinear = 0.09f, float fQuadratic = 0.032f, 
								float fCutOff = glm::cos(glm::radians(12.5f)), float fOuterCutOff = glm::cos(glm::radians(15.0f)));
	virtual int AddPointLight(glm::vec3 vec3Position, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular,
								float fConstant = 1.0f, float fLinear = 0.09f, float fQuadratic = 0.032f);
	virtual int AddSpotLight(glm::vec3 vec3Position, glm::vec3 vec3Direction, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular,
								float fConstant = 1.0f, float fLinear = 0.09f, float fQuadratic = 0.032f,
								float fCutOff = glm::cos(glm::radians(12.5f)), float fOuterCutOff = glm::cos(glm::radians(15.0f)));

	// Remove methods
	virtual void RemoveLight(const int ID);

	// Set methods
	virtual void SetEnabled(const int ID, const bool bEnabled);

	// Get methods
	virtual bool GetEnabled(const int ID);
	virtual CLight* GetDirectionalLight(void) const;
	virtual CLight* GetTorchLight(const int ID);

	// Iterator methods
	virtual CLight* IterateStart(CLight::LIGHTTYPE type);
	virtual CLight* IterateNext(CLight::LIGHTTYPE type);

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
	CLight* pDirectionalLight;
	std::map<int, CLight*> torchLightsMap;
	std::map<int, CLight*> pointLightsMap;
	const int iMaxNumPointLights = 4;
	std::map<int, CLight*> spotLightsMap;
	const int iMaxNumSpotLights = 1;

	// Iterator for the std::maps
	std::map<int, CLight*>::iterator mapIterator;
	CLight::LIGHTTYPE currentIteratorType;

	// The handle to CTimeOfTheDay class instance
	CTimeOfTheDay* pTimeOfTheDay;
};
