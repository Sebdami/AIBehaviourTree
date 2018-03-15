#include "stdafx.h"

#include "AI\Steering\Steering.h"
#include "AI\Steering\Behaviour.h"
#include "Physics\Collider.h"
#include "Core\PhysicsManager.h"

namespace crea
{
	PairFloatBehaviour::PairFloatBehaviour(float _f, Behaviour* _p) : pair<float, Behaviour*>(_f, _p) {}
	PairFloatBehaviour::~PairFloatBehaviour() { delete this->second; }

	Steering::Steering() :
		m_pTarget(nullptr), m_vSteeringDirection(0.f, 0.f)
	{
		m_pGM = GameManager::getSingleton();
		m_pCollider = nullptr;
	}

	Steering::~Steering()
	{
		clearBehaviours();
	}

	void Steering::setCollider(Collider* _pCollider)
	{
		m_pCollider = _pCollider;
	}
	
	void Steering::addBehaviour(Behaviour* _Behaviour, float _weight)
	{
		m_Behaviours.push_back(new PairFloatBehaviour(_weight, _Behaviour));
	};

	void Steering::removeBehaviour(Behaviour* _Behaviour)
	{
		if (m_Behaviours.empty())
			return;

		auto a = std::remove_if(m_Behaviours.begin(), m_Behaviours.end(),
			[=](PairFloatBehaviour* p) { return p->second == _Behaviour; });

		m_Behaviours.erase(a);
	};

	void Steering::clearBehaviours()
	{
		for (int i = 0; i < (int)m_Behaviours.size(); i++)
		{
			PairFloatBehaviour* p = m_Behaviours.back();
			delete(p);
			m_Behaviours.pop_back();
		}
		m_Behaviours.clear();
	};

	Vector2f Steering::steer()
	{
		Vector2f steeringDirection;
		for (int i = 0; i < (int)m_Behaviours.size(); i++)
		{
			PairFloatBehaviour* t = m_Behaviours[i];
			Behaviour* b = (Behaviour*)t->second;
			Vector2f v = b->Update();
 			steeringDirection += (v * t->first);
		}
		return steeringDirection;
	}

	string Steering::asString()
	{
		stringstream stream;
		string szSteering("");
		Vector2f steeringDirection;
		for (int i = 0; i < (int)m_Behaviours.size(); i++)
		{
			PairFloatBehaviour* t = m_Behaviours[i];
			Behaviour* b = (Behaviour*)t->second;
			szSteering += t->second->asString();
			szSteering += " ";
			stream << fixed << setprecision(1) << t->first;
			szSteering += stream.str();
		}
		return szSteering;
	}
	
	bool Steering::init()
	{
		m_pTarget = nullptr;

		return true;
	}

	bool Steering::update()
	{
		m_vSteeringDirection = steer();

		return true;
	}

	bool Steering::draw()
	{
		return true;
	}

	bool Steering::quit()
	{
		return true;
	}
} // namespace crea
