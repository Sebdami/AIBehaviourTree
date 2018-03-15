/***************************************************/
/* Nom:	Shape.h
/* Description: Shape
/* Auteur: BRUNEAU Colin
/***************************************************/
#ifndef _Shape_H
#define _Shape_H

namespace sf
{
	class Shape;
}
namespace crea
{

	class CREAENGINE_API Shape
	{
	public:
		sf::Shape* m_pShape;

		Shape();

		~Shape();

		virtual void draw();

		virtual void setColor(Color* _pColor);

		virtual void setOutlineColor(Color* _pColor);

		virtual void setOutlineThickness(float _fPixels);

		virtual void setPosition(float _x, float _y);
	};

	class CREAENGINE_API RectangleShape : public Shape
	{
	public:

		RectangleShape();

		~RectangleShape();

		virtual void setSize(float _x, float _y);
	};

	class CREAENGINE_API CircleShape : public Shape
	{
	public:

		CircleShape();

		~CircleShape();

		virtual void setRadius(float _r);
	};

	class CREAENGINE_API ArrowShape : public Shape
	{
	public:

		ArrowShape();

		~ArrowShape();

		virtual void setSize(float _x, float _y);

		void setStartAndEnd(float _xStart, float _yStart, float _xEnd, float _yEnd);
	};

	class CREAENGINE_API Line : public Shape
	{
	public:

		Line();

		~Line();
		
		void setStartAndEnd(float _xStart, float _yStart, float _xEnd, float _yEnd);
	};
} // namespace crea

#endif // _Shape_H