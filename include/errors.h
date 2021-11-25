#ifndef ERRORS_H
#define ERRORS_H
#include "node.h"
#include <stdarg.h>

bool has_compilation_errors();
void type_error(Node * node,const char * string, ...);
void reference_error(Node * node, const char * string, ...);
void translation_error(Node * node, const char * string, ...);
void parse_error(int line, const char * string, ...);
void unknown_error(const char * string,...);

#endif