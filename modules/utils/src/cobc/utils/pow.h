
#ifndef COBC_POW_H
#define COBC_POW_H

namespace cobc
{
	// ------------------------------------------------------------------------
	template <int N>
	struct PowerOfTwo
	{
		enum { value = 2 * PowerOfTwo<N-1>::value };
	};

	template <>
	struct PowerOfTwo<1>
	{
		enum { value = 2 };
	};

	template <>
	struct PowerOfTwo<0>
	{
		enum { value = 1 };
	};

	template <>
	struct PowerOfTwo<-1>
	{
		enum { value = 0 };
	};

	// ------------------------------------------------------------------------
	template <int Base, int Exponent>
	struct Pow
	{
		enum { value = Base * Pow<Base, Exponent-1>::value };
	};

	template <int Base>
	struct Pow<Base, 1>
	{
		enum { value = Base };
	};

	template <int Base>
	struct Pow<Base, 0>
	{
		enum { value = 1 };
	};

	template <int Base>
	struct Pow<Base, -1>
	{
		enum { value = 0 };
	};
}

#endif // COBC_POW_H
