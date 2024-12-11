#version 410 core
out vec4 FragColour;

in GS_OUT {
	vec2 textCoord;
    float colourVariation;
} fs_in;

uniform sampler2D u_textgrass;

void main(){
    // very simple but we can also add lighting to get better result
    vec4 colour = texture(u_textgrass, fs_in.textCoord);
    if (colour.a < 0.25 ) discard;
    colour.xyz = mix(colour.xyz, 0.5*colour.xyz, fs_in.colourVariation);
	FragColour = colour;
}
