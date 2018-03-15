#include "stdafx.h"

#include "AI\Steering\Vehicle.h"
#include "AI\Steering\Steering.h"
#include "Core\GameManager.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace crea
{
	Vehicle::Vehicle() :
		m_mass(1.0f),
		m_vVelocity(Vector2f(0.0f, 0.0f)),
		m_maxForce(100.0f),
		m_maxSpeed(100.0f),
		m_lastForce(Vector2f(0.0f, 0.0f)),
		m_lastR(Vector2f(1.0f, 0.0f))
	{
		m_bUpdatePosition = true;
	}

	Vehicle::~Vehicle()
	{
	}

	bool Vehicle::loadFromFileJSON(string _filename)
	{
		Json::Value root;
		std::ifstream config_doc(_filename, std::ifstream::binary);
		config_doc >> root;

		m_mass = root["Mass"].asFloat();
		m_maxForce = root["MaxForce"].asFloat();
		m_maxSpeed = root["MaxSpeed"].asFloat();

		return true;
	}

	bool Vehicle::init()
	{
		// Get Steering
		m_pSteering = m_pEntity->getComponent<Steering>();

		return true;
	}

	bool Vehicle::update()
	{
		if (m_pSteering == nullptr)
		{
			m_pSteering = m_pEntity->getComponent<Steering>();
		}

		Vector2f VehicleSteering = m_pSteering->getSteeringDirection();
		float frameTime = (float)TimeManager::getSingleton()->getFrameTime().asSeconds();

		if (!MathTools::zeroByEpsilon(VehicleSteering.lengthSq()))
		{
			Vector2f VehicleForce = MathTools::truncate(VehicleSteering, m_maxForce);
			Vector2f acceleration = VehicleForce / m_mass;
			m_vVelocity = MathTools::truncate(m_vVelocity + acceleration * frameTime, m_maxSpeed);

			m_lastForce = VehicleForce;
		}
		else
		{
			m_lastForce = Vector2f(0.0f, 0.0f);
		}

		if (m_bUpdatePosition)
		{
			Vector2f position = m_pEntity->getPosition();
			position += m_vVelocity * frameTime;
			m_pEntity->setPosition(position);
		}

		return true;
	}

	bool Vehicle::draw()
	{
		return true;
	}

	bool Vehicle::quit()
	{
		return true;
	}
} // namespace crea
