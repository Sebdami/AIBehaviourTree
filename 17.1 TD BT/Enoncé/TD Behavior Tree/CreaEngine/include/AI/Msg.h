/***************************************************/
/* Nom:	Msg.h
/* Description: Msg
/* Auteur: BRUNEAU Colin
/***************************************************/
// Portions Copyright (C) Steve Rabin, 2001
#ifndef __MSG_H__
#define __MSG_H__

#include "AI.h"

namespace crea
{

	class Msg
	{
	public:

		Msg(void);
		Msg(float deliveryTime, int name, objectID sender, objectID receiver, int state = -1);
		~Msg(void) {}

		int GetMsgName(void) { return(m_Name); }
		void SetMsgName(int name) { m_Name = name; }

		objectID GetSender(void) { return(m_Sender); }
		void SetSender(objectID sender) { m_Sender = sender; }

		objectID GetReceiver(void) { return(m_Receiver); }
		void SetReceiver(objectID receiver) { m_Receiver = receiver; }

		int GetMsgState(void) { return(m_State); }
		void SetMsgState(int state) { m_State = state; }

		void* GetMsgData(void) { return(m_Data); }
		void SetMsgData(void* data) { m_Data = data; }

		float GetDeliveryTime(void) { return(m_DeliveryTime); }
		void SetDeliveryTime(float time) { m_DeliveryTime = time; }

		bool IsDelivered(void) { return(m_Delivered); }
		void SetDelivered(bool value) { m_Delivered = value; }

	private:

		int m_Name;				//Message name
		objectID m_Sender;		//Object that sent the message
		objectID m_Receiver;	//Object that will get the message
		int m_State;			//Scene in which the receiver is allowed get the message (-1 means any state)

		float m_DeliveryTime;	//Time at which to send the message
		bool m_Delivered;		//Whether the message has been delivered

		void* m_Data;
	};

}

#endif	// __MSG_H__