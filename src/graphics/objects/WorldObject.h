#pragma once
#include "GLFunctions.h"
#include "math/Math.h"
#include "graphics/Renderable.h"
#include "components/ComponentSystem.h"
#include "components/Transform.h"


class WorldObject : public Renderable<Vertex> {
protected:
	WorldObject() = default;

public:
	virtual ~WorldObject() = default;

	virtual void Init() override {}
	virtual void Render() override { Renderable::Render(); }
	virtual void Update(float deltaTime) override
	{
		components::ComponentSystem::Update(deltaTime);
	}

	virtual void AddChild(WorldObject* child)
	{
		if (child == nullptr || HasChild(child)) return;
		m_childs.push_back(child);
		child->SetParent(this);
	}
	virtual void RemoveChild(WorldObject* child)
	{
		size_t index = FindChild(child);
		if (index == std::numeric_limits<size_t>::max()) return;
		m_childs.erase(m_childs.begin() + index);
		child->SetParent(nullptr);
	}
	virtual WorldObject* GetChild(size_t index)
	{
		if (index < m_childs.size()) return m_childs[index];
		return nullptr;
	}
	virtual bool HasChild(WorldObject* child)
	{
		return FindChild(child) != std::numeric_limits<size_t>::max();
	}
	virtual size_t FindChild(WorldObject* child)
	{
		size_t result = std::numeric_limits<size_t>::max();
		if (child == nullptr) return result;
		for (size_t i = 0; i < m_childs.size(); i++) {
			if (m_childs[i] == child) {
				result = i;
				break;
			}
		}
		return result;
	}
	virtual size_t GetChildsCount() const { return m_childs.size(); }
	virtual const std::vector<WorldObject*>& GetChildrens() const { return m_childs; }

	virtual WorldObject* GetParent() const { return m_parent; }
	virtual bool HasParent() const { return m_parent != nullptr; }

protected:
	virtual void SetParent(WorldObject* parent) { m_parent = parent; }

protected:
	WorldObject* m_parent = nullptr;
	std::vector<WorldObject*> m_childs;

};
