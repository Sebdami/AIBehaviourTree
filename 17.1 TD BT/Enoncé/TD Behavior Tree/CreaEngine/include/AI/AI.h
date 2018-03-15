/***************************************************/
/* Nom:	AI.h
/* Description: AI
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __AI_H_
#define __AI_H_

#include "Core\singleton.h"
#include <vector>

namespace crea
{
	class CREAENGINE_API VectorVector2f : public vector<Vector2f*> {};

	#define g_time TimeManager::getSingleton()
	#define g_entitymanager EntityManager::getSingleton()
	#define g_msgmanager MsgManager::getSingleton()
	#define g_debuglog DebugLog::getSingleton()

}

#include "StateMachine.h"
#include "Agent.h"
#include "MsgManager.h"
#include "Msg.h"

#endif