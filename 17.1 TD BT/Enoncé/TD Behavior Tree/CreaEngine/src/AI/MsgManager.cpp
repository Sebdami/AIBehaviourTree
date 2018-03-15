// Portions Copyright (C) Steve Rabin, 2001
#include "stdafx.h"

#include "AI\AI.h"
#include "AI\MsgManager.h"
#include "Core\TimeManager.h"

namespace crea
{

	MsgManager::MsgManager(void)
	{

	}

	MsgManager::~MsgManager(void)
	{

	}

	void MsgManager::update()
	{
		deliverDelayedMessages();
	}

	void MsgManager::sendMsg(float delay, int name, objectID sender, objectID receiver, int state, void* data)
	{

		if (delay <= 0.0f)
		{	//Deliver immediately
			Msg msg((float)g_time->getGameTime().asSeconds(), name, sender, receiver, state);
			msg.SetMsgData(data);
			routeMsg(msg);
		}
		else
		{	//Check for duplicates - then store
			MessageContainer::iterator i;
			for (i = m_delayedMessages.begin(); i != m_delayedMessages.end(); ++i)
			{
				if ((*i)->IsDelivered() == false &&
					(*i)->GetMsgName() == name &&
					(*i)->GetReceiver() == receiver &&
					(*i)->GetSender() == sender &&
					(*i)->GetMsgState() == state)
				{	//Already in list - don't add
					return;
				}
			}

			//Store in delivery list
			float deliveryTime = delay + (float)g_time->getGameTime().asSeconds();
			Msg * msg = new Msg(deliveryTime, name, sender, receiver, state);
			msg->SetMsgData(data);
			m_delayedMessages.push_back(msg);
		}

	}


	void MsgManager::deliverDelayedMessages(void)
	{
		MessageContainer::iterator i;
		for (i = m_delayedMessages.begin(); i != m_delayedMessages.end();)
		{
			float dtime = (*i)->GetDeliveryTime();
			float ctime = (float) g_time->getGameTime().asSeconds();
			if ((*i)->GetDeliveryTime() <= (float)g_time->getGameTime().asSeconds())
			{	//Deliver and delete msg
				Msg * msg = *i;
				routeMsg(*msg);
				delete(msg);
				i = m_delayedMessages.erase(i);
			}
			if (i != m_delayedMessages.end())
				i++;
		}

	}

	void MsgManager::routeMsg(Msg & msg)
	{
		Entity* object = g_entitymanager->Find(msg.GetReceiver());

		if (object != 0)
		{
			// deliver to all FSMs
			list<StateMachine*> FSMs;
			object->getComponents<StateMachine>(FSMs);
			for (list<StateMachine*>::iterator it = FSMs.begin(); it != FSMs.end(); ++it)
			{
				StateMachine* pFSM = *it;
				if (msg.GetMsgState() < 0 ||
					msg.GetMsgState() == pFSM->GetState())
				{	//State was irrelevant or current state matches msg state (for msg scoping)
					msg.SetDelivered(true);
					pFSM->Process(EVENT_Message, &msg);
				}
			}
		}

	}

}



