#include "stdafx.h"

#include "AI\Agent.h"
#include "Core\GameManager.h"
#include "json/json.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace crea
{
	Agent::Agent() : m_nStrength(10), m_nDexterity(10), m_nIntelligence(10), m_nHealth(10), m_nSize(1), m_nCapability(0)
	{
	}

	Agent::~Agent()
	{
	}

	bool Agent::loadFromFileJSON(string _filename)
	{
		Json::Value root;
		std::ifstream config_doc(_filename, std::ifstream::binary);
		config_doc >> root;

		m_nStrength = (short) root["Strength"].asInt();
		m_nDexterity = (short) root["Dexterity"].asInt();
		m_nIntelligence = (short) root["Intelligence"].asInt();
		m_nHealth = (short) root["Health"].asInt();
		m_nSize = (short)root["Size"].asInt();
		m_nCapability = (short)root["Capability"].asInt();

		return true;
	}

	bool Agent::init()
	{
		return true;
	}

	bool Agent::update()
	{
		return true;
	}

	bool Agent::draw()
	{
		return true;
	}

	bool Agent::quit()
	{
		return true;
	}

} // namespace crea
