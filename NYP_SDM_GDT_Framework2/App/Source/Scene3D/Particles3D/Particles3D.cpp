/**
 CParticles3D: Render 3D particles
 By: Toh Da Jun
 Date: May 2024
 */
#include "Particles3D.h"

 // Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include norm.hpp for glm::length2()
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/norm.hpp>

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include Camera
#include "../Camera.h"

// Include CRandomiser
#include "System/randomiser.h"

#include <iostream>
using namespace std;

/**
 @brief Default constructor
 */
CParticles3D::CParticles3D(void)
	: pParticlesContainer(NULL)
	, iMaxParticles(1000)
	, iParticlesCount(0)
	, iLastUsedParticle(0)
	, fLife(2.0f)
	, fSpread(1.5f)
	, vec3MovementVelocity(glm::vec3(0.0f, 4.0f, 0.0f))
{
}

/**
 @brief Destructor
 */
CParticles3D::~CParticles3D(void)
{
	DeInit();
}

/**
 @brief Initialise this class instance
 @return true if the initialisation was successful, otherwise false
 */
bool CParticles3D::Init(void)
{
	// Create and compile our GLSL program from the shaders
	CShaderManager::GetInstance()->Use("Shader3D_Particles");
	GLuint iProgramID = CShaderManager::GetInstance()->pActiveShader->ID;

	// Vertex shader
	iCameraRight_worldspace_ID = glGetUniformLocation(iProgramID, "CameraRight_worldspace");
	iCameraUp_worldspace_ID = glGetUniformLocation(iProgramID, "CameraUp_worldspace");
	iViewProjMatrixID = glGetUniformLocation(iProgramID, "VP");

	// fragment shader
	iTextureSampler = glGetUniformLocation(iProgramID, "myTextureSampler");

	pParticule_Position_Size_Data = new GLfloat[iMaxParticles * 4];
	pParticule_Color_Data = new GLubyte[iMaxParticles * 4];
	pParticlesContainer = new CParticle3D[iMaxParticles];

	// Reset the items in pParticlesContainer to default value
	for (int i = 0; i < iMaxParticles; i++) {
		pParticlesContainer[i].fLife = -1.0f;
		pParticlesContainer[i].fCameraDistance = -1.0f;
	}

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Particles.png", true);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Particles.png" << endl;
		return false;
	}

	// The VBO containing the 4 vertices of the particles.
	// Thanks to instancing, they will be shared by all particles.
	static const GLfloat g_vertex_buffer_data[] = {
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, &iBillboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, iBillboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &iParticles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, iParticles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, iMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &iParticles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, iParticles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, iMaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Upload the vertex attribute arrays to OpenGL
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// First attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, iBillboard_vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 2nd attribute buffer : positions of particles' centers
	glBindBuffer(GL_ARRAY_BUFFER, iParticles_position_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 3rd attribute buffer : particles' colors
	glBindBuffer(GL_ARRAY_BUFFER, iParticles_color_buffer);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	// Disable vertex attribute arrays
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);

	return true;
}

/**
 @brief DeInitialise this instance
 */
