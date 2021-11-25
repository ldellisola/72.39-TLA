#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "node.h"

void translate_to_console(Node * root);
void translate_to_file(Node * root, const char * filePath);


#endif