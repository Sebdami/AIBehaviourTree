/***************************************************/
/* Nom:	Animator.h
/* Description: Animator
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __Animator_H_
#define __Animator_H_

#include "Core\Component.h"
#include "Core\Math.h"
#include "Core\TimeManager.h"
#include "Graphics\SpriteRenderer.h"
#include "Data\Animation.h"

namespace crea
{

	class CREAENGINE_API Animator : public Component
	{
		Sprite* m_pSprite;
		Animation* m_animation;
		Time m_frameTime;
		Time m_currentTime;
		int m_currentFrame;
		bool m_isPaused;
		Texture* m_texture;

	public:
		Animator();
		virtual ~Animator();

		void setSprite(Sprite* _pSprite);
		void setAnimation(Animation& animation);
		void setFrameTime(Time& time);

		Animation* getAnimation();
		Time getFrameTime();
		IntRect getFrame();
		Texture* getTexture() { return m_texture; }
		
		bool isPlaying();

		void play();
		void play(Animation& animation);
		void pause();
		void stop();

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone() { return new Animator(*this); }
	};

} // namespace crea

#endif
