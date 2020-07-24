#version 330 core
layout (location = 0) in vec3 aPos;	//this line sets the generic attribute index (attribindex/generic vertex attribute), which we use when binding and so on. 
layout (location = 1) in vec2 aTxtCoord;

out vec2 TxtCoord;

uniform mat4 transformation;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
	TxtCoord = vec2(aTxtCoord.x, aTxtCoord.y);


}