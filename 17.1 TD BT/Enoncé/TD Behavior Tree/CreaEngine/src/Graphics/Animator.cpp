#include "stdafx.h"

#include "Graphics\Animator.h"
#include "Core\GameManager.h"
#include "Core\TimeManager.h"
#include "Core\EntityManager.h"
#include "Graphics\Sprite.h"

namespace crea
{
	Animator::Animator()
	{
	}

	Animator::~Animator()
	{
	}

	void Animator::setSprite(Sprite* _pSprite)
	{
		m_pSprite = _pSprite;
	}

	void Animator::setAnimation(Animation& animation)
	{
		m_animation = &animation;
		m_currentFrame = 0;
		m_currentTime = 0.f;
	}

	void Animator::setFrameTime(Time& time)
	{
		m_frameTime = time;
	}

	Animation* Animator::getAnimation()
	{
		return m_animation;
	}

	bool Animator::isPlaying()
	{
		return !m_isPaused;
	}

	Time Animator::getFrameTime()
	{
		return m_frameTime;
	}

	IntRect Animator::getFrame()
	{
		if (m_animation)
		{
			return m_animation->getFrame(m_currentFrame);
		}
		return IntRect();
	}

	void Animator::play()
	{
		m_isPaused = false;
	}

	void Animator::play(Animation& animation)
	{
		if (getAnimation() != &animation)
			setAnimation(animation);
		play();
	}

	void Animator::pause()
	{
		m_isPaused = true;
	}

	void Animator::stop()
	{
		m_isPaused = true;
		m_currentFrame = 0;
		m_currentTime = 0.f;
	}

	bool Animator::init()
	{
		// Get Sprite from SpriteRenderer
		SpriteRenderer* pRenderer = getEntity()->getComponent<SpriteRenderer>();
		if (pRenderer)
		{
			m_pSprite = pRenderer->getSprite();
		}
		return true;
	}
	
	bool Animator::update()
	{
		// if not paused and we have a valid animation
		if (!m_isPaused && m_animation)
		{
			// add delta time
			m_currentTime += (TimeManager::getSingleton()->getFrameTime()*m_animation->getSpeed());

			// Get the current animation frame
			float fScaledTime = (float) (m_currentTime.asSeconds() / m_animation->getDuration().asSeconds());
			int iNumFrames = m_animation->getSize();
			m_currentFrame = (int)(fScaledTime * iNumFrames);

			// If the animation is looping, calculate the correct frame
			if (m_animation->getLooping())
			{
				m_currentFrame %= iNumFrames;
			}
			else if (m_currentFrame >= iNumFrames) // If the current frame is greater than the number of frames
			{
				m_currentFrame = iNumFrames - 1; // Show last frame
			}
		}

		if (m_pSprite && m_animation)
		{
			m_pSprite->setTexture(m_animation->getSpriteSheet());
			
			IntRect rect = m_animation->getFrame(m_currentFrame);
			float fFlipH = (rect.getWidth() < 0) ? -1.0f : 1.0f;// width < 0 means flipH
			float fFlipV = (rect.getHeight() < 0) ? -1.0f : 1.0f;// height < 0 means flipV

			m_pSprite->setTextureRect(
				rect.getLeft(),
				rect.getTop(),
				rect.getWidth()*(int)fFlipH,
				rect.getHeight()*(int)fFlipV);
			
			m_pSprite->setScale(fFlipH, fFlipV);
		}
		return true;
	}
	
	bool Animator::draw()
	{				
		return true;
	}
	
	bool Animator::quit()
	{
		return true;
	}

} // namespace crea
