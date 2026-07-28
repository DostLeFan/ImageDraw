#include "../../../include/imgdraw/type/Color.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace imgdraw
{
	Color const Color::White {1.0f, 1.0f, 1.0f, 1.0f};
	Color const Color::Black {0.0f, 0.0f, 0.0f, 1.0f};
	Color const Color::Red {1.0f, 0.0f, 0.0f, 1.0f};
	Color const Color::Green {0.0f, 1.0f, 0.0f, 1.0f};
	Color const Color::Blue {0.0f, 0.0f, 1.0f, 1.0f};
	Color const Color::Yellow {1.0f, 1.0f, 0.0f, 1.0f};
	Color const Color::Orange {1.0f, 0.5f, 0.0f, 1.0f};
	Color const Color::Purple {0.5f, 0.0f, 0.5f, 1.0f};
	Color const Color::Cyan {0.0f, 1.0f, 1.0f, 1.0f};
	Color const Color::Magenta {1.0f, 0.0f, 1.0f, 1.0f};
	
	Color::Color(float r, float g, float b, float a) : m_r(r), m_g(g), m_b(b), m_a(a) {}
	
	
	Color Color::fromYCbCr(float y, float cb, float cr)
	{
		float r = y + 1.402f * cr;
		float g = y - 0.344136f * cb - 0.714136f * cr;
		float b = y + 1.772f * cb;
		
		r = std::clamp<float>(r, 0.0f, 1.0f);
		g = std::clamp<float>(g, 0.0f, 1.0f);
		b = std::clamp<float>(b, 0.0f, 1.0f);
		
		return Color(r, g, b, 1.0f);
	}
	
	Color Color::fromIntYCbCr(int y, int cb, int cr)
	{
		float yF = (static_cast<float>(y) / 255.0f);
		float cbF = (static_cast<float>((cb - 128)) / 255.0f);
		float crF = (static_cast<float>((cr - 128)) / 255.0f);
		
		return fromYCbCr({ yF, cbF, crF });
	}
	
	Color Color::fromHSL(float h, float s, float l)
	{
		h = std::fmod(h, 360.0f);
		
		if(h < 0.0f)
			h += 360.0f;
		
		s = std::clamp(s, 0.0f, 1.0f);
		l = std::clamp(l, 0.0f, 1.0f);
		
		float c = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
		float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
		float m = l - c / 2.0f;
		
		float rPrime = 0.0f;
		float gPrime = 0.0f;
		float bPrime = 0.0f;
		
		if(h >= 0.0f && h < 60.0f)
		{
			rPrime = c;
			gPrime = x;
		}
		else if(h >= 60.0f && h < 120.0f)
		{
			rPrime = x;
			gPrime = c;
		}
		else if(h >= 120.0f && h < 180.0f)
		{
			gPrime = c;
			bPrime = x;
		}
		else if(h >= 180.0f && h < 240.0f)
		{
			gPrime = x;
			bPrime = c;
		}
		else if(h >= 240.0f && h < 300.0f)
		{
			rPrime = x;
			bPrime = c;
		}
		else if(h >= 300.0f && h < 360.0f)
		{
			rPrime = c;
			bPrime = x;
		}
		
		return { rPrime + m, gPrime + m, bPrime + m, 1.0f };
	}
	
	Color Color::fromIntHSL(int h, int s, int l)
	{
		HSL hslFloat = {
			static_cast<float>(h),
			static_cast<float>(s) / 100.0f,
			static_cast<float>(l) / 100.0f
		};
		
		return fromHSL(hslFloat);
	}
	
	Color Color::fromHexRgbaInt(uint32_t hexValue)
	{
		float r = static_cast<float>(((hexValue >> 24) & 0xFF)) / 255.0f;
		float g = static_cast<float>(((hexValue >> 16) & 0xFF)) / 255.0f;
		float b = static_cast<float>(((hexValue >> 8) & 0xFF)) / 255.0f;
		float a = static_cast<float>((hexValue & 0xFF)) / 255.0f;
		
		return Color(r, g, b, a);
	}
	
	Color Color::fromHexRgbaString(std::string hexStr)
	{
		if(!hexStr.empty() && hexStr[0] == '#')
			hexStr = hexStr.substr(1);
		
		if(hexStr.length() == 6)
			hexStr += "FF";
		
		if(hexStr.length() != 8)
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		if(hexStr.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		uint32_t hexValue = 0;
		std::stringstream ss;
		
		ss << std::hex << hexStr;
		ss >> hexValue;
		
		if(ss.fail())
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		return fromHexRgbaInt(hexValue);
	}
	
	Color Color::fromHexRgbInt(uint32_t hexValue)
	{
		float r = static_cast<float>(((hexValue >> 16) & 0xFF)) / 255.0f;
		float g = static_cast<float>(((hexValue >> 8) & 0xFF)) / 255.0f;
		float b = static_cast<float>((hexValue & 0xFF)) / 255.0f;
		
		return Color(r, g, b, 1.0f);
	}
	
	Color Color::fromHexRgbString(std::string hexStr)
	{
		if(!hexStr.empty() && hexStr[0] == '#')
			hexStr = hexStr.substr(1);
		
		if(hexStr.length() != 6)
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		if(hexStr.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		uint32_t hexValue;
		std::stringstream ss;
		
		ss << std::hex << hexStr;
		ss >> hexValue;
		
		if(ss.fail())
			return Color(0.0f, 0.0f, 0.0f, 1.0f);
		
		return fromHexRgbInt(hexValue);
	}
	
	
	IntRGB Color::toIntRGB() const
	{
		int r = static_cast<int>(std::round(std::clamp(m_r, 0.0f, 1.0f) * 255.0f));
		int g = static_cast<int>(std::round(std::clamp(m_g, 0.0f, 1.0f) * 255.0f));
		int b = static_cast<int>(std::round(std::clamp(m_b, 0.0f, 1.0f) * 255.0f));
		
		return { r, g, b };
	}
	
	IntRGBA Color::toIntRGBA() const
	{
		int r = static_cast<int>(std::round(std::clamp(m_r, 0.0f, 1.0f) * 255.0f));
		int g = static_cast<int>(std::round(std::clamp(m_g, 0.0f, 1.0f) * 255.0f));
		int b = static_cast<int>(std::round(std::clamp(m_b, 0.0f, 1.0f) * 255.0f));
		int a = static_cast<int>(std::round(std::clamp(m_a, 0.0f, 1.0f) * 255.0f));
		
		return { r, g, b, a };
	}
	
	YCbCr Color::toYCbCr() const
	{
		float Y = 0.299f * m_r + 0.587f * m_g + 0.114f * m_b;
		float Cb = -0.1687f * m_r - 0.3313f * m_g + 0.5f * m_b;
		float Cr = 0.5f * m_r - 0.4187f * m_g - 0.0813f * m_b;
		
		Y = std::clamp<float>(Y, 0.0f, 1.0f);
		Cb = std::clamp<float>(Cb, -0.5f, 0.5f);
		Cr = std::clamp<float>(Cr, -0.5f, 0.5f);
		
		return { Y, Cb, Cr };
	}
	
	IntYCbCr Color::toIntYCbCr() const
	{
		YCbCr f = toYCbCr();
		
		int Y = static_cast<int>(std::round((f.y * 255.0f)));
		int Cb = static_cast<int>(std::round(((f.cb * 255.0f) + 128.0f)));
		int Cr = static_cast<int>(std::round(((f.cr * 255.0f) + 128.0f)));
		
		Y = std::clamp<int>(Y, 0, 255);
		Cb = std::clamp<int>(Cb, 0, 255);
		Cr = std::clamp<int>(Cr, 0, 255);
		
		return { Y, Cb, Cr };
	}
	
	HSL Color::toHSL() const
	{
		float r = std::clamp(m_r, 0.0f, 1.0f);
		float g = std::clamp(m_g, 0.0f, 1.0f);
		float b = std::clamp(m_b, 0.0f, 1.0f);
		
		float maxV = std::max({ r, g, b });
		float minV = std::min({ r, g, b });
		float delta = maxV - minV;
		
		float h = 0.0f;
		float s = 0.0f;
		float l = (maxV + minV) / 2.0f;
		
		if(delta > 0.0f)
		{
			s = (l < 0.5f) ? (delta / (maxV + minV)) : (delta / (2.0f - maxV - minV));
			
			if(maxV == r)
				h = std::fmod((g - b) / delta, 6.0f);
			else if(maxV == g)
				h = ((b - r) / delta) + 2.0f;
			else
				h = ((r - g) / delta) + 4.0f;
			
			h *= 60.0f;
			
			if(h < 0.0f)
				h += 360.0f;
		}
		
		return { h, s, l };
	}
	
	IntHSL Color::toIntHSL() const
	{
		HSL f = toHSL();
		
		int H = static_cast<int>(std::round(f.h));
		int S = static_cast<int>(std::round((f.s * 100.0f)));
		int L = static_cast<int>(std::round((f.l * 100.0f)));
		
		H = ((H % 360) + 360) % 360;
		S = std::clamp<int>(S, 0, 100);
		L = std::clamp<int>(L, 0, 100);
		
		return { H, S, L };
	}
	
	uint32_t Color::toHexRgbaInt() const
	{
		uint8_t r = static_cast<uint8_t>(std::clamp(m_r, 0.0f, 1.0f) * 255.0f);
		uint8_t g = static_cast<uint8_t>(std::clamp(m_g, 0.0f, 1.0f) * 255.0f);
		uint8_t b = static_cast<uint8_t>(std::clamp(m_b, 0.0f, 1.0f) * 255.0f);
		uint8_t a = static_cast<uint8_t>(std::clamp(m_a, 0.0f, 1.0f) * 255.0f);
		
		return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
	}
	
	std::string Color::toHexRgbaString() const
	{
		std::stringstream ss;
		
		ss << "#" << std::hex << std::setfill('0') << std::setw(8) << toHexRgbaInt();
		
		return ss.str();
	}
	
	uint32_t Color::toHexRgbInt() const
	{
		uint8_t r = static_cast<uint8_t>(std::clamp(m_r, 0.0f, 1.0f) * 255.0f);
		uint8_t g = static_cast<uint8_t>(std::clamp(m_g, 0.0f, 1.0f) * 255.0f);
		uint8_t b = static_cast<uint8_t>(std::clamp(m_b, 0.0f, 1.0f) * 255.0f);
		
		return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
	}
	
	std::string Color::toHexRgbString() const
	{
		std::stringstream ss;
		
		ss << "#" << std::hex << std::setfill('0') << std::setw(6) << toHexRgbInt();
		
		return ss.str();
	}
	
	
	void Color::write(std::ostream& os) const
	{
		os << "(" << m_r << "," << m_g << "," << m_b << "," << m_a << ")";
	}
	
	bool Color::isEqual(Color const& src) const
	{
		bool rEqual = (m_r == src.m_r);
		bool gEqual = (m_g == src.m_g);
		bool bEqual = (m_b == src.m_b);
		bool aEqual = (m_a == src.m_a);
		
		return rEqual && gEqual && bEqual && aEqual;
	}
}