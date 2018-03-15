/***************************************************/
/* Nom:	SceneBehaviorTree.h
/* Description: SceneBehaviorTree
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __SceneBehaviorTree_H_
#define __SceneBehaviorTree_H_

#include "Core\Scene.h"
#include "AITools.h"
#include "Scripts\CellsScriptFactory.h"

using namespace crea;

class SceneBehaviorTree : public Scene
{
	GameManager*	m_pGM;
	IntRect m_rWindowRect;

	Text* m_pTextFPS;
	Clock frameClock;

	Text* m_pTextBTMode;
	int m_iBTMode;

	// Steering
	Entity* m_pEntity3;
	Map* m_pMap;
	MapRenderer* m_pMapRenderer;

	// Entitites
	int m_iNbEntities;
	vector<Entity*> m_vEntities;
	bool m_bKeyPressedAdd;
	bool m_bKeyPressedSub;


	// AI Tools
	bool m_bUseAITools;
	AITools* m_pAITools;

	// Scripts
	CellsScriptFactory* m_pCellsScriptFactory;

	Entity* m_pMouse;
	Entity* m_pTarget; // Used to target destination
	Entity* m_pPeon;

	// Obstacles
	vector<Entity*> m_vObstacles;
	VectorVector2f m_vPath;

public:

	SceneBehaviorTree();
	virtual ~SceneBehaviorTree();

	virtual bool onInit();
	virtual bool onUpdate();
	virtual bool onDraw();
	virtual bool onQuit();

	void deleteEntities();
	void createEntities();
	void setBehaviour();

};

#endif
