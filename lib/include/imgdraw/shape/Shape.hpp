#ifndef DEF_SHAPE
#define DEF_SHAPE

#include "Point.hpp"
#include "../type/Color.hpp"

namespace imgdraw
{
	class Shape
	{
		public:
			Shape(float x, float y, Color color = Color::Black);
			Shape(Point origin);
			virtual ~Shape() = default;
			
			inline Point getOrigin() const { return m_origin; }
			inline Color getColor() const { return m_origin.getColor(); }
			inline bool getFill() const { return m_fill; }
			
			inline void setOrigin(float x, float y) { m_origin.setXY(x, y); }
			inline void setOrigin(Point origin) { m_origin = origin; }
			inline void setColor(Color color) { m_origin.setColor(color); }
			inline void setFill(bool fill) { m_fill = fill; }
			
			inline friend bool operator==(Shape const& lhs, Shape const& rhs) { return lhs.isEqual(rhs); }
			inline friend bool operator!=(Shape const& lhs, Shape const& rhs) { return !lhs.isEqual(rhs); }
		
		private:
			Point m_origin;
			bool m_fill;
		
		protected:
			virtual bool isEqual(Shape const& src) const = 0;
	};
}

#endif // DEF_SHAPE