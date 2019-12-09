#include "matrices.hpp"

using namespace alme;

kmu::mat4 kmu::Translation(float x, float y, float z)
{
	kmu::mat4 result(1);

	result.at(0, 3) = x;
	result.at(1, 3) = y;
	result.at(2, 3) = z;
	return result;
}
kmu::mat4 kmu::Translation(const kmu::vec3 & pos)
{
	kmu::mat4 result(1);

	result.at(0, 3) = pos.x;
	result.at(1, 3) = pos.y;
	result.at(2, 3) = pos.z;
	return result;
}
kmu::mat4 kmu::Rotation(float x, float y, float z)
{
	kmu::mat4 mx(1.0f);
	kmu::mat4 my(1.0f);
	kmu::mat4 mz(1.0f);
	float c_mx(cosf(x)), s_mx(sinf(x));
	float c_my(cosf(y)), s_my(sinf(y));
	float c_mz(cosf(z)), s_mz(sinf(z));

	mx.at(1, 1) = c_mx;
	mx.at(2, 2) = c_mx;
	mx.at(1, 2) = -s_mx;
	mx.at(2, 1) = s_mx;
	   
	my.at(0, 0) = c_my;
	my.at(2, 2) = c_my;
	my.at(2, 0) = -s_my;
	my.at(0, 2) = s_my;
	   
	mz.at(0, 0) = c_mz;
	mz.at(1, 1) = c_mz;
	mz.at(0, 1) = -s_mz;
	mz.at(1, 0) = s_mz;

	return mz*(my*mx);
}

kmu::mat4 kmu::Rotation(const kmu::quaternion &q)
{
	//kmu::mat4 res(1.0f);

	//res.at(0, 0) = powf(q.w, 2) + powf(q.x, 2) - powf(q.y, 2) - powf(q.z, 2);  res.at(0, 1) = 2 * q.x*q.y + 2 * q.w*q.z;  res.at(0, 2) = 2 * q.x*q.z - 2 * q.w*q.y;  res.at(0, 3) = 0;
	//res.at(1, 0) = 2 * q.z*q.y - 2 * q.w*q.z;  res.at(1, 1) = powf(q.w, 2) - powf(q.x, 2) + powf(q.y, 2) - powf(q.z, 2);  res.at(1, 2) = 2 * q.y*q.z + 2 * q.w*q.x;  res.at(1, 3) = 0;
	//res.at(2, 0) = 2 * q.x*q.z + 2 * q.w*q.y;  res.at(2, 1) = 2 * q.y*q.z - 2 * q.w*q.x;  res.at(2, 2) = powf(q.w, 2) - powf(q.x, 2) - powf(q.y, 2) + powf(q.z, 2);  res.at(2, 3) = 0;
	//res.at(3, 0) = 0;  res.at(3, 1) = 0;  res.at(3, 2) = 0;  res.at(3, 3) = powf(q.w, 2) + powf(q.x, 2) + powf(q.y, 2) + powf(q.z, 2);

	//return res;

	kmu::mat4 ret(1);

	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
					 
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
					 
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	ret.at(0, 0) = 1 - 2 * (yy + zz);
	ret.at(0, 1) = 2 * (xy - zw);
	ret.at(0, 2) = 2 * (xz + yw);

	ret.at(1, 0) = 2 * (xy + zw);
	ret.at(1, 1) = 1 - 2 * (xx + zz);
	ret.at(1, 2) = 2 * (yz - xw);

	ret.at(2, 0) = 2 * (xz - yw);
	ret.at(2, 1) = 2 * (yz + xw);
	ret.at(2, 2) = 1 - 2 * (xx + yy);
	return ret;
}

kmu::mat4 kmu::Scaling(float x, float y, float z)
{
	kmu::mat4 result(1);

	result.at(0, 0) = x;
	result.at(1, 1) = y;
	result.at(2, 2) = z;
	return result;
}
kmu::mat4 kmu::CameraMatrix(vec3 target, vec3 up)
{
	target.normalize();
	up.normalize();
	up = up.cross(target);
	vec3 v = target.cross(up);

	kmu::mat4 m(1);

	m.at(0, 0) = up[0];		m.at(0, 1) = up[1];		m.at(0, 2) = up[2];
	m.at(1, 0) = v[0];		m.at(1, 1) = v[1];		m.at(1, 2) = v[2];
	m.at(2, 0) = target[0]; m.at(2, 1) = target[1];	m.at(2, 2) = target[2];
	return m;
}
kmu::mat4 kmu::Perspective(float fov, int width, int heigth, float nr, float fr)
{
	float ar = (float)width / (float)heigth;
	float range = nr - fr;
	fov = tanf(fov / 2);
	kmu::mat4 m(1);
	m.at(0, 0) = 1.0f / (fov * ar);
	m.at(1, 1) = 1.0f / fov;
	m.at(2, 2) = (-nr - fr) / range;
	m.at(2, 3) = 2.0f * fr * nr / range;
	m.at(3, 2) = 1.0f;
	return m;
}

kmu::mat4 kmu::Orthographic(int left, int right, int top, int bottom, int near, int far)
{
	mat4 ret(1);

	float x = static_cast<float>(right - left);
	float y = static_cast<float>(top - bottom);
	float z = static_cast<float>(far - near);
	ret.at(0, 0) = 2.0f / x;
	ret.at(1, 1) = 2.0f / y;
	ret.at(2, 2) = -2.0f / z;
	ret.at(0, 3) = -(right + left) / x;
	ret.at(1, 3) = -(top + bottom) / y;
	ret.at(2, 3) = -(far + near) / z;
	return ret;
}

