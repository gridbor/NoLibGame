#pragma once
#include "GLFunctions.h"
#include "math/Math.h"
#include "graphics/Renderable.h"
#include "components/ComponentSystem.h"
#include "components/Transform.h"


class WorldObject : public Renderable<Vertex> {
protected:
	WorldObject() : m_objectID{ ++s_objectCounter } {}

public:
	virtual ~WorldObject()
	{
		if (!m_childs.empty()) {
			if (m_parent != nullptr) {
				for (auto& child : m_childs) {
					if (child == nullptr) continue;
					m_parent->AddChild(child);
				}
			}
			m_childs.clear();
		}
	}

	virtual void Init() override
	{
		if (!m_childs.empty()) {
			for (const auto& child : m_childs) {
				if (child == nullptr) continue;
				child->Init();
			}
		}
	}
	virtual void Render() override
	{
		if (!m_childs.empty()) {
			for (const auto& child : m_childs) {
				if (child == nullptr) continue;
				child->Render();
			}
		}
		Renderable::Render();
	}
	virtual void Update(float deltaTime) override
	{
		components::ComponentSystem::Update(deltaTime);
	}

	void AddChild(WorldObject* child)
	{
		if (child == nullptr || HasChild(child)) return;
		m_childs.push_back(child);
		child->SetParent(this);
	}
	void RemoveChild(WorldObject* child)
	{
		size_t index = FindChild(child);
		if (index == std::numeric_limits<size_t>::max()) return;
		m_childs.erase(m_childs.begin() + index);
		child->SetParent(nullptr);
	}
	WorldObject* GetChild(size_t index)
	{
		if (index < m_childs.size()) return m_childs[index];
		return nullptr;
	}
	bool HasChild(WorldObject* child)
	{
		return FindChild(child) != std::numeric_limits<size_t>::max();
	}
	size_t FindChild(WorldObject* child)
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
	size_t FindChild(unsigned int id)
	{
		size_t result = std::numeric_limits<size_t>::max();
		for (size_t i = 0; i < m_childs.size(); i++) {
			if (m_childs[i] == nullptr) continue;
			if (m_childs[i]->GetID() == id) {
				result = i;
				break;
			}
		}
		return result;
	}
	size_t GetChildsCount() const { return m_childs.size(); }
	const std::vector<WorldObject*>& GetChildrens() const { return m_childs; }

	WorldObject* GetParent() const { return m_parent; }
	bool HasParent() const { return m_parent != nullptr; }

	unsigned int GetID() const { return m_objectID; }

protected:
	virtual void SetParent(WorldObject* parent) { m_parent = parent; }

protected:
	unsigned int m_objectID;
	WorldObject* m_parent = nullptr;
	std::vector<WorldObject*> m_childs;

private:
	static inline unsigned int s_objectCounter = 0;

};
