#include "stdafx.h"

#include "Core\GameManager.h"
#include <SFML/Graphics.hpp>

namespace crea
{
	GameManager::GameManager()
		: m_rWindowRect(0, 0, 640, 320)
	{
		m_pWindow = nullptr;
	}

	GameManager::~GameManager()
	{
		delete m_pWindow;
	}

	GameManager* GameManager::getSingleton()
	{
		static GameManager instanceUnique;
		return
			&instanceUnique;
	}

	void GameManager::init()
	{
		m_pWindow = new sf::RenderWindow(sf::VideoMode((unsigned int)m_rWindowRect.getWidth(), (unsigned int)m_rWindowRect.getHeight()), "CreaEngine");

		// Init managers
		TimeManager::getSingleton()->init();
		EntityManager::getSingleton()->init();
		PhysicsManager::getSingleton()->init();

	}

	void GameManager::update()
	{
		while (m_pWindow->isOpen())
		{
			sf::Event event;
			while (m_pWindow->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_pWindow->close();
			}

			// Update managers
			TimeManager::getSingleton()->update();
			InputManager::getSingleton()->update();
			SceneManager::getSingleton()->update();
			EntityManager::getSingleton()->update();
			PhysicsManager::getSingleton()->update();
			MsgManager::getSingleton()->update();

			// Draw managers
			m_pWindow->clear();

			EntityManager::getSingleton()->draw();
			SceneManager::getSingleton()->draw();

			m_pWindow->display();
		}

	}

	void GameManager::quit()
	{
		// Clear managers
		EntityManager::getSingleton()->clear();
		PhysicsManager::getSingleton()->clear();
	}


} // namespace crea