kmu::quaternion kmu::Quaternion(const kmu::mat4 & mtx)
{
	float S;
	float Qw{ 0 }, Qx{ 0 }, Qy{ 0 }, Qz{ 1 };
	float T{ mtx.at(0, 0) + mtx.at(1, 1) + mtx.at(2, 2) + 1 };

	if (T > 0)
	{
		S = 0.5f / sqrt(T);
		Qw = 0.25f / S;
		Qx = (mtx[9] - mtx[6]) * S;
		Qy = (mtx[2] - mtx[8]) * S;
		Qz = (mtx[4] - mtx[1]) * S;
	}
	else
	{
		//Столбец 0:
		if (0)
		{
			S = sqrt(1.0f + mtx[0] - mtx[5] - mtx[10]) * 2.0f;

			Qx = 0.5f / S;
			Qy = (mtx[1] + mtx[4]) / S;
			Qz = (mtx[2] + mtx[8]) / S;
			Qw = (mtx[6] + mtx[9]) / S;
		}
		else if (1)
		{
			//Столбец 1:
			S = sqrt(1.0f + mtx[5] - mtx[0] - mtx[10]) * 2.0f;

			Qx = (mtx[1] + mtx[4]) / S;
			Qy = 0.5f / S;
			Qz = (mtx[6] + mtx[9]) / S;
			Qw = (mtx[2] + mtx[8]) / S;
		}
		else if (2)
		{
			//Столбец 2:
			S = sqrt(1.0f + mtx[10] - mtx[0] - mtx[5]) * 2.0f;

			Qx = (mtx[2] + mtx[8]) / S;
			Qy = (mtx[6] + mtx[9]) / S;
			Qz = 0.5f / S;
			Qw = (mtx[1] + mtx[4]) / S;
		}
	}

	return quaternion(Qx, Qy, Qz, Qw);
}

void kmu::CalculateModelMatrix(kmu::mat4 & result, const kmu::vec3 & pos, const kmu::vec3 & scl, const kmu::quaternion & rot)
{
	float xx = rot.x * rot.x;
	float xy = rot.x * rot.y;
	float xz = rot.x * rot.z;
	float xw = rot.x * rot.w;
			   		   
	float yy = rot.y * rot.y;
	float yz = rot.y * rot.z;
	float yw = rot.y * rot.w;
			   		   
	float zz = rot.z * rot.z;
	float zw = rot.z * rot.w;

	result.at(0, 0) = ((1.0f)*(((1.0f - 2.0f * (yy + zz))*(scl.x)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(scl.x)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(scl.x)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((pos.x)*(((0.0f)*(scl.x)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(0, 1) = ((1.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(scl.y)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(scl.y)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(scl.y)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((pos.x)*(((0.0f)*(0.0f)) + ((0.0f)*(scl.y)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(0, 2) = ((1.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(scl.z)) + ((0.0f)*(0.0f)))) + ((pos.x)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(scl.z)) + ((1.0f)*(0.0f))));
	result.at(0, 3) = ((1.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(1.0f)))) + ((pos.x)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(1.0f))));
	result.at(1, 0) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(scl.x)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((2.0f * (xy + zw))*(scl.x)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(scl.x)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((pos.y)*(((0.0f)*(scl.x)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(1, 1) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(scl.y)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(scl.y)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(scl.y)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((pos.y)*(((0.0f)*(0.0f)) + ((0.0f)*(scl.y)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(1, 2) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(scl.z)) + ((0.0f)*(0.0f)))) + ((pos.y)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(scl.z)) + ((1.0f)*(0.0f))));
	result.at(1, 3) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((1.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(1.0f)))) + ((pos.y)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(1.0f))));
	result.at(2, 0) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(scl.x)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(scl.x)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((2.0f * (xz - yw))*(scl.x)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((pos.z)*(((0.0f)*(scl.x)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(2, 1) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(scl.y)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(scl.y)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(scl.y)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((pos.z)*(((0.0f)*(0.0f)) + ((0.0f)*(scl.y)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(2, 2) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(scl.z)) + ((0.0f)*(0.0f)))) + ((pos.z)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(scl.z)) + ((1.0f)*(0.0f))));
	result.at(2, 3) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((1.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(1.0f)))) + ((pos.z)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(1.0f))));
	result.at(3, 0) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(scl.x)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(scl.x)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(scl.x)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((0.0f)*(scl.x)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(3, 1) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(scl.y)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(scl.y)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(scl.y)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((0.0f)*(0.0f)) + ((0.0f)*(scl.y)) + ((0.0f)*(0.0f)) + ((1.0f)*(0.0f))));
	result.at(3, 2) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(scl.z)) + ((0.0f)*(0.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(scl.z)) + ((0.0f)*(0.0f)))) + ((1.0f)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(scl.z)) + ((1.0f)*(0.0f))));
	result.at(3, 3) = ((0.0f)*(((1.0f - 2.0f * (yy + zz))*(0.0f)) + ((2.0f * (xy - zw))*(0.0f)) + ((2.0f * (xz + yw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((0.0f)*(((2.0f * (xy + zw))*(0.0f)) + ((1.0f - 2.0f * (xx + zz))*(0.0f)) + ((2.0f * (yz - xw))*(0.0f)) + ((0.0f)*(1.0f)))) + ((0.0f)*(((2.0f * (xz - yw))*(0.0f)) + ((2.0f * (yz + xw))*(0.0f)) + ((1.0f - 2.0f * (xx + yy))*(0.0f)) + ((0.0f)*(1.0f)))) + ((1.0f)*(((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((0.0f)*(0.0f)) + ((1.0f)*(1.0f))));

}
