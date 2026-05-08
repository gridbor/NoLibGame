#pragma once
#include "math/Math.h"


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
