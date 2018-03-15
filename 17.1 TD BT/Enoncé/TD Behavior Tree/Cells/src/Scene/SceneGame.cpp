#include "stdafx.h"

#include "Scene\SceneGame.h"
#include "Scene\SceneMenu.h"
#include "Core\SceneManager.h"
#include "Scene\SceneMap.h"
#include "Scene\SceneBehaviorTree.h"

SceneGame::SceneGame()
{

}

SceneGame::~SceneGame()
{

}
	
bool SceneGame::onInit()
{
	m_pGM = GameManager::getSingleton();

	// Sprite
	m_pSprite1 = new Sprite();
	m_pSprite1->setTexture(m_pGM->getTexture("image.png"));
	m_pSprite1->setPosition(300, 150);

	// SpriteRenderer
	m_pSpriteRenderer = new SpriteRenderer();
	m_pSpriteRenderer->setSprite(m_pSprite1);

	// Entity
	m_pEntity1 = new Entity();
	m_pEntity1->setName(string("sprite 1"));
	m_pEntity1->addComponent(m_pSpriteRenderer);

	m_pGM->addEntity(m_pEntity1);

	
	return true;
}

bool SceneGame::onUpdate()
{
	if (m_pGM->isKeyPressed(Key::Num1))
	{
		m_pGM->setScene(new SceneMenu());
		return true;
	}
	if (m_pGM->isKeyPressed(Key::Num3))
	{
		m_pGM->setScene(new SceneMap());
		return true;
	}
	if (m_pGM->isKeyPressed(Key::Num4))
	{
		m_pGM->setScene(new SceneBehaviorTree());
		return true;
	}

	return true;
}

bool SceneGame::onDraw()
{
	return true;
}

bool SceneGame::onQuit()
{
	m_pGM->clearAllEntities();

	return true;
}

