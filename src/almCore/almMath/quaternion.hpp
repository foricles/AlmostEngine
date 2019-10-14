#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "vec4.hpp"
#include "vec3.hpp"
namespace alme
{
#define MY_PI 3.1415926535897932f
namespace kmu
{
	class quaternion : public vec4
	{
	public:
		quaternion();
		quaternion(float a, float b, float c, float d);
		quaternion(const vec4 &vec);
		quaternion(const quaternion &qut);
		~quaternion();

		 void conjugate();
		 quaternion conjugated() const;
		const quaternion &operator=(const quaternion &qut);

		 kmu::vec3 rotate(const kmu::vec3 &vect) const;

		static quaternion euler(float angle, float X, float Y, float Z);
		static quaternion euler(float angle, const vec3 &axes);

		friend  quaternion operator*(const quaternion &q1, const quaternion &q2);
		friend  quaternion operator*(const quaternion &q1, const vec3 &vec);
	};
}
}
#endif // !_QUATERNION_H_