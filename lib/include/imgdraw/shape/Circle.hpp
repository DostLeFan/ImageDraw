#ifndef DEF_CIRCLE
#define DEF_CIRCLE

#include "Shape.hpp"
#include "../drawable/Drawable.hpp"

namespace imgdraw
{
	class Circle : public Shape, public Drawable
	{
		public:
			Circle(float x, float y, float radius, Color color = Color::Black);
			Circle(Point origin, float radius);
			
			inline float getRadius() const { return m_radius; }
			inline void setRadius(float radius) { m_radius = radius; }
			
			void draw(ICanvas& canvas) const override;
			std::unique_ptr<Drawable> clone() const override;
		
		protected:
			bool isEqual(Shape const& src) const override;
		
		private:
			float m_radius;
	};
}

#endif // DEF_CIRCLE