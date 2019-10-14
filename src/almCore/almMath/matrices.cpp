#include "matrices.hpp"

using namespace alme;
using namespace kmu;

mat::mat(int n, int m) :oN(n), oM(m), oData(new float[oN * oM])
{
	memset(oData, 0, getSize());
}

mat::mat(int n, int m, const float *arr) : mat(n, m)
{
	memcpy(oData, arr, getSize());
}

mat::mat(int n, float k) : mat(n, n)
{
	for (int i{ 0 }; i < n; ++i)
	{
		at(i, i) = k;
	}
}

mat::mat(const mat & mtx) :oN(mtx.oN), oM(mtx.oM), oData(new float[oN * oM])
{
	memcpy(oData, mtx.oData, getSize());
}

mat::~mat()
{
	delete[]oData;
}

 float &mat::at(int i, int j) const
{
	return oData[i * oN + j];
}

 float &mat::at(int i, int j)
{
	return oData[i * oN + j];
}

 const mat & mat::mult(const mat & other)
{
	mat res(oN, other.oM);
	if (oN == other.oM)
	{
		int n = oN;
		for (register int i{ 0 }; i < n; i++)
			for (register int j{ 0 }; j < n; j++)
				for (register int k{ 0 }; k < n; k++)
					res.at(i, j) += other.at(i, k) * at(k, j);
	}
	*this = res;
	return *this;
}

 int mat::getN() const
{
	return oN;
}

 int mat::getM() const
{
	return oM;
}

 const mat & mat::operator*=(const mat & other)
{
	if (oN == other.oM)
	{
		mat res(oN, other.oM);
		int n = oN;
		for (register int i{0}; i < n; ++i)
		{
			for (register int j{0}; j < n; ++j)
			{
				res.at(i, j) = 0;
				for (register int z{0}; z < n; ++z)
				{
					res.at(i, j) += at(i, z) * other.at(z, j);//i z * z j
				}
			}
		}
		*this = res;
	}

	return *this;
}

 const mat & mat::operator=(const mat & other)
{
	if (other.oN != oN || other.oM != oM)
	{
		oN = other.oN;
		oM = other.oM;
		delete[]oData;
		oData = new float[oN * oM];
	}
	memcpy(oData, other.oData, getSize());
	return *this;
}

 mat mat::operator *(const mat & other) const
{
	if (oN == other.oM)
	{
		mat res(oN, other.oM);
		int n = oN;
		for (register int i{ 0 }; i < n; ++i)
		{
			for (register int j{ 0 }; j < n; ++j)
			{
				res.at(i, j) = 0;
				for (register int z{ 0 }; z < n; ++z)
				{
					res.at(i, j) += at(i, z) * other.at(z, j);//i z * z j
				}
			}
		}
		return res;
	}

	return *this;
}

 int mat::getSize() const
{
	return (sizeof(float) * oN * oM);
}

 float & mat::operator[](size_t id)
{
	return oData[id];
}

 const float & mat::operator[](size_t id) const
{
	return oData[id];
}

 bool mat::isSquere() const
{
	return oN == oM;
}

 std::string mat::toString() const
{
	std::string res = "";

	for (int i{ 0 }; i < oN; ++i)
	{
		res += "| ";
		for (int j{ 0 }; j < oM; ++j)
			res += std::to_string(at(i, j)) + " | ";
		res += '\n';
	}

	return res;
}

//------------------------------------------------------------------------------------------------------------------------------------------------
kmu::mat4::mat4(float n) : mat(4, n)
{
}

kmu::mat4::mat4(const mat4 & mat) : mat(mat)
{
}

kmu::mat4::~mat4()
{
}

const kmu::mat4 & kmu::mat4::operator*(const mat4 & right)
{
	mat4 temp(1.0f);
	for (register size_t i(0); i < 4; i++)
	{
		for (register size_t j(0); j < 4; j++)
		{
				temp.at(i, j) = at(i, 0) * right.at(0, j) +
				at(i, 1) * right.at(1, j) +
				at(i, 2) * right.at(2, j) +
				at(i, 3) * right.at(3, j);
		}
	}
	*this = temp;
	return *this;
}

 const kmu::mat4 & kmu::mat4::operator=(const mat4 & other)
{
	oN = other.oN;
	oM = other.oM;
	delete[]oData;
	oData = new float[oN * oM];
	memcpy(oData, other.oData, getSize());
	return *this;
}

 const kmu::vec4 kmu::mat4::operator*(const vec4 & right)
{
	kmu::vec4 ret;
	ret.x = right.x * at(0, 0) + right.y * at(0, 1) + right.x * at(0, 2) + right.w * at(0, 3);
	ret.y = right.x * at(1, 0) + right.y * at(1, 1) + right.x * at(1, 2) + right.w * at(1, 3);
	ret.z = right.x * at(2, 0) + right.y * at(2, 1) + right.x * at(2, 2) + right.w * at(2, 3);
	ret.w = right.x * at(3, 0) + right.y * at(3, 1) + right.x * at(3, 2) + right.w * at(3, 3);
	return ret;
}

 const kmu::vec3 kmu::mat4::operator*(const vec3 & right)
{
	kmu::vec3 ret;
	ret.x = right.x * at(0, 0) + right.y * at(0, 1) + right.x * at(0, 2) + 1 * at(0, 3);
	ret.y = right.x * at(1, 0) + right.y * at(1, 1) + right.x * at(1, 2) + 1 * at(1, 3);
	ret.z = right.x * at(2, 0) + right.y * at(2, 1) + right.x * at(2, 2) + 1 * at(2, 3);
	return ret;
}

 void mat4::Identity()
{
	memset(oData, 0, getSize());
	at(0, 0) = at(1, 1) = at(2, 2) = at(3, 3) = 1;
}

