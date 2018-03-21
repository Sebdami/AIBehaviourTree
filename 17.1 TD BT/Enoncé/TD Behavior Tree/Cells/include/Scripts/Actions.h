/***************************************************/
/* Nom:	Actions.h
/* Description: Actions
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Actions_H_
#define __Actions_H_

#include "AI\BehaviorTree\Behavior.h"

using namespace crea;

class ActionSeekTo : public Behavior
{
	Steering* m_pSteering;
	Seek* m_pSeek;
	float m_fArrivedDistance;

public:
	ActionSeekTo(Seek* _pSeek, float _fArrivedDistance)
		: m_fArrivedDistance(_fArrivedDistance)
	{
		m_pSeek = _pSeek;
		m_pSteering = m_pSeek->getEntity()->getComponent<Steering>();
	}

	virtual ~ActionSeekTo()
	{
	}

	virtual void onInitialize()
	{
		if (m_pSteering)
		{
			m_pSteering->addBehaviour(m_pSeek, 1.0f);
		}
	}

	virtual void onTerminate(Status s)
	{
		if (m_pSteering)
		{
			m_pSteering->removeBehaviour(m_pSeek);
		}
	}

	virtual Status update()
	{
		if (!m_pSteering)
		{
			return BH_FAILURE;
		}

		Vector2f targetOffset = m_pSeek->getTarget()->getPosition() - m_pSeek->getEntity()->getPosition();
		double distance = targetOffset.length();

		if (distance < m_fArrivedDistance)
		{
			return BH_SUCCESS;
		}
		else
		{
			return BH_RUNNING;
		}
	}

	virtual Behavior* clone()
	{
		return new ActionSeekTo(*this);
	}
};

class ActionFleeUntil : public Behavior
{
	Steering* m_pSteering;
	Flee* m_pFlee;
	float m_fDistanceToFlee;

public:
	ActionFleeUntil(Flee* _pFlee, float _fDistanceToFlee)
		: m_fDistanceToFlee(_fDistanceToFlee)
	{
		m_pFlee = _pFlee;
		m_pSteering = m_pFlee->getEntity()->getComponent<Steering>();
	}

	virtual ~ActionFleeUntil()
	{
	}

	virtual void onInitialize()
	{
		if (m_pSteering)
		{
			m_pSteering->addBehaviour(m_pFlee, 1.0f);
		}
	}

	virtual void onTerminate(Status s)
	{
		if (m_pSteering)
		{
			m_pSteering->removeBehaviour(m_pFlee);
		}
	}

	virtual Status update()
	{
		if (!m_pSteering)
		{
			return BH_FAILURE;
		}

		Vector2f targetOffset = m_pFlee->getTarget()->getPosition() - m_pFlee->getEntity()->getPosition();
		double distance = targetOffset.length();

		if (distance > m_fDistanceToFlee)
		{
			return BH_SUCCESS;
		}
		else
		{
			return BH_RUNNING;
		}
	}

	virtual Behavior* clone()
	{
		return new ActionFleeUntil(*this);
	}
};


class ActionWander : public Behavior
{
	Steering* m_pSteering;
	Wander* m_pWander;
	float timer;
public:
	ActionWander(Wander* _pWander)
	{
		m_pWander = _pWander;
		m_pSteering = m_pWander->getEntity()->getComponent<Steering>();
	}

	virtual ~ActionWander()
	{
	}

	virtual void onInitialize()
	{
		if (m_pSteering)
		{
			m_pSteering->addBehaviour(m_pWander, 1.0f);
		}
	}

	virtual void onTerminate(Status s)
	{
		if (m_pSteering)
		{
			m_pSteering->removeBehaviour(m_pWander);
		}
	}

	virtual Status update()
	{
		if (!m_pSteering)
		{
			return BH_FAILURE;
		}

		return BH_RUNNING;
	}

	virtual Behavior* clone()
	{
		return new ActionWander(*this);
	}
};

class IsNearTargetDecorator : public Decorator
{
	Entity* m_pSelfEntity;
	Entity* m_pTarget;
	float m_fMaxDistance;

public:
	IsNearTargetDecorator() {}
	IsNearTargetDecorator(Behavior* pChild, Entity* pSelf, Entity* pTarget, float fMaxDistance)
		: Decorator(pChild), m_pSelfEntity(pSelf), m_pTarget(pTarget), m_fMaxDistance(fMaxDistance){}

	virtual void onInitialize()
	{
		m_pChild->onInitialize();
	}

	virtual Status update()
	{
		if ((m_pTarget->getPosition() - m_pSelfEntity->getPosition()).length() < m_fMaxDistance)
		{
			return m_pChild->update();
		}
		else
		{
			return Status::BH_FAILURE;
		}
	}

	virtual void onTerminate(Status s)
	{
		m_pChild->onTerminate(s);
	}

	virtual Behavior* clone()
	{
		return new IsNearTargetDecorator(m_pChild, m_pSelfEntity, m_pTarget, m_fMaxDistance);
	}
};

class IsAwayFromTargetDecorator : public Decorator
{
	Entity* m_pSelfEntity;
	Entity* m_pTarget;
	float m_fMaxDistance;

public:
	IsAwayFromTargetDecorator() {}
	IsAwayFromTargetDecorator(Behavior* pChild, Entity* pSelf, Entity* pTarget, float fMaxDistance)
		: Decorator(pChild), m_pSelfEntity(pSelf), m_pTarget(pTarget), m_fMaxDistance(fMaxDistance) {}

	virtual void onInitialize()
	{
		m_pChild->onInitialize();
	}

	virtual Status update()
	{
		if ((m_pTarget->getPosition() - m_pSelfEntity->getPosition()).length() >= m_fMaxDistance)
		{
			return m_pChild->update();
		}
		else
		{
			return Status::BH_FAILURE;
		}
	}

	virtual void onTerminate(Status s)
	{
		m_pChild->onTerminate(s);
	}

	virtual Behavior* clone()
	{
		return new IsAwayFromTargetDecorator(m_pChild, m_pSelfEntity, m_pTarget, m_fMaxDistance);
	}
};

class RepeatDecorator : public Decorator
{
	int m_iMaxIterations;
	int m_iCurrentIterations;
public:
	RepeatDecorator() {}
	RepeatDecorator(Behavior* pChild, int iMaxIterations)
		: Decorator(pChild), m_iMaxIterations(iMaxIterations), m_iCurrentIterations(0){}

	virtual void onInitialize()
	{
		m_pChild->onInitialize();
		m_iCurrentIterations = 0;
	}

	virtual Status update()
	{
		if (m_iCurrentIterations >= m_iMaxIterations)
			return BH_FAILURE;

		Status s = m_pChild->update();
		if (s == BH_SUCCESS)
		{
			m_iCurrentIterations++;
			m_pChild->onTerminate(BH_SUCCESS);
			if (m_iCurrentIterations >= m_iMaxIterations)
				return BH_FAILURE;
			m_pChild->onInitialize();
			return BH_RUNNING;
		}
		return s;
	}

	virtual void onTerminate(Status s)
	{
		m_pChild->onTerminate(s);
		m_iCurrentIterations = 0;
	}

	virtual Behavior* clone()
	{
		return new RepeatDecorator(m_pChild, m_iMaxIterations);
	}
};

class TimerFilter : public Decorator
{
	float m_fMaxTime;
	float m_fCurrentTime;
public:
	TimerFilter() {}
	TimerFilter(Behavior* pChild, float fMaxTime)
		: Decorator(pChild), m_fMaxTime(fMaxTime), m_fCurrentTime(0.0f) {}

	virtual void onInitialize()
	{
		m_pChild->onInitialize();
		m_fCurrentTime = 0.0f;
	}

	virtual Status update()
	{
		if (m_fCurrentTime >= m_fMaxTime)
		{
			return BH_FAILURE;
		}
		m_fCurrentTime += TimeManager::getSingleton()->getFrameTime().asSeconds();
		if (m_fCurrentTime >= m_fMaxTime)
		{
			//m_pChild->onTerminate(BH_SUCCESS);
			return BH_FAILURE;
		}
		else
		{
			return m_pChild->update();
		}
		
	}

	virtual void onTerminate(Status s)
	{
		m_pChild->onTerminate(s);
		m_fCurrentTime = 0.0f;
	}

	virtual Behavior* clone()
	{
		return new TimerFilter(m_pChild, m_fMaxTime);
	}
};
class IsMyName : public Decorator
{
	Entity* m_pSelfEntity;
	string m_sName;

public:
	IsMyName() {}
	IsMyName(Behavior* pChild, Entity* pSelf, string sName)
		: Decorator(pChild), m_pSelfEntity(pSelf), m_sName(sName) {}

	virtual void onInitialize()
	{
		m_pChild->onInitialize();
	}

	virtual Status update()
	{
		if (m_pSelfEntity->getName() == m_sName)
			return m_pChild->update();
		else
			return Status::BH_FAILURE;
	}

	virtual void onTerminate(Status s)
	{
		m_pChild->onTerminate(s);
	}

	virtual Behavior* clone()
	{
		return new IsMyName(m_pChild, m_pSelfEntity, m_sName);
	}
};

#endif
