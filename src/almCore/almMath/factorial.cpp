#include "vec2.hpp"
#include "factorial.hpp"

using namespace alme;

static float cache[13] = {1u,1u,2u,6u,24u,120u,720u,5040u,40320u,362880u,3628800u,39916800u,479001600u};

kmu::factorial::factorial(uint32_t num)
	: m_num(num)
{
}

float kmu::factorial::operator()() const
{
	if (m_num < 13) return cache[m_num];

	float ret = cache[12];
	for (uint32_t i = 13; i <= m_num; ++i)
		ret *= static_cast<float>(i);
	return ret;
}

float kmu::operator*(const factorial & rhv, const factorial & lhv)
{
	return static_cast<float>(rhv()) * static_cast<float>(lhv());
}

float kmu::operator/(const factorial & rhv, const factorial & lhv)
{
	if (rhv.m_num == lhv.m_num)
		return 1;

	if ((rhv.m_num < 13) && (lhv.m_num < 13))
		return cache[rhv.m_num] / cache[lhv.m_num];

	if (rhv.m_num > lhv.m_num)
	{
		float ret = 1;
		uint32_t start = lhv.m_num < 13 ? lhv.m_num : 12;
		for (uint32_t i(start+1); i <= rhv.m_num; ++i)
			ret *= static_cast<float>(i);
		return ret;
	}

	return (1.0f / (lhv / rhv));
}