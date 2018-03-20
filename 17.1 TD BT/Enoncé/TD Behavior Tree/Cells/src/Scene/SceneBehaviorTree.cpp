#include "stdafx.h"

#include "Scene\SceneBehaviorTree.h"
#include "Scene\SceneMenu.h"
#include "Scene\SceneGame.h"
#include "Scene\SceneMap.h"
#include "AI\Steering\Behaviour.h"
#include "Scripts\Actions.h"


SceneBehaviorTree::SceneBehaviorTree()
{
	// AI Tools
	m_bUseAITools = true;
	m_pAITools = new AITools();
}

SceneBehaviorTree::~SceneBehaviorTree()
{
	// AI Tools
	delete m_pAITools;
}

bool SceneBehaviorTree::onInit()
{
	m_pGM = GameManager::getSingleton();
	m_rWindowRect = m_pGM->getWindowRect();

	// Set Script factory
	m_pCellsScriptFactory = new CellsScriptFactory;
	m_pGM->setScriptFactory(m_pCellsScriptFactory);

	// Load Map
	m_pEntity3 = m_pGM->getEntity("MapSteering");
	m_pGM->addEntity(m_pEntity3);
	m_pMap = m_pGM->getMap("MapSteering.json"); // CB: TO CHANGE: Map id loaded after entity added to display Map first (order in tree matters)
	m_pMapRenderer = m_pGM->getMapRenderer("MapRenderer1");
	m_pMapRenderer->setMap(m_pMap);
	m_pEntity3->addComponent(m_pMapRenderer);

	// AI Tools
	if (m_bUseAITools)
	{
		m_pAITools->onInit();
	}

	// Mouse Entity
	m_pMouse = m_pGM->getEntity("mouse");
	// Add it to root
	m_pGM->addEntity(m_pMouse, nullptr);

	// Target Entity
	m_pTarget = m_pGM->getEntity("target");
	// Add it to root
	m_pGM->addEntity(m_pTarget, nullptr);

	// Text
	Color* pRed = m_pGM->getColor("Red");
	pRed->setValues(255, 0, 0, 255);
	m_pTextFPS = m_pGM->getText("fps");
	m_pTextFPS->setFont(m_pGM->getFont("arial.ttf"));
	m_pTextFPS->setColor(pRed);
	m_pTextFPS->setCharacterSize(20);
	m_pTextFPS->setString("fps");
	TextRenderer* pTextRenderer = m_pGM->getTextRenderer("TextRenderer1");
	pTextRenderer->setText(m_pTextFPS);
	Entity* pEntityFPS = m_pGM->getEntity("text 1");
	pEntityFPS->addComponent(pTextRenderer);
	pEntityFPS->setPosition(Vector2f(1100, 0));
	m_pGM->addEntity(pEntityFPS);

	Entity* pEntityObstacle = m_pGM->getEntity("plant1");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("plant2");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("plant3");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("rock1");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("cactus1");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("panel1");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("hq1");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());
	pEntityObstacle = m_pGM->getEntity("mine1");
	m_vObstacles.push_back(pEntityObstacle);
	m_vPath.push_back(&pEntityObstacle->getPosition());

	// n entity
	m_iNbEntities = 1;
	m_bKeyPressedAdd = false;
	m_bKeyPressedSub = false;

	// Steering mode
	m_pTextBTMode = m_pGM->getText("steering");
	m_pTextBTMode->setFont(m_pGM->getFont("arial.ttf"));
	m_pTextBTMode->setColor(pRed);
	m_pTextBTMode->setCharacterSize(20);
	m_pTextBTMode->setString("steering mode");
	TextRenderer* pTextRendererSteering = m_pGM->getTextRenderer("TextRenderer steering");
	pTextRendererSteering->setText(m_pTextBTMode);
	Entity* pEntitySteering = m_pGM->getEntity("text steering mode");
	pEntitySteering->addComponent(pTextRendererSteering);
	pEntitySteering->setPosition(Vector2f(0, 0));
	m_pGM->addEntity(pEntitySteering);
	m_iBTMode = 0;
	m_pTextBTMode->setString("Sequence: SeekTo / FleeUntil");

	createEntities();
	setBehaviour();

	// FPS
	Time frameTime = TimeManager::getSingleton()->getFrameTime();
	m_pTextFPS->setString(to_string((int)(1 / frameTime.asSeconds())) + " fps");


	return true;
}

