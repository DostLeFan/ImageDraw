#ifndef DEF_POINT
#define DEF_POINT

#include "../type/Color.hpp"

namespace imgdraw
{
	class Point
	{
		public:
			Point(float x, float y, float thickness = 5.0f, Color color = Color::Black);
			virtual ~Point() = default;
			
			inline float getX() const { return m_x; }
			inline float getY() const { return m_y; }
			inline float getThickness() const { return m_thickness; }
			inline Color getColor() const { return m_color; }
			
			inline void setX(float x) { m_x = x; }
			inline void setY(float y) { m_y = y; }
			inline void setXY(float x, float y) { setX(x); setY(y); }
			inline void setThickness(float thickness) { m_thickness = thickness; }
			inline void setColor(Color color) { m_color = color; }
			
			inline friend bool operator==(Point const& lhs, Point const& rhs) { return lhs.isEqual(rhs); }
			inline friend bool operator!=(Point const& lhs, Point const& rhs) { return !lhs.isEqual(rhs); }
		
		private:
			float m_x;
			float m_y;
			float m_thickness;
			Color m_color;
		
		protected:
			virtual bool isEqual(Point const& src) const;
	};
}

#endif // DEF_POINT