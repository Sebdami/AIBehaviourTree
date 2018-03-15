#include "stdafx.h"

#include "Scene\SceneSteering.h"
#include "Scene\SceneMenu.h"
#include "Scene\SceneGame.h"
#include "Scene\SceneMap.h"
#include "AI\Steering\Behaviour.h"


SceneSteering::SceneSteering()
{
	// AI Tools
	m_bUseAITools = true;
	m_pAITools = new AITools();
}

SceneSteering::~SceneSteering()
{
	// AI Tools
	delete m_pAITools;
}

bool SceneSteering::onInit()
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
	m_iNbEntities = 2;
	m_bKeyPressedAdd = false;
	m_bKeyPressedSub = false;

	// Steering mode
	m_pTextSteeringMode = m_pGM->getText("steering");
	m_pTextSteeringMode->setFont(m_pGM->getFont("arial.ttf"));
	m_pTextSteeringMode->setColor(pRed);
	m_pTextSteeringMode->setCharacterSize(20);
	m_pTextSteeringMode->setString("steering mode");
	TextRenderer* pTextRendererSteering = m_pGM->getTextRenderer("TextRenderer steering");
	pTextRendererSteering->setText(m_pTextSteeringMode);
	Entity* pEntitySteering = m_pGM->getEntity("text steering mode");
	pEntitySteering->addComponent(pTextRendererSteering);
	pEntitySteering->setPosition(Vector2f(0, 0));
	m_pGM->addEntity(pEntitySteering);
	m_iSteeringMode = 0;
	m_pTextSteeringMode->setString("Seek/Flee");

	createEntities();
	setBehaviour();

	// FPS
	Time frameTime = TimeManager::getSingleton()->getFrameTime();
	m_pTextFPS->setString(to_string((int)(1 / frameTime.asSeconds())) + " fps");


	return true;
}

