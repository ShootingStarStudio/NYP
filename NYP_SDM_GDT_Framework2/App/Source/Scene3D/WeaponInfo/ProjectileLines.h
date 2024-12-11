/**
 CProjectileLines
 By: Toh Da Jun
 Date: July 2024
 */
#pragma once

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

class CProjectileLines
{
public:
	// Attributes of the Shader
	glm::mat4 projectileLineModel; // make sure to initialize matrix to identity matrix first
	glm::mat4 projectileLineView;
	glm::mat4 projectileLineProjection;

	// Line specifications
	// Line start position
	glm::vec3 vec3StartPosition;
	// Line end position
	glm::vec3 vec3EndPosition;
	// Line colour
	glm::vec4 vec4ProjectileLineColour;

	// Boolean flag to indicate if Projectile Line is displayed
	bool bIsDisplayed;
	// Line width
	float fLineWidth;

	// Constructor(s)
	CProjectileLines(void);
	// Destructor
	virtual ~CProjectileLines(void);

	// Activate the CProjectileLines for this class instance
	virtual bool InitProjectileLines(	const std::string& _name,
								glm::vec4 vec4Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
								const float fBlendValue = 1.0f);

	// Set a shader to this class instance
	virtual void SetProjectileLineShader(const std::string& _name);

	virtual void SetStartPosition(glm::vec3 vec3StartPosition);
	virtual void SetEndPosition(glm::vec3 vec3EndPosition);
	virtual void SetMaxBlendValue(const float fBlendValue);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	std::string sProjectileLinesShaderName;
	unsigned int projectileLinesVAO, projectileLinesVBO;
	
	// Variable to control the blend value of the lines
	float fBlendValue;
};
