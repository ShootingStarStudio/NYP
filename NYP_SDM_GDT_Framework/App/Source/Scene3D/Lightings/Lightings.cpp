/**
 CLightings
 By: Toh Da Jun
 Date: November 2023
 */
#include "Lightings.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CLightings::CLightings(void)
	: pDirectionalLight(NULL)
	, pTimeOfTheDay(NULL)
{
	// Get the instance for CTimeOfTheDay
	pTimeOfTheDay = CTimeOfTheDay::GetInstance();
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CLightings::~CLightings(void)
{
	// Set pTimeOfTheDay to NULL since it was instanced somewhere else
	if (pTimeOfTheDay)
	{
		pTimeOfTheDay = NULL;
	}

	// Iterate through the spotLightsMap
	for (std::map<int, CLight*>::iterator it = spotLightsMap.begin(); it != spotLightsMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	// Remove all elements in spotLightsMap
	spotLightsMap.clear();

	// Iterate through the pointLightsMap
	for (std::map<int, CLight*>::iterator it = pointLightsMap.begin(); it != pointLightsMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	// Remove all elements in pointLightsMap
	pointLightsMap.clear();

	// Iterate through the torchLightsMap
	for (std::map<int, CLight*>::iterator it = torchLightsMap.begin(); it != torchLightsMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	// Remove all elements in torchLightsMap
	torchLightsMap.clear();

	if (pDirectionalLight)
	{
		delete pDirectionalLight;
		pDirectionalLight = NULL;
	}
}

/**
@brief Init Initialise this instance
@return true if the initialisation is successful, else false
*/
bool CLightings::Init(void)
{
	torchLightsMap.clear();
	pointLightsMap.clear();
	spotLightsMap.clear();

	return true;
}

/**
 @brief Add the directional light
 @return An int variable which is the ID of the added light
 */
int CLightings::AddDirectionalLight(glm::vec3 vec3Direction, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular, CTimeOfTheDay::PARTSOFTHEDAY partOfTheDay)
{
	pDirectionalLight = new CLight();
	pDirectionalLight->SetLightType(CLight::LIGHTTYPE::DIRECTIONAL);
	pDirectionalLight->SetFront(glm::normalize(vec3Direction));
	pDirectionalLight->SetAmbient(vec3Ambient);
	pDirectionalLight->SetDiffuse(vec3Diffuse);
	pDirectionalLight->SetSpecular(vec3Specular);
	return pDirectionalLight->GetID();
}

/**
 @brief Add the torch light
 @return An int variable which is the ID of the added light
 */
int CLightings::AddTorchLight(glm::vec3 vec3Position, glm::vec3 vec3Direction, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular,
							float fConstant, float fLinear, float fQuadratic, float fCutOff, float fOuterCutOff)
{
	CLight* aLight = new CLight();
	aLight->SetLightType(CLight::LIGHTTYPE::TORCH);
	aLight->SetPosition(vec3Position);
	aLight->SetFront(glm::normalize(vec3Direction));
	aLight->SetAmbient(vec3Ambient);
	aLight->SetDiffuse(vec3Diffuse);
	aLight->SetSpecular(vec3Specular);
	aLight->SetConstant(fConstant);
	aLight->SetLinear(fLinear);
	aLight->SetQuadratic(fQuadratic);
	aLight->SetCutOff(fCutOff);
	aLight->SetOuterCutOff(fOuterCutOff);
	// Add to the torchLightsMap
	torchLightsMap[aLight->GetID()] = aLight;

	return aLight->GetID();
}

/**
 @brief Add a pointlight
 @return An int variable which is the ID of the added light
 */
int CLightings::AddPointLight(glm::vec3 vec3Position, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular,
							float fConstant, float fLinear, float fQuadratic)
{
	CLight* aLight = new CLight();
	aLight->SetLightType(CLight::LIGHTTYPE::POINT);
	aLight->SetPosition(vec3Position);
	aLight->SetAmbient(vec3Ambient);
	aLight->SetDiffuse(vec3Diffuse);
	aLight->SetSpecular(vec3Specular);
	aLight->SetConstant(fConstant);
	aLight->SetLinear(fLinear);
	aLight->SetQuadratic(fQuadratic);
	// Add to the pointLightsMap
	pointLightsMap[aLight->GetID()] = aLight;

	return aLight->GetID();
}

/**
 @brief Add a spotlight
 @return An int variable which is the ID of the added light
 */
int CLightings::AddSpotLight(glm::vec3 vec3Position, glm::vec3 vec3Direction, glm::vec3 vec3Ambient, glm::vec3 vec3Diffuse, glm::vec3 vec3Specular,
							float fConstant, float fLinear, float fQuadratic, float fCutOff, float fOuterCutOff)
{
	CLight* aLight = new CLight();
	aLight->SetLightType(CLight::LIGHTTYPE::SPOT);
	aLight->SetPosition(vec3Position);
	aLight->SetFront(glm::normalize(vec3Direction));
	aLight->SetAmbient(vec3Ambient);
	aLight->SetDiffuse(vec3Diffuse);
	aLight->SetSpecular(vec3Specular);
	aLight->SetConstant(fConstant);
	aLight->SetLinear(fLinear);
	aLight->SetQuadratic(fQuadratic);
	aLight->SetCutOff(fCutOff);
	aLight->SetOuterCutOff(fOuterCutOff);
	// Add to the spotLightsMap
	spotLightsMap[aLight->GetID()] = aLight;

	return aLight->GetID();
}

/**
 @brief Remove a light
 @param ID A const int variable which is the ID of the light to remove
 */
void CLightings::RemoveLight(const int ID)
{
	if ((pDirectionalLight) && (pDirectionalLight->GetID() == ID))
	{
		delete pDirectionalLight;
		pDirectionalLight = NULL;
	}

	if (torchLightsMap.count(ID) != 0)
	{
		CLight* target = torchLightsMap[ID];
		delete target;
		torchLightsMap.erase(ID);
		return;
	}
	if (pointLightsMap.count(ID) != 0)
	{
		CLight* target = pointLightsMap[ID];
		delete target;
		pointLightsMap.erase(ID);
		return;
	}
	if (spotLightsMap.count(ID) != 0)
	{
		CLight* target = spotLightsMap[ID];
		delete target;
		spotLightsMap.erase(ID);
		return;
	}
}

// Set methods
void CLightings::SetEnabled(const int ID, const bool bEnabled)
{
	if (pDirectionalLight)
		pDirectionalLight->SetEnabled(bEnabled);

	if (torchLightsMap[ID] != 0)
	{
		torchLightsMap[ID]->SetEnabled(bEnabled);
		return;
	}

	if (pointLightsMap[ID] != 0)
	{
		pointLightsMap[ID]->SetEnabled(bEnabled);
		return;
	}

	if (spotLightsMap[ID] != 0)
	{
		spotLightsMap[ID]->SetEnabled(bEnabled);
		return;
	}
}

// Get methods
bool CLightings::GetEnabled(const int ID)
{
	if ((pDirectionalLight) && (pDirectionalLight->GetID() == ID))
		return pDirectionalLight->GetEnabled();

	if (torchLightsMap[ID] != 0)
		return torchLightsMap[ID]->GetEnabled();

	if (pointLightsMap[ID] != 0)
		return pointLightsMap[ID]->GetEnabled();

	if (spotLightsMap[ID] != 0)
		return spotLightsMap[ID]->GetEnabled();

	return false;
}

CLight* CLightings::GetDirectionalLight(void) const
{
	return pDirectionalLight;
}

CLight* CLightings::GetTorchLight(const int ID)
{
	return torchLightsMap[ID];
}

// Iterator methods
CLight* CLightings::IterateStart(CLight::LIGHTTYPE type)
{
	// Store the current type of light which we are iterating through
	currentIteratorType = type;

	// Get the first light from the map and return it
	if (currentIteratorType == CLight::LIGHTTYPE::TORCH)
	{
		mapIterator = torchLightsMap.begin();
		//if ((mapIterator != torchLightsMap.end()) && (((CLight*)mapIterator->second)->GetEnabled() == true))
		if (mapIterator != torchLightsMap.end())
			return (CLight*)mapIterator->second;
	}
	else if (currentIteratorType == CLight::LIGHTTYPE::POINT)
	{
		mapIterator = pointLightsMap.begin();
		if ((mapIterator != pointLightsMap.end()) && (((CLight*)mapIterator->second)->GetEnabled() == true))
				return (CLight*)mapIterator->second;
	}
	else if (currentIteratorType == CLight::LIGHTTYPE::SPOT)
	{
		mapIterator = spotLightsMap.begin();
		if ((mapIterator != spotLightsMap.end()) && (((CLight*)mapIterator->second)->GetEnabled() == true))
				return (CLight*)mapIterator->second;
	}
	return NULL;
}
CLight* CLightings::IterateNext(CLight::LIGHTTYPE type)
{
	if (currentIteratorType != type)
	{
		cout << "Error: The Light Type has changed since IterateStart() was called!" << endl;
		return NULL;
	}

	// Get the first light from the map and return it
	if (currentIteratorType == CLight::LIGHTTYPE::TORCH)
	{
		while (mapIterator != torchLightsMap.end())
		{
			// Increment the iterator
			++mapIterator;
			if ((mapIterator != torchLightsMap.end()) && (((CLight*)mapIterator->second)->GetEnabled() == true))
				return (CLight*)mapIterator->second;
		}
	}
	else if (currentIteratorType == CLight::LIGHTTYPE::POINT)
	{
		while (mapIterator != pointLightsMap.end())
		{
			// Increment the iterator
			++mapIterator;
			if ((mapIterator != pointLightsMap.end()) && (((CLight*)mapIterator->second)->GetEnabled() == true))
				return (CLight*)mapIterator->second;
		}
	}
	else if (currentIteratorType == CLight::LIGHTTYPE::SPOT)
	{
		while (mapIterator != spotLightsMap.end())
		{
			// Increment the iterator
			++mapIterator;
			if ((mapIterator != spotLightsMap.end()) && (((CLight*)mapIterator->second)->GetEnabled() == true))
				return (CLight*)mapIterator->second;
		}
	}
	return NULL;
}

/**
 @brief Update
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CLightings::Update(const double dElapsedTime)
{
	switch (pTimeOfTheDay->GetCurrentPartOfTheDay())
	{
	case CTimeOfTheDay::PARTSOFTHEDAY::SUNRISE:
	case CTimeOfTheDay::PARTSOFTHEDAY::MORNING:
	case CTimeOfTheDay::PARTSOFTHEDAY::NOON:
	case CTimeOfTheDay::PARTSOFTHEDAY::AFTERNOON:
	{
		pDirectionalLight = new CLight();
		pDirectionalLight->SetLightType(CLight::LIGHTTYPE::DIRECTIONAL);
		pDirectionalLight->SetFront(glm::vec3(-0.2f, -1.0f, -0.3f));
		pDirectionalLight->SetAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
		pDirectionalLight->SetDiffuse(glm::vec3(0.2f, 0.2f, 0.2f));
		pDirectionalLight->SetSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
		break;
	}
	case CTimeOfTheDay::PARTSOFTHEDAY::EVENING:
		pDirectionalLight = new CLight();
		pDirectionalLight->SetLightType(CLight::LIGHTTYPE::DIRECTIONAL);
		pDirectionalLight->SetFront(glm::vec3(0.0, -1.0, 0.0));
		pDirectionalLight->SetAmbient(glm::vec3(0.75f, 0.75f, 0.75f));
		pDirectionalLight->SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
		pDirectionalLight->SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
		break;
	case CTimeOfTheDay::PARTSOFTHEDAY::NIGHT:
	case CTimeOfTheDay::PARTSOFTHEDAY::MIDNIGHT:
		pDirectionalLight = new CLight();
		pDirectionalLight->SetLightType(CLight::LIGHTTYPE::DIRECTIONAL);
		pDirectionalLight->SetFront(glm::vec3(0.2f, -1.0f, 0.3f));
		pDirectionalLight->SetAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
		pDirectionalLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		pDirectionalLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		break;
	default:
		break;
	}

	return true;
}

/**
@brief PreRender Update the OpenGL display environment with lighting information before rendering
*/
void CLightings::PreRender(void)
{
	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// directional light
	CShaderManager::GetInstance()->pActiveShader->setVec3("dirLight.direction", pDirectionalLight->GetFront());
	CShaderManager::GetInstance()->pActiveShader->setVec3("dirLight.ambient", pDirectionalLight->GetAmbient());
	CShaderManager::GetInstance()->pActiveShader->setVec3("dirLight.diffuse", pDirectionalLight->GetDiffuse());
	CShaderManager::GetInstance()->pActiveShader->setVec3("dirLight.specular", pDirectionalLight->GetSpecular());

	// Point Light
	string pointLightStr;
	CLight* pLight = CLightings::GetInstance()->IterateStart(CLight::LIGHTTYPE::POINT);
	for (int i =0; i< iMaxNumPointLights; i++)
	{
		if (pLight == NULL)
			continue;

		pointLightStr = "pointLights[" + to_string(i) + "]";
		CShaderManager::GetInstance()->pActiveShader->setVec3((pointLightStr + ".position").c_str(), pLight->GetPosition());
		CShaderManager::GetInstance()->pActiveShader->setVec3((pointLightStr + ".ambient").c_str(), pLight->GetAmbient());
		CShaderManager::GetInstance()->pActiveShader->setVec3((pointLightStr + ".diffuse").c_str(), pLight->GetDiffuse());
		CShaderManager::GetInstance()->pActiveShader->setVec3((pointLightStr + ".specular").c_str(), pLight->GetSpecular());
		if (pLight->GetEnabled() == false)
			CShaderManager::GetInstance()->pActiveShader->setFloat((pointLightStr + ".constant").c_str(), 0.0f);
		else
			CShaderManager::GetInstance()->pActiveShader->setFloat((pointLightStr + ".constant").c_str(), pLight->GetConstant());
		CShaderManager::GetInstance()->pActiveShader->setFloat((pointLightStr + ".linear").c_str(), pLight->GetLinear());
		CShaderManager::GetInstance()->pActiveShader->setFloat((pointLightStr + ".quadratic").c_str(), pLight->GetQuadratic());

		// Get the next light
		pLight = CLightings::GetInstance()->IterateNext(CLight::LIGHTTYPE::POINT);
	}

	// Torch Light / Spot Lights
	string spotLightStr;
	pLight = CLightings::GetInstance()->IterateStart(CLight::LIGHTTYPE::TORCH);
	for (int i = 0; i < iMaxNumSpotLights; i++)
	{
		if (pLight == NULL)
			continue;

		spotLightStr = "spotLights[" + to_string(i) + "]";
		CShaderManager::GetInstance()->pActiveShader->setVec3((spotLightStr + ".position").c_str(), pLight->GetPosition());
		CShaderManager::GetInstance()->pActiveShader->setVec3((spotLightStr + ".direction").c_str(), pLight->GetFront());
		CShaderManager::GetInstance()->pActiveShader->setVec3((spotLightStr + ".ambient").c_str(), pLight->GetAmbient());
		CShaderManager::GetInstance()->pActiveShader->setVec3((spotLightStr + ".diffuse").c_str(), pLight->GetDiffuse());
		CShaderManager::GetInstance()->pActiveShader->setVec3((spotLightStr + ".specular").c_str(), pLight->GetSpecular());
		if (pLight->GetEnabled() == false)
			CShaderManager::GetInstance()->pActiveShader->setFloat((spotLightStr + ".constant").c_str(), 0.0f);
		else
			CShaderManager::GetInstance()->pActiveShader->setFloat((spotLightStr + ".constant").c_str(), pLight->GetConstant());
		CShaderManager::GetInstance()->pActiveShader->setFloat((spotLightStr + ".linear").c_str(), pLight->GetLinear());
		CShaderManager::GetInstance()->pActiveShader->setFloat((spotLightStr + ".quadratic").c_str(), pLight->GetQuadratic());
		CShaderManager::GetInstance()->pActiveShader->setFloat((spotLightStr + ".cutOff").c_str(), pLight->GetCutOff());
		CShaderManager::GetInstance()->pActiveShader->setFloat((spotLightStr + ".outerCutOff").c_str(), pLight->GetOuterCutOff());

		// Get the next light
		pLight = CLightings::GetInstance()->IterateNext(CLight::LIGHTTYPE::TORCH);
	}
}

/**
@brief Render Render this instance
@param cShader A Shader* variable which contains the Shader to use in this class instance
*/
void CLightings::Render(void)
{
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CLightings::PostRender(void)
{
}

/**
 @brief PrintSelf
 */
void CLightings::PrintSelf(void)
{
	cout << "CLightings::PrintSelf()" << endl;
	cout << "=======================" << endl;
	cout << " Directional lights" << endl;
	if (pDirectionalLight)
		pDirectionalLight->PrintSelf();
	else
		cout << "No Directional lights" << endl;	

	int i = 0;
	// Iterate through the torchLightsMap
	cout << endl << torchLightsMap.size() << " sets of Torch lights" << endl;
	for (std::map<int, CLight*>::iterator it = torchLightsMap.begin(); it != torchLightsMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			cout << i++ << ". ";
			((CLight*)it->second)->PrintSelf();
		}
	}

	// Iterate through the pointLightsMap
	cout << endl << pointLightsMap.size() << " sets of Point lights" << endl;
	for (std::map<int, CLight*>::iterator it = pointLightsMap.begin(); it != pointLightsMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			cout << i++ << ". ";
			((CLight*)it->second)->PrintSelf();
		}
	}

	// Iterate through the spotLightsMap
	cout << endl << spotLightsMap.size() << " sets of Spot lights" << endl;
	for (std::map<int, CLight*>::iterator it = spotLightsMap.begin(); it != spotLightsMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			cout << i++ << ". ";
			((CLight*)it->second)->PrintSelf();
		}
	}
}
