#ifndef ERRORS_H
#define ERRORS_H
#include "node.h"
#include <stdarg.h>

// This function returns true if the program had any compilation errors
bool has_compilation_errors();

// It raises a compilation TypeError with a custom message
void type_error(Node * node,const char * string, ...);

// It raises a compilation ReferenceError with a custom message
void reference_error(Node * node, const char * string, ...);

// It raises a compilation TranslationError with a custom message
void translation_error(Node * node, const char * string, ...);

// It raises a compilation ParseError with a custom message
void parse_error(int line, const char * string, ...);

// It raises a compilation UnknownError with a custom message
void unknown_error(const char * string,...);

#endif