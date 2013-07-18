/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <stdint.h>

namespace cobc
{
	/**
	 * Wrapper around C-style error codes.
	 *
	 * Should be used as a replacement for raw `int` return codes. Can be
	 * extended by own error codes by sub-classing this class and defining
	 * new static functions (like `success()` or `error()`). The name of these
	 * functions define the name of the corresponding error code. The user is
	 * responsible to ensure that error codes are unique.
	 *
	 * @author  Fabian Greif <fabian.greif@dlr.de>
	 */
	class ErrorCode
	{
		/*
		 * These type and member function is needed to implement the
		 * safe bool idiom (see e.g. http://www.artima.com/cppsource/safeboolP.html).
		 *
		 * A normal cast operator to bool would enable some invalid sematics
		 * for this class. For a C++11 a explicit cast operator could be used
		 * instead.
		 */
		typedef void (ErrorCode::*boot_type)() const;

		void
		this_type_does_no_support_comparisons() const
		{
		}

	public:
		/**
		 * Indicate a successful operation.
		 */
		static inline ErrorCode
		success()
		{
			return ErrorCode(0);
		}

		/**
		 * Indicate a successful operation with a success code.
		 *
		 * @param  successCode
		 *
		 * @return
		 */
		static inline ErrorCode
		success(uint16_t successCode)
		{
			return ErrorCode(successCode);
		}

		/**
		 * Indicate a generic error.
		 */
		static inline ErrorCode
		error()
		{
			return ErrorCode(-1);
		}

		/**
		 * Indicate a error with a specific error code.
		 *
		 * @param errorCode
		 *        Error code specifying the error that appeared. The error code
		 *        must be negative, `-1` is reserved for a generic error.
		 */
		static inline ErrorCode
		error(int16_t errorCode)
		{
			return ErrorCode(errorCode);
		}

		ErrorCode(const ErrorCode& other) :
			errorCode(other.errorCode)
		{
		}

		/**
		 * This class can be directly used in a boolean expression.
		 * A positive value (`true`) indicates a successful operation.
		 */
		// When using C++11 this function can be rewritten as:
		//     inline explicit
		//     operator bool() const
		//     {
		//         return isSuccess();
		//     }
		operator boot_type() const
		{
			return (isSuccess() ? &ErrorCode::this_type_does_no_support_comparisons : 0);
		}

		/**
		 * Check if the operation was successful.
		 *
		 * @retval `true`  if the operation was successful.
		 * @retval `false` if the operation failed.
		 */
		inline bool
		isSuccess() const
		{
			return (errorCode >= 0);
		}

		inline bool
		isError() const
		{
			return (errorCode < 0);
		}

		/**
		 * Query the error/success code.
		 *
		 * Every value smaller than zero is an error. `-1` indicates a
		 * generic error.
		 *
		 * @return  Error code.
		 */
		inline int16_t
		getCode() const
		{
			return errorCode;
		}

		inline bool
		operator == (const ErrorCode& other) const
		{
			return (errorCode == other.errorCode);
		}

		inline bool
		operator != (const ErrorCode& other) const
		{
			return (errorCode != other.errorCode);
		}

		inline bool
		operator !() const
		{
			return !isSuccess();
		}

	protected:
		explicit ErrorCode(int16_t code) :
			errorCode(code)
		{
		}

		// Disable assignment operator as errorCode is const and can not be
		// overwritten.
		ErrorCode&
		operator = (const ErrorCode& other);

	private:
		const int16_t errorCode;
	};
}

#endif // ERROR_CODE_H
