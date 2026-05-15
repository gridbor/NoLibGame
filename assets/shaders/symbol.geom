#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 128) out;

in VS_OUT {
	unsigned int opType;
	vec2 v0;
	vec2 v1;
	vec2 v2;
	vec3 color;
} gs_in[];

out vec3 vColor;

const float PI = 3.14159265358979323846;
const float T = 0.1;
const int ELLIPSE_SEGMENTS = 32;

void AddVertex(vec2 local)
{
	gl_Position = vec4(local, 0.0, 1.0);
	EmitVertex();
}

void DrawLine(vec2 start, vec2 end)
{
	if (abs(start.y - end.y) < 0.001) {
		AddVertex(vec2(start.x, start.y + T));
		AddVertex(vec2(start.x, start.y - T));
		AddVertex(vec2(end.x, end.y + T));
		AddVertex(vec2(end.x, end.y - T));
	} else {
		AddVertex(vec2(start.x - T, start.y));
		AddVertex(vec2(start.x + T, start.y));
		AddVertex(vec2(end.x - T, end.y));
		AddVertex(vec2(end.x + T, end.y));
	}
	EndPrimitive();
}

void DrawEllipse(vec2 center, vec2 size)
{
	float tt = T * 0.5;
	float step = 2.0 * PI / float(ELLIPSE_SEGMENTS);
	for (int i = 0; i < ELLIPSE_SEGMENTS; i++) {
		float a = i * step;
		float b = (i + 1) * step;
		vec2 outerStart = vec2(center.x + (size.x + tt) * cos(a), center.y + (size.y + tt) * sin(a));
		vec2 innerStart = vec2(center.x + (size.x - tt) * cos(a), center.y + (size.y - tt) * sin(a));
		vec2 outerEnd = vec2(center.x + (size.x + tt) * cos(b), center.y + (size.y + tt) * sin(b));
		vec2 innerEnd = vec2(center.x + (size.x - tt) * cos(b), center.y + (size.y - tt) * sin(b));
		AddVertex(innerEnd);
		AddVertex(innerStart);
		AddVertex(outerEnd);
		AddVertex(outerStart);
		EndPrimitive();
	}
}

void main()
{
	vec4 pos = gl_in[0].gl_Position;
	vColor = gs_in[0].color;
	if (gs_in[0].opType == 0) {
		DrawLine(gs_in[0].v0, gs_in[0].v1);
	}
	else if (gs_in[0].opType == 1) {
		DrawEllipse(gs_in[0].v0, gs_in[0].v1);
	}
	else {

	}
}
