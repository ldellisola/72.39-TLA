#ifndef HEADER_H
#define HEADER_H

#include <stdlib.h>
#include <stdbool.h>

// This enum define the types of node that our expression tree can have
typedef enum {

// Values represent data such as 123 or "hello"
  NODE_VALUE_STRING, NODE_VALUE_INT, NODE_VALUE_FLOAT,
  NODE_VALUE_BOOL,
  
// Statements are control flow elements
  NODE_STATEMENT_IF, NODE_STATEMENT_WHILE, NODE_STATEMENT_ELSE,

// It represents the name of a variable
  NODE_VARIABLE_NAME,

// It represents an operator
  NODE_OPERATOR_EQUALS, NODE_OPERATOR_NOT_EQUALS,
  NODE_OPERATOR_ADDITION, NODE_OPERATOR_SUBSTRACTION, 
  NODE_OPERATOR_DIVISION, NODE_OPERATOR_MULTIPLICATION,
  NODE_OPERATOR_GREATER_THAN, NODE_OPERATOR_LESS_THAN,
  NODE_OPERATOR_LOGIC_OR, NODE_OPERATOR_LOGIC_AND, NODE_OPERATOR_NOT,

//  It represents a function
  NODE_FUNCTION_PRINT,NODE_FUNCTION_READ, NODE_FUNCTION_TRANSCODE_VIDEO,
  NODE_FUNCTION_TRANSCODE_AUDIO, NODE_FUNCTION_APPEND_STREAM, 
  NODE_FUNCTION_SAVE_VIDEO, NODE_FUNCTION_OPEN_VIDEO, NODE_FUNCTION_SAVE_OVERWRITE_VIDEO,
  NODE_FUNCTION_PARSE_STRING,

// It represents either a declaration or an assignment
  NODE_ASSIGNMENT, NODE_DECLARATION  ,NODE_DECLARATION_ASSIGNMENT, NODE_ASSIGNMENT_OPEN_VIDEO, NODE_ASSIGNMENT_TRANSCODE,
  
// It represents a data type such as Int or String
  NODE_VARIABLE_TYPE_INT,NODE_VARIABLE_TYPE_FLOAT ,
  NODE_VARIABLE_TYPE_STRING,NODE_VARIABLE_TYPE_BOOL,
  NODE_VARIABLE_TYPE_VIDEO, NODE_VARIABLE_TYPE_AUDIO_STREAM , 
  NODE_VARIABLE_TYPE_VIDEO_STREAM,

// It represents the parameters of the transcode function
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

// It creates a node with it's type and children
Node * node(NodeId id, Node * left, Node * right, int line);

// It creates a leaf node without any children
Node * leaf(NodeId id,char * value, int line);

// It returns true if the node represents a stream parameter key word
bool node_is_stream_keyword(Node * node);

// It returns true when the node represents a variable type such as String
bool node_is_variable_type(Node * node);

// It returns true when the node represents a value, such as "hello"
bool node_is_value_type(Node * node);

// It returns true when the node represents a binary operator
bool node_is_binary_operator(Node * node);

// It returns true when the node represents an unary operator
bool node_is_unary_operator(Node * node);

// It returns true when the node represents a data type that is parseable, like Int
bool node_is_parseable(Node * node);

// It translates the type of the stream parameter into a string
const char * get_parameter_name(Node* node);

// It releases the memory from the node as well as 
void free_node(Node* root);


#endif