bool SceneBehaviorTree::onUpdate()
{
	m_pMouse->setPosition(InputManager::getSingleton()->getMousePosition());

	// entities leaving the window
	for (int i = 0; i < m_iNbEntities; i++)
	{
		Entity* pEntity = m_vEntities[i];
		Vector2f vPos = pEntity->getPosition();
		if (vPos.getX() < 0.f)
		{
			vPos.setX(0.f);
			pEntity->setPosition(vPos);
		}
		else if (vPos.getX() > 900)
		{
			vPos.setX((float)900);
			pEntity->setPosition(vPos);
		}
		if (vPos.getY() < 0.f)
		{
			vPos.setY(0.f);
			pEntity->setPosition(vPos);
		}
		else if (vPos.getY() > m_rWindowRect.getHeight())
		{
			vPos.setY((float)m_rWindowRect.getHeight());
			pEntity->setPosition(vPos);
		}
	}

	if (m_pGM->isKeyPressed(Key::Num1))
	{
		m_pGM->setScene(new SceneMenu());
		return true;
	}
	if (m_pGM->isKeyPressed(Key::Num2))
	{
		m_pGM->setScene(new SceneGame());
		return true;
	}
	if (m_pGM->isKeyPressed(Key::Num3))
	{
		m_pGM->setScene(new SceneMap());
		return true;
	}
	// Steering mode
	if (m_pGM->isKeyPressed(Key::Numpad0))
	{
		m_iBTMode = 0;
		m_pTextBTMode->setString("Sequence: SeekTo -> FleeUntil");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad1))
	{
		m_iBTMode = 1;
		m_pTextBTMode->setString("Sequence: SeekTo -> FleeUntil -> Wander");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad2))
	{
		m_iBTMode = 2;
		m_pTextBTMode->setString("Sequence: IsAwayFromTarget / SeekTo -> FleeUntil -> Wander");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad3))
	{
		m_iBTMode = 3;
		m_pTextBTMode->setString("Selector: IsAwayFromTarget / Selector / SeekTo -> FleeUntil OR Wander");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad4))
	{
		m_iBTMode = 4;
		m_pTextBTMode->setString("Repeat: 3 Sequence / SeekTo / FleeUntil THEN Wander");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad5))
	{
		m_iBTMode = 5;
		m_pTextBTMode->setString("Timer 3s on Wander THEN SeekTo");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad6))
	{
		m_iBTMode = 6;
		m_pTextBTMode->setString("Wander if EntityNameIs ...");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad6))
	{
		m_iBTMode = 7;
		m_pTextBTMode->setString("Scenario individuel");
		setBehaviour();
	}

	// Entities
	if (m_pGM->isKeyPressed(Key::Add))
	{
		if (!m_bKeyPressedAdd && m_iNbEntities < 100)
		{
			m_pGM->unselectEntities();
			deleteEntities();
			m_iNbEntities += 10;
			createEntities();
			setBehaviour();
		}
		m_bKeyPressedAdd = true;
	}
	else
	{
		m_bKeyPressedAdd = false;
	}

	if (m_pGM->isKeyPressed(Key::Subtract))
	{
		if (!m_bKeyPressedSub && m_iNbEntities > 10)
		{
			m_pGM->unselectEntities();
			deleteEntities();
			m_iNbEntities -= 10;
			createEntities();
			setBehaviour();
		}
		m_bKeyPressedSub = true;
	}
	else
	{
		m_bKeyPressedSub = false;
	}

	// AI Tools
	if (m_bUseAITools)
	{
		m_pAITools->onUpdate();
	}

	return true;
}

