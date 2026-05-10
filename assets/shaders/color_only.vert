#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

layout(std140, binding = 0) uniform CameraData {
	mat4 projection;
	mat4 view;
};
uniform mat4 uModel;

out vec3 vColor;

void main()
{
	vColor = aColor;
	gl_Position = projection * view * uModel * vec4(aPosition, 1.0);
}
