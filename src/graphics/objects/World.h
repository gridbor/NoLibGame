#pragma once
#include "WorldObject.h"


class World : public WorldObject {
public:
	World();
	~World();

	virtual void Init() override;
	virtual void Render() override;

};
