#include "vec3.hpp"

using namespace alme;

kmu::vec3::vec3()
	: vec3(0.0f, 0.0f, 0.0f)
{
}
kmu::vec3::vec3(float a, float b, float c)
	: x(a)
	, y(b)
	, z(c)
{
}
kmu::vec3::vec3(const vec3 & vec)
	: vec3(vec.x, vec.y, vec.z)
{
}
kmu::vec3::~vec3()
{

}

inline float kmu::vec3::magnitude()const
{
	return sqrtf(this->sqrMagnitude());
}
inline float kmu::vec3::sqrMagnitude()const
{
	return (x*x + y*y + z*z);
}
inline float kmu::vec3::dot(const vec3 & vec)const
{
	return (x*vec.x + y*vec.y + z*vec.z);
}
inline kmu::vec3 kmu::vec3::cross(const vec3 & vec)const
{
	return vec3(y * vec.z - z * vec.y,
		z * vec.x - x * vec.z,
		x * vec.y - y * vec.x);
}
inline kmu::vec3 & kmu::vec3::normalize()
{
	float len = this->magnitude();
	x /= len;
	y /= len;
	z /= len;
	return *this;
}
inline kmu::vec3 kmu::vec3::normalized()const
{
	float len = this->magnitude();
	return vec3(x / len, y / len, z / len);
}
inline std::string kmu::vec3::toString()const
{
	return std::string('(' + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ')');
}
inline void kmu::vec3::set(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}
inline float & kmu::vec3::operator[](size_t id)
{
	return n[id];
}
inline const kmu::vec3 & kmu::vec3::operator=(const vec3 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	return *this;
}
inline kmu::vec3 & kmu::vec3::operator+=(const vec3 & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}
inline kmu::vec3 & kmu::vec3::operator-=(const vec3 & vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}
inline kmu::vec3 & kmu::vec3::operator*=(float n)
{
	x *= n;
	y *= n;
	z *= n;
	return *this;
}
inline kmu::vec3 & kmu::vec3::operator/=(float n)
{
	x /= n;
	y /= n;
	z /= n;
	return *this;
}
inline kmu::vec3 kmu::operator+(const vec3 &v, const vec3 & vec)
{
	return vec3(v.x + vec.x,
		v.y + vec.y,
		v.z + vec.z);
}
inline kmu::vec3 kmu::operator-(const vec3 &v, const vec3 & vec)
{
	return vec3(v.x - vec.x,
		v.y - vec.y,
		v.z - vec.z);
}
inline kmu::vec3 kmu::operator*(const vec3 &v, float n)
{
	return vec3(v.x * n,
		v.y * n,
		v.z * n);
}
inline kmu::vec3 kmu::operator/(const vec3 &v, float n)
{
	return vec3(v.x / n,
		v.y / n,
		v.z / n);
}

kmu::vec3 kmu::lerp(const vec3 & a, const vec3 & b, float t)
{
	return a*t - b*(1-t);
}
