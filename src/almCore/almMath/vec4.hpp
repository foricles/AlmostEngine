#ifndef _VEC4_H_
#define _VEC4_H_

#include <string>
namespace alme
{
#define MY_PI 3.1415926535897932f
namespace kmu
{
	class vec4
	{
	public:
		union
		{
			float n[4];
			struct { float x; float y; float z; float w; };
		};
	public:
		vec4();
		vec4(float a, float b, float c, float d);
		vec4(const vec4 &vec);
		~vec4();

		 float dot(const vec4 &vec)const;
		 float magnitude()const;
		 float sqrMagnitude()const;
		 vec4 &normalize();
		 vec4 normalized()const;
		 std::string toString()const;

		 float &operator[](size_t id);
		 const vec4 &operator=(const vec4 &vec);

		 vec4 &operator+=(const vec4 &vec);
		 vec4 &operator-=(const vec4 &vec);
		 vec4 &operator*=(float n);
		 vec4 &operator/=(float n);

		 vec4 operator+(const vec4 &vec)const;
		 vec4 operator-(const vec4 &vec)const;
		 vec4 operator*(float n)const;
		 vec4 operator/(float n)const;
	};
#define VEC4_IDENT (kmu::vec4(1.0f, 1.0f, 1.0f, 1.0f))
#define VEC4_PI    (kmu::vec4(MY_PI, MY_PI, MY_PI, MY_PI))
}
}
#endif // !_VEC4_H_