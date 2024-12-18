#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vWorldPos;
smooth in vec4 vEyeSpacePos;

uniform sampler2D gSampler[5];
uniform sampler2D shadowMap;

uniform vec4 vColour;

//#include "dirLight.frag"
//uniform DirectionalLight sunLight;
uniform float fRenderHeight;
uniform float fMaxTextureU;
uniform float fMaxTextureV;

out vec4 outputColour;

void main()
{
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColour = vec4(0.0);
	
	float fScale = vWorldPos.y/fRenderHeight;

	const float fRange1 = 0.15f;		//fungus
	const float fRange2 = 0.3f;			//grass
	const float fRange3 = 0.65f;		//grass
	const float fRange4 = 0.85f;		//rock

	if(fScale >= 0.0 && fScale <= fRange1)vTexColour = texture(gSampler[0], vTexCoord);
	else if(fScale <= fRange2)
	{
		fScale -= fRange1;
		fScale /= (fRange2-fRange1);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColour += texture(gSampler[0], vTexCoord)*fScale;
		vTexColour += texture(gSampler[1], vTexCoord)*fScale2;
	}
	else if(fScale <= fRange3)vTexColour = texture(gSampler[1], vTexCoord);
	else if(fScale <= fRange4)
	{
		fScale -= fRange3;
		fScale /= (fRange4-fRange3);
		
		float fScale2 = fScale;
		fScale = 1.0-fScale; 
		
		vTexColour += texture(gSampler[1], vTexCoord)*fScale;
		vTexColour += texture(gSampler[2], vTexCoord)*fScale2;		
	}
	else vTexColour = texture(gSampler[2], vTexCoord);

	//vec2 vPathCoord = vec2(vTexCoord.x/fMaxTextureU, vTexCoord.y/fMaxTextureV);
	//vec4 vPathIntensity = texture(gSampler[4], vPathCoord);
	//fScale = vPathIntensity.x;
  
	//vec4 vPathColour = texture(gSampler[3], vTexCoord); // Black Colour means there is a path
	//vec4 vFinalTexColour = fScale*vTexColour+(1-fScale)*vPathColour;
	vec4 vFinalTexColour = vTexColour;

	vec4 vMixedColour = vFinalTexColour*vColour;
	//vec4 vDirLightColour = GetDirectionalLightColour(sunLight, vNormal);

	outputColour = vMixedColour;//*(vDirLightColour);
  
}                      