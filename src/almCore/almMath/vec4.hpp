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

		inline float dot(const vec4 &vec)const;
		inline float magnitude()const;
		inline vec4 &normalize();
		inline vec4 normalized()const;
		inline std::string toString()const;

		inline float &operator[](size_t id);
		inline const vec4 &operator=(const vec4 &vec);

		inline vec4 &operator+=(const vec4 &vec);
		inline vec4 &operator-=(const vec4 &vec);
		inline vec4 &operator*=(float n);
		inline vec4 &operator/=(float n);

		inline vec4 operator+(const vec4 &vec)const;
		inline vec4 operator-(const vec4 &vec)const;
		inline vec4 operator*(float n)const;
		inline vec4 operator/(float n)const;
	};
#define VEC4_IDENT (kmu::vec4(1.0f, 1.0f, 1.0f, 1.0f))
#define VEC4_PI    (kmu::vec4(MY_PI, MY_PI, MY_PI, MY_PI))
}
}
#endif // !_VEC4_H_