bool SceneBehaviorTree::onDraw()
{
	// AI Tools
	if (m_bUseAITools)
	{
		m_pAITools->onDraw();
	}

	return true;
}

bool SceneBehaviorTree::onQuit()
{
	// AI Tools
	if (m_bUseAITools)
	{
		m_pAITools->onQuit();
	}

	m_pGM->clearAllData();
	m_pGM->clearAllEntities();

	delete m_pCellsScriptFactory;

	return true;
}

void SceneBehaviorTree::deleteEntities()
{
	for (int i = 0; i < m_iNbEntities; i++)
	{
		m_pGM->clearEntity(m_vEntities[i]);
	}
	m_vEntities.clear();
}

void SceneBehaviorTree::createEntities()
{
	Entity* pEntityBase = m_pGM->getEntity("peonSteering");
	Entity* pEntityCell1 = nullptr;
	for (int i = 0; i < m_iNbEntities; i++)
	{
		std::string s = std::to_string(i);

		pEntityCell1 = m_pGM->instanciate(s, pEntityBase);
		m_pGM->addEntity(pEntityCell1);
		m_vEntities.push_back(pEntityCell1); // Keep pointers on entities

		// Position random
		int x = rand() % 900;
		int y = rand() % m_rWindowRect.getHeight();
		pEntityCell1->setPosition(Vector2f((float)x, (float)y));
	}
}

void SceneBehaviorTree::setBehaviour()
{
	for (int i = 0; i < m_iNbEntities; i++)
	{
		std::string s = std::to_string(i);
		Steering* pSteering = m_vEntities[i]->getComponent<Steering>();
		pSteering->init();
		pSteering->clearBehaviours();

		BehaviorTree* pBT = m_vEntities[i]->getComponent<BehaviorTree>();
		if (pBT)
		{
			m_vEntities[i]->removeComponent(pBT);
		}
		
		// Behavior Tree modes
		switch (m_iBTMode)
		{
		case 0: 
		{
			Seek* pSeek = new Seek(m_vEntities[i], m_pMouse);
			ActionSeekTo* pSeekTo = new ActionSeekTo(pSeek, 10.0f);
			Flee* pFlee = new Flee(m_vEntities[i], m_pMouse);
			ActionFleeUntil* pFleeUntil = new ActionFleeUntil(pFlee, 300.0f);
			Sequence* pSeq = new Sequence;
			pSeq->addChild(pSeekTo);
			pSeq->addChild(pFleeUntil);
			pBT = new BehaviorTree();
			pBT->setRootBehavior(pSeq);
			m_vEntities[i]->addComponent(pBT);
		}
			break;
		case 1: 
		{
			Seek* pSeek = new Seek(m_vEntities[i], m_pMouse);
			ActionSeekTo* pSeekTo = new ActionSeekTo(pSeek, 10.0f);
			Flee* pFlee = new Flee(m_vEntities[i], m_pMouse);
			ActionFleeUntil* pFleeUntil = new ActionFleeUntil(pFlee, 300.0f);
			Wander* pWander = new Wander(m_vEntities[i], 200.0f, 100.0f, 10.0f);
			ActionWander* pWanderAction = new ActionWander(pWander, 2.0f);
			Sequence* pSeq = new Sequence;
			pSeq->addChild(pSeekTo);
			pSeq->addChild(pFleeUntil);
			pSeq->addChild(pWanderAction);
			pBT = new BehaviorTree();
			pBT->setRootBehavior(pSeq);
			m_vEntities[i]->addComponent(pBT);
		}
			break;
		case 2: 
			break;
		case 3: 
			break;
		case 4: 
			break;
		case 5: 
			break;
		case 6:
			break;
		case 7:
			break;
		}
	}
}