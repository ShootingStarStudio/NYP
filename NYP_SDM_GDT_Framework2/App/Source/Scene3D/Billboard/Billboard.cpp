/**
 CBillboard: Render 3D particles
 By: Toh Da Jun
 Date: May 2024
 */
#include "Billboard.h"

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

#define DRAW_CUBE // Comment or uncomment this to simplify the code

/**
 @brief Default constructor
 */
CBillboard::CBillboard(void)
	: dCurrentTime(0.0)
{
}

/**
 @brief Destructor
 */
CBillboard::~CBillboard(void)
{
	DeInit();
}

/**
 @brief Initialise this class instance
 @return true if the initialisation was successful, otherwise false
 */
bool CBillboard::Init(void)
{
	// Create and compile our GLSL program from the shaders
	CShaderManager::GetInstance()->Use("Shader3D_Billboard");
	GLuint iProgramID = CShaderManager::GetInstance()->pActiveShader->ID;

	// Vertex shader
	iCameraRight_worldspace_ID = glGetUniformLocation(iProgramID, "CameraRight_worldspace");
	iCameraUp_worldspace_ID = glGetUniformLocation(iProgramID, "CameraUp_worldspace");
	iViewProjMatrixID = glGetUniformLocation(iProgramID, "VP");

	iBillboardPosID = glGetUniformLocation(iProgramID, "BillboardPos");
	iBillboardSizeID = glGetUniformLocation(iProgramID, "BillboardSize");
	iLifeLevelID = glGetUniformLocation(iProgramID, "LifeLevel");

	iTextureSampler = glGetUniformLocation(iProgramID, "myTextureSampler");

	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Billboard.png", true);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Billboard.png" << endl;
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &iBillboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, iBillboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// First attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, iBillboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return true;
}

/**
 @brief DeInitialise this instance
 */
void CBillboard::DeInit(void)
{
	// Disable vertex attribute arrays
	glDisableVertexAttribArray(0);

	// Delete the rendering objects in the graphics card
	glDeleteBuffers(1, &iTextureID);
	glDeleteBuffers(1, &iBillboard_vertex_buffer);
	glDeleteTextures(1, &iTextureSampler);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

/**
 @brief Update the elapsed time
 @param dElapsedTime A const double variable containing the elapsed time since the last frame
 @return A bool variable to indicate this method successfully completed its tasks
 */
bool CBillboard::Update(double dElapsedTime)
{
	dCurrentTime += dElapsedTime;

	return true;
}

/**
@brief PreRender Set up the OpenGL display environment before rendering
*/
void CBillboard::PreRender(void)
{
	// Change depth function so depth test passes when values are less than or equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Enable the blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Use the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render the particles
 */
void CBillboard::Render(void)
{
	// Calculate the view-projection matrix
	mat4ViewProjection = projection * view;

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(iTextureSampler, 0);

	// This is the only interesting part of the tutorial.
	// This is equivalent to multiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
	// ViewMatrix is orthogonal (it was made this way), 
	// so its inverse is also its transpose, 
	// and transposing a matrix is "free" (inversing is slooow)
	glUniform3f(iCameraRight_worldspace_ID, view[0][0], view[1][0], view[2][0]);
	glUniform3f(iCameraUp_worldspace_ID, view[0][1], view[1][1], view[2][1]);

	glUniform3f(iBillboardPosID, vec3Position.x, vec3Position.y, vec3Position.z); // The billboard will be just above the cube
	glUniform2f(iBillboardSizeID, 1.0f, 0.125f);     // and 1m*12cm, because it matches its 256*32 resolution =)

	// Generate some fake life level and send it to glsl
	float LifeLevel = sin(dCurrentTime) * 0.1f + 0.7f;
	glUniform1f(iLifeLevelID, LifeLevel);

	glUniformMatrix4fv(iViewProjMatrixID, 1, GL_FALSE, &mat4ViewProjection[0][0]);

	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming
	glBindVertexArray(VAO);

	// Draw the billboard !
	// This draws a triangle_strip which looks like a quad.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Disable Vertex Array
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
@brief PostRender Set up the OpenGL display environment after rendering.
*/
void CBillboard::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);

	// Set depth function back to default
	glDepthFunc(GL_LESS);
}
