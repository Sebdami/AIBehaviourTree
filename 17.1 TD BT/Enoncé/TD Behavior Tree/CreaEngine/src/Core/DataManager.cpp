#include "stdafx.h"

#include "Core\DataManager.h"

namespace crea
{
	DataManager::DataManager()
	{
		m_bIsCleared = false;
	}

	DataManager::~DataManager()
	{
		clear();
	}

	DataManager* DataManager::getSingleton()
	{
		static DataManager instanceUnique;
		return
			&instanceUnique;
	}

	Font* DataManager::getFont(string _szName, bool _bCloned)
	{
		MapStringFont::iterator it = m_pFonts.find(_szName);
		if (it == m_pFonts.end())
		{
			Font* pFont = new Font();

			if (!pFont->loadFromFile(DATAFONTPATH + _szName))
			{
				delete pFont;
				cerr << "Unable to open font" << endl;
				return nullptr;
			}
			m_pFonts[_szName] = pFont;
			return pFont;
		}
		else
		{
			if (_bCloned)
			{
				return new Font(*it->second);
			}
			else
			{
				return it->second;
			}
		}
	}

	Texture* DataManager::getTexture(string _szName, bool _bCloned)
	{
		MapStringTexture::iterator it = m_pTextures.find(_szName);
		if (it == m_pTextures.end())
		{
			Texture* pTexture = new Texture();

			if (!pTexture->loadFromFile(DATATEXTUREPATH + _szName))
			{
				delete pTexture;
				cerr << "Unable to open Texture" << endl;
				return nullptr;
			}
			m_pTextures[_szName] = pTexture;
			return pTexture;
		}
		else
		{
			if (_bCloned)
			{
				return new Texture(*it->second);
			}
			else
			{
				return it->second;
			}
		}
	}

