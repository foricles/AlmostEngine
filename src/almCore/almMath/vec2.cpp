#include "vec2.hpp"

using namespace alme;

kmu::vec2::vec2()
	: vec2(0.0f, 0.0f)
{
}
kmu::vec2::vec2(float a, float b)
	: x(a)
	, y(b)
{
}
kmu::vec2::vec2(const vec2 & vec)
	: vec2(vec.x, vec.y)
{
}
kmu::vec2::~vec2()
{
}
inline float kmu::vec2::magnitude()const
{
	return sqrtf(this->sqrMagnitude());
}
inline float kmu::vec2::sqrMagnitude()const
{
	return (x*x + y*y);
}
inline float kmu::vec2::angle()const
{
	return atan2f(y, x);
}
inline float kmu::vec2::dot(const vec2 & vec)const
{
	return (x*vec.x +
		y*vec.y);
}
inline float kmu::vec2::cross(const vec2 & vec)const
{
	return (x*vec.y - vec.x*y);
}
inline kmu::vec2 & kmu::vec2::normalize()
{
	float len = this->magnitude();
	if (len != 0)
	{
		x /= len;
		y /= len;
	}
	return *this;
}

inline kmu::vec2 kmu::vec2::normalized()const
{
	float len = this->magnitude();
	return vec2(x / len, y / len);
}

inline std::string kmu::vec2::toString()const
{
	return std::string('(' + std::to_string(x) + ", " + std::to_string(y) + ')');
}
inline void kmu::vec2::set(float X, float Y)
{
	x = X;
	y = Y;
}
inline float & kmu::vec2::operator[](size_t id)
{
	return n[id];
}
inline const kmu::vec2 &kmu::vec2::operator=(const vec2 & vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}
inline kmu::vec2 & kmu::vec2::operator+=(const vec2 & vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}
inline kmu::vec2 & kmu::vec2::operator-=(const vec2 & vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}
inline kmu::vec2 & kmu::vec2::operator*=(float n)
{
	x *= n;
	y *= n;
	return *this;
}
inline kmu::vec2 & kmu::vec2::operator/=(float n)
{
	x /= n;
	y /= n;
	return *this;
}

inline kmu::vec2 kmu::operator+(const vec2 & v, const vec2 & vec)
{
	return vec2(v.x + vec.x, v.y + vec.y);
}
inline kmu::vec2 kmu::operator-(const vec2 & v, const vec2 & vec)
{
	return vec2(v.x - vec.x, v.y - vec.y);
}
inline kmu::vec2 kmu::operator*(const vec2 & v, float n)
{
	return vec2(v.x*n, v.y*n);
}
inline kmu::vec2 kmu::operator/(const vec2 & v, float n)
{
	return vec2(v.x / n, v.y / n);
}

inline kmu::vec2 kmu::lerp(const vec2 & a, const vec2 & b, float t)
{
	return a*t - b*(1 - t);
}