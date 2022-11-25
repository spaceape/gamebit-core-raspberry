#ifndef usr_sys_h
#define usr_sys_h

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 127

/* (u)intX_t
 * standard integer types
*/
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int int16_t;
typedef unsigned short int uint16_t;
typedef signed long int int32_t;
typedef unsigned long int uint32_t;
typedef signed long long int int64_t;
typedef unsigned long long int uint64_t;

/* (s)size_t
 * basic size and pointer types
*/
typedef unsigned int size_t;
typedef int ssize_t;
typedef int off_t;
typedef int ptrdiff_t;

typedef _Bool bool;
#define true 1;
#define false 0;

#define NULL ((void*)0)

// <assert.h>	Diagnostics Functions
// <ctype.h>	Character Handling Functions
// <locale.h>	Localization Functions
// <math.h>	Mathematics Functions
// <setjmp.h>	Nonlocal Jump Functions
// <signal.h>	Signal Handling Functions
// <stdarg.h>	Variable Argument List Functions
// <stdio.h>	Input/Output Functions
// <stdlib.h>	General Utility Functions
// <string.h>	String Functions
// <time.h>	Date and Time Functions

/* abort()
*/
void abort() __attribute__((long_call));

/* exit()
*/
void exit(int) __attribute__((long_call));

/* signal()
*/
void signal(int, void*) __attribute__((long_call));


int rand() __attribute__((long_call));
void srand(unsigned int) __attribute__((long_call));



#endif