bool SceneSteering::onUpdate()
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
		m_iSteeringMode = 0;
		m_pTextSteeringMode->setString("Seek/Flee");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad1))
	{
		m_iSteeringMode = 1;
		m_pTextSteeringMode->setString("Pursuit/Evasion");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad2))
	{
		m_iSteeringMode = 2;
		m_pTextSteeringMode->setString("Arrival/Obstacle Avoidance");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad3))
	{
		m_iSteeringMode = 3;
		m_pTextSteeringMode->setString("Wander");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad4))
	{
		m_iSteeringMode = 4;
		m_pTextSteeringMode->setString("Pathfinding");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad5))
	{
		m_iSteeringMode = 5;
		m_pTextSteeringMode->setString("Unaligned Collision Avoidance");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad6))
	{
		m_iSteeringMode = 6;
		m_pTextSteeringMode->setString("Flocking");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad7))
	{
		m_iSteeringMode = 7;
		m_pTextSteeringMode->setString("Lead Following");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Numpad8))
	{
		m_iSteeringMode = 8;
		m_pTextSteeringMode->setString("Swarming");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::A))
	{
		m_iSteeringMode = 10;
		m_pTextSteeringMode->setString("Formation V");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Z))
	{
		m_iSteeringMode = 11;
		m_pTextSteeringMode->setString("Formation L");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::E))
	{
		m_iSteeringMode = 12;
		m_pTextSteeringMode->setString("Formation Circle");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::R))
	{
		m_iSteeringMode = 13;
		m_pTextSteeringMode->setString("Formation 2-Level");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::T))
	{
		m_iSteeringMode = 14;
		m_pTextSteeringMode->setString("Formation of formation");
		setBehaviour();
	}
	if (m_pGM->isKeyPressed(Key::Y))
	{
		m_iSteeringMode = 15;
		m_pTextSteeringMode->setString("Dynamic formation");
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

bool SceneSteering::onDraw()
{
	// AI Tools
	if (m_bUseAITools)
	{
		m_pAITools->onDraw();
	}

	return true;
}

bool SceneSteering::onQuit()
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

void SceneSteering::deleteEntities()
{
	for (int i = 0; i < m_iNbEntities; i++)
	{
		m_pGM->clearEntity(m_vEntities[i]);
	}
	m_vEntities.clear();
}

void SceneSteering::createEntities()
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

void SceneSteering::setBehaviour()
{
	for (int i = 0; i < m_iNbEntities; i++)
	{
		std::string s = std::to_string(i);
		Steering* pSteering = m_vEntities[i]->getComponent<Steering>();
		pSteering->init();
		pSteering->clearBehaviours();
		// Behaviour
		switch (m_iSteeringMode)
		{
		case 0: 
			if (i % 2 == 0)	pSteering->addBehaviour(new Seek(m_vEntities[i], m_pMouse), 1.0f);
			else pSteering->addBehaviour(new Flee(m_vEntities[i], m_pMouse), 1.0f);
			break;
		case 1: 
			if (i % 2 == 0)	pSteering->addBehaviour(new Pursuit(m_vEntities[i], m_pMouse, 1.0f), 1.0f);
			else pSteering->addBehaviour(new Evasion(m_vEntities[i], m_pMouse, 1.f), 1.0f);
			break;
		case 2: pSteering->addBehaviour(new Arrival(m_vEntities[i], m_pMouse, 200.0f), 1.0f);
			pSteering->addBehaviour(new ObstacleAvoidance(m_vEntities[i], 32.f, 100.f, &m_vObstacles), 1.0f);
			break;
		case 3: pSteering->addBehaviour(new Wander(m_vEntities[i], 100.f, 50.f, 10.0f), 1.0f);
			break;
		case 4: pSteering->addBehaviour(new PathFollowing(m_vEntities[i], m_pTarget, 40.f, &m_vPath), 1.0f);
			break;
		case 5: pSteering->addBehaviour(new UnalignedCollisionAvoidance(m_vEntities[i], 60.f, 1.0f, &m_vEntities), 1.0f);
			break;
		case 6:
			pSteering->addBehaviour(new Separation(m_vEntities[i], 60.f, &m_vEntities), 1.0f);
			pSteering->addBehaviour(new Cohesion(m_vEntities[i], 60.f, &m_vEntities), 1.0f);
			pSteering->addBehaviour(new Alignment(m_vEntities[i], 60.f, &m_vEntities), 1.0f);
			break;
		case 7:
			if (i == 0)
			{
				pSteering->addBehaviour(new Arrival(m_vEntities[0], m_pMouse, 200.f), 1.0f);
			}
			else
			{
				pSteering->addBehaviour(new Separation(m_vEntities[i], 60.f, &m_vEntities), 2.0f);
				pSteering->addBehaviour(new LeadFollowing(m_vEntities[i], m_vEntities[0], 180.f, 1.57f, 80.f, 50.f), 1.0f);
			}
			break;
		case 8:
			pSteering->addBehaviour(new Swarming(m_vEntities[i], i, m_pMouse, 100.f), 2.0f);
			break;
		case 10: pSteering->addBehaviour(new FormationV(m_vEntities[i], m_pMouse, true, 10, i, m_iNbEntities, 60.0f, 100.0f, MathTools::degreetoradian(45.0f)), 1.0f);
			break;
		case 11: pSteering->addBehaviour(new FormationV(m_vEntities[i], m_pMouse, false, 10, i, m_iNbEntities, 60.0f, 100.0f, 0.0f), 1.0f);
			break;
		case 12: pSteering->addBehaviour(new FormationCircle(m_vEntities[i], m_pMouse, false, 10, i, m_iNbEntities, 60.0f, 100.0f, -180.0f, 180.0f, 60.0f), 1.0f);
			break;
		case 13:
			pSteering->addBehaviour(new FormationV(m_vEntities[i], m_pMouse, true, 10, i, m_iNbEntities, 60.0f, 100.0f, MathTools::degreetoradian(45.0f)), 1.0f);
			pSteering->addBehaviour(new ObstacleAvoidance(m_vEntities[i], 12, 100, &m_vObstacles), 8.0f);
			break;
		case 14:
			if (i >= 23)
			{
				pSteering->addBehaviour(new FormationV(m_vEntities[i], m_vEntities[1], false, 10, i - 23, m_iNbEntities - 23, 60.0f, 100.0f, MathTools::degreetoradian(0.0f)), 1.f);
			}
			else if (i >= 13)
			{
				pSteering->addBehaviour(new FormationCircle(m_vEntities[i], m_vEntities[2], false, 10, i - 13, 10, 60.0f, 100.0f, 0.0f, 360.0f, 60.0f), 1.f);
			}
			else if (i >= 3)
			{
				pSteering->addBehaviour(new FormationCircle(m_vEntities[i], m_vEntities[0], false, 10, i - 3, 10, 60.0f, 100.0f, 0.0f, 360.0f, 60.0f), 1.f);
			}
			else
			{
				pSteering->addBehaviour(new FormationV(m_vEntities[i], m_pMouse, false, 10, i, 3, 200.0f, 100.0f, MathTools::degreetoradian(-45.0f)), 1.f);
			}
			break;
		case 15:
			pSteering->addBehaviour(new FormationDynamic(m_vEntities[i], m_pMouse, false, 10, i, m_iNbEntities, 60.0f, 100.0f, -90.0f, 90.0f, 60.0f), 1.f);
			break;
		}
	}
}