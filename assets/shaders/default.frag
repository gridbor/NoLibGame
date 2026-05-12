#version 460 core

in vec3 vColor;
in vec3 vNormal;
in vec2 vUV;

layout(binding = 0) uniform sampler2D uMainTexture;

out vec4 ResultColor;

void main()
{
	vec4 textureColor = texture(uMainTexture, vUV);
	ResultColor = textureColor * vec4(vColor, 1.0);
}
