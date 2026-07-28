#include "../../../include/imgdraw/canvas/ICanvas.hpp"

namespace imgdraw
{
	ICanvas::ICanvas(unsigned int width, unsigned int height, std::string title) : m_width(width), m_height(height), m_title(title) {}
}