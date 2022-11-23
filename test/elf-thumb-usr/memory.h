#ifndef usr_memory_h
#define usr_memory_h
#include "sys.h"

void* malloc(size_t) __attribute__((long_call));
void* realloc(void*, size_t) __attribute__((long_call));
void  free(void*) __attribute__((long_call));

void* memcpy(void* restrict, const void* restrict s2, size_t) __attribute__((long_call));

// finds the next token in a byte string
// (function)
// Character array manipulation
// memchr
//  
// searches an array for the first occurrence of a character
// (function)
// memcmp
//  
// compares two buffers
// (function)
// memset
//  
// fills a buffer with a character
// (function)
// memcpy
//  
// copies one buffer to another
// (function)
// memmove
//  
// moves one buffer to another
// (function)
// 

#endif
