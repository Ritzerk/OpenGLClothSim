#version 330 core
out vec4 FragColour;

in vec2 TxtCoord;

uniform float myVariation;
uniform sampler2D texture1;	//built in sampler for what we need
uniform sampler2D texture2;

void main()
{
	FragColour = mix(texture(texture1, TxtCoord), texture(texture2, vec2(1.0 - TxtCoord.x, TxtCoord.y)), myVariation);
}