// Portions Copyright (C) Steve Rabin, 2001
#include "stdafx.h"

#include "Core\DebugLog.h"
#include "Core\TimeManager.h"

#define MAX_DEBUG_LOG_SIZE 100

namespace crea
{
	LogEntry::LogEntry()
	{
		m_owner = INVALID_OBJECT_ID;
		m_handled = false;

		m_timestamp = -1.0;
		strcpy_s(m_statename, "");
		strcpy_s(m_eventmsgname, "");
		m_receiver = INVALID_OBJECT_ID;
		m_sender = INVALID_OBJECT_ID;

	}


	DebugLog::DebugLog()
	{

	}


	DebugLog::~DebugLog()
	{

	}


	void DebugLog::LogStateMachineEvent(objectID id, Msg* _msg, char* statename, char* eventmsgname, bool handled)
	{
		LogEntry * entry = new LogEntry();

		entry->m_owner = id;
		entry->m_handled = handled;
		entry->m_timestamp = g_time->getGameTime().asSeconds();
		strcpy_s(entry->m_statename, statename);
		strcpy_s(entry->m_eventmsgname, eventmsgname);

		if (_msg) {
			entry->m_receiver = _msg->GetReceiver();
			entry->m_sender = _msg->GetSender();
		}

		m_log.push_back(entry);
		if (m_log.size() > MAX_DEBUG_LOG_SIZE) {
			delete(m_log.front());
			m_log.erase(m_log.begin());
		}
	}

}