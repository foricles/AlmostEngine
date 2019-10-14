#ifndef _MATRICES_H_
#define _MATRICES_H_

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "quaternion.hpp"

namespace alme
{

namespace kmu
{
	class mat
	{
	public:
		mat(int n, int m);
		mat(int n, int m, const float *arr);
		mat(int n, float k);
		mat(const mat & mtx);
		~mat();

		 float &at(int i, int j) const;
		 float &at(int i, int j);
		
		 int getN() const;
		 int getM() const;
		
		 const mat & mult(const mat & other);
		const mat & operator*=(const mat & other);
		const mat & operator=(const mat & other);
		mat operator*(const mat & other) const;
		
		 float &operator[](size_t id);
		 const float &operator[](size_t id)const;

		 bool isSquere() const;
		 std::string toString() const;

	protected:
		int getSize() const;
		int oN;
		int oM;
		float *oData;
	};

	class mat4 : public mat
	{
	public:
		mat4(float n = 1.0f);
		mat4(const mat4 &mat);
		~mat4();

		 const mat4 &operator*(const mat4 &right);
		 const mat4 & operator=(const mat4 & other);

		 const vec4 operator*(const vec4 &right);
		 const vec3 operator*(const vec3 &right);

		 void Identity();

		static mat4 Translation(float x, float y, float z);
		static mat4 Translation(const kmu::vec3 &pos);

		static mat4 Rotation(float x, float y, float z);
		static mat4 Rotation(const kmu::quaternion &q);
		static mat4 Scaling(float x, float y, float z);
		static mat4 CameraMatrix(vec3 target, vec3 up);
		static mat4 Perspective(float fov, int width, int heigth, float nr, float fr);

		static quaternion Quaternion(const kmu::mat4 &mtx);
	};
}

}
#endif // !_MATRICES_H_
