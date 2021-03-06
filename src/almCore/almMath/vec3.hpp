#ifndef _VEC3_H_
#define _VEC3_H_

#include <string>
#define MY_PI 3.1415926535897932f
namespace alme
{
namespace kmu
{
	class vec3
	{
	public:
		union
		{
			float n[3];
			struct { float x; float y; float z; };
		};

	public:
		vec3();
		vec3(float a, float b, float c);
		vec3(const vec3 &vec);
		~vec3();

		 float magnitude()const;
		 float sqrMagnitude()const;
		 float dot(const vec3 &vec)const;
		 vec3 cross(const vec3 &vec)const;
		 vec3 &normalize();
		 vec3 normalized()const;
		 std::string toString()const;

		 void set(float X, float Y, float Z);

		 float &operator[](size_t id);
		 const vec3 &operator=(const vec3 &vec);
		 vec3 &operator+=(const vec3 &vec);
		 vec3 &operator-=(const vec3 &vec);
		 vec3 &operator*=(float n);
		 vec3 &operator/=(float n);

		extern friend vec3 operator+(const vec3 &v, const vec3 &vec);
		extern friend vec3 operator-(const vec3 &v, const vec3 &vec);
		extern friend vec3 operator*(const vec3 &v, float n);
		extern friend vec3 operator/(const vec3 &v, float n);

		extern friend vec3 lerp(const vec3 &a, const vec3 &b, float t);
	};
#define VEC3_RIGHT (kmu::vec3(1.0f, 0.0f, 0.0f))
#define VEC3_UP    (kmu::vec3(0.0f, 1.0f, 0.0f))
#define VEC3_FRONT (kmu::vec3(0.0f, 0.0f, 1.0f))
#define VEC3_IDENT (kmu::vec3(1.0f, 1.0f, 1.0f))
#define VEC3_PI    (kmu::vec3(MY_PI, MY_PI, MY_PI))
}
}
#endif // !_VEC3_H_