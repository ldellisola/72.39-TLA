#ifndef VARIABLES_H
#define VARIABLES_H

#include "node.h"
#include "types.h"

typedef struct _var
{
  const char * name;
  NodeType type;
  int references;
  int first_appearence;
  bool was_assigned;
} Variable;

// It will validate that all the variables defined in the program are valid, and initialized.
// If there are any errors, they will be printed using the errors.c functions.
void validate_variables(Node* root);

// It clears the memory used up by storing all the variables and their references.
void dispose_variables();

// It returns a variable by its name
Variable * find_variable(const char * name);

// It returns the data type of a variable given its name
NodeType get_variable_type(const char * name);

// It sets the data type of a variable given its name
void set_variable_type(const char * name, NodeType type);



#endif