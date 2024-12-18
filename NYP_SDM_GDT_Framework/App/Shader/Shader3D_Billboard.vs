#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
//This code is in Shader3D_Particles.fs: layout(location = 1) in vec4 xyzs; // Position of the center of the particle and size of the square
//This code is in Shader3D_Particles.fs: layout(location = 2) in vec4 color; // Position of the center of the particle and size of the square

// Output data ; will be interpolated for each fragment.
out vec2 UV;
//This code is in Shader3D_Particles.fs: out vec4 particlecolor;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 BillboardPos; // Position of the center of the billboard
uniform vec2 BillboardSize; // Size of the billboard, in world units (probably meters)

void main()
{
//This code is in Shader3D_Particles.fs: float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = BillboardPos;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * BillboardSize.x
		+ CameraUp_worldspace * squareVertices.y * BillboardSize.y;

	// Output position of the vertex
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	// Or, if BillboardSize is in percentage of the screen size (1,1 for fullscreen) :
	//vertexPosition_worldspace = particleCenter_wordspace;
	//gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f); // Get the screen-space position of the particle's center
	//gl_Position /= gl_Position.w; // Here we have to do the perspective division ourselves.
	//gl_Position.xy += squareVertices.xy * vec2(0.2, 0.05); // Move the vertex in directly screen space. No need for CameraUp/Right_worldspace here.
	// Or, if BillboardSize is in pixels : 
	// Same thing, just use (ScreenSizeInPixels / BillboardSizeInPixels) instead of BillboardSizeInScreenPercentage.

	// UV of the vertex. No special space for this one.
	UV = squareVertices.xy + vec2(0.5, 0.5);
//This code is in Shader3D_Particles.fs: particlecolor = color;
}

