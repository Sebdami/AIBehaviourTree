#include "stdafx.h"

#include "Core\TimeManager.h"

namespace crea
{
	Time::Time()
	{
		m_dTime = 0.0;
	}

	Time::Time(double _dTime)
	{
		m_dTime = _dTime;
	}

	Time::~Time()
	{
	}

	Time& Time::operator=(double _dTime) { m_dTime = _dTime; return *this; }
	Time& Time::operator+=(Time& _Time) { m_dTime += _Time.m_dTime; return *this; }
	bool Time::operator>=(Time& _Time) { return m_dTime >= _Time.m_dTime; }
	Time Time::operator*(float _f) { return Time(m_dTime*_f); }

	int Time::asMicroseconds() { return (int)(m_dTime * 1E6); }

	int Time::asMilliseconds() { return (int)(m_dTime * 1E3); }

	double Time::asSeconds() { return (double)m_dTime; }
	
	void Time::setAsMicroSeconds(int _iTime) { m_dTime = _iTime * 1E-6; }

	void Time::setAsMilliSeconds(int _iTime) { m_dTime = _iTime * 1E-3; }

	void Time::setAsSeconds(double _dTime) { m_dTime = _dTime; }

	Clock::Clock()
	{
		m_t1 = high_resolution_clock::now();
	}

	Clock::~Clock()
	{
	}

	Time Clock::getElapsedTime()
	{
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - m_t1);
		return time_span.count();
	}
	Time Clock::restart()
	{
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(t2 - m_t1);
		m_t1 = t2;
		return time_span.count();
	}

	TimeManager::TimeManager()
	{
	}

	TimeManager::~TimeManager()
	{
	}
	TimeManager* TimeManager::getSingleton()
	{
		static TimeManager instanceUnique;
		return
			&instanceUnique;
	}

	Time TimeManager::getFrameTime() 
	{ 
		return m_FrameTime;
	}

	Time TimeManager::getGameTime()
	{
		return m_GameClock.getElapsedTime();
	}

	void TimeManager::init() 
	{ 
		m_FrameClock.restart();
		m_GameClock.restart();
	}

	void TimeManager::update()
	{
		m_FrameTime = m_FrameClock.restart();
	}

} // namespace crea
