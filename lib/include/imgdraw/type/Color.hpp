#ifndef COLOR_HPP
#define COLOR_HPP

#include <ostream>
#include <string>
#include <cstdint>
#include <cmath>

namespace imgdraw
{
	struct RGB
	{
		float r;
		float g;
		float b;
	};
	
	struct IntRGB
	{
		int r;
		int g;
		int b;
	};
	
	struct RGBA
	{
		float r;
		float g;
		float b;
		float a;
	};
	
	struct IntRGBA
	{
		int r;
		int g;
		int b;
		int a;
	};
	
	struct YCbCr
	{
		float y;
		float cb;
		float cr;
	};
	
	struct IntYCbCr
	{
		int y;
		int cb;
		int cr;
	};
	
	struct HSL
	{
		float h;
		float s;
		float l;
	};
	
	struct IntHSL
	{
		int h;
		int s;
		int l;
	};
	
	class Color
	{
		public:
			Color() = default;
			Color(float r, float g, float b, float a = 1.0f);
			virtual ~Color() = default;
			
			inline float const& r() const { return m_r; }
			inline float& r() { return m_r; }
			
			inline float const& g() const { return m_g; }
			inline float& g() { return m_g; }
			
			inline float const& b() const { return m_b; }
			inline float& b() { return m_b; }
			
			inline float const& a() const { return m_a; }
			inline float& a() { return m_a; }
			
			static inline Color fromRGB(float r, float g, float b) { return Color(r, g, b, 1.0f); }
			static inline Color fromRGB(RGB const& src) { return fromRGB(src.r, src.g, src.b); }
			
			static inline Color fromIntRGB(int r, int g, int b) { return Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, 1.0f); }
			static inline Color fromIntRGB(IntRGB const& src) { return fromIntRGB(src.r, src.g, src.b); }
			
			static Color fromRGBA(float r, float g, float b, float a = 1.0f) { return Color(r, g, b, a); }
			static inline Color fromRGBA(RGBA const& src) { return fromRGBA(src.r, src.g, src.b, src.a); }
			
			static Color fromIntRGBA(int r, int g, int b, int a = 255) { return Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f); }
			static inline Color fromIntRGBA(IntRGBA const& src) { return fromIntRGBA(src.r, src.g, src.b, src.a); }
			
			static Color fromYCbCr(float y, float cb, float cr);
			static inline Color fromYCbCr(YCbCr const& src) { return fromYCbCr(src.y, src.cb, src.cr); }
			
			static Color fromIntYCbCr(int y, int cb, int cr);
			static inline Color fromIntYCbCr(IntYCbCr const& src) { return fromIntYCbCr(src.y, src.cb, src.cr); }
			
			static Color fromHSL(float h, float s, float l);
			static inline Color fromHSL(HSL const& src) { return fromHSL(src.h, src.s, src.l); }
			
			static Color fromIntHSL(int h, int s, int l);
			static inline Color fromIntHSL(IntHSL const& src) { return fromIntHSL(src.h, src.s, src.l); }
			
			static Color fromHexRgbaInt(uint32_t hexValue);
			static Color fromHexRgbaString(std::string hexStr);
			
			static Color fromHexRgbInt(uint32_t hexValue);
			static Color fromHexRgbString(std::string hexStr);
			
			inline RGB toRGB() const { return { m_r, m_g, m_b }; }
			IntRGB toIntRGB() const;
			inline RGBA toRGBA() const { return { m_r, m_g, m_b, m_a }; }
			IntRGBA toIntRGBA() const;
			YCbCr toYCbCr() const;
			IntYCbCr toIntYCbCr() const;
			HSL toHSL() const;
			IntHSL toIntHSL() const;
			uint32_t toHexRgbaInt() const;
			std::string toHexRgbaString() const;
			uint32_t toHexRgbInt() const;
			std::string toHexRgbString() const;
			
			inline friend std::ostream& operator<<(std::ostream& os, Color const& src) { src.write(os); return os; }
			inline friend bool operator==(Color const& lhs, Color const& rhs) { return lhs.isEqual(rhs); }
			inline friend bool operator!=(Color const& lhs, Color const& rhs) { return !lhs.isEqual(rhs); }
			
			static Color const White;
			static Color const Black;
			static Color const Red;
			static Color const Green;
			static Color const Blue;
			static Color const Yellow;
			static Color const Orange;
			static Color const Purple;
			static Color const Cyan;
			static Color const Magenta;
		
		private:
			float m_r = 0.0f; /*!< Red value of color ; used to be between [0.0f, 1.0f]. */
			float m_g = 0.0f; /*!< Green value of color ; used to be between [0.0f, 1.0f]. */
			float m_b = 0.0f; /*!< Blue value of color ; used to be between [0.0f, 1.0f]. */
			float m_a = 1.0f; /*!< Alpha value (transparent) of color ; used to be between [0.0f, 1.0f]. */
		
		protected:
			virtual void write(std::ostream& os) const;
			virtual bool isEqual(Color const& src) const;
	};
}

#endif // COLOR_HPP