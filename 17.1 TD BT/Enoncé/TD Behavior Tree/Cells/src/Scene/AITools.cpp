#include "stdafx.h"

#include "Scene\AITools.h"
#include "Scene\SceneMenu.h"
#include "Scene\SceneMap.h"
#include "Core\SceneManager.h"
#include "Core\PhysicsManager.h"
#include "Scripts\Messages.h"
#include <string>

AITools::AITools()
{

}

AITools::~AITools()
{

}
	
bool AITools::onInit()
{
	m_pGM = GameManager::getSingleton();

	Color* pRed = m_pGM->getColor("Red");
	pRed->setValues(255, 0, 0, 255);
	Color* pBlue = m_pGM->getColor("Blue");
	pBlue->setValues(0, 0, 255, 255);
	Color* pBlack = m_pGM->getColor("Black");
	pBlack->setValues(0, 0, 0, 255);
	Color* pTransparent = m_pGM->getColor("Transparent");
	pTransparent->setValues(0, 0, 0, 0);
	Color* pRedTransparent = m_pGM->getColor("RedTransparent");
	pRedTransparent->setValues(255, 0, 0, 125);
	Color* pGreenTransparent = m_pGM->getColor("GreenTransparent");
	pGreenTransparent->setValues(0, 255, 0, 255);

	// Selection
	m_bSelection = false;
	m_pSelectionShape = new RectangleShape;
	m_pSelectionShape->setOutlineColor(pBlue);
	m_pSelectionShape->setColor(pTransparent);
	m_pSelectionShape->setOutlineThickness(1.f);

	// Commands
	Texture* pCommandsTexture = m_pGM->getTexture("debug/Commands.png");
	m_pCommandsSprite = m_pGM->getSprite("Commands");
	m_pCommandsSprite->setTexture(pCommandsTexture);

	m_rCommandWindow = FloatRect(896, 640, 256, 256);
	m_pCommandsSprite->setPosition(m_rCommandWindow.m_fX, m_rCommandWindow.m_fY);

	// Diagnostics
	m_pTextFPS = m_pGM->getText("fps");
	m_pTextFPS->setFont(m_pGM->getFont("arial.ttf"));
	m_pTextFPS->setColor(pRed);
	m_pTextFPS->setCharacterSize(14);
	m_pTextFPS->setString("fps:");
	m_pTextFPS->setPosition(900, 0);

	m_pTextCommand = m_pGM->getText("Command");
	m_pTextCommand->setFont(m_pGM->getFont("arial.ttf"));
	m_pTextCommand->setColor(pRed);
	m_pTextCommand->setCharacterSize(14);
	m_pTextCommand->setString("Last command:");
	m_pTextCommand->setPosition(900, 20);

	m_pTextDiagnostics = m_pGM->getText("Diagnostics");
	m_pTextDiagnostics->setFont(m_pGM->getFont("arial.ttf"));
	m_pTextDiagnostics->setColor(pBlue);
	m_pTextDiagnostics->setCharacterSize(14);
	m_pTextDiagnostics->setPosition(-100.f, -100.f);

	m_fCommandDisplayTime = 1.0f;

	// Grid
	m_pMap = m_pGM->getMapRenderer("MapRenderer1")->getMap();
	m_pMap->getSize(m_nWidth, m_nHeight);
	m_pMap->getTileSize(m_nTileWidth, m_nTileHeight);
	m_pMap->getTileIndexLimits(m_iMin, m_iMax, m_jMin, m_jMax);
	m_pNodeShape = new RectangleShape;
	m_pNodeShape->setOutlineColor(pBlack);
	m_pNodeShape->setColor(pTransparent);
	m_pNodeShape->setOutlineThickness(0.5f); 
	m_pNodeShape->setSize(m_nTileWidth, m_nTileHeight);

	// CharacterController
	//m_pCharacterController = (CharacterController*) m_pGM->getScript("CharacterController");
	m_pCharacterController = nullptr;

	// Collisions
	m_pBoxColliderShape = new RectangleShape;
	m_pBoxColliderShape->setOutlineColor(pRed);
	m_pBoxColliderShape->setColor(pTransparent);
	m_pBoxColliderShape->setOutlineThickness(1.0f);
	m_pCircleColliderShape = new CircleShape;
	m_pCircleColliderShape->setOutlineColor(pRed);
	m_pCircleColliderShape->setColor(pTransparent);
	m_pCircleColliderShape->setOutlineThickness(1.0f);
	m_pCollisionNodeShape = new RectangleShape;
	m_pCollisionNodeShape->setColor(pRedTransparent);

	// Cluster
	m_pClusterShape = new RectangleShape;
	m_pClusterShape->setOutlineColor(pBlue);
	m_pClusterShape->setColor(pTransparent);
	m_pClusterShape->setOutlineThickness(2.0f);
	m_pMap->getClusterSize(m_nClusterWidth, m_nClusterHeight);
	m_pClusterShape->setSize(m_nClusterWidth, m_nClusterHeight);
	m_pMap->getClusterIndexLimits(m_iClusterMin, m_iClusterMax, m_jClusterMin, m_jClusterMax);

	m_pEntranceShape = new RectangleShape;
	m_pEntranceShape->setOutlineColor(pBlue);
	m_pEntranceShape->setColor(pBlue);
	m_pEntranceShape->setSize(m_nTileWidth, m_nTileHeight);

	m_pTransitionShape = new ArrowShape;
	m_pTransitionShape->setOutlineColor(pGreenTransparent);
	m_pTransitionShape->setColor(pGreenTransparent);
	m_pTransitionShape->setSize(m_nTileWidth, m_nTileHeight*0.5f);

	// Steering
	m_pTargetShape = new ArrowShape;
	m_pTargetShape->setOutlineColor(pBlue);
	m_pTargetShape->setColor(pBlue);
	m_pTargetShape->setSize(m_nTileWidth, m_nTileHeight*0.5f);

	//m_pTarget = m_pGM->getEntity("target");
	m_pTargetSprite = new Sprite;
	m_pTargetSprite->setTexture(m_pGM->getSingleton()->getTexture("/debug/target.png"));
	m_pTargetSprite->setOrigin(32.f, 32.f);


	return true;
}

