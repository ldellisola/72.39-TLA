#include "../include/errors.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "../include/node.h"

static int error_count = 0;


void print_error(int line,const char * error, const char * message, va_list params);

/*******************************************************************
*                       PUBLIC FUNCTIONS                           * 
*******************************************************************/

bool has_compilation_errors(){
  return error_count != 0;
}

void type_error(Node * node,const char * string, ...){
  va_list argptr;
  va_start(argptr, string);
  print_error(node->line,"TypeError",string,argptr);
  va_end(argptr);

}

void reference_error(Node * node, const char * string, ...){
  va_list argptr;
  va_start(argptr, string);
  print_error(node->line,"ReferenceError",string,argptr);
  va_end(argptr);
}

void translation_error(Node * node, const char * string, ...){
  va_list argptr;
  va_start(argptr, string);
  print_error(node->line,"TranslationError",string,argptr);
  va_end(argptr);
}

void parse_error(int line, const char * string, ...){
  va_list argptr;
  va_start(argptr, string);
  print_error(line,"ParseError",string,argptr);
  va_end(argptr);
}

void unknown_error(const char * string,...){
  va_list argptr;
  va_start(argptr, string);
  print_error(0,"UnknownError",string,argptr);
  va_end(argptr);
}



/*******************************************************************
*                      PRIVATE FUNCTIONS                           * 
*******************************************************************/

void print_error(int line,const char * error, const char * message, va_list params){
  error_count++;

  fprintf(stderr,"At line %d -> %s: ",line,error);
  vfprintf(stderr, message, params);
  fprintf(stderr,"\n");
}

