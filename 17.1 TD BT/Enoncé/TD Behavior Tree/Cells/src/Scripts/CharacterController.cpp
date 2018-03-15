#include "stdafx.h"

#include "Scripts\CharacterController.h"


CharacterController::CharacterController()
{
	m_pGM = crea::GameManager::getSingleton();
	m_pActionTable = nullptr;
	m_pAnimator = nullptr;
	m_pCollider = nullptr;
}

CharacterController::~CharacterController()
{
}

void CharacterController::setActionTable(crea::ActionTable* _pActionTable)
{
	m_pActionTable = _pActionTable;
}

void CharacterController::setAnimator(crea::Animator* _pAnimator)
{
	m_pAnimator = _pAnimator;
}

void CharacterController::setCondition(EnumAnimCondition _eCondition)
{
	m_eCondition = _eCondition;
}

void CharacterController::setAction(EnumAction _eAction)
{
	m_eAction = _eAction;

	// CB: should be given by the action table or the animation?
	if (m_eAction == kAct_Die)
	{
		m_bAlive = false;
		m_bMoving = false;
	}
	else if (m_eAction == kAct_Chop) // CB: Not sure Chop should be in engine... Static instead?
	{
		m_bAlive = true;
		m_bMoving = false;
	}
	else
	{
		m_bAlive = true;
		m_bMoving = true;
	}
}

void CharacterController::setDirection(EnumCharacterDirection _eDirection)
{
	assert(_eDirection >= 0 && _eDirection <= kADir_UpLeft);
	m_eDirection = _eDirection;
}

// Move the controller by a vector, only constrained by collisions
void CharacterController::move(crea::Vector2f _vMotion)
{
	m_vMotion = _vMotion;

	if (m_vMotion.length() > 0.f)
	{
		// Find direction from move...
		int iDirection = 0;
		Vector2f vRight(0.f, -1.f);
		float fAngle = vRight.angle(_vMotion);
		if (_vMotion.getX() > 0)
		{
			iDirection = (int)(0.5f + fAngle * 4 / 3.14f);
			setDirection((EnumCharacterDirection)iDirection);
		}
		else
		{
			iDirection = 8 - (int)(0.5f + fAngle * 4 / 3.14f);
			iDirection = (iDirection == 8) ? 0 : iDirection;
			setDirection((EnumCharacterDirection)iDirection);
		}
	}
}

bool CharacterController::init()
{
	m_pActionTable = getEntity()->getComponent<crea::ActionTable>();
	m_pAnimator = getEntity()->getComponent<crea::Animator>();
	m_pCollider = getEntity()->getComponent<Collider>();

	m_bAlive = true;
	m_bMoving = false;
	m_eDirection = kADir_Down;
	m_eCondition = kACond_Default;
	m_eAction = kAct_Default;
	m_pCurrentAnimation = m_pGM->getAnimation(*m_pActionTable->getAnimation(m_eDirection, m_eCondition, m_eAction, nullptr));

	m_fVelocityMax = 300.f;

	return true;
}

bool CharacterController::update()
{
	// Preconditions
	if (!m_pCollider || !m_pAnimator || !m_pActionTable)
	{
		return false;
	}

	m_pCurrentAnimation->setSpeed(1.0f); // Play full speed by default

	if (m_bMoving)
	{
		// Friction
		float fSpeedFactor = 1.f;
		Map* pCurrentMap = PhysicsManager::getSingleton()->getCurrentMap();
		if (pCurrentMap)
		{
			fSpeedFactor = 1 - pCurrentMap->getFrictionAtPosition(getEntity()->getPosition());
		}

		// Move
		// Limit to max velocity
		float fMotion = m_vMotion.length();
		if (fMotion > m_fVelocityMax)
		{
			m_vMotion.normalize();
			m_vMotion *= m_fVelocityMax; // TOR Speed controller (0 or SpeedMax)
		}
		Vector2f vAdjustedMotion = m_vMotion * fSpeedFactor;
		m_pEntity->move(vAdjustedMotion * (float)crea::TimeManager::getSingleton()->getFrameTime().asSeconds());
		
		// Collision
		if (PhysicsManager::getSingleton()->isColliding(m_pCollider))
		{
			// Revert move
			//m_pEntity->move(-vAdjustedMotion * (float)crea::TimeManager::getSingleton()->getFrameTime().asSeconds());
			//m_vMotion = vAdjustedMotion = Vector2f(0.f, 0.f);
			//m_eAction = kAct_Idle;
		}
		// Adjust anim speed to velocity
		m_pCurrentAnimation->adjustToTranslationSpeed(vAdjustedMotion.length());
	}

	// Play animation
	m_pCurrentAnimation = m_pGM->getAnimation(*m_pActionTable->getAnimation(m_eDirection, m_eCondition, m_eAction));
	m_pAnimator->play(*m_pCurrentAnimation);

	// update AnimatedSprite
	//m_pAnimator->update();

	return true;
}

bool CharacterController::draw()
{
	return true;
}

bool CharacterController::quit()
{
	return true;
}
