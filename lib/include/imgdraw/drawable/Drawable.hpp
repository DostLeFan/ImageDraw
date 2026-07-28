#ifndef DEF_DRAWABLE
#define DEF_DRAWABLE

#include <memory>

namespace imgdraw
{
	class ICanvas;
	
	class Drawable
	{
		public:
			virtual ~Drawable() = default;
			
			virtual void draw(ICanvas& canvas) const = 0;
			virtual std::unique_ptr<Drawable> clone() const = 0;
	};
}

#endif // DEF_DRAWABLE