	Color* DataManager::getColor(string _szName, bool _bCloned)
	{
		MapStringColor::iterator it = m_pColors.find(_szName);
		if (it == m_pColors.end())
		{
			Color* pColor = new Color(); // Create a default Color if none exist
			m_pColors[_szName] = pColor;
			return pColor;
		}
		else
		{
			if (_bCloned)
			{
				return new Color(*it->second);
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Text* DataManager::getText(string _szName, bool _bCloned)
	{
		MapStringText::iterator it = m_pTexts.find(_szName);
		if (it == m_pTexts.end())
		{
			Text* pText = new Text(); // Create a default Text if none exist
			m_pTexts[_szName] = pText;
			return pText;
		}
		else
		{
			if (_bCloned)
			{
				return new Text(*it->second);
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Sprite* DataManager::getSprite(string _szName, bool _bCloned)
	{
		MapStringSprite::iterator it = m_pSprites.find(_szName);
		if (it == m_pSprites.end())
		{
			Sprite* pSprite = new Sprite(); // Create a default Sprite if none exist
			m_pSprites[_szName] = pSprite;
			return pSprite;
		}
		else
		{
			if (_bCloned)
			{
				return new Sprite(*it->second);
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Map* DataManager::getMap(string _szName, bool _bCloned)
	{
		MapStringMap::iterator it = m_pMaps.find(_szName);
		if (it == m_pMaps.end())
		{
			Map* pMap = new Map();

			if (!pMap->loadFromFileJSON(DATAMAPPATH + _szName))
			{
				delete pMap;
				cerr << "Unable to open Map file" << endl;
				return nullptr;
			}
			m_pMaps[_szName] = pMap;
			return pMap;
		}
		else
		{
			if (_bCloned)
			{
				return new Map(*it->second);
			}
			else
			{
				return it->second;
			}
		}
	}

	Agent* DataManager::getAgent(string _szName, bool _bCloned)
	{
		MapStringAgent::iterator it = m_pAgents.find(_szName);
		if (it == m_pAgents.end())
		{
			Agent* pAgent = new Agent();

			if (!pAgent->loadFromFileJSON(DATAAGENTPATH + _szName))
			{
				delete pAgent;
				cerr << "Unable to open Agent file" << endl;
				return nullptr;
			}
			m_pAgents[_szName] = pAgent;
			return pAgent;
		}
		else
		{
			if (_bCloned)
			{
				return new Agent(*it->second);
			}
			else
			{
				return it->second;
			}
		}
	}

	Animation* DataManager::getAnimation(string _szName, bool _bCloned)
	{
		MapStringAnimation::iterator it = m_pAnimations.find(_szName);
		if (it == m_pAnimations.end())
		{
			Animation* pAnimation = new Animation();

			if (!pAnimation->loadFromFileJSON(DATAANIMATIONPATH + _szName))
			{
				delete pAnimation;
				cerr << "Unable to open Animation" << endl;
				return nullptr;
			}
			m_pAnimations[_szName] = pAnimation;
			return pAnimation;
		}
		else
		{
			if (_bCloned)
			{
				return new Animation(*it->second);
			}
			else
			{
				return it->second;
			}
		}
	}

	ActionTable* DataManager::getActionTable(string _szName, bool _bCloned)
	{
		MapStringActionTable::iterator it = m_pActionTables.find(_szName);
		if (it == m_pActionTables.end())
		{
			ActionTable* pActionTable = new ActionTable(); // Create a default ActionTable if none exist
			if (!pActionTable->loadFromFileJSON(DATAANIMATIONPATH + _szName))
			{
				delete pActionTable;
				cerr << "Unable to open ActionTable" << endl;
				return nullptr;
			}
			m_pActionTables[_szName] = pActionTable;
			return pActionTable;
		}
		else
		{
			if (_bCloned)
			{
				//return new ActionTable(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	Vehicle* DataManager::getVehicle(string _szName, bool _bCloned)
	{
		MapStringVehicle::iterator it = m_pVehicles.find(_szName);
		if (it == m_pVehicles.end())
		{
			Vehicle* pVehicle = new Vehicle(); // Create a default Vehicle if none exist
			if (!pVehicle->loadFromFileJSON(DATAAGENTPATH + _szName))
			{
				delete pVehicle;
				cerr << "Unable to open Vehicle" << endl;
				return nullptr;
			}
			m_pVehicles[_szName] = pVehicle;
			return pVehicle;
		}
		else
		{
			if (_bCloned)
			{
				//return new Vehicle(it->second); // CB is it useful to clone?
			}
			else
			{
				return it->second;
			}
		}
		return nullptr;
	}

	void DataManager::clear()
	{
		if (!m_bIsCleared)
		{
			m_bIsCleared = true;
		}
		else
		{
			return;
		}

		MapStringFont::iterator itFont = m_pFonts.begin();
		while (itFont != m_pFonts.end()) {
			delete (*itFont).second;
			itFont = m_pFonts.erase(itFont);
		}

		MapStringTexture::iterator itTexture = m_pTextures.begin();
		while (itTexture != m_pTextures.end()) {
			delete (*itTexture).second;
			itTexture = m_pTextures.erase(itTexture);
		}

		MapStringColor::iterator itColor = m_pColors.begin();
		while (itColor != m_pColors.end()) {
			delete (*itColor).second;
			itColor = m_pColors.erase(itColor);
		}

		MapStringText::iterator itText = m_pTexts.begin();
		while (itText != m_pTexts.end()) {
			delete (*itText).second;
			itText = m_pTexts.erase(itText);
		}

		MapStringSprite::iterator itSprite = m_pSprites.begin();
		while (itSprite != m_pSprites.end()) {
			delete (*itSprite).second;
			itSprite = m_pSprites.erase(itSprite);
		}
		
		MapStringMap::iterator itMap = m_pMaps.begin();
		while (itMap != m_pMaps.end()) {
			delete (*itMap).second;
			itMap = m_pMaps.erase(itMap);
		}

		MapStringAgent::iterator itAgent = m_pAgents.begin();
		while (itAgent != m_pAgents.end()) {
			delete (*itAgent).second;
			itAgent = m_pAgents.erase(itAgent);
		}

		MapStringAnimation::iterator itAnimation = m_pAnimations.begin();
		while (itAnimation != m_pAnimations.end()) {
			delete (*itAnimation).second;
			itAnimation = m_pAnimations.erase(itAnimation);
		}

		MapStringActionTable::iterator itActionTable = m_pActionTables.begin();
		while (m_pActionTables.size()) {
			delete (*itActionTable).second;
			itActionTable = m_pActionTables.erase(itActionTable);
		}

		MapStringVehicle::iterator itVehicle = m_pVehicles.begin();
		while (m_pVehicles.size()) {
			delete (*itVehicle).second;
			itVehicle = m_pVehicles.erase(itVehicle);
		}

	}

} // namespace crea
