#ifndef PRINT_H
#define PRINT_H

#include "node.h"

// It prints a node's information to standard out.
// The output depends on the type of node that is printed, but it generally follows this structure:
// "NODE: {pointer} | LINE: {line at} | TYPE: {data type} | {node type}"
void print_node(Node * node);


// This function prints to the console the current state of the sintax tree.
void print_tree(Node * root, int depth);


#endif