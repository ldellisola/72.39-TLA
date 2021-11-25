#include "../include/node.h"
#include "../include/types.h"
#include "../target/y.tab.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

Node * create_node(NodeId id, Node * left, Node * right, int line);
void delete_node(Node * node);

/*******************************************************************
*                       PUBLIC FUNCTIONS                           * 
*******************************************************************/

Node * node(NodeId id, Node * left, Node * right, int line){
  return create_node(id,left,right,line);
}

Node * leaf(NodeId id,char * value, int line){
  Node * ptr = create_node(id,NULL,NULL,line);

  if (value != NULL){
    ptr->value = calloc(strlen(value),sizeof(char));
    strcpy(ptr->value,value);
  }

  return ptr;
}

void free_node(Node* root){
  if (root == NULL)
    return;

  free_node(root->left);
  free_node(root->right);

  delete_node(root);
}


bool node_is_stream_keyword(Node * node){
  return  NODE_STREAM_CODEC <= node->id  && node->id <= NODE_STREAM_CHANNELS;
}

bool node_is_variable_type(Node * node){
  return NODE_VARIABLE_TYPE_INT <= node->id && node->id <= NODE_VARIABLE_TYPE_VIDEO_STREAM;
}

bool node_is_value_type(Node * node){
  return NODE_VALUE_STRING <= node->id && node->id <= NODE_VALUE_BOOL;
}

bool node_is_binary_operator(Node * node){
  return NODE_OPERATOR_EQUALS <= node->id && node->id <= NODE_OPERATOR_LOGIC_AND;
}

bool node_is_unary_operator(Node * node){
  return NODE_OPERATOR_NOT <= node->id && node->id <= NODE_OPERATOR_NOT;
}


bool node_is_parseable(Node * node){
  printf("%d\n",node->id);
  return NODE_VARIABLE_TYPE_BOOL == node->id || node->id == NODE_VARIABLE_TYPE_FLOAT || node->id == NODE_VARIABLE_TYPE_INT;
}



/*******************************************************************
*                      PRIVATE FUNCTIONS                           * 
*******************************************************************/


Node * create_node(NodeId id, Node * left, Node * right, int line){
  Node * ptr = (Node *)malloc(sizeof(Node));
  ptr->id = id;
  ptr->left = left;
  ptr->right = right;
  ptr->inferedType = TYPE_UNKOWN;
  ptr->line = line;
  return ptr;
}

void delete_node(Node * node){
  if (node->value != NULL){
    free(node->value);
  }

  free(node);
}










