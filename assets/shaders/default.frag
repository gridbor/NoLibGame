#version 460 core

in vec3 vColor;
in vec3 vNormal;
in vec2 vUV;

out vec4 ResultColor;

void main()
{
	ResultColor = vec4(vColor, 1.0);
}
