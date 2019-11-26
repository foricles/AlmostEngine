#ifndef _MTX_HPP_
#define _MTX_HPP_

#include <string>

namespace alme
{

namespace kmu
{

	template <uint32_t N, class T = float>
	class mtx
	{
	public:

		mtx() {}
		mtx(const T &v) 
		{
			for (uint32_t i(0); i < N; ++i)
				at(i, i) = v;
		}

		T        & at(uint32_t i, uint32_t j) { return m_mtx[i + N * j]; };
		const T  & at(uint32_t i, uint32_t j) const { return m_mtx[i + N * j]; };

		T        & operator[](uint32_t i) { return m_mtx[i]; };
		const T  & operator[](uint32_t i) const { return m_mtx[i]; };

		void identity()
		{
			memset(m_mtx, 0, sizeof(T) * N * N);
			for (uint32_t i(0); i < N; ++i) at(i, i) = 1;
		}

		T determinant() const
		{
			if (N == 2) return at(0, 0)*at(1, 1) - at(0, 1)*at(1, 0);

			T result(0);
			for (uint32_t c(0); c < N; ++c)
			{
				T plus(1);
				T minus(1);
				for (uint32_t i(0); i < N; ++i)
				{
					uint32_t j = (i + c) % N;
					plus *= at(i, j);
					minus *= at(N - 1 - i, j);
				}
				result += plus;
				result -= minus;
			}
			return result;
		}

		void transpose()
		{
			T mtx[N * N]{ 0 };
			for (uint32_t i(0); i < N; ++i)
				for (uint32_t j(0); j < N; ++j)
					mtx[j + N * i] = m_mtx[i + N * j];
			memcpy(m_mtx, mtx, sizeof(T) * N * N);
		}

		void inverse()
		{
			T d = determinant();
			if (d > 0)
			{
				mtx<N, T> adj;
			}
		}

		friend mtx<N, T> operator *(const mtx<N, T> &lhv, const mtx<N, T> &rhv)
		{
			mtx<N, T> res;
			for (uint32_t i(0); i < N; ++i)
				for (uint32_t j(0); j < N; ++j)
					for (uint32_t z(0); z < N; ++z)
						res.at(i, j) += lhv.at(i, z) * rhv.at(z, j);
			return res;
		}

		friend mtx<N, T> operator +(const mtx<N, T> &lhv, const mtx<N, T> &rhv)
		{
			mtx<N, T> res;
			for (uint32_t i(0); i < N; ++i)
				for (uint32_t j(0); j < N; ++j)
					res.at(i, j) = lhv.at(i, j) + rhv.at(i, j);
			return res;
		}

		friend mtx<N, T> operator -(const mtx<N, T> &lhv, const mtx<N, T> &rhv)
		{
			mtx<N, T> res;
			for (uint32_t i(0); i < N; ++i)
				for (uint32_t j(0); j < N; ++j)
					res.at(i, j) = lhv.at(i, j) - rhv.at(i, j);
			return res;
		}

		friend mtx<N, T> operator *(const mtx<N, T> &lhv, const T &rhv)
		{
			mtx<N, T> res;
			for (uint32_t i(0); i < N * N; ++i)
				res.m_mtx[i] = lhv.m_mtx[i] * rhv;
			return res;
		}

		friend mtx<N, T> operator /(const mtx<N, T> &lhv, const T &rhv)
		{
			mtx<N, T> res;
			for (uint32_t i(0); i < N * N; ++i)
				res.m_mtx[i] = lhv.m_mtx[i] / rhv;
			return res;
		}

		mtx<N, T> & operator *=(const T &rhv)
		{
			for (uint32_t i(0); i < N * N; ++i)
				m_mtx[i] *= rhv;
			return *this;
		}

		mtx<N, T> & operator /=(const T &rhv)
		{
			for (uint32_t i(0); i < N * N; ++i)
				m_mtx[i] /= rhv;
			return *this;
		}

		mtx<N, T> & operator +=(const mtx<N, T> &rhv)
		{
			for (uint32_t i(0); i < N * N; ++i)
				m_mtx[i] += rhv.m_mtx[i];
			return *this;
		}

		mtx<N, T> & operator -=(const mtx<N, T> &rhv)
		{
			for (uint32_t i(0); i < N * N; ++i)
				m_mtx[i] -= rhv.m_mtx[i];
			return *this;
		}

		std::string toString() const
		{
			std::string res = "";

			for (uint32_t i(0); i < N; ++i)
			{
				res += "| ";
				for (uint32_t j(0); j < N; ++j)
					res += std::to_string(at(i, j)) + " | ";
				res += '\n';
			}
			return res;
		}

	private:
		T m_mtx[N * N]{ 0 };

	};

	using mat2 = mtx<2>;
	using mat3 = mtx<3>;
	using mat4 = mtx<4>;

}
}
#endif /* !_MTX_HPP_ */
