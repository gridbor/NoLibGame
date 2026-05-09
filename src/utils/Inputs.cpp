#include "Inputs.h"
#include "Logger.h"


Inputs::Inputs()
{
	s_instance = this;
	m_keyboard.reserve(10);
	m_mouseButtons.reserve(6);
}

Inputs::~Inputs()
{
	m_keyboard.clear();
	m_mouseButtons.clear();
}

bool Inputs::IsKeyDown(unsigned int key) const
{
	if (m_keyboard.empty()) return false;
	return std::find(m_keyboard.begin(), m_keyboard.end(), key) != m_keyboard.end();
}

bool Inputs::IsMouseButtonPressed(const EMouseButton& button) const
{
	if (m_mouseButtons.empty()) return false;
	return std::find(m_mouseButtons.begin(), m_mouseButtons.end(), button) != m_mouseButtons.end();
}

void Inputs::Update(float deltaTime)
{
	m_mouseDelta = Vector2();
}

void Inputs::AddKey(unsigned int key)
{
	if (IsKeyDown(key)) return;
	m_keyboard.push_back(key);
}

void Inputs::RemoveKey(unsigned int key)
{
	m_keyboard.erase(std::find(m_keyboard.begin(), m_keyboard.end(), key));
}

void Inputs::UpdateMousePosition(int x, int y)
{
	Vector2 v{ (float)x, (float)y };
	m_mouseDelta = v - m_mousePosition;
	m_mousePosition = v;
}

void Inputs::AddMouseButton(const EMouseButton& button)
{
	if (IsMouseButtonPressed(button)) return;
	m_mouseButtons.push_back(button);
}

void Inputs::RemoveMouseButton(const EMouseButton& button)
{
	m_mouseButtons.erase(std::find(m_mouseButtons.begin(), m_mouseButtons.end(), button));
}
