/***************************************************/
/* Nom:	TextRenderer.h
/* Description: TextRenderer
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef __TextRenderer_H_
#define __TextRenderer_H_

#include "Core\Component.h"

namespace sf
{
	class Text;
}

namespace crea
{

	class CREAENGINE_API TextRenderer : public Component
	{
		Text* m_pText;

	public:
		TextRenderer();
		virtual ~TextRenderer();

		inline void setText(Text* _pText) { m_pText = _pText; }

		virtual bool init();
		virtual bool update();
		virtual bool draw();
		virtual bool quit();

		virtual Component* clone() { TextRenderer* p = new TextRenderer(*this); p->m_pText = new Text(*m_pText); return p; }
	};

} // namespace crea

#endif
