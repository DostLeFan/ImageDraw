#ifndef DEF_SCENE
#define DEF_SCENE

#include <vector>
#include <memory>
#include <utility>
#include "../drawable/Drawable.hpp"

namespace imgdraw
{
	class ICanvas;
	
	class Scene
	{
		public:
			Scene() = default;
			Scene(Scene const& src) { *this = src; }
			Scene(Scene&&) = default;
			~Scene() = default;
			
			Scene& operator=(Scene const& src);
			Scene& operator=(Scene&&) = default;
			
			inline void add(std::unique_ptr<Drawable> drawable) { m_drawables.push_back(std::move(drawable)); }
			inline std::size_t size() const { return m_drawables.size(); }
			inline bool isEmpty() const { return m_drawables.empty(); }
			void renderTo(ICanvas& canvas) const;
		
		private:
			std::vector<std::unique_ptr<Drawable> > m_drawables;
	};
}

#endif // DEF_SCENE