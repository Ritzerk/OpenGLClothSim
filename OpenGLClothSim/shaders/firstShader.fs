#version 330 core

out vec4 FragColor;

in vec2 TxtCoord;

uniform sampler2D texture1;	//built in sampler for what we need

void main()
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = texture(texture1, TxtCoord);

}