#ifndef DEF_ICANVAS
#define DEF_ICANVAS

#include <string>
#include "../shape/Point.hpp"
#include "../type/Error.hpp"

namespace imgdraw
{
	class ICanvas
	{
		public:
			virtual ~ICanvas() = default;
			
			inline unsigned int getWidth() const { return m_width; }
			inline unsigned int getHeight() const { return m_height; }
			inline std::string getTitle() const { return m_title; }
			
			inline void setWidth(unsigned int width) { m_width = width; }
			inline void setHeight(unsigned int height) { m_height = height; }
			inline void setTitle(std::string title) { m_title = title; }
			
			// Drawing primitives. Color is carried by Point itself.
			virtual void drawPoint(Point const& point) = 0;
			virtual void drawCircle(Point const& point, float radius) = 0;
			
			// Writing on disk. Format can be deducted, depending on implementation and/or extension.
			virtual DrawError save(std::string const& path) const = 0;
		
		private:
			unsigned int m_width;
			unsigned int m_height;
			std::string m_title;
		
		protected:
			ICanvas(unsigned int width, unsigned int height, std::string title = "");
	};
}

#endif // DEF_ICANVAS