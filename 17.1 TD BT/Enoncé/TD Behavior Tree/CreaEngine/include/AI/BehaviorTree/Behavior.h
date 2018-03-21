/***************************************************/
/* Nom:	Behavior.h
/* Description: Behavior
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __BEHAVIOR_H_
#define __BEHAVIOR_H_

#include "AI\Steering\Behaviour.h"
#include "AI\Steering\Steering.h"
#include <vector>

namespace crea
{
	// ============================================================================

	enum Status
		/**
		* Return values of and valid states for Behaviors.
		*/
	{
		BH_INVALID,
		BH_SUCCESS,
		BH_FAILURE,
		BH_RUNNING,
		BH_ABORTED,
	};

	class CREAENGINE_API Behavior
		/**
		* Base class for actions, conditions and composites.
		*/
	{
	public:
		virtual Status update() = 0;

		virtual void onInitialize() {}
		virtual void onTerminate(Status) {}

		Behavior()
			: m_eStatus(BH_INVALID)
		{
		}

		virtual ~Behavior()
		{
		}

		Status tick()
		{
			if (m_eStatus != BH_RUNNING)
			{
				onInitialize();
			}

			m_eStatus = update();

			if (m_eStatus != BH_RUNNING)
			{
				onTerminate(m_eStatus);
			}
			return m_eStatus;
		}

		void reset()
		{
			m_eStatus = BH_INVALID;
		}

		void abort()
		{
			onTerminate(BH_ABORTED);
			m_eStatus = BH_ABORTED;
		}

		bool isTerminated() const
		{
			return m_eStatus == BH_SUCCESS || m_eStatus == BH_FAILURE;
		}

		bool isRunning() const
		{
			return m_eStatus == BH_RUNNING;
		}

		Status getStatus() const
		{
			return m_eStatus;
		}

		virtual Behavior* clone() = 0;

	private:
		Status m_eStatus;
	};

// ============================================================================

class CREAENGINE_API Decorator : public Behavior
{
protected:
	Behavior* m_pChild;

public:
	Decorator() {}
	Decorator(Behavior* child) : m_pChild(child) {}

	void addChild(Behavior* _pChild) { m_pChild = _pChild; }
};

// ============================================================================

class CREAENGINE_API Composite : public Behavior
{
protected:
	typedef std::vector<Behavior*> Behaviors;
	Behaviors m_Children;
public:
	void addChild(Behavior* child) { m_Children.push_back(child); }
	void removeChild(Behavior* _behavior)
	{ 
		auto a = std::remove_if(m_Children.begin(), m_Children.end(),
			[=](Behavior* p) { return p == _behavior; });

		m_Children.erase(a);
	}
	void clearChildren()
	{
		for (int i = 0; i < (int)m_Children.size(); i++)
		{
			Behavior* p = m_Children.back();
			delete(p);
			m_Children.pop_back();
		}
		m_Children.clear();
	};
};

class CREAENGINE_API Sequence : public Composite
{
protected:
	virtual ~Sequence()
	{
	}

	virtual void onInitialize()
	{
		m_CurrentChild = m_Children.begin();
	}

	virtual Status update()
	{
		// Keep going until a child Behavior says it's running.
		for (;;)
		{
			Status s = (*m_CurrentChild)->tick();
			
			// If the child fails, or keeps running, do the same.
			if (s != BH_SUCCESS)
			{
				return s;
			}

			// Hit the end of the array, job done!
			if (++m_CurrentChild == m_Children.end())
			{
				return BH_SUCCESS;
			}
		}
	}

	virtual void onTerminate(Status s)
	{
		if (m_CurrentChild != m_Children.end())
		{
			(*m_CurrentChild)->onTerminate(s);
		}			
	}

	virtual Behavior* clone()
	{
		Sequence* p = new Sequence(*this);
		for (int i = 0; i < (int)m_Children.size(); i++)
		{
			Behavior* pChild = m_Children[i];
			p->addChild(pChild->clone());
		}
		return p;
	}

		Behaviors::iterator m_CurrentChild;
	};

	// ============================================================================

	class CREAENGINE_API Selector : public Composite
	{
	protected:
		virtual ~Selector()
		{
		}

		virtual void onInitialize()
		{
			m_Current = m_Children.begin();
		}

		virtual Status update()
		{
			// Keep going until a child Behavior says its running.
			for (;;)
			{
				Status s = (*m_Current)->tick();

				// If the child succeeds, or keeps running, do the same.
				if (s != BH_FAILURE)
				{
					return s;
				}

				// Hit the end of the array, it didn't end well...
				if (++m_Current == m_Children.end())
				{
					return BH_FAILURE;
				}
			}
		}

		virtual Behavior* clone()
		{
			Selector* p = new Selector(*this);
			for (int i = 0; i < (int)m_Children.size(); i++)
			{
				Behavior* pChild = m_Children[i];
				p->addChild(pChild->clone());
			}
			return p;
		}

		Behaviors::iterator m_Current;
	};

	// ----------------------------------------------------------------------------

	class CREAENGINE_API Parallel : public Composite
	{
	public:
		enum Policy
		{
			RequireOne,
			RequireAll,
		};

		Parallel(Policy forSuccess, Policy forFailure)
			: m_eSuccessPolicy(forSuccess)
			, m_eFailurePolicy(forFailure)
		{
		}

		virtual ~Parallel() {}

	protected:
		Policy m_eSuccessPolicy;
		Policy m_eFailurePolicy;

		virtual Status update()
		{
			size_t iSuccessCount = 0, iFailureCount = 0;

			for (Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Behavior& b = **it;
				if (!b.isTerminated())
				{
					b.tick();
				}

				if (b.getStatus() == BH_SUCCESS)
				{
					++iSuccessCount;
					if (m_eSuccessPolicy == RequireOne)
					{
						return BH_SUCCESS;
					}
				}

				if (b.getStatus() == BH_FAILURE)
				{
					++iFailureCount;
					if (m_eFailurePolicy == RequireOne)
					{
						return BH_FAILURE;
					}
				}
			}

			if (m_eFailurePolicy == RequireAll  &&  iFailureCount == m_Children.size())
			{
				return BH_FAILURE;
			}

			if (m_eSuccessPolicy == RequireAll  &&  iSuccessCount == m_Children.size())
			{
				return BH_SUCCESS;
			}

			return BH_RUNNING;
		}

		virtual void onTerminate(Status)
		{
			for (Behaviors::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
			{
				Behavior& b = **it;
				if (b.isRunning())
				{
					b.abort();
				}
			}
		}
	};

	class CREAENGINE_API Monitor : public Parallel
	{
	public:
		Monitor()
			: Parallel(Parallel::RequireOne, Parallel::RequireOne)
		{
		}

		void addCondition(Behavior* condition)
		{
			m_Children.insert(m_Children.begin(), condition);
		}

		void addAction(Behavior* action)
		{
			m_Children.push_back(action);
		}
	};


	// ============================================================================

	class CREAENGINE_API ActiveSelector : public Selector
	{
	protected:

		virtual void onInitialize()
		{
			m_Current = m_Children.end();
		}

		virtual Status update()
		{
			Behaviors::iterator previous = m_Current;

			Selector::onInitialize();
			Status result = Selector::update();

			if (previous != m_Children.end() && m_Current != previous)
			{
				(*previous)->onTerminate(BH_ABORTED);
			}
			return result;
		}
	};

}

#endif //__Behavior_H_
