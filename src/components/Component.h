#pragma once


namespace components {

	class IComponent {
	protected:
		IComponent() = default;
	public:
		virtual ~IComponent() = default;
		virtual void Update(float deltaTime) {}
	};


	template<typename T>
	class Component : public IComponent {
	protected:
		Component() = default;
	public:
		virtual ~Component() = default;
		static inline int GetID() { static int id = ++m_idCounter; return id; }
	private:
		static inline int m_idCounter = 0;
	};

}
