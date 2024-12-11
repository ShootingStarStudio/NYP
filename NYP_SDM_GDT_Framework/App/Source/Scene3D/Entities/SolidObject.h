/**
 CSolidObject: This class is a CEntity3D with collider properties and LOD
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include Collider
#include <Primitives/Collider.h>

// Include LevelOfDetails
#include "Primitives/LevelOfDetails.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

class CSolidObject : public CEntity3D, public CCollider, public CLevelOfDetails
{
public:
	// Default Constructor
	CSolidObject(void);

	// Constructor with vectors
	CSolidObject(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CSolidObject(void);

	// Initialise this class instance
	bool Init(void);
	// Initialise the collider
	bool InitCollider(	const std::string& _name,
						glm::vec4 vec4BoxColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
						glm::vec3 vec3BoxMin = glm::vec3(-0.5f, -0.5f, -0.5f),
						glm::vec3 vec3BoxMax = glm::vec3(0.5f, 0.5f, 0.5f));
	// DeInitialise this instance
	virtual void DeInit(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

	// Print Self
	virtual void PrintSelf(void);

protected:
};
