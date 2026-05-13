#version 460 core

in vec3 vColor;
in vec3 vNormal;
in vec2 vUV;

layout(binding = 0) uniform sampler2D uMainTexture;
layout(location = 4) uniform vec3 uLightDirection;

out vec4 ResultColor;

void main()
{
	vec3 normal = normalize(vNormal);
	float dotNL = dot(normal, normalize(-uLightDirection));
	float diff = max(dotNL, 0.0);
	vec4 textureColor = texture(uMainTexture, vUV);

	ResultColor = vec4(diff * textureColor.rgb * vColor, textureColor.a);
}