bool AITools::isButton(int _i, Vector2f& _vMousePosition)
{
	int i = _i % 5;
	int j = _i / 5;
	FloatRect r(m_rCommandWindow.m_fX + i * 46, m_rCommandWindow.m_fY + j * 38, 46.f, 38.f);
	if (r.contains(_vMousePosition))
	{
		return true;
	}
	return false;
}

bool AITools::onUpdate()
{
	// FPS
	Time frameTime = TimeManager::getSingleton()->getFrameTime();
	m_pTextFPS->setString(to_string((int)(1 / frameTime.asSeconds())) + " fps");

	// Selection
	if (m_pGM->isMouseButtonPressed(Button::MouseLeft))
	{
		if (!m_bSelection) // just pressed
		{
			m_vStartSelection = m_pGM->getMousePosition();
		}
		m_bSelection = true;
	}
	else
	{
		if (m_bSelection) // just released
		{
			m_vEndSelection = m_pGM->getMousePosition();
			m_pGM->unselectEntities();
			m_pGM->selectEntities(m_vStartSelection, m_vEndSelection);
		}
		m_bSelection = false;
	}

	// Command
	if (m_pGM->isMouseButtonPressed(Button::MouseRight))
	{
		if (!m_bCommand) // just pressed
		{
			m_eCommandType = Command_Invalid;
			Vector2f vMousePosition = m_pGM->getMousePosition();
			if (m_rCommandWindow.contains(vMousePosition))
			{
				if (isButton(0, vMousePosition))
				{
					m_eCommandType = Command_Reset;
				}
				else if (isButton(1, vMousePosition))
				{
					m_eCommandType = Command_Kill;
				}
				else if (isButton(2, vMousePosition))
				{
					m_eCommandType = Command_Stop;
				}
				else if (isButton(3, vMousePosition))
				{
					m_eCommandType = Command_GoToHQ;
				}
				else if (isButton(4, vMousePosition))
				{
					m_eCommandType = Command_GoTo;
				}
				else if (isButton(5, vMousePosition))
				{
					m_eCommandType = Command_Build;
				}
				else if (isButton(6, vMousePosition))
				{
					m_eCommandType = Command_Mine;
				}
				else if (isButton(7, vMousePosition))
				{
					m_eCommandType = Command_Harvest;
				}
				else if (isButton(8, vMousePosition))
				{
					m_eCommandType = Command_Die;
				}
				else if (isButton(9, vMousePosition))
				{
					m_eCommandType = Command_Boost;
				}
				else if (isButton(10, vMousePosition))
				{
					m_eCommandType = Command_GoToHQWithLumber;
				}
				else if (isButton(11, vMousePosition))
				{
					m_eCommandType = Command_Seek;
				}
				else if (isButton(12, vMousePosition))
				{
					m_eCommandType = Command_Flee;
				}
				else if (isButton(13, vMousePosition))
				{
					m_eCommandType = Command_Pursuit;
				}
				else if (isButton(14, vMousePosition))
				{
					m_eCommandType = Command_Evasion;
				}
				else if (isButton(15, vMousePosition))
				{
					m_eCommandType = Command_Arrival;
				}
				else if (isButton(16, vMousePosition))
				{
					m_eCommandType = Command_Wander;
				}
				else if (isButton(17, vMousePosition))
				{
					m_eCommandType = Command_PathFollowing;
				}
				else if (isButton(18, vMousePosition))
				{
					m_eCommandType = Command_UCA;
				}
			}
		}
		m_bCommand = true;
	}
	else
	{
		if (m_bCommand) // just released
		{
			// Restart the command display timer
			m_CommandDisplayClock.restart();

			// Do the command
			ListEntity* pSelectedEntities = m_pGM->getSelectedEntities();
			for (ListEntity::iterator i = pSelectedEntities->begin(); i != pSelectedEntities->end(); ++i)
			{
				Entity* pEntity = (Entity*)*i;

				// CharacterController
				m_pCharacterController = pEntity->getComponent<CharacterController>();

				if (m_eCommandType == Command_Reset)
				{
					m_pTextCommand->setString("Reset");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Reset, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Kill)
				{
					m_pTextCommand->setString("Kill");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Kill, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Stop)
				{
					m_pTextCommand->setString("Stop");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Stop, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_GoToHQ)
				{
					m_pTextCommand->setString("GoToWithGold");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_GoToWithGold, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_GoTo)
				{
					m_pTextCommand->setString("GoTo");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_GoTo, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Build)
				{
					m_pTextCommand->setString("Build");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Build, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Mine)
				{
					m_pTextCommand->setString("Mine");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Mine, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Harvest)
				{
					m_pTextCommand->setString("Harvest");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Harvest, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Die)
				{
					m_pTextCommand->setString("Die");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Hit, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Boost)
				{
					m_pTextCommand->setString("Boost");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Boost, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_GoToHQWithLumber)
				{
					m_pTextCommand->setString("GoToWithLumber");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_GoToWithLumber, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Seek)
				{
					m_pTextCommand->setString("Seek");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Seek, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Flee)
				{
					m_pTextCommand->setString("Flee");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Flee, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Pursuit)
				{
					m_pTextCommand->setString("Pursuit");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Pursuit, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Evasion)
				{
					m_pTextCommand->setString("Evasion");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Evasion, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Arrival)
				{
					m_pTextCommand->setString("Arrival");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Arrival, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_Wander)
				{
					m_pTextCommand->setString("Wander");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_Wander, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_PathFollowing)
				{
					m_pTextCommand->setString("PathFollowing");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_PathFollowing, 0, pEntity->GetID(), -1);
				}
				else if (m_eCommandType == Command_UCA)
				{
					m_pTextCommand->setString("UCA");
					MsgManager::getSingleton()->sendMsg(0.f, MSG_UCA, 0, pEntity->GetID(), -1);
				}
				else
				{
					m_pTextCommand->setString("?");
				}
			}

			m_eCommandType = Command_Invalid;
		}
		m_bCommand = false;
	}

	return true;
}

