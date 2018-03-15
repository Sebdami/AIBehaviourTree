#include "stdafx.h"

#include "Data\Animation.h"

#include "json\json.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace crea
{
	Animation::Animation() : m_texture(NULL), m_duration(1.0), m_bLooping(true), m_fSpeed(1.0f)
	{

	}

	void Animation::addFrame(IntRect rect)
	{
		m_frames.push_back(rect);
	}

	void Animation::setSpriteSheet(Texture& texture)
	{
		m_texture = &texture;
	}

	void Animation::adjustToTranslationSpeed(float _fTranslationSpeed)
	{
		if (m_iTranslationSpeed)
		{
			m_fSpeed = _fTranslationSpeed / m_iTranslationSpeed;
		}
	}

	bool Animation::loadFromFileJSON(string _filename)
	{
		m_szName = _filename;

		Json::Value root;
		std::ifstream config_doc(_filename, std::ifstream::binary);
		config_doc >> root;

		Json::Value spritesheet = root["spritesheet"];
		setSpriteSheet(*DataManager::getSingleton()->getTexture(spritesheet.asString()));

		Json::Value frames = root["frames"];
		for (unsigned int iFrame = 0; iFrame < frames.size(); ++iFrame)
		{
			Json::Value frame = frames[iFrame];
			addFrame(IntRect(frame["x"].asInt(), frame["y"].asInt(), frame["w"].asInt(), frame["h"].asInt()));
		}

		Json::Value loop = root["loop"];
		if (loop.isBool())
		{
			m_bLooping = loop.asBool();
		}

		Json::Value multiframes = root["multiframes"];
		for (unsigned int imultiFrame = 0; imultiFrame < multiframes.size(); ++imultiFrame)
		{
			Json::Value multiframe = multiframes[imultiFrame];
			for (int i = 0; i < multiframe["frames"].asInt(); i++)
			{
				addFrame(IntRect(	multiframe["x"].asInt()+i*multiframe["offsetx"].asInt(), 
									multiframe["y"].asInt()+i*multiframe["offsety"].asInt(), 
									multiframe["w"].asInt(), 
									multiframe["h"].asInt()));
			}
		}
		
		// Translation Speed
		Json::Value translationspeed = root["translationspeed"];
		m_iTranslationSpeed = translationspeed.asInt();

		return true;
	}
} // namespace crea