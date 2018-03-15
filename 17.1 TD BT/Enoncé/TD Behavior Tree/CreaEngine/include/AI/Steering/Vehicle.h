/***************************************************/
/* Nom:	Vehicle.h
/* Description: Vehicle
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Vehicle_H_
#define __Vehicle_H_

#include "Core\Component.h"
#include "Core\Math.h"

namespace crea
{
	// Predefinitions
	class Steering;

	class CREAENGINE_API Vehicle : public Component
	{
	protected:
		Steering* m_pSteering;

		// Locomotion
		float m_mass;
		float m_maxForce;
		float m_maxSpeed;

		Vector2f m_vVelocity;

		//last force to render
		Vector2f m_lastForce;
		Vector2f m_lastR;

		bool m_bUpdatePosition;
		
	public:
		Vehicle();
		virtual ~Vehicle();

		bool loadFromFileJSON(string _filename);

		float getMass() { return m_mass; }
		Vector2f getVelocity() { return m_vVelocity; }
		float getMaxForce() { return m_maxForce; }
		float getMaxSpeed() { return m_maxSpeed; }
		Vector2f getLastForce() { return m_lastForce; }
		Vector2f getLastR() { return m_lastR; }

		void setMass(float _mass) { m_mass = _mass; }
		void setVelocity(Vector2f _velocity) { m_vVelocity = _velocity; }
		void setMaxForce(float _maxForce) { m_maxForce = _maxForce; }
		void setMaxSpeed(float _maxSpeed) { m_maxSpeed = _maxSpeed; }
		void setLastR(Vector2f _lastR) { m_lastR = _lastR; }

		void setUpdatePosition(bool _bUpdatePosition) { m_bUpdatePosition = _bUpdatePosition;  }
					
		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone() { Vehicle* p = new Vehicle(*this); m_pSteering = nullptr; return p; }
	};

} // namespace crea

#endif
