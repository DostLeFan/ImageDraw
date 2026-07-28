#include "../../../include/imgdraw/canvas/RasterCanvas.hpp"

#ifndef DEF_RASTERCANVAS
#define DEF_RASTERCANVAS

#include <vector>
#include <algorithm>
#include <utility>
#include <cctype>
#include <cmath>
#include <cstdint>
#include "ICanvas.hpp"

namespace imgdraw
{
	namespace
	{
		inline uint8_t toByte(float v)
		{
			return static_cast<uint8_t>(std::round(std::clamp(v, 0.0f, 1.0f) * 255.0f));
		}
		
		inline std::string toLower(std::string s)
		{
			std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			
			return s;
		}
		
		inline std::string extensionOf(std::string const& path)
		{
			auto const dot = path.find_last_of('.');
			
			if(dot == std::string::npos)
				return "";
			
			return toLower(path.substr(dot + 1));
		}
		
		// Distance from a point (px, py) to the line segment [x0, y0]–[x1, y1].
		inline float distancePointToSegment(float px, float py, float x0, float y0, float x1, float y1)
		{
			float const dx = x1 - x0;
			float const dy = y1 - y0;
			float const lenSq = dx * dx + dy * dy;
			
			float const t = lenSq > 0.0f ? std::clamp(((px - x0) * dx + (py - y0) * dy) / lenSq, 0.0f, 1.0f) : 0.0f;
			
			float const projX = x0 + t * dx;
			float const projY = y0 + t * dy;
			
			float const ddx = px - projX;
			float const ddy = py - projY;
			
			return std::sqrt(ddx * ddx + ddy * ddy);
		}
	}
	
	RasterCanvas::RasterCanvas(unsigned int width, unsigned int height, std::string title, Color background) : ICanvas(width, height, std::move(title)), m_pixels(static_cast<std::size_t>(width) * height * 4)
	{
		for(std::size_t i=0;i<m_pixels.size();i+=4)
		{
			m_pixels[(i + 0)] = toByte(background.r());
			m_pixels[(i + 1)] = toByte(background.g());
			m_pixels[(i + 2)] = toByte(background.b());
			m_pixels[(i + 3)] = toByte(background.a());
		}
	}
	
	
	void RasterCanvas::drawPoint(Point const& point)
	{
		drawCircle(point, point.getThickness() / 2.0f);
	}
	
	void RasterCanvas::drawCircle(Point const& center, float radius, bool filled)
	{
		if(!filled)
		{
			drawArc(center, radius, 0.0f, 360.0f);
			
			return;
		}
		
		constexpr int SEGMENTS = 64; // Sufficient to remain smooth at the size of a standard raster circle.
		
		std::vector<Point> outline;
		outline.reserve(SEGMENTS);
		
		for(int i=0;i<SEGMENTS;++i)
		{
			float const angle = (2.0f * 3.14159265358979323846f * static_cast<float>(i)) / SEGMENTS;
			
			outline.emplace_back(center.getX() + radius * std::cos(angle), center.getY() + radius * std::sin(angle), 0.0f, center.getColor());
		}
		
		fill(outline);
	}
	
	void RasterCanvas::drawLine(Point const& from, Point const& to)
	{
		drawThickSegment(from.getX(), from.getY(), to.getX(), to.getY(), from.getThickness(), from.getColor());
	}
	
