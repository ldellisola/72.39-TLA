#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <stdbool.h>


typedef enum {

  NODE_VALUE_STRING, NODE_VALUE_INT, NODE_VALUE_FLOAT,
  NODE_VALUE_BOOL,
  
  NODE_STATEMENT_IF, NODE_STATEMENT_WHILE, NODE_STATEMENT_ELSE,

  NODE_VARIABLE_NAME,

  NODE_OPERATOR_EQUALS, NODE_OPERATOR_NOT_EQUALS,
  NODE_OPERATOR_ADDITION, NODE_OPERATOR_SUBSTRACTION, 
  NODE_OPERATOR_DIVISION, NODE_OPERATOR_MULTIPLICATION,
  NODE_OPERATOR_GREATER_THAN, NODE_OPERATOR_LESS_THAN,
  NODE_OPERATOR_LOGIC_OR, NODE_OPERATOR_LOGIC_AND, NODE_OPERATOR_NOT,
  
 
  NODE_FUNCTION_PRINT,NODE_FUNCTION_READ, NODE_FUNCTION_TRANSCODE_VIDEO,
  NODE_FUNCTION_TRANSCODE_AUDIO, NODE_FUNCTION_APPEND_STREAM, 
  NODE_FUNCTION_SAVE_VIDEO, NODE_FUNCTION_OPEN_VIDEO, NODE_FUNCTION_SAVE_OVERWRITE_VIDEO,
  NODE_FUNCTION_PARSE_STRING,

  NODE_ASSIGNMENT, NODE_DECLARATION  ,NODE_DECLARATION_ASSIGNMENT, NODE_ASSIGNMENT_OPEN_VIDEO, NODE_ASSIGNMENT_TRANSCODE,
  

  NODE_VARIABLE_TYPE_INT,NODE_VARIABLE_TYPE_FLOAT ,
  NODE_VARIABLE_TYPE_STRING,NODE_VARIABLE_TYPE_BOOL,
  NODE_VARIABLE_TYPE_VIDEO, NODE_VARIABLE_TYPE_AUDIO_STREAM , 
  NODE_VARIABLE_TYPE_VIDEO_STREAM,

  NODE_TRANSCODE_PARAMETER_LIST,  NODE_TRANSCODE_PARAMETER, 
  
  NODE_STREAM_CODEC, NODE_STREAM_SIZE, 
  NODE_STREAM_SPEED, NODE_STREAM_FRAMERATE, NODE_STREAM_BITRATE, NODE_STREAM_CHANNELS,

 
  NODE_NEXT_LINE,
  NODE_C_SHARP_CODE,

  NODE_UNKNOWN
  
} NodeId;


typedef struct _node {
  NodeId id;
  int inferedType;
  char * value;
  int line;
  struct _node *left, *right;
} Node;


Node * node(NodeId id, Node * left, Node * right, int line);

Node * leaf(NodeId id,char * value, int line);

bool node_is_stream_keyword(Node * node);
bool node_is_variable_type(Node * node);
bool node_is_value_type(Node * node);
bool node_is_binary_operator(Node * node);
bool node_is_unary_operator(Node * node);

bool node_is_parseable(Node * node);

void free_node(Node* root);


#endif