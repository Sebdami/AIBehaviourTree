/***************************************************/
/* Nom:	ShapeRenderer.h
/* Description: ShapeRenderer
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __ShapeRenderer_H_
#define __ShapeRenderer_H_

#include "Core\Component.h"

namespace sf
{
	class Shape;
}

namespace crea
{

	class CREAENGINE_API ShapeRenderer : public Component
	{
		Shape* m_pShape;

	public:
		ShapeRenderer();
		virtual ~ShapeRenderer();

		inline void setShape(Shape* _pShape) { m_pShape = _pShape; }

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

	};

} // namespace crea

#endif
