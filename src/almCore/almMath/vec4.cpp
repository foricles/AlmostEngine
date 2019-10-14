#include "vec4.hpp"

using namespace alme;

kmu::vec4::vec4()
	: vec4(0.0f, 0.0f, 0.0f, 1.0f)
{
}
kmu::vec4::vec4(float a, float b, float c, float d)
	: x(a)
	, y(b)
	, z(c)
	, w(d)
{
}
kmu::vec4::vec4(const vec4 & vec)
	: vec4(vec.x, vec.y, vec.z, vec.w)
{
}
kmu::vec4::~vec4()
{
}

 float kmu::vec4::dot(const vec4 & vec)const
{
	return (x*vec.x
		+ y*vec.y
		+ z*vec.z
		+ w*vec.w);
}
 float kmu::vec4::magnitude()const
{
	return sqrtf(x*x
		+ y*y
		+ z*z
		+ w*w);
}
 kmu::vec4 & kmu::vec4::normalize()
{
	float l(this->magnitude());
	x /= l;
	y /= l;
	z /= l;
	w /= l;
	return *this;
}
 kmu::vec4 kmu::vec4::normalized()const
{
	float l(this->magnitude());
	return vec4(x / l, y / l, z / l, w / l);
}
 std::string kmu::vec4::toString()const
{
	return std::string('(' + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w) + ')');
}
 float & kmu::vec4::operator[](size_t id)
{
	return n[id];
}
 const kmu::vec4 & kmu::vec4::operator=(const vec4 & vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
	return *this;
}
 kmu::vec4 & kmu::vec4::operator+=(const vec4 & vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}
 kmu::vec4 & kmu::vec4::operator-=(const vec4 & vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}
 kmu::vec4 & kmu::vec4::operator*=(float n)
{
	x *= n;
	y *= n;
	z *= n;
	w *= n;
	return *this;
}
 kmu::vec4 & kmu::vec4::operator/=(float n)
{
	n = (n == 0) ? 0.00001f : n;
	x /= n;
	y /= n;
	z /= n;
	w /= n;
	return *this;
}
 kmu::vec4 kmu::vec4::operator+(const vec4 & vec)const
{
	return vec4(x + vec.x,
		y + vec.y,
		z + vec.z,
		w + vec.w);
}
 kmu::vec4 kmu::vec4::operator-(const vec4 & vec)const
{
	return vec4(x - vec.x,
		y - vec.y,
		z - vec.z,
		w - vec.w);
}
 kmu::vec4 kmu::vec4::operator*(float n)const
{
	return vec4(x * n, y * n, z * n, w * n);
}
 kmu::vec4 kmu::vec4::operator/(float n)const
{
	return vec4(x / n, y / n, z / n, w / n);
}