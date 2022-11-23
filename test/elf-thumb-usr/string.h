#ifndef usr_string_h
#define usr_string_h
#include "sys.h"

//   double atof(const char* nptr);
//   int atoi(const char* nptr);
//   long int atol(const char* nptr);
//   long long int atoll(const char* nptr);
//   double strtod(const char* nptr, char** endptr);
//   float strtof(const char* nptr, char** endptr);
//   long double strtold(const char* nptr, char** endptr);
//   long int strtol(const char* nptr, char** endptr, int base);
//   long long int strtoll(const char* nptr, char** endptr, int base);
//   unsigned long int strtoul(const char* nptr, char** endptr, int base);
//   unsigned long long int strtoull(const char* nptr, char** endptr, int base);
//  


// String manipulation
// strcpy
//  
// copies one string to another
// (function)
// strncpy
//  
// copies a certain amount of characters from one string to another
// (function)
// strcat
//  
// concatenates two strings
// (function)
// strncat
//  
// concatenates a certain amount of characters of two strings
// (function)
// strxfrm
//  
// transform a string so that strcmp would produce the same result as strcoll
// (function)
// String examination
// strlen
//  
// returns the length of a given string
// (function)
// strcmp
//  
// compares two strings
// (function)
// strncmp
//  
// compares a certain number of characters from two strings
// (function)
// strcoll
//  
// compares two strings in accordance to the current locale
// (function)
// strchr
//  
// finds the first occurrence of a character
// (function)
// strrchr
//  
// finds the last occurrence of a character
// (function)
// strspn
//  
// returns the length of the maximum initial segment that consists
// of only the characters found in another byte string
// (function)
// strcspn
//  
// returns the length of the maximum initial segment that consists
// of only the characters not found in another byte string
// (function)
// strpbrk
//  
// finds the first location of any character from a set of separators
// (function)
// strstr
//  
// finds the first occurrence of a substring of characters
// (function)
// strtok
//


// isalnum	Test for Alphanumeric
// isalpha	Test for Alphabetic
// iscntrl	Test for Control Character
// isdigit	Test for Digit
// isgraph	Test for Graphical Character (does not include a space)
// islower	Test for Lowercase Letter
// isprint	Test for Printing Character (does include a space)
// ispunct	Test for Punctuation Character
// isspace	Test for White-Space Character
// isupper	Test for Uppercase Letter
// isxdigit	Test for Hexadecimal Digit

#endif
