#include "stdafx.h"

#include "Input\InputManager.h"

#include <SFML/Graphics.hpp>

namespace crea
{
	InputManager::InputManager()
	{
		m_pGM = GameManager::getSingleton();
	}

	InputManager::~InputManager()
	{

	}

	InputManager* InputManager::getSingleton()
	{
		static InputManager instanceUnique;
		return
			&instanceUnique;
	}

	bool InputManager::update()
	{
		return true;
	}

	bool InputManager::isKeyPressed(Key _key)
	{
		return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)_key); // Key mapping is the same as SFML
	}

	bool InputManager::isMouseButtonPressed(Button _button)
	{
		return sf::Mouse::isButtonPressed((sf::Mouse::Button)_button); // Mouse mapping is the same as SFML
	}

	Vector2f InputManager::getMousePosition()
	{
		sf::Vector2i vMousePosition = sf::Mouse::getPosition(*m_pGM->getWindow()); // Get position relative to window
		return Vector2f((float)vMousePosition.x, (float)vMousePosition.y);
	}
} // namespace crea
