#version 460 core

layout(location = 0) in unsigned int aOpType;
layout(location = 1) in vec2 aV0;
layout(location = 2) in vec2 aV1;
layout(location = 3) in vec2 aV2;
layout(location = 4) in vec3 aColor;

out VS_OUT {
	unsigned int opType;
	vec2 v0;
	vec2 v1;
	vec2 v2;
	vec3 color;
} vs_out;

void main()
{
	vs_out.opType = aOpType;
	vs_out.v0 = aV0;
	vs_out.v1 = aV1;
	vs_out.v2 = aV2;
	vs_out.color = aColor;
	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