	void RasterCanvas::drawArc(Point const& center, float radius, float startAngleDeg, float endAngleDeg)
	{
		float const thickness = center.getThickness();
		float const halfThickness = thickness / 2.0f;
		Color const color = center.getColor();
		
		// Normalises angles within [0, 360[. Special case: an arc of 360deg
		// or more (e.g. an unfilled drawCircle) must cover the entire circle; otherwise,
		// normalise(360) == normalise(0) == 0 would cause the angle test to fail.
		bool const isFullCircle = (endAngleDeg - startAngleDeg) >= 360.0f;
		
		auto normalize = [](float deg) { deg = std::fmod(deg, 360.0f); return deg < 0.0f ? deg + 360.0f : deg; };
		float const start = normalize(startAngleDeg);
		float const end = normalize(endAngleDeg);
		
		int const minX = static_cast<int>(std::floor(center.getX() - radius - halfThickness - 1.0f));
		int const maxX = static_cast<int>(std::ceil(center.getX() + radius + halfThickness + 1.0f));
		int const minY = static_cast<int>(std::floor(center.getY() - radius - halfThickness - 1.0f));
		int const maxY = static_cast<int>(std::ceil(center.getY() + radius + halfThickness + 1.0f));
		
		for(int y=minY;y<=maxY;++y)
		{
			for(int x=minX;x<=maxX;++x)
			{
				float const dx = (static_cast<float>(x) + 0.5f) - center.getX();
				float const dy = (static_cast<float>(y) + 0.5f) - center.getY();
				float const dist = std::sqrt(dx * dx + dy * dy);
				
				// Radial coverage: anti-aliasing band on either side of the radius (the ring).
				float const radialCoverage = std::clamp(halfThickness - std::abs(dist - radius) + 0.5f, 0.0f, 1.0f);
				
				if(radialCoverage <= 0.0f)
					continue;
				
				// Angular coverage: the pixel must fall within [start, end] (0deg = east, clockwise).
				float angle = normalize(std::atan2(dy, dx) * 180.0f / 3.14159265358979323846f);
				
				bool const inRange = isFullCircle || (start <= end ? (angle >= start && angle <= end) : (angle >= start || angle <= end));
				
				if(inRange)
					blendPixel(x, y, color, radialCoverage);
			}
		}
	}
	
	
	void RasterCanvas::fill(std::vector<Point> const& outline)
	{
		if(outline.size() < 3)
			return;
		
		Color const fillColor = outline.front().getColor();
		
		float minXf = outline.front().getX();
		float maxXf = minXf;
		float minYf = outline.front().getY();
		float maxYf = minYf;
		
		for(auto const& p : outline)
		{
			minXf = std::min(minXf, p.getX());
			maxXf = std::max(maxXf, p.getX());
			minYf = std::min(minYf, p.getY());
			maxYf = std::max(maxYf, p.getY());
		}
		
		int const minX = static_cast<int>(std::floor(minXf));
		int const maxX = static_cast<int>(std::ceil(maxXf));
		int const minY = static_cast<int>(std::floor(minYf));
		int const maxY = static_cast<int>(std::ceil(maxYf));
		
		int constexpr SUPERSAMPLE = 4; // 4x4 grid = 16 samples per pixel, for anti-aliasing of the polygon edges.
		
		for(int y=minY;y<=maxY;++y)
		{
			for(int x=minX;x<=maxX;++x)
			{
				int inside = 0;
				
				for(int sy=0;sy<SUPERSAMPLE;++sy)
				{
					for(int sx=0;sx<SUPERSAMPLE;++sx)
					{
						float const px = static_cast<float>(x) + (static_cast<float>(sx) + 0.5f) / SUPERSAMPLE;
						float const py = static_cast<float>(y) + (static_cast<float>(sy) + 0.5f) / SUPERSAMPLE;
						
						if(isPointInPolygon(px, py, outline))
							++inside;
					}
				}
				
				float const coverage = static_cast<float>(inside) / (SUPERSAMPLE * SUPERSAMPLE);
				
				if(coverage > 0.0f)
					blendPixel(x, y, fillColor, coverage);
			}
		}
	}
	
	
	DrawError RasterCanvas::save(std::string const& path) const
	{
		if(path.empty())
			return DrawError::InvalidPath;
		
		std::string const ext = extensionOf(path);
		int const width = static_cast<int>(getWidth());
		int const height = static_cast<int>(getHeight());
		
		if(ext == "png")
		{
			int const ok = stbi_write_png(path.c_str(), width, height, 4, m_pixels.data(), width * 4);
			
			return ok != 0 ? DrawError::Success : DrawError::WriteFailed;
		}
		
		if(ext == "jpg" || ext == "jpeg")
		{
			// JPEG does not support alpha channels: the image is composited onto an opaque white background.
			std::vector<uint8_t> rgb(static_cast<std::size_t>(width) * height * 3);
			
			for(std::size_t i=0,j=0;i<m_pixels.size();i+=4,j+=3)
			{
				float const a = m_pixels[i + 3] / 255.0f;
				
				rgb[j + 0] = toByte((m_pixels[i + 0] / 255.0f) * a + 1.0f * (1.0f - a));
				rgb[j + 1] = toByte((m_pixels[i + 1] / 255.0f) * a + 1.0f * (1.0f - a));
				rgb[j + 2] = toByte((m_pixels[i + 2] / 255.0f) * a + 1.0f * (1.0f - a));
			}
			
			int const ok = stbi_write_jpg(path.c_str(), width, height, 3, rgb.data(), 90);
			
			return ok != 0 ? DrawError::Success : DrawError::WriteFailed;
		}
		
		return DrawError::UnsupportedFormat;
	}
	
	
	void RasterCanvas::blendPixel(int x, int y, Color const& color, float coverage)
	{
		if(x < 0 || y < 0 || static_cast<unsigned int>(x) >= getWidth() || static_cast<unsigned int>(y) >= getHeight())
			return;
		
		float const srcA = std::clamp(color.a() * coverage, 0.0f, 1.0f);
		
		if(srcA <= 0.0f)
			return;
		
		std::size_t const idx = (static_cast<std::size_t>(y) * getWidth() + static_cast<std::size_t>(x)) * 4;
		
		float const dstR = m_pixels[idx + 0] / 255.0f;
		float const dstG = m_pixels[idx + 1] / 255.0f;
		float const dstB = m_pixels[idx + 2] / 255.0f;
		float const dstA = m_pixels[idx + 3] / 255.0f;
		
		// "Source over" alpha blending (standard formula of Porter-Duff).
		float const outA = srcA + dstA * (1.0f - srcA);
		
		if(outA <= 0.0f)
		{
			m_pixels[idx + 0] = m_pixels[idx + 1] = m_pixels[idx + 2] = m_pixels[idx + 3] = 0;
			
			return;
		}
		
		float const outR = (color.r() * srcA + dstR * dstA * (1.0f - srcA)) / outA;
		float const outG = (color.g() * srcA + dstG * dstA * (1.0f - srcA)) / outA;
		float const outB = (color.b() * srcA + dstB * dstA * (1.0f - srcA)) / outA;
		
		m_pixels[idx + 0] = toByte(outR);
		m_pixels[idx + 1] = toByte(outG);
		m_pixels[idx + 2] = toByte(outB);
		m_pixels[idx + 3] = toByte(outA);

	}
	
