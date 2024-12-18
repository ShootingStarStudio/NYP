/**
 CStructure3D: Render a Tree model with textures
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include CSolidObject
#include "SolidObject.h"

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

class CStructure3D : public CSolidObject
{
public:
	// Default Constructor
	CStructure3D(void);

	// Constructor with vectors
	CStructure3D(	const glm::vec3 vec3Position,
					const glm::vec3 vec3Front = glm::vec3(0.0f, 0.0f, -1.0f));

	// Destructor
	virtual ~CStructure3D(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	unsigned int specularMap;
};
