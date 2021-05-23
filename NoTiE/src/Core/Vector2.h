#pragma once
#include <cstdint>
#ifdef NOTIE_CORE
	#include <SFML/System/Vector2.hpp>
#endif

template<class T>
struct Vector2
{
	T x = 0;
	T y = 0;

	//Constructors
	Vector2() : x(0), y(0) {}
	Vector2(T x, T y) : x(x), y(y) {}
	Vector2(const Vector2& otherVector) : x(otherVector.x), y(otherVector.y) {}

	//The length of the vector
	T Magnitude() { return sqrt(x * x + y * y); }
	//The squared length of the vector
	T sqrMagnitude() { return x * x + y * y; }

	//Dividing 1 by the magnitude and multiplying each component by the result saves performance compared to dividing each component by the magnitude directly as multiplication is faster than division
	T Normalized() { T t = 1 / Magnitude(); return Vector2(x * t, y * t); }

	T Perpendicular() { return Vector2(-y, x); }

	T Dot(const Vector2& rhs) { return this->x * rhs.x, +this->y * rhs.y; }

	T Cross(const Vector2& rhs) { return this->x * rhs.y, this->y* rhs.x; }


	//Operators
	Vector2 operator + (const Vector2& rhs) const { return Vector2(this->x + rhs.x, this->y + rhs.y); }
	Vector2& operator += (const Vector2& rhs) { this->x += rhs.x, this->y += rhs.y; return *this; }
	
	Vector2 operator - (const Vector2& rhs) const { return Vector2(this->x - rhs.x, this->y - rhs.y); }
	Vector2& operator -= (const Vector2& rhs) { this->x -= rhs.x, this->y -= rhs.y; return *this; }
	
	Vector2 operator * (const T& rhs) const { return Vector2(this->x * rhs, this->y * rhs); }
	Vector2& operator *= (const T& rhs) { this->x *= rhs, this->y *= rhs; return *this; }
	
	Vector2 operator / (const T& rhs) const { return Vector2(this->x / rhs, this->y / rhs); }
	Vector2& operator /= (const T& rhs) { this->x /= rhs, this->y /= rhs; return *this; }

	bool operator==(const Vector2& rhs) const { return (this->x == rhs.x) && (this->y == rhs.y); }
	bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }

	operator Vector2<int>() const { return { static_cast<int32_t>(this->x), static_cast<int32_t>(this->y) }; }
	operator Vector2<float>() const { return { static_cast<float>(this->x), static_cast<float>(this->y) }; }

	//In-Engine Conversion only
#ifdef NOTIE_CORE
	template<class U> Vector2(const sf::Vector2<U> other) { this->x = static_cast<U>(other.x), this->y = static_cast<U>(other.y); }
#endif

};

template<class T> Vector2<T> operator * (const float& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs * rhs.x, lhs * rhs.y); }
template<class T> Vector2<T> operator * (const double& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs * rhs.x, lhs * rhs.y); }
template<class T> Vector2<T> operator * (const int& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs * rhs.x, lhs * rhs.y); }
template<class T> Vector2<T> operator / (const float& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs / rhs.x, lhs / rhs.y); }
template<class T> Vector2<T> operator / (const double& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs / rhs.x, lhs / rhs.y); }
template<class T> Vector2<T> operator / (const int& lhs, const Vector2<T>& rhs) { return Vector2<T>(lhs / rhs.x, lhs / rhs.y); }

typedef Vector2<int> Vector2Int;
typedef Vector2<float> Vector2Float;
