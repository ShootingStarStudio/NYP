/**
 CParticles3D: Render 3D particles
 By: Toh Da Jun
 Date: May 2024
 */
#pragma once

#include <vector>

// Include Entity3D
#include <Primitives/Entity3D.h>

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

#include <string>
using namespace std;

// CPU representation of a particle
class CParticle3D {
public:
	glm::vec3 vec3Position, vec3Speed;
	unsigned char r, g, b, a;		// Color
	float fSize, fAngle, fWeight;
	float fLife;					// Remaining life of the particle. if <0 : dead and unused.
	float fCameraDistance;			// *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const CParticle3D& that) const
	{
		// Sort in reverse order : far particles drawn first.
		return this->fCameraDistance > that.fCameraDistance;
	}
};

// CParticles3D acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class CParticles3D : public CEntity3D
{
public:
	// default constructor
	CParticles3D(void);
	// destructor
	~CParticles3D(void);

	// Initialise this class instance
	bool Init(void);
	// DeInitialise this instance
	void DeInit(void);

	// Set the maximum number of particles
	void SetMaxParticles(const int iMaxParticles);
	// Set the life of particles
	void SetLife(const float fLife);
	// Set the spread of particles
	void SetSpread(const float fSpread);
	// Set the movement direction of particles
	void SetMovementDirection(const glm::vec3 vec3MovementVelocity);

	// Get the maximum number of particles
	int GetMaxParticles(void);
	// Get the life of particles
	float GetLife(void);
	// Get the spread of particles
	float GetSpread(void);
	// Get the movement direction of particles
	glm::vec3 GetMovementDirection(void);

	// Update this class instance
	bool Update(double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// Finds a Particle in ParticlesContainer which isn't used yet. (i.e. life < 0);
	int FindUnusedParticle(void);
	// Sort the particles so that they are arranged from the particles which are furthest from the camera to the nearest
	void SortParticles(void);

	// For OpenGL
	GLfloat* pParticule_Position_Size_Data;
	GLubyte* pParticule_Color_Data;
	GLuint iBillboard_vertex_buffer;
	GLuint iParticles_position_buffer;
	GLuint iParticles_color_buffer;
	GLuint iTextureSampler;
	GLuint iCameraRight_worldspace_ID;
	GLuint iCameraUp_worldspace_ID;
	GLuint iViewProjMatrixID;
	glm::mat4 mat4ViewProjection;

	// Variables for the particles
	CParticle3D* pParticlesContainer;

	int iMaxParticles;
	int iParticlesCount;
	int iLastUsedParticle;

	float fLife;
	float fSpread;

	// vec3 variables
	glm::vec3 vec3MovementVelocity;
	glm::vec3 vec3Random;
};
