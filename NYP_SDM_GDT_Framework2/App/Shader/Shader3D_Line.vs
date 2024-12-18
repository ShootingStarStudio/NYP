#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColour;

out vec4 ourColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float fBlendValue=0;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    ourColour = vec4(aColour.r, aColour.g, aColour.b, fBlendValue);
}
