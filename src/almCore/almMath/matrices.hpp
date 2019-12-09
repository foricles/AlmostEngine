#ifndef _MATRICES_H_
#define _MATRICES_H_

#include "mtx.hpp"
#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "quaternion.hpp"

namespace alme
{

namespace kmu
{
	mat4 Translation(float x, float y, float z);
	mat4 Translation(const kmu::vec3 &pos);

	mat4 Rotation(float x, float y, float z);
	mat4 Rotation(const kmu::quaternion &q);
	mat4 Scaling(float x, float y, float z);
	mat4 CameraMatrix(vec3 target, vec3 up);

	mat4 Perspective(float fov, int width, int heigth, float nr, float fr);
	mat4 Orthographic(int left, int right, int top, int bottom, int near, int far);

	quaternion Quaternion(const kmu::mat4 &mtx);

	void CalculateModelMatrix(kmu::mat4 & result, const kmu::vec3 &pos, const kmu::vec3 &scl, const kmu::quaternion &rot);
}

}
#endif // !_MATRICES_H_
