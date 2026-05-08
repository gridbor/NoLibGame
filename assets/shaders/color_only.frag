#version 460 core

in vec3 vColor;

out vec4 ResultColor;

void main()
{
	ResultColor = vec4(vColor, 1.0);
}
