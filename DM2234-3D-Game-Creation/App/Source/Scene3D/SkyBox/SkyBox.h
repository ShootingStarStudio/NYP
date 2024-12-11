/**
 CSkyBox
 By: Toh Da Jun
 Date: Apr 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

#include <Primitives\Entity3D.h>

// Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <vector>

#include "GameControl\Settings.h"

class CSkyBox : public CEntity3D, public CSingletonTemplate<CSkyBox>
{
	friend CSingletonTemplate<CSkyBox>;
public:
	// Initialise this class instance
	bool Init(void);

	// Set model
	virtual void SetModel(glm::mat4 model);
	// Set view
	virtual void SetView(glm::mat4 view);
	// Set projection
	virtual void SetProjection(glm::mat4 projection);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	virtual void Increment(void);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);
	
	// Print Self
	void PrintSelf(void);
			
	int skybox_Index = 0;		//iterator for rendering skybox vector
	std::vector<GLuint> skybox_Time;

protected:

	// Constructor
	CSkyBox(void);

	// Destructor
	~CSkyBox(void);

	// Load SkyBox textures
	GLuint LoadSkyBoxTextures(std::vector<std::string> TextureString);

	//double timecycle = 60;			//seconds needed to change the skybox
	double timecycle = 60;			//seconds needed to change the skybox
	double timecurrent = 0;			//current time in seconds

};
