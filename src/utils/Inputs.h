#pragma once
#include <vector>
#include <Windows.h>

#include "math/Math.h"


enum class EMouseButton : unsigned char {
	None = 0,
	Left,
	Right,
	Middle
};


class Inputs {
	friend class App;

public:
	Inputs();
	~Inputs();

	static inline Inputs& Get() { return *s_instance; }

	bool HasAnyPressedKey() const { return !m_keyboard.empty(); }
	bool IsKeyDown(unsigned int key) const;
	Vector2 GetMousePosition() const { return m_mousePosition; }
	Vector2 GetMouseDelta() const { return m_mouseDelta; }
	bool IsMouseButtonPressed(const EMouseButton& button) const;

private:
	void Update(float deltaTime);

	void AddKey(unsigned int key);
	void RemoveKey(unsigned int key);
	void UpdateMousePosition(int x, int y);
	void AddMouseButton(const EMouseButton& button);
	void RemoveMouseButton(const EMouseButton& button);

private:
	static inline Inputs* s_instance = nullptr;
	std::vector<unsigned int> m_keyboard;
	std::vector<EMouseButton> m_mouseButtons;
	Vector2 m_mousePosition{};
	Vector2 m_mouseDelta{};

};
