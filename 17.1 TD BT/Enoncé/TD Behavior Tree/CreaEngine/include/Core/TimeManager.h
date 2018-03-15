/***************************************************/
/* Nom:	TimeManager.h
/* Description: TimeManager
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef _TimeManager_H
#define _TimeManager_H
#include <chrono>

using namespace std::chrono;

namespace crea
{
	class CREAENGINE_API Time
	{
		double m_dTime;
	public:

		Time();

		Time(double _dTime);

		~Time();

		Time& operator=(double _dTime);
		Time& operator+=(Time& _Time);
		bool operator>=(Time& _Time);
		Time operator*(float _fTime);

		int asMicroseconds();
		int asMilliseconds();
		double asSeconds();

		void setAsMicroSeconds(int _iTime);
		void setAsMilliSeconds(int _iTime);
		void setAsSeconds(double _dTime);
	};

	class CREAENGINE_API Clock
	{
		high_resolution_clock::time_point m_t1;
		Time m_elapsedTime;
	public:

		Clock();
		
		~Clock();
		
		Time getElapsedTime();
		Time restart();
	};

	class CREAENGINE_API TimeManager
	{
		TimeManager();

		Clock m_FrameClock;
		Time m_FrameTime;

		Clock m_GameClock;
		Time m_GameTime;

	public:
		~TimeManager();

		static TimeManager* getSingleton();

		Time getFrameTime();

		Time getGameTime();

		void init();

		void update();

	};

} // namespace crea

#endif // _TimeManager_H