bool AITools::onDraw()
{
	// Selection
	if (m_pGM->isMouseButtonPressed(Button::MouseLeft))
	{
		Vector2f vMousePosition = m_pGM->getMousePosition();
		m_pSelectionShape->setPosition(m_vStartSelection.getX(), m_vStartSelection.getY());
		m_pSelectionShape->setSize(vMousePosition.getX() - m_vStartSelection.getX(), vMousePosition.getY() - m_vStartSelection.getY());
		m_pSelectionShape->draw();
	}

	// Diagnostics
	ListEntity* pSelectedEntities = m_pGM->getSelectedEntities();
	for (ListEntity::iterator i = pSelectedEntities->begin(); i != pSelectedEntities->end(); ++i)
	{
		Entity* pEntity = (Entity*)*i;
		Vector2f position = pEntity->getPosition();
		m_pTextDiagnostics->setPosition(position.getX() + 36, position.getY() - 50);
		string szDiagnostics;
		szDiagnostics += pEntity->getName();
		Agent* pAgent = pEntity->getComponent<Agent>();
		if (pAgent)
		{
			// Stats
			szDiagnostics += "\nHea: ";
			szDiagnostics += to_string(pAgent->getHealth());
			szDiagnostics += " Str: ";
			szDiagnostics += to_string(pAgent->getStrength());
			szDiagnostics += " Int: ";
			szDiagnostics += to_string(pAgent->getIntelligence());
			szDiagnostics += " Dex: ";
			szDiagnostics += to_string(pAgent->getDexterity());

		}

		// CharacterController
		m_pCharacterController = pEntity->getComponent<CharacterController>();
		if (m_pCharacterController)
		{
			szDiagnostics += "\nAct°: ";
			szDiagnostics += to_string(m_pCharacterController->getAction());
			szDiagnostics += " Condit°: ";
			szDiagnostics += to_string(m_pCharacterController->getCondition());
			szDiagnostics += " Direct°: ";
			szDiagnostics += to_string(m_pCharacterController->getDirection());
		}

		// Steering
		m_pSteering = pEntity->getComponent<Steering>();
		if (m_pSteering)
		{
			szDiagnostics += "\nSteering: ";
			szDiagnostics += m_pSteering->asString();
		}

		m_pTextDiagnostics->setString(szDiagnostics);
		m_pTextDiagnostics->draw();

		// Steering
		if (m_pSteering)
		{
			bool bDisplayTarget = szDiagnostics.find("Seek") != string::npos 
				|| szDiagnostics.find("Pursuit") != string::npos
				|| szDiagnostics.find("Arrival") != string::npos;
			Entity* pTarget = m_pSteering->getTarget();
			if (pTarget /*&& bDisplayTarget*/)
			{
				Vector2f vEntity = pEntity->getPosition();
				Vector2f vTarget = pTarget->getPosition() + m_pSteering->getTargetOffset();

				m_pTransitionShape->setStartAndEnd(
					vEntity.getX(), vEntity.getY(),
					vTarget.getX(), vTarget.getY()
				);
				m_pTransitionShape->draw();
			}

		}

		Vector2f v = pEntity->getFuturePosition(1.0f);
		m_pTargetSprite->setPosition(v.getX(), v.getY());
		m_pTargetSprite->draw();

	}

	// Commands
	if (!pSelectedEntities->empty())
	{
		m_pCommandsSprite->draw();
	}

	// FPS
	m_pTextFPS->draw();

	if (m_CommandDisplayClock.getElapsedTime().asSeconds() < m_fCommandDisplayTime)
	{
		m_pTextCommand->draw();
	}

	// Grid
	for (short i = m_iMin; i <= m_iMax; i++)
	{
		for (short j = m_jMin; j <= m_jMax; j++)
		{
			if (m_pMap->getNode(i, j)->getTileCollisionId() == 9)
			{
				m_pCollisionNodeShape->setPosition((float)i*m_nTileWidth, (float)j*m_nTileHeight);
				m_pCollisionNodeShape->setSize((float)m_nTileWidth, (float)m_nTileHeight);
				m_pCollisionNodeShape->draw();
			}
			else
			{
				m_pNodeShape->setPosition((float)i*m_nTileWidth, (float)j*m_nTileHeight);
				m_pNodeShape->draw();
			}
		}
	}

	// Collisions
	MapStringCollider* pStaticColliders = PhysicsManager::getSingleton()->getStaticColliders();
	DisplayCollider(pStaticColliders);
	MapStringCollider* pDynamicColliders = PhysicsManager::getSingleton()->getDynamicColliders();
	DisplayCollider(pDynamicColliders);
	/*
	// Cluster
	short nVariableClusterWidth, nVariableClusterHeigth;
	for (short i = m_iClusterMin; i < m_iClusterMax; i++)
	{
		for (short j = m_jClusterMin; j < m_jClusterMax; j++)
		{
			Cluster* pCluster = m_pMap->getCluster(i, j);
			pCluster->getSize(nVariableClusterWidth, nVariableClusterHeigth);
			m_pClusterShape->setPosition((float)i*m_nClusterWidth*m_nTileWidth, (float)j*m_nClusterHeight*m_nTileHeight);
			m_pClusterShape->setSize((float)nVariableClusterWidth*m_nTileWidth, (float)nVariableClusterHeigth*m_nTileHeight);
			m_pClusterShape->draw();

			// Entrances
			vector<Entrance*>* vEntrances = pCluster->getEntrances();
			for (unsigned int e = 0; e < vEntrances->size(); e++)
			{
				Entrance* pEntrance = (*vEntrances)[e];
				m_pEntranceShape->setPosition((float)pEntrance->m_pStart->getX()*m_nTileWidth, (float)pEntrance->m_pStart->getY()*m_nTileHeight);
				m_pEntranceShape->draw();
				m_pEntranceShape->setPosition((float)pEntrance->m_pEnd->getX()*m_nTileWidth, (float)pEntrance->m_pEnd->getY()*m_nTileHeight);
				m_pEntranceShape->draw();
			}
		}
	}

	for (short i = m_iClusterMin; i < m_iClusterMax; i++)
	{
		for (short j = m_jClusterMin; j < m_jClusterMax; j++)
		{
			Cluster* pCluster = m_pMap->getCluster(i, j);
			// Transitions
			vector<Transition*>* vTransitions = pCluster->getTransitions();
			for (unsigned int t = 0; t < vTransitions->size(); t++)
			{
				Transition* pTransition = (*vTransitions)[t];
				m_pTransitionShape->setStartAndEnd(
					(float)(pTransition->m_pStart->getX() + 0.5f)*m_nTileWidth, (float)(pTransition->m_pStart->getY() + 0.5f)*m_nTileWidth,
					(float)(pTransition->m_pEnd->getX() + 0.5f)*m_nTileHeight, (float)(pTransition->m_pEnd->getY() + 0.5f)*m_nTileHeight
				);
				m_pTransitionShape->draw();
			}
			// Edges
			vector<Edge*>* vEdges = pCluster->getEdges();
			for (unsigned int t = 0; t < vEdges->size(); t++)
			{
				Edge* pEdge = (*vEdges)[t];
				m_pTransitionShape->setStartAndEnd(
					(float)(pEdge->m_pStart->getX() + 0.5f)*m_nTileWidth, (float)(pEdge->m_pStart->getY() + 0.5f)*m_nTileWidth,
					(float)(pEdge->m_pEnd->getX() + 0.5f)*m_nTileHeight, (float)(pEdge->m_pEnd->getY() + 0.5f)*m_nTileHeight
				);
				m_pTransitionShape->draw();
			}
		}
	}
	*/


	return true;
}

