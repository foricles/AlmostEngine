#ifndef _FACTORIAL_HPP_
#define _FACTORIAL_HPP_

#include <string>
namespace alme
{
namespace kmu
{

class factorial
{
public:
	factorial(uint32_t num);
	factorial() = delete;

	float operator()() const;
	friend float operator*(const factorial &rhv, const factorial &lhv);
	friend float operator/(const factorial &rhv, const factorial &lhv);

private:
	uint32_t m_num;
};

}
}
#endif // !_FACTORIAL_HPP_