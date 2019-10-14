#ifndef _VEC2_H_
#define _VEC2_H_

#include <string>
#define MY_PI 3.1415926535897932f
namespace alme
{
namespace kmu
{
	class vec2
	{
	public:
		union
		{
			float n[2];
			struct { float x; float y; };
			struct { float u; float w; };
		};
	public:
		vec2();
		vec2(float a, float b);
		vec2(const vec2 &another);
		~vec2();

		 float magnitude() const;
		 float sqrMagnitude() const;
		 float angle() const;
		 float dot(const vec2 &vec) const;
		 float cross(const vec2 &vec) const;
		 vec2 &normalize();
		 vec2 normalized()const;
		 std::string toString()const;

		 void set(float X, float Y);

		 float &operator[](size_t id);
		 const vec2 &operator=(const vec2 &vec);
		 vec2 &operator+=(const vec2 &vec);
		 vec2 &operator-=(const vec2 &vec);
		 vec2 &operator*=(float n);
		 vec2 &operator/=(float n);

		extern friend vec2 operator+(const vec2 &v, const vec2 &vec);
		extern friend vec2 operator-(const vec2 &v, const vec2 &vec);
		extern friend vec2 operator*(const vec2 &v, float n);
		extern friend vec2 operator/(const vec2 &v, float n);

		extern friend vec2 lerp(const vec2 &a, const vec2 &b, float t);
	};
#define VEC2_RIGHT (kmu::vec2(1.0f, 0.0f))
#define VEC2_UP    (kmu::vec2(0.0f, 1.0f))
#define VEC2_IDENT (kmu::vec2(1.0f, 1.0f))
#define VEC2_PI    (kmu::vec2(MY_PI, MY_PI))
}
}
#endif // !_VEC2_H_