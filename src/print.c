#include "../include/print.h"
#include "variables.h"

#include <stdio.h>


void print_tree(Node * node, int depth){
  if (depth == 0){
    printf("--------------------------\n TREE \n--------------------------\n");
  }

  for(int i = 0; i < depth ; i++){
    printf("| ");
  }


  printf("-> ");
  print_node(node);

  if (node == NULL)
  {
    return;
  }

  print_tree(node->left,depth+1);
  print_tree(node->right,depth +1);

}


void print_node(Node * node){
  printf("NODE: %p |",node);

  if (node == NULL)
  {
    printf("\n");
    return;
  }
  printf(" LINE: %d |", node->line);
  printf(" TYPE: %s | ",get_type_name(node->inferedType));

  switch (node->id)
  {
  case NODE_STATEMENT_IF:
    printf("IF \n");
    break;
  case NODE_STATEMENT_ELSE:
    printf("ELSE \n");
    break;
  case NODE_STATEMENT_WHILE:
    printf("WHILE \n");
    break;
  case NODE_VARIABLE_NAME:
    const Variable * var = find_variable(node->value);

    if(var != NULL) {
    printf("NAME: %s | FIRST LINE: %d | REFERENCES: %d\n",
          node->value,
          var->first_appearence,
          var->references
          );
    }
    else {
      printf("NAME: %s\n",node->value);
    }
    break;
  case NODE_VALUE_STRING:
  printf("STRING: %s\n",node->value);         
    break;
  case NODE_VALUE_INT:
  printf("INT: %s\n",node->value);
    break;
  case NODE_VALUE_FLOAT:
  printf("FLOAT: %s\n",node->value);
    break;
  case NODE_VALUE_BOOL:
    printf("BOOL: %s\n",node->value);
    break;
  case NODE_NEXT_LINE:
    printf("NEXT LINE\n");
    break;
  case NODE_OPERATOR_EQUALS:
    printf("EQUALS\n");
    break;
  case NODE_OPERATOR_NOT_EQUALS:
    printf("NOT EQUALS\n");
    break;
  case NODE_OPERATOR_GREATER_THAN:
    printf("GREATER\n");
    break;
  case NODE_OPERATOR_LESS_THAN:
    printf("LESSER\n");

    break;
  case NODE_OPERATOR_LOGIC_AND:
        printf("AND\n");

    break;
  case NODE_OPERATOR_LOGIC_OR:
      printf("OR\n");
    break;
  case NODE_OPERATOR_NOT:
      printf("NOT\n");
    break;
  case NODE_ASSIGNMENT:
    printf("ASSIGNMENT\n");
    break;
  case NODE_ASSIGNMENT_OPEN_VIDEO:
    printf("OPEN VIDEO\n");
    break;
  case NODE_DECLARATION:
    printf("DECLARATION\n");
    break;
  case NODE_DECLARATION_ASSIGNMENT:
    printf("DECLARATION AND ASS\n");
    break;
  case NODE_OPERATOR_ADDITION:
    printf("ADDITION\n");
    break;
  case NODE_OPERATOR_DIVISION:
    printf("DIVITION\n");
    break;
  case NODE_OPERATOR_SUBSTRACTION:
    printf("SUBSTRACTION\n");
    break;
  case NODE_OPERATOR_MULTIPLICATION:
    printf("PRODUCT\n");
    break;
  case NODE_FUNCTION_PRINT:
    printf("PRINT\n");
    break;
  case NODE_FUNCTION_READ:
    printf("READ\n");
    break;

  case NODE_VARIABLE_TYPE_INT:
    printf("INT\n");
    break;
  case NODE_VARIABLE_TYPE_FLOAT:
    printf("FLOAT\n");
    break;
  case NODE_VARIABLE_TYPE_STRING:
    printf("STRING\n");
    break;
  case NODE_VARIABLE_TYPE_BOOL:
    printf("BOOL\n");
    break;
  case NODE_VARIABLE_TYPE_VIDEO:
    printf("VIDEO\n");
    break;
  case NODE_VARIABLE_TYPE_VIDEO_STREAM:
    printf("VIDEO STREAM\n");
    break;
  case NODE_VARIABLE_TYPE_AUDIO_STREAM:
    printf("AUDIO STREAM\n");
    break;
  case NODE_STREAM_BITRATE:
    printf("BITRATE KEYWORD\n");
    break;
  case NODE_STREAM_FRAMERATE:
    printf("FRAMERATE KEYWORD\n");
    break;
  case NODE_STREAM_SPEED:
    printf("SPEED KEYWORD\n");
    break;
  case NODE_STREAM_SIZE:
    printf("SIZE KEYWORD\n");
    break;
  case NODE_STREAM_CODEC:
    printf("CODEC KEYWORD\n");
    break;
  case NODE_TRANSCODE_PARAMETER:
    printf("TRANSCODE PARAMETER\n");
    break;
  case NODE_TRANSCODE_PARAMETER_LIST:
    printf("TRANSCODE PARAMETER LIST\n");
    break;
  case NODE_FUNCTION_TRANSCODE_VIDEO:
    printf("TRANSCODE VIDEO\n");
    break;
  case NODE_ASSIGNMENT_TRANSCODE:
    printf("ASSIGNMENT TRANSCODE\n");
    break;
  case NODE_FUNCTION_APPEND_STREAM:
    printf("APPEND STRING\n");
  break;
  case NODE_FUNCTION_SAVE_VIDEO:
    printf("SAVE VIDEO\n");
  break;
  case NODE_FUNCTION_TRANSCODE_AUDIO:
    printf("TRANSCODE AUDIO\n");
  break;
  case NODE_STREAM_CHANNELS:
    printf("CHANNELS KEYWORD\n");
  break;
  default:
    printf("?????\n");
    break;
  }
}


