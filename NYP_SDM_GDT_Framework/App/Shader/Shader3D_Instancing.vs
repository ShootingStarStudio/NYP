#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColour;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in mat4 aInstanceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec4 Colour;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 newModel = aInstanceMatrix * model;
    FragPos = vec3(newModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(newModel))) * aNormal;  
	Colour = aColour;
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * vec4(FragPos, 1.0f);
}