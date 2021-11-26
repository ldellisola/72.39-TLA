#include "../include/types.h"
#include "../include/node.h"
#include "../include/print.h"
#include "../include/variables.h"
#include "../include/errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// It returns true if it is possible to compare with the equality operators (== !=) two different 
// data types
bool can_use_equality_operator(NodeType left_node_type, NodeType right_node_type);

// It returns the result data type of adding up two different data types. If the return type is
// unknown, then the operation is not valid.
NodeType get_addition_return_type(NodeType left_node_type, NodeType right_node_type);

// It returns the result data type of an aritmetic operation between two different data types. If the return type is
// unknown, then the operation is not valid.
NodeType get_aritmetic_operator_return_type(NodeType left_node_type, NodeType right_node_type);

// It prints an error if we mix up data types at an assignment
void check_type_error_new_assignment(NodeType left_node_type, NodeType right_node_type, Node * node );

// It prints an error if we try to open a video into a data type thats not Video
void check_type_variable_open_video(NodeType type,Node * node);

// Internal recursive function that will calculate and verify the types of the program
NodeType get_node_type(Node * node);

// It assigns the data type to a variable stored in variables.c
void node_assignment(Node * node, NodeType type);

// It returns true if the type is Int or Float
bool is_type_numeric(NodeType type);

// It returns true if the type can be converted to string, like bool or Int
bool can_type_be_stringified(NodeType type);


/*******************************************************************
*                       LOOKUP TABLES                              * 
*******************************************************************/

static NodeType type_lookup_table[] = {
    TYPE_STRING,  //  NODE_VALUE_STRING
    TYPE_INT,     //  NODE_VALUE_INT
    TYPE_FLOAT,   //  NODE_VALUE_FLOAT
    TYPE_BOOL     //  NODE_VALUE_BOOL 
};

static NodeType variable_type_lookup_table[] = {
    TYPE_INT,        //  NODE_VARIABLE_TYPE_INT
    TYPE_FLOAT,           //  NODE_VARIABLE_TYPE_FLOAT  
    TYPE_STRING,         //  NODE_VARIABLE_TYPE_STRING
    TYPE_BOOL,          //  NODE_VARIABLE_TYPE_BOOL    
    TYPE_VIDEO,         //  NODE_VARIABLE_TYPE_VIDEO
    TYPE_AUDIO_STREAM,  //  NODE_VARIABLE_TYPE_AUDIO_STREAM
    TYPE_VIDEO_STREAM,  //  NODE_VARIABLE_TYPE_VIDEO_STREAM
};

static NodeType stream_parameter_type_lookup_table[] = {
  TYPE_STRING,    // NODE_STREAM_CODEC 
  TYPE_STRING,    // NODE_STREAM_SIZE
  TYPE_FLOAT,     // NODE_STREAM_SPEED
  TYPE_FLOAT,     // NODE_STREAM_FRAMERATE
  TYPE_FLOAT,     // NODE_STREAM_BITRATE
  TYPE_INT,       // NODE_STREAM_CHANNELS
};

static const char * type_name_lookup_table[] = {
  "Int",           // TYPE_INT /
  "String",        // TYPE_STRING
  "Float",         // TYPE_FLOAT
  "Bool",          // TYPE_BOOL
  "Video",         // TYPE_VIDEO
  "Audio Stream",  // TYPE_AUDIO_STREAM
  "Video Stream",  // TYPE_VIDEO_STREAM
};

