// Portions Copyright (C) Steve Rabin, 2001
#ifndef __MSGMANAGER_H__
#define __MSGMANAGER_H__

#include "AI.h"
#include <list>

namespace crea
{
	class Msg;

	typedef std::list<Msg*> MessageContainer;

	class CREAENGINE_API MsgManager : public Singleton <MsgManager>
	{
	public:

		MsgManager(void);
		~MsgManager(void);

		void update();
		void sendMsg(float delay, int name, objectID sender, objectID receiver, int state, void* data = NULL);

	private:

		MessageContainer m_delayedMessages;

		void deliverDelayedMessages(void);
		void routeMsg(Msg & msg);

	};

}

#endif	// __MSGMANAGER_H__