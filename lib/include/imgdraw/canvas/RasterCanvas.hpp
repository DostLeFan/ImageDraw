#ifndef DEF_RASTERCANVAS
#define DEF_RASTERCANVAS

#include <vector>
#include <cstdint>
#include "ICanvas.hpp"

namespace imgdraw
{
	class RasterCanvas : ICanvas
	{
		public:
			RasterCanvas(unsigned int width, unsigned int height, std::string title = "", Color background = Color::White);
			virtual ~RasterCanvas() = default;
			
			virtual void drawPoint(Point const& point) override;
			virtual void drawCircle(Point const& center, float radius, bool filled = true) override;
			virtual void drawLine(Point const& from, Point const& to) override;
			virtual void drawArc(Point const& center, float radius, float startAngleDeg, float endAngleDeg) override;
			
			virtual void fill(std::vector<Point> const& outline) override;
			
			virtual DrawError save(std::string const& path) const override;
		
		private:
			std::vector<uint8_t> m_pixels; // RGBA8, size = width * height * 4.
			
			void blendPixel(int x, int y, Color const& color, float coverage);
			void drawThickSegment(float x0, float y0, float x1, float y1, float thickness, Color const& color);
			static bool isPointInPolygon(float px, float py, std::vector<Point> const& outline);
	};
}

#endif // DEF_RASTERCANVAS