kmu::mat4 kmu::mat4::Translation(float x, float y, float z)
{
	mat4 result(1.0f);
	result.at(0, 3) = x;
	result.at(1, 3) = y;
	result.at(2, 3) = z;
	return result;
}
kmu::mat4 kmu::mat4::Translation(const kmu::vec3 & pos)
{
	mat4 result(1.0f);
	result.at(0, 3) = pos.x;
	result.at(1, 3) = pos.y;
	result.at(2, 3) = pos.z;
	return result;
}
kmu::mat4 kmu::mat4::Rotation(float x, float y, float z)
{
	mat4 mx(1.0f);
	mat4 my(1.0f);
	mat4 mz(1.0f);
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

kmu::mat4 kmu::mat4::Rotation(const kmu::quaternion &q)
{
	//kmu::mat4 res(1.0f);

	//res.at(0, 0) = powf(q.w, 2) + powf(q.x, 2) - powf(q.y, 2) - powf(q.z, 2);  res.at(0, 1) = 2 * q.x*q.y + 2 * q.w*q.z;  res.at(0, 2) = 2 * q.x*q.z - 2 * q.w*q.y;  res.at(0, 3) = 0;
	//res.at(1, 0) = 2 * q.z*q.y - 2 * q.w*q.z;  res.at(1, 1) = powf(q.w, 2) - powf(q.x, 2) + powf(q.y, 2) - powf(q.z, 2);  res.at(1, 2) = 2 * q.y*q.z + 2 * q.w*q.x;  res.at(1, 3) = 0;
	//res.at(2, 0) = 2 * q.x*q.z + 2 * q.w*q.y;  res.at(2, 1) = 2 * q.y*q.z - 2 * q.w*q.x;  res.at(2, 2) = powf(q.w, 2) - powf(q.x, 2) - powf(q.y, 2) + powf(q.z, 2);  res.at(2, 3) = 0;
	//res.at(3, 0) = 0;  res.at(3, 1) = 0;  res.at(3, 2) = 0;  res.at(3, 3) = powf(q.w, 2) + powf(q.x, 2) + powf(q.y, 2) + powf(q.z, 2);

	//return res;

	kmu::mat4 ret;
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

kmu::mat4 kmu::mat4::Scaling(float x, float y, float z)
{
	mat4 result;
	result.at(0, 0) = x;
	result.at(1, 1) = y;
	result.at(2, 2) = z;
	result.at(3, 3) = 1.0;
	return result;
}
kmu::mat4 kmu::mat4::CameraMatrix(vec3 target, vec3 up)
{
	target.normalize();
	up.normalize();
	up = up.cross(target);
	vec3 v = target.cross(up);

	mat4 m(1.0f);

	m.at(0, 0) = up[0];		m.at(0, 1) = up[1];		m.at(0, 2) = up[2];
	m.at(1, 0) = v[0];		m.at(1, 1) = v[1];		m.at(1, 2) = v[2];
	m.at(2, 0) = target[0]; m.at(2, 1) = target[1];	m.at(2, 2) = target[2];
	return m;
}
kmu::mat4 kmu::mat4::Perspective(float fov, int width, int heigth, float nr, float fr)
{
	float ar = (float)width / (float)heigth;
	float range = nr - fr;
	fov = tanf(fov / 2);
	mat4 m;
	m.at(0, 0) = 1.0f / (fov * ar);
	m.at(1, 1) = 1.0f / fov;
	m.at(2, 2) = (-nr - fr) / range;
	m.at(2, 3) = 2.0f * fr * nr / range;
	m.at(3, 2) = 1.0f;
	return m;
}

kmu::quaternion kmu::mat4::Quaternion(const kmu::mat4 & mtx)
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