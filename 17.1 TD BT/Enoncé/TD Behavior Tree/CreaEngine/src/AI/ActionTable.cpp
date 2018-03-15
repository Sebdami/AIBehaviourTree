#include "stdafx.h"

#include "AI\ActionTable.h"
#include "json\json.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace crea
{
	ActionTable::ActionTable()
	{
		m_pGM = crea::GameManager::getSingleton();
	}

	ActionTable::~ActionTable()
	{
		// delete multimap
		MapConditionAction::iterator ca_it = m_condActionMap.begin();
		while (ca_it != m_condActionMap.end())
		{
			delete (*ca_it).second;
			ca_it = m_condActionMap.erase(ca_it);
		}
	}

	void ActionTable::read()
	{

	}

	string* ActionTable::getAnimation(
		unsigned char _ucAnimCond1,
		unsigned char _ucAnimCond2,
		unsigned short _unAction,
		string* _pszActionDesc)
	{
		short keyDirCond = MERGE2CONDITIONS(_ucAnimCond1, _ucAnimCond2);
		return getAnimation(keyDirCond, _unAction, _pszActionDesc);
	}

	string* ActionTable::getAnimation(
		unsigned short _unAnimCond,
		unsigned short _unAction,
		string* _pszActionDesc)
	{
		MapConditionAction::iterator ca_it;
		ActionAnimInfo*  pAnimInfo = nullptr;
		long keyDirCond = CREATE_KEY(_unAnimCond, _unAction);
		ca_it = m_condActionMap.find(keyDirCond);
		// Get list of actions for this animation condition
		if (ca_it != m_condActionMap.end())
		{
			// Get number of animations listed for this action.
			long nCount = m_condActionMap.count(keyDirCond);

			// If only one action is listed, return the animation.
			if (nCount == 1)
			{
				pAnimInfo = (ca_it->second);
				if (_pszActionDesc)
				{
					*_pszActionDesc = pAnimInfo->szActionDesc;
				}
				return &pAnimInfo->szAnimFileName;
			}

			// Else pick randomly from a list of animations for this action.
			else if (nCount > 1)
			{
				long nIndex = (long)(((float)rand() / (float)RAND_MAX) * (float)nCount);
				for (long i = 0; i<nIndex; ++i, ++ca_it);// Position ca_it on nIndexth AnimInfoStruct
				pAnimInfo = (ca_it->second);

				if (pAnimInfo)
				{
					*_pszActionDesc = pAnimInfo->szActionDesc;
				}
				return &pAnimInfo->szAnimFileName;
			}
		}
		// No animation was found for the specified eAnimCond and eAction, so get default animation
		if (_unAnimCond != 0)
		{
			return getAnimation(0, _unAction, _pszActionDesc);
		}
		return nullptr;
	}

	bool ActionTable::addAnimation(
		unsigned short _unAnimCond,
		unsigned short _unAction,
		string* _pszAnimFileName,
		string* _pszActionDesc)
	{
		MapConditionAction::iterator ca_it;
		long keyDirCond = CREATE_KEY(_unAnimCond, _unAction);
		ActionAnimInfo*  pAnimInfo = new ActionAnimInfo;
		pAnimInfo->szAnimFileName = *_pszAnimFileName;
		pAnimInfo->szActionDesc = (_pszActionDesc ? *_pszActionDesc : "");
		m_condActionMap.insert(MapConditionAction::value_type(keyDirCond, pAnimInfo));

		// Load animation
		m_pGM->getAnimation(pAnimInfo->szAnimFileName);

		return true;
	}

	bool ActionTable::init()
	{
		return true;
	}

	bool ActionTable::update()
	{
		return true;
	}

	bool ActionTable::draw()
	{
		return true;
	}

	bool ActionTable::quit()
	{
		return true;
	}

	bool ActionTable::loadFromFileJSON(string& _filename)
	{
		Json::Value root;
		std::ifstream config_doc(_filename, std::ifstream::binary);
		config_doc >> root;

		Json::Value conditions = root["conditions"];

		for (unsigned int iCond = 0; iCond < conditions.size(); ++iCond)
		{
			Json::Value condition = conditions[iCond];
			unsigned char ucCondition = condition["id"].asInt();
			Json::Value actions = condition["actions"];
			for (unsigned int iAct = 0; iAct < actions.size(); ++iAct)
			{
				Json::Value action = actions[iAct];
				short unAction = action["id"].asInt();
				Json::Value animations = action["animations"];
				for (unsigned int iAnimation = 0; iAnimation < animations.size(); ++iAnimation)
				{
					Json::Value animation = animations[iAnimation];
					unsigned char ucDir = iAnimation;
					short unCondition = MERGE2CONDITIONS(ucDir, ucCondition);
					addAnimation(unCondition, unAction, &animation.asString());
				}
			}
		}

		return true;
	}

} // namespace crea
