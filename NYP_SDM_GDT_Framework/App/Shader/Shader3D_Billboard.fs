#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
//This code is in Shader3D_Particles.fs: in vec4 particlecolor;

// Output data
out vec4 color;

uniform sampler2D myTextureSampler;

uniform float LifeLevel;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV );
	

	// Hardcoded life level, should be in a separate texture.
	if (UV.x < LifeLevel && UV.y > 0.3 && UV.y < 0.7 && UV.x > 0.04 )
		color = vec4(0.2, 0.8, 0.2, 1.0); // Opaque green
}