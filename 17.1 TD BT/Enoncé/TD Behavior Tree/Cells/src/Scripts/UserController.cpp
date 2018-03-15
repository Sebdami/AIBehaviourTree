#include "stdafx.h"

#include "Scripts/UserController.h"
#include "Scripts/CharacterController.h"

// UserController
UserController::UserController()
{
	m_pCharacterController = nullptr;
	m_pGM = GameManager::getSingleton();
}

UserController::~UserController()
{
}

void UserController::setCharacterController(CharacterController* _pCharacterController)
{ 
	m_pCharacterController = _pCharacterController; 
}

bool UserController::init()
{
	m_pCharacterController = getEntity()->getComponent<CharacterController>();

	return true;
}

bool UserController::update()
{
	// Preconditions
	if (!m_pCharacterController)
	{
		return false;
	}
	if (!m_pEntity->getSelected())
	{
		return false;
	}

	// Conditions
	if (m_pGM->isKeyPressed(Key::D))
	{
		m_pCharacterController->setCondition(kACond_Default);
	}

	if (m_pGM->isKeyPressed(Key::G))
	{
		m_pCharacterController->setCondition(kACond_Gold);
	}

	if (m_pGM->isKeyPressed(Key::L))
	{
		m_pCharacterController->setCondition(kACond_Lumber);
	}

	// Actions
	if (m_pGM->isKeyPressed(Key::C))
	{
		m_pCharacterController->setCondition(kACond_Default);
		m_pCharacterController->setAction(kAct_Chop);
	}
	else if (m_pGM->isKeyPressed(Key::K))
	{
		m_pCharacterController->setCondition(kACond_Default);
		m_pCharacterController->setAction(kAct_Die);
	}
	else
	{
		m_pCharacterController->setAction(kAct_Default);
	}

	// Directions
	m_vDirection = Vector2f(0.f, 0.f);
	if (m_pGM->isKeyPressed(Key::Up))
	{
		m_vDirection = Vector2f(0.f, -1.f);
		m_pCharacterController->setDirection(kADir_Up);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Down))
	{
		m_vDirection = Vector2f(0.f, 1.f);
		m_pCharacterController->setDirection(kADir_Down);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Left))
	{
		m_vDirection = Vector2f(-1.f, 0.f);
		m_pCharacterController->setDirection(kADir_Left);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Right))
	{
		m_vDirection = Vector2f(1.f, 0.f);
		m_pCharacterController->setDirection(kADir_Right);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Up) && m_pGM->isKeyPressed(Key::Right))
	{
		m_vDirection = Vector2f(1.f, -1.f);
		m_pCharacterController->setDirection(kADir_UpRight);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Up) && m_pGM->isKeyPressed(Key::Left))
	{
		m_vDirection = Vector2f(-1.f, -1.f);
		m_pCharacterController->setDirection(kADir_UpLeft);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Down) && m_pGM->isKeyPressed(Key::Right))
	{
		m_vDirection = Vector2f(1.f, 1.f);
		m_pCharacterController->setDirection(kADir_DownRight);
		m_pCharacterController->setAction(kAct_Walk);
	}
	if (m_pGM->isKeyPressed(Key::Down) && m_pGM->isKeyPressed(Key::Left))
	{
		m_vDirection = Vector2f(-1.f, 1.f);
		m_pCharacterController->setDirection(kADir_DownLeft);
		m_pCharacterController->setAction(kAct_Walk);
	}

	// Mouse control
	if (m_pGM->isMouseButtonPressed(Button::MouseRight))
	{
		Vector2f vMousePosition = m_pGM->getMousePosition();
		Vector2f vEntityPosition = getEntity()->getPosition();
		m_vDirection = vMousePosition - vEntityPosition;
		if (m_vDirection.length() > 1 && vMousePosition.getX() < 900)
		{
			m_pCharacterController->setAction(kAct_Walk);
		}
		else
		{
			m_vDirection = Vector2f(0.f, 0.f);
		}
	}

	m_vDirection.normalize();
	m_vDirection *= 200;
	m_pCharacterController->move(m_vDirection);

	return true;
}

bool UserController::draw()
{
	return true;
}

bool UserController::quit()
{
	return true;
}
