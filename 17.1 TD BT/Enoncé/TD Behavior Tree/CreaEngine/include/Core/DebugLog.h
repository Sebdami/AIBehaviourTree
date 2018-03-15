// Portions Copyright (C) Steve Rabin, 2001
#ifndef __DEBUGLOG_H__
#define __DEBUGLOG_H__

#include "Core\Singleton.h"
#include "AI\AI.h"
#include <list>

namespace crea
{
	class Msg;

	class LogEntry
	{
	public:

		LogEntry();
		~LogEntry() {}

		objectID m_owner;
		bool m_handled;

		double m_timestamp;
		char m_statename[64];
		char m_eventmsgname[64];

		//msg only info
		objectID m_receiver;
		objectID m_sender;

	};

	class CREAENGINE_API DebugLog : public Singleton <DebugLog>
	{
	public:

		DebugLog(void);
		~DebugLog(void);

		void LogStateMachineEvent(objectID id, Msg* _msg, char* statename, char* eventmsgname, bool handled);

	private:

		std::list<LogEntry*> m_log;

	};

}

#endif	// __DEBUGLOG_H__