#include "quaternion.hpp"

using namespace alme;

kmu::quaternion::quaternion() : vec4(0, 0, 0, 1)
{
}
kmu::quaternion::quaternion(float a, float b, float c, float d) : vec4(a, b, c, d)
{
}
kmu::quaternion::quaternion(const vec4 & vec) : vec4(vec.x, vec.y, vec.z, vec.w)
{
}
kmu::quaternion::quaternion(const quaternion & qut) : vec4(qut.x, qut.y, qut.z, qut.w)
{
}
kmu::quaternion::~quaternion()
{
}

inline void kmu::quaternion::conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

inline kmu::quaternion kmu::quaternion::conjugated() const
{
	return kmu::quaternion(-x, -y, -z, w);
}

inline const kmu::quaternion & kmu::quaternion::operator=(const quaternion & qut)
{
	return *this;
}

inline kmu::vec3 kmu::quaternion::rotate(const kmu::vec3 & vect) const
{
	//TODO:
	auto temp = ((*this) * vect) * this->conjugated();
	return kmu::vec3(temp.x, temp.y, temp.z);
}

kmu::quaternion kmu::quaternion::euler(float angle, float X, float Y, float Z)
{
	return quaternion::euler(angle, kmu::vec3(X, Y, Z));
}
kmu::quaternion kmu::quaternion::euler(float angle, const vec3 & axes)
{
	float hs = sinf(angle / 2.0f);
	float hc = cosf(angle / 2.0f);
	float X = axes.x * hs;
	float Y = axes.y * hs;
	float Z = axes.z * hs;
	return quaternion(X, Y, Z, hc);
}

inline kmu::quaternion kmu::operator*(const quaternion &q1, const quaternion &q2)
{
	float x1 = q1.x;
	float x2 = q2.x;

	float y1 = q1.y;
	float y2 = q2.y;

	float z1 = q1.z;
	float z2 = q2.z;

	float w1 = q1.w;
	float w2 = q2.w;

	float X = x1*w2 + w1*x2 + y1*z2 - z1*y2;
	float Y = y1*w2 + w1*y2 + z1*x2 - x1*z2;
	float Z = z1*w2 + w1*z2 + x1*y2 - y1*x2;
	float W = w1*w2 - x1*x2 - y1*y2 - z1*z2;
	return quaternion(X, Y, Z, W);
}
inline kmu::quaternion kmu::operator*(const quaternion &q1, const vec3 & vec)
{
	float W = -q1.x*vec.x - q1.y*vec.y - q1.z*vec.z;
	float X = q1.w*vec.x + q1.y*vec.z - q1.z*vec.y;
	float Y = q1.w*vec.y + q1.z*vec.x - q1.x*vec.z;
	float Z = q1.w*vec.z + q1.x*vec.y - q1.y*vec.x;
	return quaternion(X, Y, Z, W);
}