#include "stdafx.h"

#include "Graphics\ShapeRenderer.h"
#include "Core\GameManager.h"
#include <SFML/Graphics.hpp>

namespace crea
{
	ShapeRenderer::ShapeRenderer()
	{
		m_pShape = nullptr;
	}

	ShapeRenderer::~ShapeRenderer()
	{

	}

	bool ShapeRenderer::init()
	{
		return true;
	}
	
	bool ShapeRenderer::update()
	{
		return true;
	}
	
	bool ShapeRenderer::draw()
	{
		if (m_pShape)
		{
			// Draw
			GameManager::getSingleton()->getWindow()->draw(*m_pShape->m_pShape);
		}

		return true;
	}
	
	bool ShapeRenderer::quit()
	{
		return true;
	}

} // namespace crea
