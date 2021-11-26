#include "../include/variables.h"
#include "../include/node.h"
#include "../include/print.h"
#include "../include/errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Internal recursive function to validate the variables in a program.
void count_variables(Node * node);

// Internal function that creates a variable
Variable * create_variable(const char * name, int line);

#define MAX_VARIABLES  256

static Variable * variables[MAX_VARIABLES];

static int total_variables = 0;

/*******************************************************************
*                       PUBLIC FUNCTIONS                           * 
*******************************************************************/


void validate_variables(Node* root){
  count_variables(root);
}

void dispose_variables(){
  for(int i = 0; i< MAX_VARIABLES && variables[i] != NULL; i++){
    free(variables[i]);
  }
}

NodeType get_variable_type(const char * name){
  Variable * var = find_variable(name);
  return (var != NULL) ? var->type : TYPE_UNKOWN;
}

void set_variable_type(const char * name, NodeType type){
  Variable * var = find_variable(name);
  if (var != NULL)
  {
    var->type = type;
  }
}

Variable * find_variable(const char * name){
  bool found = false;

  for (int i = 0 ; i < total_variables && i < MAX_VARIABLES; i++){
    found |= strcmp(name,variables[i]->name) == 0;
    if(found){
      return variables[i];
    }
  }
  return NULL;
}


/*******************************************************************
*                  ACTUAL VARIABLE VERIFICATION                    * 
*******************************************************************/

void validate_variable_name(Node * node);
void validate_declaration_and_assignment(Node * node);
void validate_declaration(Node * node);
void validate_assignment(Node * node);

void count_variables(Node * node){

  if (node == NULL){
    return;
  }

  switch(node->id){
    case NODE_VARIABLE_NAME:
      validate_variable_name(node);
    break;

    
    
    // caso String a = "hola";
    case NODE_DECLARATION_ASSIGNMENT:
      validate_declaration_and_assignment(node);
    break;

    // caso String a;
    case NODE_DECLARATION:
      validate_declaration(node);
    break;
    case NODE_FUNCTION_READ:
    case NODE_ASSIGNMENT_TRANSCODE:
    case NODE_ASSIGNMENT_OPEN_VIDEO:
    case NODE_ASSIGNMENT:
      validate_assignment(node);
    break;

    default:
      count_variables(node->left);
      count_variables(node->right);
    break;
  }
}

// It prints an error if the variable was not initialized
bool check_variable_was_assigned(Variable * var,Node * node);

// It prints an error if the variable was not defined
bool check_variable_use(Variable * var,const char * name,Node * node);

// It prints an error if the variable was already defined
void check_variable_assignment(const char * name,Node * node);


void validate_variable_name(Node * node){
      const char * var_name = node->value;
      Variable * var = find_variable(var_name);
      if (check_variable_use(var,var_name,node) && 
          check_variable_was_assigned(var,node)) {
        var->references++;
      } 
}

void validate_declaration_and_assignment(Node * node){
      count_variables(node->right->right);
      const char * var_name = node->right->left->value;
      check_variable_assignment(var_name,node);
      Variable * var = create_variable(var_name,node->line);
      var->was_assigned = true;
}

void validate_declaration(Node * node){
      const char * var_name = node->right->value;
      check_variable_assignment(var_name,node);
      create_variable(var_name,node->line);
      count_variables(node->right);
}

void validate_assignment(Node * node){
      count_variables(node->right);
      Variable * var = find_variable(node->left->value);
      if( var != NULL){
        var->was_assigned = true;
      }
      count_variables(node->left);
}

/*******************************************************************
*                       PRIVATE FUNCTIONS                          * 
*******************************************************************/


Variable * create_variable(const char * name, int line){

  if (total_variables +1 == MAX_VARIABLES){
    unknown_error("There too many variables defined. Shutting down.");
    exit(-1);
  }

  Variable * var = malloc(sizeof(Variable));
  var->name = name;
  var->references = 1;
  var->first_appearence = line;
  var->type = TYPE_UNKOWN;
  var->was_assigned = false;

  variables[total_variables] = var;
  total_variables++;

  return var;
}


/*******************************************************************
*                             VALIDATIONS                          * 
*******************************************************************/


bool check_variable_use(Variable * var,const char * name,Node * node){
  if (var == NULL){
    reference_error(node,"The variable '%s' was not defined",
      name
    );
  }
  return var != NULL;
}

bool check_variable_was_assigned(Variable * var,Node * node){
  if( !var->was_assigned && var->references > 1){
    reference_error(node,"The variable '%s' was not initialized",
      var->name
    );
    return false;
  }

  return true;
}

void check_variable_assignment(const char * name,Node * node){
 Variable * var = find_variable(name);
  if (var != NULL){
    reference_error(node,"The variable '%s' has already been defined on line %d",
      var->name,
      var->first_appearence
    );
  }
}



