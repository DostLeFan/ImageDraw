#include "../../../include/imgdraw/shape/Point.hpp"

namespace imgdraw
{
	Point::Point(float x, float y, float thickness, Color color) : m_x(x), m_y(y), m_thickness(thickness), m_color(color) {}
	
	bool Point::isEqual(Point const& src) const
	{
		bool xEqual = (m_x == src.m_x);
		bool yEqual = (m_y == src.m_y);
		bool thicknessEqual = (m_thickness == src.m_thickness);
		bool colorEqual = (m_color == src.m_color);
		
		return xEqual && yEqual && thicknessEqual && colorEqual;
	}
}