void CParticles3D::DeInit(void)
{
	if (pParticlesContainer)
	{
		delete[] pParticlesContainer;
		pParticlesContainer = NULL;
	}

	if (pParticule_Color_Data)
	{
		delete[] pParticule_Color_Data;
		pParticule_Color_Data = NULL;
	}

	if (pParticule_Position_Size_Data)
	{
		delete[] pParticule_Position_Size_Data;
		pParticule_Position_Size_Data = NULL;
	}

	// Disable vertex attribute arrays
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Delete the rendering objects in the graphics card
	glDeleteBuffers(1, &iTextureID);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Set the maximum number of particles
 */
void CParticles3D::SetMaxParticles(const int iMaxParticles)
{
	this->iMaxParticles = iMaxParticles;
}

// Set the life of particles
void CParticles3D::SetLife(const float fLife)
{
	this->fLife = fLife;
}

// Set the spread of particles
void CParticles3D::SetSpread(const float fSpread)
{
	this->fSpread = fSpread;
}

// Set the movement direction of particles
void CParticles3D::SetMovementDirection(const glm::vec3 vec3MovementVelocity)
{
	this->vec3MovementVelocity = vec3MovementVelocity;
}

// Get the life of particles
float CParticles3D::GetLife(void)
{
	return fLife;
}

// Get the spread of particles
float CParticles3D::GetSpread(void)
{
	return fSpread;
}

// Get the movement direction of particles
glm::vec3 CParticles3D::GetMovementDirection(void)
{
	return vec3MovementVelocity;
}

/**
 @brief Get the maximum number of particles
 @return the maximum number particles in this instance
 */
int CParticles3D::GetMaxParticles(void)
{
	return iMaxParticles;
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CParticles3D::Update(double dElapsedTime)
{
	// We will need the camera's position in order to sort the particles
	// w.r.t the camera's distance.
	// There should be a getCameraPosition() function in common/controls.cpp, 
	// but this works too.
	//glm::vec3 CameraPosition(glm::inverse(theViewMatrix)[3]);
	glm::vec3 CameraPosition(glm::inverse(view)[3]);

	// Generate 10 new particle each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// new particles will be huge and the next frame even longer.
	int newparticles = (int)(dElapsedTime * 10000.0);
	if (newparticles > (int)(0.016f * 10000.0))
		newparticles = (int)(0.016f * 10000.0);

	for (int i = 0; i < newparticles; i++) {
		int particleIndex = FindUnusedParticle();
		// Set the life of this particle
		pParticlesContainer[particleIndex].fLife = fLife;
		// Set the position of this particle
		pParticlesContainer[particleIndex].vec3Position = vec3Position;
		// Get a vec3Random
		vec3Random = CRandomiser::GetRandomVec3();

		// Set the speed of the particle
		pParticlesContainer[particleIndex].vec3Speed = vec3MovementVelocity + vec3Random * fSpread;

		// Randomise the colours of the particle
		vec3Random = CRandomiser::GetRandomVec3(255.0f);
		pParticlesContainer[particleIndex].r = vec3Random.x;
		pParticlesContainer[particleIndex].g = vec3Random.y;
		pParticlesContainer[particleIndex].b = vec3Random.z;
		pParticlesContainer[particleIndex].a = (rand() % 256) / 3;

		pParticlesContainer[particleIndex].fSize = (rand() % 1000) / 10000.0f + 0.05f;
	}

	// Simulate all particles
	iParticlesCount = 0;
	for (int i = 0; i < iMaxParticles; i++) {

		// Get a handle to an element in pParticlesContainer
		CParticle3D& p = pParticlesContainer[i];

		if (p.fLife > 0.0f) {

			// Decrease life
			p.fLife -= dElapsedTime;
			if (p.fLife > 0.0f) {

				// Simulate simple physics : gravity only, no collisions
				p.vec3Speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)dElapsedTime * 0.5f;
				p.vec3Position += p.vec3Speed * (float)dElapsedTime;
				p.fCameraDistance = glm::length2(p.vec3Position - CameraPosition);

				// Fill the GPU buffer
				pParticule_Position_Size_Data[4 * iParticlesCount + 0] = p.vec3Position.x;
				pParticule_Position_Size_Data[4 * iParticlesCount + 1] = p.vec3Position.y;
				pParticule_Position_Size_Data[4 * iParticlesCount + 2] = p.vec3Position.z;

				pParticule_Position_Size_Data[4 * iParticlesCount + 3] = p.fSize;

				pParticule_Color_Data[4 * iParticlesCount + 0] = p.r;
				pParticule_Color_Data[4 * iParticlesCount + 1] = p.g;
				pParticule_Color_Data[4 * iParticlesCount + 2] = p.b;
				pParticule_Color_Data[4 * iParticlesCount + 3] = p.a;

			}
			else {
				// Particles that just died will be put at the end of the buffer in SortParticles();
				p.fCameraDistance = -1.0f;
			}

			iParticlesCount++;
		}
	}

	// Sort the particles
	SortParticles();

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CParticles3D::PreRender(void)
{
	// Change depth function so depth test passes when values are less than or equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Enable the blending mode
	glEnable(GL_BLEND);
	// This blend function sorts the primitives from farthest to nearest
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// This blend function will make the particles glow when they overlap each other
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// Use the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render the particles
 */
void CParticles3D::Render(void)
{
	// Calculate the view-projection matrix
	mat4ViewProjection = projection * view;

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(iTextureSampler, 0);

	glBindBuffer(GL_ARRAY_BUFFER, iParticles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, iMaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, iParticlesCount * sizeof(GLfloat) * 4, pParticule_Position_Size_Data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, iParticles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, iMaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, iParticlesCount * sizeof(GLubyte) * 4, pParticule_Color_Data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Upload the values to let the Quad face the camera, a.k.a. Billboard
	glUniform3f(iCameraRight_worldspace_ID, view[0][0], view[1][0], view[2][0]);
	glUniform3f(iCameraUp_worldspace_ID, view[0][1], view[1][1], view[2][1]);

	glUniformMatrix4fv(iViewProjMatrixID, 1, GL_FALSE, &mat4ViewProjection[0][0]);

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming
	glBindVertexArray(VAO);

	// Disable vertex attribute arrays
	glEnableVertexAttribArray(0);
	// Disable vertex attribute arrays
	glEnableVertexAttribArray(1);
	// Disable vertex attribute arrays
	glEnableVertexAttribArray(2);

	// Draw the particles !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in iParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
	// but faster.
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, iParticlesCount);

	// Disable vertex attribute arrays
	glDisableVertexAttribArray(0);
	// Disable vertex attribute arrays
	glDisableVertexAttribArray(1);
	// Disable vertex attribute arrays
	glDisableVertexAttribArray(2);

	// Disable Vertex Array
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CParticles3D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);

	// Set depth function back to default
	glDepthFunc(GL_LESS);
}


/**
 @brief Finds a Particle in pParticlesContainer which isn't used yet. (i.e. life < 0);
 @return the index number of an unused particles in pParticlesContainer
 */
int CParticles3D::FindUnusedParticle(void) {

	for (int i = iLastUsedParticle; i < iMaxParticles; i++) {
		if (pParticlesContainer[i].fLife < 0) {
			iLastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < iLastUsedParticle; i++) {
		if (pParticlesContainer[i].fLife < 0) {
			iLastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

/**
 @brief Sort the particles so that they are arranged from the particles which are furthest from the camera
		to the nearest
 */
void CParticles3D::SortParticles(void)
{
	std::sort(&pParticlesContainer[0], &pParticlesContainer[iMaxParticles]);
}
