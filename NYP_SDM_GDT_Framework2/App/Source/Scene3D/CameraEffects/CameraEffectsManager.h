/**
 CCameraEffectsManager
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include CBloodScreen
#include "CameraEffects.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include map
#include <map>

class CCameraEffectsManager : public CSingletonTemplate<CCameraEffectsManager>
{
	friend class CSingletonTemplate<CCameraEffectsManager>;

public:
	// Initialise this class instance
	bool Init(void);

	// Check if a CCameraEffects* exists in the map
	virtual bool CheckGameStateExist(const std::string& _name);
	// Add a CCameraEffects* to this class instance
	virtual bool Add(const std::string& _name, CCameraEffects* cCameraEffects);
	// Remove a CCameraEffects* from this class instance
	virtual bool Erase(const std::string& _name);
	// Get a CCameraEffects* from this class instance
	virtual CCameraEffects* Get(const std::string& _name);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// Render this class instance
	virtual void Render(void);
	
	// Print Self
	virtual void PrintSelf(void);

protected:
	// Default Constructor
	CCameraEffectsManager(void);
	// Destructor
	virtual ~CCameraEffectsManager(void);

	// Map of CCameraEffects
	std::map<std::string, CCameraEffects*> CameraEffectsMap;
};
