#include "../../../include/imgdraw/shape/Circle.hpp"
#include "../../../include/imgdraw/canvas/ICanvas.hpp"

#include <memory>

namespace imgdraw
{
	Circle::Circle(float x, float y, float radius, Color color) : Shape(x, y, color), m_radius(radius) {}
	
	Circle::Circle(Point origin, float radius) : Shape(origin), m_radius(radius) {}
	
	void Circle::draw(ICanvas& canvas) const
	{
		canvas.drawCircle(getOrigin(), m_radius, this->getFill());
	}
	
	std::unique_ptr<Drawable> Circle::clone() const
	{
		return std::make_unique<Circle>(*this);
	}
	
	bool Circle::isEqual(Shape const& src) const
	{
		auto const* other = dynamic_cast<Circle const*>(&src);
		
		if (other == nullptr)
			return false;
		
		return m_radius == other->m_radius && getOrigin().getX() == other->getOrigin().getX() && getOrigin().getY() == other->getOrigin().getY() && getColor() == other->getColor();
	}
}