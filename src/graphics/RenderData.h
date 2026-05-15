#pragma once
#include "math/Math.h"


struct SymbolCommand {
	unsigned int opType;
	Vector2 v0;
	Vector2 v1;
	Vector2 v2;
	Vector3 color;
};

struct SimpleVertex {
	Vector3 position;
	Vector3 color;
};

struct Vertex {
	Vector3 position;
	Vector3 color;
	Vector3 normal;
	Vector2 uv;
};

struct CameraData {
	Matrix4 projection;
	Matrix4 view;
};
