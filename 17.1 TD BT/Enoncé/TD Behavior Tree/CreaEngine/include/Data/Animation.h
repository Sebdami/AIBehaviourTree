/***************************************************/
/* Nom:	Animation.h
/* Description: Animation
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Animation_H_
#define __Animation_H_

#include <vector>

namespace crea
{
	class IntRect;

	class CREAENGINE_API Animation
	{
	public:
		Animation();

		void addFrame(IntRect rect);
		void setSpriteSheet(Texture& texture);
		inline void setSpeed(float _fSpeed)		{ m_fSpeed = _fSpeed; }

		inline Texture* getSpriteSheet()		{ return m_texture; }
		inline size_t getSize()					{ return m_frames.size(); }
		inline IntRect& getFrame(std::size_t n) { return m_frames[n]; }
		inline string getName()					{ return m_szName; }
		inline Time& getDuration()				{ return m_duration; }
		inline bool getLooping()				{ return m_bLooping; }
		inline float getSpeed()					{ return m_fSpeed; }
		
		void adjustToTranslationSpeed(float _fTranslationSpeed);

		bool loadFromFileJSON(string _filename);

	private:
		string m_szName;
		vector<IntRect> m_frames;
		Texture* m_texture;
		Time m_duration;
		bool m_bLooping;
		float m_fSpeed;
		int m_iTranslationSpeed;
	};

} // namespace crea

#endif // __Animation_H_