bool AITools::onQuit()
{
	delete m_pSelectionShape;
	delete m_pNodeShape;

	return true;
}

void AITools::DisplayCollider(MapStringCollider* _pColliders)
{
	for (MapStringCollider::iterator it = _pColliders->begin(); it != _pColliders->end(); ++it)
	{
		Collider* pCollider = (Collider*)(it->second);
		EnumColliderType type = pCollider->getColliderType();
		if (type == Collider_Box)
		{
			BoxCollider* pBoxCollider = (BoxCollider*)pCollider;
			Vector2f position = pBoxCollider->getOrigin();
			Vector2f size = pBoxCollider->getSize();
			m_pBoxColliderShape->setPosition(position.getX(), position.getY());
			m_pBoxColliderShape->setSize(size.getX(), size.getY());
			m_pBoxColliderShape->draw();

		}
		else if (type == Collider_Circle)
		{
			CircleCollider* pCircleCollider = (CircleCollider*)pCollider;
			Vector2f position = pCircleCollider->getWorldCenter();
			float radius = pCircleCollider->getRadius();
			m_pCircleColliderShape->setPosition(position.getX() - radius, position.getY() - radius);
			m_pCircleColliderShape->setRadius(radius);
			m_pCircleColliderShape->draw();
		}
	}
}