#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

smooth out vec3 FragPos;
smooth out vec3 Normal;
smooth out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 heightmapScale;

void main()
{
  vec4 aPosScaled = heightmapScale * vec4(aPos, 1.0);

  FragPos = vec3(model * aPosScaled);
  //Normal = aNormal;
  Normal = mat3(transpose(inverse(model))) * aNormal;  
  TexCoord = aTexCoord;
    
  gl_Position = projection * view * model * aPosScaled;
}