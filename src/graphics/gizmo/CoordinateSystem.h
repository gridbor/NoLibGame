#pragma once
#include "graphics/Renderable.h"


namespace gizmo {

	class CoordinateSystem : public Renderable<SimpleVertex> {
	public:
		CoordinateSystem();
		~CoordinateSystem();

		void Init() override;
		void Update(float deltaTime);
		void Render() override;

	};

}
