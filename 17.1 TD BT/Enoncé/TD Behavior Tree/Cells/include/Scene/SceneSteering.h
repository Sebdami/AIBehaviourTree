/***************************************************/
/* Nom:	SceneSteering.h
/* Description: SceneSteering
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __SceneSteering_H_
#define __SceneSteering_H_

#include "Core\Scene.h"
#include "AITools.h"
#include "Scripts\CellsScriptFactory.h"

using namespace crea;

class SceneSteering : public Scene
{
	GameManager*	m_pGM;
	IntRect m_rWindowRect;

	Text* m_pTextFPS;
	Clock frameClock;

	Text* m_pTextSteeringMode;
	int m_iSteeringMode;

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

	SceneSteering();
	virtual ~SceneSteering();

	virtual bool onInit();
	virtual bool onUpdate();
	virtual bool onDraw();
	virtual bool onQuit();

	void deleteEntities();
	void createEntities();
	void setBehaviour();

};

#endif
