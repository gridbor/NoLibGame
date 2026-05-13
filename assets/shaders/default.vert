#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV;

layout(std140, binding = 0) uniform CameraData {
	mat4 projection;
	mat4 view;
};
uniform mat4 uModel;

out vec3 vColor;
out vec3 vNormal;
out vec2 vUV;

void main()
{
	vColor = aColor;
	mat3 normalMatrix = transpose(inverse(mat3(uModel)));
	vNormal = normalMatrix * aNormal;
	vUV = aUV;
	gl_Position = projection * view * uModel * vec4(aPosition, 1.0);
}
