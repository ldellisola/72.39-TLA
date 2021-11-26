#ifndef TYPES_H
#define TYPES_H
#include "node.h"


// It defines the possible types of the nodes and variables
typedef enum {
  TYPE_UNKOWN,
  TYPE_INT,
  TYPE_STRING,
  TYPE_FLOAT,
  TYPE_BOOL,
  TYPE_VIDEO,
  TYPE_AUDIO_STREAM,
  TYPE_VIDEO_STREAM,
} NodeType;



// It translates a data type to it's representation in string
const char * get_type_name(NodeType type);

// It will verify that all the datatypes are valid. If there are any errors they will be printed 
// using the error.h functions.
void infer_types(Node * node);


#endif