#pragma once
#include <unordered_map>
#include <memory>

#include "Component.h"


namespace components {

	class ComponentSystem {
	protected:
		ComponentSystem();

	public:
		virtual ~ComponentSystem();

		template<typename T>
		bool HasComponent() const { return m_components.find(Component<T>::GetID()) != m_components.end(); }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			int compID = Component<T>::GetID();
			if (!HasComponent<T>()) {
				m_components[compID] = std::make_unique<T>(std::forward<Args>(args)...);
			}
			return *static_cast<T*>(m_components[compID].get());
		}

		template<typename T>
		T* GetComponent()
		{
			if (!HasComponent<T>()) return nullptr;
			int compID = Component<T>::GetID();
			return static_cast<T*>(m_components[compID].get());
		}

		template<typename T>
		void RemoveComponent()
		{
			if (!HasComponent<T>()) return;
			m_components.erase(Component<T>::GetID());
		}

		virtual void Update(float deltaTime);

	protected:
		std::unordered_map<int, std::unique_ptr<IComponent>> m_components;

	};

}
