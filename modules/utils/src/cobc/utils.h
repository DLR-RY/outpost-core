
#ifndef COBC__UTILS_H
#define COBC__UTILS_H

#include "utils/deque.h"
#include "utils/pow.h"

/**
 * Get the size of C style array
 *
 * Example:
 *
 *     uint32_t a[2] = { ... };
 *     SIZEOF_ARRAY(a) => 2
 */
#define	SIZEOF_ARRAY(array)		(sizeof(array)/sizeof(array[0]))

/**
 * Align a variable to n byte border:
 *
 *     // 32 bit alignment
 *     uint8_t foo[10] ATTRIBUTE_ALIGNED(4) = { ... };
 *
 *     // 16 bit alignment
 *     uint8_t bar[123] ATTRIBUTE_ALIGNED(2) = { ... };
 *
 *     ...
 */
#define	ATTRIBUTE_ALIGNED(n)	__attribute__((aligned(n)))

namespace cobc
{
	/**
	 * Empty class used as dummy
	 */
	class Nil
	{
	};

	/**
	* @brief Selects type Result = (flag == true) ? T : U
	*/
	template <bool flag, typename T, typename U>
	struct Select
	{
		typedef T Result;
	};

	template <typename T, typename U>
	struct Select<false, T, U>
	{
		typedef U Result;
	};

	template<typename T> struct remove_const { typedef T type; };
	template<typename T> struct remove_const<T const volatile> {
	    typedef typename remove_const<T>::type volatile type;
	};
	template<typename T> struct remove_const<T volatile> {
	    typedef typename remove_const<T>::type volatile type;
	};
	template<typename T> struct remove_const<T const> {
	    typedef typename remove_const<T>::type type;
	};
	template<typename T> struct remove_const<T&> {
	    typedef typename remove_const<T>::type& type;
	};
	template<typename T> struct remove_const<T*> {
	    typedef typename remove_const<T>::type* type;
	};
}

/**
 * Avoid using virtual inheritance but be able to mock the class
 * for unit-testing.
 *
 * Example:
 *
 *     class CommandScheduleInterface
 *     {
 *     public:
 *         virtual void
 *         reset();
 *     }
 *
 *     class CommandSchedule : UNITTEST_INHERIT(CommandScheduleInterface)
 *     {
 *     public:
 *         void
 *         reset();
 *     };
 *
 * When UNITTEST is defined CommandSchedule will inherit from
 * CommandScheduleInterface thus making reset() virtual. In any other case
 * it will only inherit from cobc::Nil which is an empty class not altering
 * CommandSchedule in any way.
 */
#if defined(UNITTEST) || defined(__DOXYGEN__)
#	define UNITTEST_INHERIT(x)	public x
#else
#	define UNITTEST_INHERIT(x)	public ::cobc::Nil
#endif

#endif	// COBC__UTILS_H