	void RasterCanvas::drawThickSegment(float x0, float y0, float x1, float y1, float thickness, Color const& color)
	{
		float const halfThickness = thickness / 2.0f;
		
		int const minX = static_cast<int>(std::floor(std::min(x0, x1) - halfThickness - 1.0f));
		int const maxX = static_cast<int>(std::ceil(std::max(x0, x1) + halfThickness + 1.0f));
		int const minY = static_cast<int>(std::floor(std::min(y0, y1) - halfThickness - 1.0f));
		int const maxY = static_cast<int>(std::ceil(std::max(y0, y1) + halfThickness + 1.0f));
		
		for(int y=minY;y<=maxY;++y)
		{
			for(int x=minX;x<=maxX;++x)
			{
				float const dist = distancePointToSegment(static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f, x0, y0, x1, y1);
				float const coverage = std::clamp(halfThickness - dist + 0.5f, 0.0f, 1.0f);
				
				if(coverage > 0.0f)
					blendPixel(x, y, color, coverage);
			}
		}
	}
	
	bool RasterCanvas::isPointInPolygon(float px, float py, std::vector<Point> const& outline)
	{
		bool inside = false;
		std::size_t const n = outline.size();
		
		for(std::size_t i=0,j=(n-1);i<n;j=i++)
		{
			float const xi = outline[i].getX();
			float const yi = outline[i].getY();
			float const xj = outline[j].getX();
			float const yj = outline[j].getY();
			
			bool const crosses = (yi > py) != (yj > py);
			
			if(crosses && px < (xj - xi) * (py - yi) / (yj - yi) + xi)
				inside = !inside;
		}
		
		return inside;
	}
}

#endif // DEF_RASTERCANVAS