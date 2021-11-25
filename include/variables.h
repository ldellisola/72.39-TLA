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

void validate_variables(Node* root);

void dispose_variables();

Variable * find_variable(const char * name);

NodeType get_variable_type(const char * name);
void set_variable_type(const char * name, NodeType type);



#endif