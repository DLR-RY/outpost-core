/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef DETECT_H
#define DETECT_H

/**
 * Possible values for `COBC_RTOS`.
 *
 * @addtogroup rtos
 * @{
 */
#define COBC_RTOS_NONE		0
#define COBC_RTOS_RTEMS		1
#define COBC_RTOS_FREERTOS	2
#define COBC_RTOS_POSIX		3
/**@}*/

// Check that COBC_RTOS is set.
#if !defined(COBC_RTOS)
#	error "No operating system defined. Please define 'COBC_RTOS' to specify" \
	      "the used operating system. Possible values are 'COBC_RTOS_(FREERTOS|RTEMS|POSIX|NONE)'." \
	      "For GCC use something like '-DCOBC_RTOS=COBC_RTOS_RTEMS'."
#endif

// Check that the value of COBC_RTOS is valid.
#if COBC_RTOS == COBC_RTOS_NONE
#elif COBC_RTOS == COBC_RTOS_RTEMS
#	if !defined(__rtems__)
#		error "'COBC_RTOS_RTEMS' defined by not '__rtems__'. Please check your toolchain!"
#	endif
#elif COBC_RTOS == COBC_RTOS_FREERTOS
#elif COBC_RTOS == COBC_RTOS_POSIX
// TODO check what defined are needed for posix conformance and test against these.
#	if !defined(__unix__)
#		error "'COBC_RTOS_POSIX' defined by not '__unix__'. Please check your toolchain!"
#	endif
#else
#	error "Invalid operating system defined. Please define 'COBC_RTOS' to specify" \
	      "the used operating system. Possible values are 'COBC_RTOS_(FREERTOS|RTEMS|UNIX|NONE)'."
#endif

#endif // DETECT_H
