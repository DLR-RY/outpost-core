/*
 * @file	harness.h
 * @brief	Test harness
 * 
 * @author	Fabian Greif <fabian.greif@dlr.de>
 */

#ifndef COBC__HARNESS_H
#define COBC__HARNESS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cobc/utils.h>

#define EXPECT_ARRAY_EQ(type, reference, actual, element_count) 			\
	do {																	\
		const type* reference_ = static_cast<const type *> (reference);		\
		const type* actual_ = static_cast<const type *> (actual);			\
		for(std::size_t i = 0; i < element_count; ++i ) {					\
			EXPECT_EQ(reference_[i], actual_[i]);							\
		}																	\
	} while (0)

#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

#endif // COBC__HARNESS_H
