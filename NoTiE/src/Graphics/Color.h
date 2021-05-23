#pragma once
#ifdef NOTIE_CORE
	#include <SFML/Graphics/Color.hpp>
#endif


typedef unsigned char uChar;

namespace notie
{
    struct Color
    {
    public:
        Color() : r(0), g(0), b(0), a(255) {}
        Color(uChar red, uChar green, uChar blue, uChar alpha = 255) : r(red), g(green), b(blue), a(alpha) {}
        explicit Color(unsigned int color) : r((color & 0xff000000) >> 24), g((color & 0x00ff0000) >> 16), b((color & 0x0000ff00) >> 8), a((color & 0x000000ff) >> 0) {}
        //unsigned int toInteger() const;

        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Cyan;
        static const Color Transparent;

        static const Color Cornflower;

        uChar r;
        uChar g;
        uChar b;
        uChar a;

#ifdef NOTIE_CORE
        Color(const sf::Color& other) { this->r = other.r, this->g = other.g, this->b = other.b, this->a = other.a; }
#endif
    };
    //bool operator ==(const Color& left, const Color& right);
    //bool operator !=(const Color& left, const Color& right);
    //Color operator +(const Color& left, const Color& right);
    //Color operator -(const Color& left, const Color& right);
    //Color operator *(const Color& left, const Color& right);
    //Color& operator +=(Color& left, const Color& right);
    //Color& operator -=(Color& left, const Color& right);
    //Color& operator *=(Color& left, const Color& right);
}