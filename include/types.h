#ifndef TYPES_H
#define TYPES_H


#include "node.h"

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




const char * get_type_name(NodeType type);

void infer_types(Node * node);


#endif