static NodeType addition_operator_type_lookup_table[7][7] = 
            { //        INT           STRING        FLOAT          BOOL          VIDEO      AUDIO STREAM    VIDEO STREAM
/*INT   */          {TYPE_INT,    TYPE_STRING,  TYPE_FLOAT,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*STRING*/          {TYPE_STRING, TYPE_STRING,  TYPE_STRING,  TYPE_STRING,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*FLOAT */          {TYPE_FLOAT,  TYPE_STRING,  TYPE_FLOAT,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*BOOL  */          {TYPE_UNKOWN, TYPE_STRING,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*VIDEO  */         {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*AUDIO STREAM  */  {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*VIDEO STREAM */   {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
            };

static NodeType aritmetic_operator_type_lookup_table[7][7] = 
            { //       INT           STRING        FLOAT          BOOL           VIDEO      AUDIO STREAM    VIDEO STREAM
      /*INT   */    {TYPE_INT,    TYPE_UNKOWN,  TYPE_FLOAT,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
      /*STRING*/    {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
      /*FLOAT */    {TYPE_FLOAT,  TYPE_UNKOWN,  TYPE_FLOAT,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
      /*BOOL  */    {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
      /*VIDEO  */   {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*AUDIO STREAM  */  {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
/*VIDEO STREAM */   {TYPE_UNKOWN, TYPE_UNKOWN,  TYPE_UNKOWN,  TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN,   TYPE_UNKOWN},
            };

static bool equality_operator_type_lookup_table[7][7] = 
            { //        INT           STRING        FLOAT          BOOL            VIDEO        AUDIO STREAM     VIDEO STREAM
/*INT   */            {true,            false,        true,         false ,         false   ,         false  ,         false  },
/*STRING*/            {false,           true,         false,        false ,         false   ,         false  ,         false  },
/*FLOAT */            {true,            false,        true,         false ,         false   ,         false  ,         false  },
/*BOOL  */            {false,           false,        false,        true ,          false   ,         false  ,         false  },
/*VIDEO */            {false,           false,        false,        false ,         true    ,         false  ,         false  },
/*AUDIO STREAM */     {false,           false,        false,        false ,         false   ,         true   ,         false  },
/*VIDEO STREAM */     {false,           false,        false,        false ,         false   ,         false  ,         true   },
            };


/*******************************************************************
*                       PUBLIC FUNCTIONS                           * 
*******************************************************************/

const char * get_type_name(NodeType type){
  if (TYPE_INT <= type && type <= TYPE_VIDEO_STREAM ) {
    return type_name_lookup_table[type-TYPE_INT];
  } else {
    return "Unknown";
  }
}

void infer_types(Node * node){
  get_node_type(node);
}

/*******************************************************************
*                      PRIVATE FUNCTIONS                           * 
*******************************************************************/

bool can_use_equality_operator(NodeType left_node_type, NodeType right_node_type){
  if (left_node_type == TYPE_UNKOWN || right_node_type == TYPE_UNKOWN)
  {
    return false;
  }
  int positionFix = TYPE_INT;
  return equality_operator_type_lookup_table[left_node_type-positionFix][right_node_type - positionFix];
}

NodeType get_addition_return_type(NodeType left_node_type, NodeType right_node_type){
  if (left_node_type == TYPE_UNKOWN || right_node_type == TYPE_UNKOWN)
  {
    return TYPE_UNKOWN;
  }
  int positionFix = TYPE_INT;
  return addition_operator_type_lookup_table[left_node_type-positionFix][right_node_type - positionFix];
}

NodeType get_aritmetic_operator_return_type(NodeType left_node_type, NodeType right_node_type){
  if (left_node_type == TYPE_UNKOWN || right_node_type == TYPE_UNKOWN)
  {
    return TYPE_UNKOWN;
  }
  int positionFix = TYPE_INT;
  return aritmetic_operator_type_lookup_table[left_node_type-positionFix][right_node_type - positionFix];
}

/*******************************************************************
*                        TYPES DEFINITION                          * 
*******************************************************************/

NodeType inspect_node_value_type(Node * node);
NodeType inspect_default_type(Node * node);
NodeType inspect_variable_name_type(Node * node);
NodeType inspect_equality_operator_type(Node * node);
NodeType inspect_aritmetic_operator_type(Node * node);
NodeType inspect_comparable_operator_type(Node * node);
NodeType inspect_logical_operator_type(Node * node);
NodeType inspect_unary_operator_type(Node * node);
NodeType inspect_print_function_type(Node* node);
NodeType inspect_read_function_type(Node* node);
NodeType inspect_transcode_function_type(Node* node);
NodeType inspect_append_stream_function_type(Node *node);
NodeType inspect_save_video_function_type(Node * node);
NodeType inspect_open_video_function_type(Node * node);
NodeType inspect_parse_string_function_type(Node * node);
NodeType inspect_assignment_type(Node * node);
NodeType inspect_declaration_type(Node * node);
NodeType inspect_declaration_and_assignment_type(Node * node);
NodeType inspect_assignment_video_type(Node * node);
NodeType inspect_assignment_transcode_type(Node* node);
NodeType inspect_variable_type(Node* node);
NodeType inspect_parameter_type(Node* node);
NodeType inspect_keyword_type(Node * node);

NodeType (* inspect_type[])(Node *) = {
  inspect_node_value_type,                        // NODE_VALUE_STRING
  inspect_node_value_type,                        // NODE_VALUE_INT
  inspect_node_value_type,                        // NODE_VALUE_FLOAT
  inspect_node_value_type,                        // NODE_VALUE_BOOL
  inspect_default_type,                           // NODE_STATEMENT_IF
  inspect_default_type,                           // NODE_STATEMENT_WHILE
  inspect_default_type,                           // NODE_STATEMENT_ELSE
  inspect_variable_name_type,                     // NODE_VARIABLE_NAME
  inspect_equality_operator_type,                 // NODE_OPERATOR_EQUALS
  inspect_equality_operator_type,                 // NODE_OPERATOR_NOT_EQUALS
  inspect_aritmetic_operator_type,                // NODE_OPERATOR_ADDITION
  inspect_aritmetic_operator_type,                // NODE_OPERATOR_SUBSTRACTION
  inspect_aritmetic_operator_type,                // NODE_OPERATOR_DIVISION
  inspect_aritmetic_operator_type,                // NODE_OPERATOR_MULTIPLICATION
  inspect_comparable_operator_type,               // NODE_OPERATOR_GREATER_THAN
  inspect_comparable_operator_type,               // NODE_OPERATOR_LESS_THAN
  inspect_logical_operator_type,                  // NODE_OPERATOR_LOGIC_OR
  inspect_logical_operator_type,                  // NODE_OPERATOR_LOGIC_AND
  inspect_unary_operator_type,                    // NODE_OPERATOR_NOT
  inspect_print_function_type,                     // NODE_FUNCTION_PRINT
  inspect_read_function_type,                     // NODE_FUNCTION_READ
  inspect_transcode_function_type,                // NODE_FUNCTION_TRANSCODE_VIDEO
  inspect_transcode_function_type,                // NODE_FUNCTION_TRANSCODE_AUDIO
  inspect_append_stream_function_type,            // NODE_FUNCTION_APPEND_STREAM
  inspect_save_video_function_type,               // NODE_FUNCTION_SAVE_VIDEO
  inspect_open_video_function_type,               // NODE_FUNCTION_OPEN_VIDEO
  inspect_save_video_function_type,               // NODE_FUNCTION_SAVE_OVERWRITE_VIDEO
  inspect_parse_string_function_type,             // NODE_FUNCTION_PARSE_STRING
  inspect_assignment_type,                        // NODE_ASSIGNMENT
  inspect_declaration_type,                       // NODE_DECLARATION
  inspect_declaration_and_assignment_type,        // NODE_DECLARATION_ASSIGNMENT
  inspect_assignment_video_type,                  // NODE_ASSIGNMENT_OPEN_VIDEO
  inspect_assignment_transcode_type,              // NODE_ASSIGNMENT_TRANSCODE
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_INT
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_FLOAT
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_STRING
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_BOOL
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_VIDEO
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_AUDIO_STREAM
  inspect_variable_type,                          // NODE_VARIABLE_TYPE_VIDEO_STREAM
  inspect_default_type,                           // NODE_TRANSCODE_PARAMETER_LIST
  inspect_parameter_type,                         // NODE_TRANSCODE_PARAMETER, 
  inspect_keyword_type,                           // NODE_STREAM_CODEC
  inspect_keyword_type,                           // NODE_STREAM_SIZE
  inspect_keyword_type,                           // NODE_STREAM_SPEED
  inspect_keyword_type,                           // NODE_STREAM_FRAMERATE
  inspect_keyword_type,                           // NODE_STREAM_BITRATE
  inspect_keyword_type,                           // NODE_STREAM_CHANNELS
  inspect_default_type,                           // NODE_NEXT_LINE
  inspect_default_type,                           // NODE_C_SHARP_CODE
  inspect_default_type,                           // NODE_UNKNOWN
};

/*******************************************************************
*                   RECURSIVE TYPE DEFINITION                      * 
*******************************************************************/

NodeType get_node_type(Node * node) {
  if (node == NULL){
    return TYPE_UNKOWN;
  }

  if(node->inferedType != TYPE_UNKOWN){
    return node->inferedType;
  }

  return inspect_type[node->id](node);

}

/*******************************************************************
*                   TYPE DEFINITION FUNCTIONS                      * 
*******************************************************************/

NodeType inspect_node_value_type(Node * node){
  if (!node_is_value_type(node)) {
    return TYPE_UNKOWN;
  }

  return type_lookup_table[node->id];
}

NodeType inspect_default_type(Node * node){
  get_node_type(node->left);
  get_node_type(node->right);
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_variable_name_type(Node * node){
  return node->inferedType = get_variable_type(node->value);
}

NodeType inspect_equality_operator_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);

  if (!can_use_equality_operator(left_type, right_type))
  {
    fprintf(stderr,
        "At line %d -> TypeError: The values of type '%s' and '%s' are not comparable",
        node->line,
        get_type_name(left_type),
        get_type_name(right_type)
      );
  }
  
  return node->inferedType = TYPE_BOOL;
}

NodeType inspect_aritmetic_operator_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);
  NodeType return_type;

  if(node->id == NODE_OPERATOR_ADDITION){
    return_type = get_addition_return_type(left_type,right_type);
  } else {
    return_type =  get_aritmetic_operator_return_type(left_type,right_type); 
  }

  if (return_type == TYPE_UNKOWN){
    type_error(node,"Aritmetic operator cannot be applied to operands of type '%s' and '%s'",
      get_type_name(left_type),
      get_type_name(right_type)
    );
  }
  
  return node->inferedType = return_type;
}

NodeType inspect_comparable_operator_type(Node * node){
        // check that both sides are numbers
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);
  if (!is_type_numeric(left_type)|| !is_type_numeric(right_type ))
    {
      type_error(node,"The values of type '%s' and '%s' are not comparable",
        get_type_name(left_type),
        get_type_name(right_type)
      );
    }

  return node->inferedType = TYPE_BOOL;
}

NodeType inspect_logical_operator_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->left);
  
  if (left_type != TYPE_BOOL || right_type != TYPE_BOOL){
    type_error(node,"Logical operators cannot be applied to operands of type '%s' and '%s'",
      get_type_name(left_type),
      get_type_name(right_type)
    );
  }
  
  return node->inferedType = TYPE_BOOL;
}

NodeType inspect_unary_operator_type(Node * node){
  NodeType type = get_node_type(node->left);

  if(type != TYPE_BOOL){
    type_error(node,"The operator '%c' cannot be used with the type %s",
      '!',
      get_type_name(type)
      );
  }
  return node->inferedType = TYPE_BOOL;
}

NodeType inspect_print_function_type(Node* node){
  NodeType left_type = get_node_type(node->left);
  if( ! can_type_be_stringified(left_type) ) {
    type_error(node,"The type %s cannot be printed",
      get_type_name(left_type)
    );
  }
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_read_function_type(Node* node){
  NodeType left_type = get_node_type(node->left);
  if( left_type != TYPE_STRING ) {
    type_error(node,"A %s cannot be assigned to a variable of type '%s'",
      get_type_name(TYPE_STRING),
      get_type_name(left_type)
    );
  }
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_transcode_function_type(Node* node){
  NodeType left_type = get_node_type(node->left);
  
  if (left_type != TYPE_VIDEO){
    type_error(node,"It is not possible to get the streams of a variable of type %s",
      get_type_name(left_type)
    );
  }
  get_node_type(node->right);
  
  return node->inferedType = (node->id == NODE_FUNCTION_TRANSCODE_VIDEO  ? TYPE_VIDEO_STREAM :TYPE_AUDIO_STREAM);
}

NodeType inspect_append_stream_function_type(Node *node){
  NodeType left_type = get_node_type(node->left);

  if(left_type != TYPE_AUDIO_STREAM && left_type != TYPE_VIDEO_STREAM){
    type_error(node,"The operator << does not accept values of the type %s",
      get_type_name(left_type)
    );
  }
  
  get_node_type(node->right);
   
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_save_video_function_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  
  if(left_type != TYPE_STRING){
   type_error(node,"The expression expected is of type %s, not %s",
      get_type_name(TYPE_STRING),
      get_type_name(left_type)
    );
  }
  
  get_node_type(node->right);
  
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_open_video_function_type(Node * node){
  NodeType left_type = get_node_type(node->left);

  if(left_type != TYPE_STRING){
    type_error(node,"It is not possible to open a %s into a %s",
      get_type_name(left_type),
      get_type_name(TYPE_VIDEO)
    );
  }
  get_node_type(node->right);
  return node->inferedType = TYPE_VIDEO;
}

NodeType inspect_parse_string_function_type(Node * node){

  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);

  if (right_type != TYPE_STRING){
    type_error(node, "It is not possible to parse data from %s",
      get_type_name(right_type)
    );
  }

  return node->inferedType = left_type;
}

NodeType inspect_assignment_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);

  check_type_error_new_assignment(left_type,right_type, node);
  
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_declaration_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  node_assignment(node->right,left_type);    
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_declaration_and_assignment_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  node_assignment(node->right->left,left_type);
  get_node_type(node->right);
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_assignment_video_type(Node * node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);

  check_type_variable_open_video(left_type, node);
  check_type_error_new_assignment(TYPE_VIDEO,right_type, node);
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_assignment_transcode_type(Node* node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);
  check_type_error_new_assignment(left_type,right_type, node);

  if (left_type != TYPE_AUDIO_STREAM && left_type != TYPE_VIDEO_STREAM)
  {
    type_error(node,"The video can only be decomposed into stream types, not %s",
      get_type_name(left_type)
    );
  }

  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_variable_type(Node* node){
  if (!node_is_variable_type(node)) {
    return TYPE_UNKOWN;
  }
  
  int position_fix = NODE_VARIABLE_TYPE_INT;
  return variable_type_lookup_table[node->id - position_fix];
}

NodeType inspect_parameter_type(Node* node){
  NodeType left_type = get_node_type(node->left);
  NodeType right_type = get_node_type(node->right);

    if(left_type != right_type){
      type_error(node,"The parameter '%s' does not accept values of the type %s",
        get_parameter_name(node->left),
        get_type_name(right_type)
      );
  }
    
  return node->inferedType = TYPE_UNKOWN;
}

NodeType inspect_keyword_type(Node * node){
  if(!node_is_stream_keyword(node)){
    // tood error
    return TYPE_UNKOWN;
  }
  int position_fix = NODE_STREAM_CODEC;
  return node->inferedType = stream_parameter_type_lookup_table[node->id - position_fix];
}

/*******************************************************************
*                       FUNCTION HELPERS                           * 
*******************************************************************/


void check_type_variable_open_video(NodeType type,Node * node){
  if (type != TYPE_VIDEO){
    type_error(node,"It is not possible to open a %s into the variable '%s' of type %s",
      get_type_name(TYPE_VIDEO),
      node->left->value,
      get_type_name(type)
    );
  }
}


void node_assignment(Node * node, NodeType type){
  node->inferedType = type;
  set_variable_type(node->value, type);
}


void check_type_error_new_assignment(NodeType left_node_type, NodeType right_node_type, Node * node ){
  
  if (left_node_type != right_node_type)
  {
    type_error(node,"The variable named '%s' is declared as type %s but then it is assigned type %s",
    node->left->value,
    get_type_name(left_node_type),
    get_type_name(right_node_type)
    );
  }
}


// It returns true if the type is Int or Float
bool is_type_numeric(NodeType type){
  return type == TYPE_INT || type == TYPE_FLOAT;
}

// It returns true if the type can be converted to string, like bool or Int
bool can_type_be_stringified(NodeType type){
  return type == TYPE_INT || type == TYPE_FLOAT || type == TYPE_STRING  || type == TYPE_BOOL;
}


