#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float dx;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x - dx,aPos.y,aPos.z, 1.0);
	
}