#include "stdafx.h"

#include "Graphics\Shape.h"

#include <SFML/Graphics.hpp>

namespace crea
{

	Shape::Shape()
	{
		m_pShape = nullptr;
	}

	Shape::~Shape()
	{
	}

	void Shape::draw()
	{
		GameManager::getSingleton()->getWindow()->draw(*m_pShape);
	}

	void Shape::setColor(Color* _pColor)
	{
		m_pShape->setFillColor(*_pColor->m_pColor);
	}

	void Shape::setOutlineColor(Color* _pColor)
	{
		m_pShape->setOutlineColor(*_pColor->m_pColor);
	}

	void Shape::setOutlineThickness(float _fPixels)
	{
		m_pShape->setOutlineThickness(_fPixels);
	}

	void Shape::setPosition(float _x, float _y)
	{
		m_pShape->setPosition(_x, _y);
	}

	// RectangleShape
	RectangleShape::RectangleShape()
	{
		m_pShape = new sf::RectangleShape;
	}

	RectangleShape::~RectangleShape()
	{
		delete m_pShape;
	}

	void RectangleShape::setSize(float _x, float _y)
	{
		((sf::RectangleShape*)m_pShape)->setSize(sf::Vector2f(_x, _y));
	}

	// CircleShape
	CircleShape::CircleShape()
	{
		m_pShape = new sf::CircleShape;
	}

	CircleShape::~CircleShape()
	{
		delete m_pShape;
	}

	void CircleShape::setRadius(float _r)
	{
		((sf::CircleShape*)m_pShape)->setRadius(_r);
	}	
	
	// ArrowShape
	ArrowShape::ArrowShape()
	{
		sf::ConvexShape* pArrow = new sf::ConvexShape(3);
		pArrow->setPoint(0, sf::Vector2f(0.0f, 0.0f));
		pArrow->setPoint(1, sf::Vector2f(0.0f, 1.0f));
		pArrow->setPoint(2, sf::Vector2f(1.0f, 0.5f));
		m_pShape = pArrow;
	}

	ArrowShape::~ArrowShape()
	{
		delete m_pShape;
	}

	void ArrowShape::setSize(float _x, float _y)
	{
		sf::ConvexShape* pArrow = (sf::ConvexShape*) m_pShape;
		pArrow->setScale(sf::Vector2f(_x, _y));
	}

	void ArrowShape::setStartAndEnd(float _xStart, float _yStart, float _xEnd, float _yEnd)
	{
		float dx = _xEnd - _xStart;
		float dy = _yEnd - _yStart;
		Vector2f v(dx, dy);
		Vector2f right(1.0f, 0.f);
		float fAngle = right.angle(v)*180/3.14f;
		fAngle = dy > 0.f ? fAngle : -fAngle;
		sf::ConvexShape* pArrow = (sf::ConvexShape*) m_pShape;
		pArrow->setPosition(_xStart, _yStart);
		sf::Vector2f scale = pArrow->getScale();
		scale.x = v.length();
		pArrow->setScale(scale);
		pArrow->setOrigin(0.0f, 0.5f);
		pArrow->setRotation(fAngle);
	}

	// Line
	Line::Line()
	{
		sf::ConvexShape* pArrow = new sf::ConvexShape(2);
		pArrow->setPoint(0, sf::Vector2f(0.0f, 0.0f));
		pArrow->setPoint(1, sf::Vector2f(0.0f, 1.0f));
		m_pShape = pArrow;
	}

	Line::~Line()
	{
		delete m_pShape;
	}
	
	void Line::setStartAndEnd(float _xStart, float _yStart, float _xEnd, float _yEnd)
	{
		float dx = _xEnd - _xStart;
		float dy = _yEnd - _yStart;
		Vector2f v(dx, dy);
		Vector2f right(1.0f, 0.f);
		float fAngle = right.angle(v) * 180 / 3.14f;
		fAngle = dy > 0.f ? fAngle : -fAngle;
		sf::ConvexShape* pArrow = (sf::ConvexShape*) m_pShape;
		pArrow->setPosition(_xStart, _yStart);
		sf::Vector2f scale = pArrow->getScale();
		scale.x = v.length();
		pArrow->setScale(scale);
		pArrow->setRotation(fAngle);
	}
} // namespace crea
