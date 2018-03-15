/***************************************************/
/* Nom:	AITools.h
/* Description: AITools
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __AITools_H_
#define __AITools_H_

#include "Core\Scene.h"
#include "Scripts\CharacterController.h"
using namespace crea;

enum EnumCommandType
{
	Command_Invalid = -1,
	Command_Reset = 0,
	Command_Kill,
	Command_Stop,
	Command_GoToHQ,
	Command_GoTo,
	Command_Build,
	Command_Mine,
	Command_Harvest,
	Command_Die,
	Command_Boost,
	Command_GoToHQWithLumber,
	Command_Seek,
	Command_Flee,
	Command_Pursuit,
	Command_Evasion,
	Command_Arrival,
	Command_Wander,
	Command_PathFollowing,
	Command_UCA

}; 


class AITools
{
	GameManager*	m_pGM;

	// Selection
	bool m_bSelection;
	Vector2f m_vStartSelection;
	Vector2f m_vEndSelection;
	RectangleShape* m_pSelectionShape;

	// Command
	FloatRect m_rCommandWindow;
	bool m_bCommand;
	EnumCommandType m_eCommandType;
	Sprite* m_pCommandsSprite;

	// Diagnostic
	Text* m_pTextFPS;
	Text* m_pTextCommand;
	Text* m_pTextDiagnostics;
	Clock m_CommandDisplayClock;
	float m_fCommandDisplayTime;

	// Grid
	Map* m_pMap;
	short m_nWidth;
	short m_nHeight;
	short m_nTileWidth;
	short m_nTileHeight;
	RectangleShape* m_pNodeShape;

	// Tile index limits
	int m_iMin;
	int m_iMax;
	int m_jMin;
	int m_jMax;

	bool isButton(int _i, Vector2f& _vMousePosition);

	// CharacterController
	CharacterController* m_pCharacterController;

	// Collisions
	RectangleShape* m_pBoxColliderShape;
	CircleShape* m_pCircleColliderShape;
	RectangleShape* m_pCollisionNodeShape;
	void DisplayCollider(MapStringCollider* _pColliders);

	// Cluster
	RectangleShape* m_pClusterShape;
	RectangleShape* m_pEntranceShape;
	ArrowShape* m_pTransitionShape;
	int m_iClusterMin;
	int m_iClusterMax;
	int m_jClusterMin;
	int m_jClusterMax;
	short m_nClusterWidth;
	short m_nClusterHeight;

	// Steering
	Steering* m_pSteering;
	ArrowShape* m_pTargetShape;
	
	Sprite* m_pTargetSprite;

public:
	AITools();
	virtual ~AITools();

	virtual bool onInit();
	virtual bool onUpdate();
	virtual bool onDraw();
	virtual bool onQuit();

};

#endif
