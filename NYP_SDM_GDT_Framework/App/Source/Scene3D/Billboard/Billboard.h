/**
 CBillboard: Render 3D particles
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

// CBillboard acts as a container for rendering a large number of 
// particles by repeatedly spawning and updating particles and killing 
// them after a given amount of time.
class CBillboard : public CEntity3D
{
public:
	// default constructor
	CBillboard(void);
	// destructor
	~CBillboard(void);

	// Initialise this class instance
	bool Init(void);
	// DeInitialise this instance
	void DeInit(void);

	// Update this class instance
	bool Update(double dElapsedTime);

	// PreRender
	virtual void PreRender(void);
	// Render
	virtual void Render(void);
	// PostRender
	virtual void PostRender(void);

protected:
	// For OpenGL
	GLuint iBillboard_vertex_buffer;
	GLuint iCameraRight_worldspace_ID;
	GLuint iCameraUp_worldspace_ID;
	GLuint iBillboardPosID;
	GLuint iBillboardSizeID;
	GLuint iLifeLevelID;
	GLuint iTextureSampler;

	GLuint iViewProjMatrixID;
	glm::mat4 mat4ViewProjection;

	double dCurrentTime;
};




//
//#ifdef DRAW_CUBE
//	// Everything here comes from Tutorial 4
//	GLuint cubeProgramID = LoadShaders("../tutorial04_colored_cube/TransformVertexShader.vertexshader", "../tutorial04_colored_cube/ColorFragmentShader.fragmentshader");
//	GLuint cubeMatrixID = glGetUniformLocation(cubeProgramID, "MVP");
//	static const GLfloat g_cube_vertex_buffer_data[] = { -1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f, 1.0f, 1.0f,1.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f,-1.0f,1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f, 1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,-1.0f,-1.0f,-1.0f, 1.0f, 1.0f,-1.0f,-1.0f, 1.0f,1.0f,-1.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f,-1.0f,-1.0f,1.0f, 1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f, 1.0f, 1.0f,1.0f,-1.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f, 1.0f,-1.0f,-1.0f, 1.0f,-1.0f,1.0f, 1.0f, 1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f,-1.0f, 1.0f, 1.0f,1.0f,-1.0f, 1.0f };
//	static const GLfloat g_cube_color_buffer_data[] = { 0.583f,  0.771f,  0.014f,0.609f,  0.115f,  0.436f,0.327f,  0.483f,  0.844f,0.822f,  0.569f,  0.201f,0.435f,  0.602f,  0.223f,0.310f,  0.747f,  0.185f,0.597f,  0.770f,  0.761f,0.559f,  0.436f,  0.730f,0.359f,  0.583f,  0.152f,0.483f,  0.596f,  0.789f,0.559f,  0.861f,  0.639f,0.195f,  0.548f,  0.859f,0.014f,  0.184f,  0.576f,0.771f,  0.328f,  0.970f,0.406f,  0.615f,  0.116f,0.676f,  0.977f,  0.133f,0.971f,  0.572f,  0.833f,0.140f,  0.616f,  0.489f,0.997f,  0.513f,  0.064f,0.945f,  0.719f,  0.592f,0.543f,  0.021f,  0.978f,0.279f,  0.317f,  0.505f,0.167f,  0.620f,  0.077f,0.347f,  0.857f,  0.137f,0.055f,  0.953f,  0.042f,0.714f,  0.505f,  0.345f,0.783f,  0.290f,  0.734f,0.722f,  0.645f,  0.174f,0.302f,  0.455f,  0.848f,0.225f,  0.587f,  0.040f,0.517f,  0.713f,  0.338f,0.053f,  0.959f,  0.120f,0.393f,  0.621f,  0.362f,0.673f,  0.211f,  0.457f,0.820f,  0.883f,  0.371f,0.982f,  0.099f,  0.879f };
//	GLuint cubevertexbuffer;
//	glGenBuffers(1, &cubevertexbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, cubevertexbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_vertex_buffer_data), g_cube_vertex_buffer_data, GL_DYNAMIC_DRAW);
//	GLuint cubecolorbuffer;
//	glGenBuffers(1, &cubecolorbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cube_color_buffer_data), g_cube_color_buffer_data, GL_DYNAMIC_DRAW);
//#endif
//
//#ifdef DRAW_CUBE
//		// Again : this is just Tutorial 4 !
//		glDisable(GL_BLEND);
//		glUseProgram(cubeProgramID);
//		glm::mat4 cubeModelMatrix(1.0f);
//		cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
//		glm::mat4 cubeMVP = ProjectionMatrix * ViewMatrix * cubeModelMatrix;
//		glUniformMatrix4fv(cubeMatrixID, 1, GL_FALSE, &cubeMVP[0][0]);
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, cubevertexbuffer);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//		glEnableVertexAttribArray(1);
//		glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
//		glDisableVertexAttribArray(0);
//		glDisableVertexAttribArray(1);
//#endif
//
//
//
//
//#ifdef DRAW_CUBE
//	glDeleteProgram(cubeProgramID);
//	glDeleteVertexArrays(1, &cubevertexbuffer);
//	glDeleteVertexArrays(1, &cubecolorbuffer);
//#endif
