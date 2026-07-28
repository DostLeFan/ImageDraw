#include "../../../include/imgdraw/shape/Shape.hpp"

namespace imgdraw
{
	Shape::Shape(float x, float y, Color color) : m_origin(x, y, 5.0f, color) {}
	
	Shape::Shape(Point origin) : m_origin(origin) {}
}