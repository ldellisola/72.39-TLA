#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "node.h"

// It translates the sintax tree to C# code and prints it to the console
void translate_to_console(Node * root);

// It prints the sintax tree to C# code and saves it in a given file
void translate_to_file(Node * root, const char * filePath);


#endif