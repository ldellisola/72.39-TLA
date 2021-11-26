#include "../include/translator.h"
#include "../include/node.h"
#include "../include/errors.h"
#include "../include/types.h"
#include <stdio.h>


static FILE * output;

void translate(Node * root);

/*******************************************************************
*                       PUBLIC FUNCTIONS                           * 
*******************************************************************/

void translate_to_console(Node * root){
  output = stdout;
  translate(root);
}

void translate_to_file(Node * root, const char * filePath){
  output = fopen(filePath,"w+");

  if( output == NULL)
    unknown_error("Cannot open file %s",filePath);
  
  translate(root);

  fclose(output);
}


/*******************************************************************
*                   TRANSLATION DEFINITION                         * 
*******************************************************************/

void translate_value(Node * node);
void translate_variable_name(Node * node);
void translate_value_float(Node * node);
void translate_if(Node * node);
void translate_while(Node * node);
void translate_else(Node * node);
void translate_binary_operator(Node * node);
void translate_unary_operator(Node * node);
void translate_function_print(Node * node);
void translate_function_read(Node * node);
void translate_function_transcode_video(Node * node);
void translate_function_transcode_audio(Node * node);
void translate_function_append_stream(Node * node);
void translate_function_save_video(Node * node);
void translate_function_save_overwrite_video(Node * node);
void translate_function_parse_string(Node * node);
void translate_function_open_video(Node * node);
void translate_assignment(Node * node);
void translate_declaration(Node * node);
void translate_declaration_assignment(Node * node);
void translate_assignment_open_video(Node * node);
void translate_type(Node * node);
void translate_parameters_list(Node * node);
void translate_parameter(Node * node);
void translate_stream_keyword(Node * node);
void translate_next_line(Node * node);
void translate_error(Node * node);
void translate_c_sharp_code(Node * node);


void (* translations[])(Node *) = {
  translate_value,                        // NODE_VALUE_STRING
  translate_value,                        // NODE_VALUE_INT
  translate_value_float,                  // NODE_VALUE_FLOAT
  translate_value,                        // NODE_VALUE_BOOL
  translate_if,                           // NODE_STATEMENT_IF
  translate_while,                         // NODE_STATEMENT_WHILE
  translate_else,                         // NODE_STATEMENT_ELSE,
  translate_variable_name,                        // NODE_VARIABLE_NAME
  translate_binary_operator,              // NODE_OPERATOR_EQUALS
  translate_binary_operator,              // NODE_OPERATOR_NOT_EQUALS
  translate_binary_operator,              // NODE_OPERATOR_ADDITION
  translate_binary_operator,              // NODE_OPERATOR_SUBSTRACTION
  translate_binary_operator,              // NODE_OPERATOR_DIVISION
  translate_binary_operator,              // NODE_OPERATOR_MULTIPLICATION
  translate_binary_operator,              // NODE_OPERATOR_GREATER_THAN
  translate_binary_operator,              // NODE_OPERATOR_LESS_THAN
  translate_binary_operator,              // NODE_OPERATOR_LOGIC_OR
  translate_binary_operator,              // NODE_OPERATOR_LOGIC_AND
  translate_unary_operator,               // NODE_OPERATOR_NOT
  translate_function_print,               // NODE_FUNCTION_PRINT
  translate_function_read,                // NODE_FUNCTION_READ
  translate_function_transcode_video,     // NODE_FUNCTION_TRANSCODE_VIDEO
  translate_function_transcode_audio,     // NODE_FUNCTION_TRANSCODE_AUDIO
  translate_function_append_stream,       // NODE_FUNCTION_APPEND_STREAM
  translate_function_save_video,          // NODE_FUNCTION_SAVE_VIDEO
  translate_function_open_video,          // NODE_FUNCTION_OPEN_VIDEO
  translate_function_save_overwrite_video,// NODE_FUNCTION_SAVE_OVERWRITE_VIDEO
  translate_function_parse_string,        // NODE_FUNCTION_PARSE_STRING
  translate_assignment,                   // NODE_ASSIGNMENT
  translate_declaration,                  // NODE_DECLARATION
  translate_declaration_assignment,       // NODE_DECLARATION_ASSIGNMENT
  translate_assignment_open_video,        // NODE_ASSIGNMENT_OPEN_VIDEO
  translate_assignment,                   // NODE_ASSIGNMENT_TRANSCODE
  translate_type,                         // NODE_VARIABLE_TYPE_INT
  translate_type,                         // NODE_VARIABLE_TYPE_FLOAT
  translate_type,                         // NODE_VARIABLE_TYPE_STRING
  translate_type,                         // NODE_VARIABLE_TYPE_BOOL
  translate_type,                         // NODE_VARIABLE_TYPE_VIDEO
  translate_type,                         // NODE_VARIABLE_TYPE_AUDIO_STREAM
  translate_type,                         // NODE_VARIABLE_TYPE_VIDEO_STREAM
  translate_parameters_list,              // NODE_TRANSCODE_PARAMETER_LIST
  translate_parameter,                    // NODE_TRANSCODE_PARAMETER, 
  translate_stream_keyword,               // NODE_STREAM_CODEC
  translate_stream_keyword,               // NODE_STREAM_SIZE
  translate_stream_keyword,               // NODE_STREAM_SPEED
  translate_stream_keyword,               // NODE_STREAM_FRAMERATE
  translate_stream_keyword,               // NODE_STREAM_BITRATE
  translate_stream_keyword,               // NODE_STREAM_CHANNELS
  translate_next_line,                    // NODE_NEXT_LINE
  translate_c_sharp_code,                 // NODE_C_SHARP_CODE
  translate_error                         // NODE_UNKNOWN
};

/*******************************************************************
*                     RECURSIVE TRANSLATION                        * 
*******************************************************************/

void translate(Node *root) {
  if (root == NULL)
    return;
  translations[root->id](root);
}

/*******************************************************************
*                   TRANSLATION FUNCTIONS                          * 
*******************************************************************/

const char * get_binary_operator(Node * node);
const char * get_unary_operator(Node * node);
const char * get_type_definition(Node * node);
const char * get_parameter(Node * node);

void translate_value(Node * node){
  fprintf(output," %s ",node->value);
}


void translate_variable_name(Node * node){
  fprintf(output," _%s ",node->value);
}

void translate_value_float(Node * node){
  fprintf(output," %sf ",node->value);
}

void translate_if(Node * node){
  fprintf(output,"if ");
  translate(node->left);
  fprintf(output, "{\n");
  translate(node->right);
  fprintf(output, "}\n");
}

void translate_while(Node * node){
  fprintf(output, "while (");
  translate(node->left);
  fprintf(output, " ) {\n");
  translate(node->right);
  fprintf(output, "\n}\n");
}

void translate_else(Node * node){
  translate(node->left);
  fprintf(output,"\n } else { \n");
  translate(node->right);
}

void translate_binary_operator(Node * node){
    fprintf(output," ( ");
    translate(node->left);
    fprintf(output," %s ",get_binary_operator(node));
    translate(node->right);
    fprintf(output," ) ");
}

void translate_unary_operator(Node * node){
    fprintf(output,"(%s ",get_unary_operator(node));
    translate(node->left);
    fprintf(output," ) ");
}

void translate_function_print(Node * node){
  fprintf(output,"Console.WriteLine($\"{");
  translate(node->left);
  fprintf(output,"}\")");
}

void translate_function_read(Node * node){
  translate(node->left);
  fprintf(output," = Console.ReadLine();");
}

void translate_function_transcode_video(Node * node){
  translate(node->left);
  fprintf(output,".VideoStreams.FirstOrDefault()");
  translate(node->right);
}

void translate_function_transcode_audio(Node * node){
  translate(node->left);
  fprintf(output,".AudioStreams.FirstOrDefault()");
  translate(node->right);
}

void translate_function_append_stream(Node * node){
  fprintf(output,".AddStream(");
  translate(node->left);
  fprintf(output,")\n");
  translate(node->right);
}

void translate_function_save_video(Node * node){
  fprintf(output, "FFmpeg.Conversions.New().SetOutput(");
  translate(node->left);
  fprintf(output,")");
  translate(node->right);
  fprintf(output,".Start().Wait()");
}

void translate_function_save_overwrite_video(Node * node){
  fprintf(output, "FFmpeg.Conversions.New().SetOverwriteOutput(true).SetOutput(");
  translate(node->left);
  fprintf(output,")");
  translate(node->right);
  fprintf(output,".Start().Wait()");
}

void translate_function_parse_string(Node * node){
  if(!node_is_parseable(node->left)){
    translation_error(node,"The type %s does not support parsing from a %s",
      get_type_name(node->left->inferedType),
      get_type_name(TYPE_STRING)
    );
  } else {
    translate(node->left);
    fprintf(output,".Parse(");
    translate(node->right);
    fprintf(output,")");
  }
}

void translate_function_open_video(Node * node){
  fprintf(output,"Helpers.OpenVideo(");
  translate(node->left);
  fprintf(output,")");
}


void translate_assignment(Node * node){
  translate(node->left);
  fprintf(output," = ");
  translate(node->right);
}

void translate_declaration(Node * node){
  translate(node->left);
  fprintf(output, " ");
  translate(node->right);
}

void translate_declaration_assignment(Node * node){
  translate(node->left);
  fprintf(output, " ");
  translate(node->right);
}

void translate_assignment_open_video(Node * node){
  translate(node->left);
  fprintf(output," = ");
  translate(node->right);
  
}

void translate_type(Node * node){
  fprintf(output, "%s", get_type_definition(node));
}

void translate_parameters_list(Node * node){
  translate(node->left);
  translate(node->right);
}

void translate_parameter(Node * node){
  fprintf(output,"\n?");
  translate(node->left);
  fprintf(output,"(");
  translate(node->right);
  fprintf(output,")");
}

void translate_stream_keyword(Node * node){
  fprintf(output," %s ", get_parameter(node));
}

void translate_next_line(Node * node){
  translate(node->left);
  fprintf(output,";\n\n");
  translate(node->right);
}

void translate_error(Node * node){

  translation_error(node,"Unknown error");
}

void translate_c_sharp_code(Node * node){
  fprintf(output,"%s",node->value);
}

/*******************************************************************
*                       LOOKUP TABLES                              * 
*******************************************************************/
        
const char * parameter_lookup_table[] = {
      ".SetCodec",           // NODE_STREAM_CODEC
      ".SetSize",            // NODE_STREAM_SIZE
      ".ChangeSpeed",        // NODE_STREAM_SPEED
      ".SetFramerate",       // NODE_STREAM_FRAMERATE
      ".SetBitrate",         // NODE_STREAM_BITRATE
      ".SetChannels",        // NODE_STREAM_CHANNELS
};

const char * get_parameter(Node * node){
  
  if( node_is_stream_keyword(node)){
    int position_fix = NODE_STREAM_CODEC;
    return parameter_lookup_table[node->id - position_fix];
  }
  translation_error(node,"Unkown error");
  return ";";
}

const char * binary_operator_lookup_table[] = {
      "==",          //  NODE_OPERATOR_EQUALS
      "!=",          //  NODE_OPERATOR_NOT_EQUALS
      "+",           //  NODE_OPERATOR_ADDITION
      "-",           //  NODE_OPERATOR_SUBSTRACTION
      "/",           //  NODE_OPERATOR_DIVISION
      "*",           //  NODE_OPERATOR_MULTIPLICATION
      ">",           //  NODE_OPERATOR_GREATER_THAN
      "<",           //  NODE_OPERATOR_LESS_THAN
      "||",          //  NODE_OPERATOR_LOGIC_OR
      "&&"           //  NODE_OPERATOR_LOGIC_AND
};

const char * get_binary_operator(Node * node){
  
  if( node_is_binary_operator(node)){
    int position_fix = NODE_OPERATOR_EQUALS;
    return binary_operator_lookup_table[node->id - position_fix];
  } 

  translation_error(node,"Unknown error");
  return ";";  
}

const char * type_definition_lookup_table[] = {
      "int",              // NODE_VARIABLE_TYPE_INT
      "float",            // NODE_VARIABLE_TYPE_FLOAT
      "string",           // NODE_VARIABLE_TYPE_STRING
      "bool",             // NODE_VARIABLE_TYPE_BOOL
      "IMediaInfo",       // NODE_VARIABLE_TYPE_VIDEO
      "IAudioStream",     // NODE_VARIABLE_TYPE_AUDIO_STREAM
      "IVideoStream",     // NODE_VARIABLE_TYPE_VIDEO_STREAM
};

const char * get_type_definition(Node * node){
  
  if(node_is_variable_type(node)){
    int position_fix = NODE_VARIABLE_TYPE_INT;
    return type_definition_lookup_table[node->id - position_fix];
  }
  translation_error(node,"Unknown error");
  return ";";
}

const char * unary_operator_lookup_table[] = {
      "!",                // NODE_OPERATOR_NOT
};

const char * get_unary_operator(Node * node){
  
  if(node_is_unary_operator(node)){
    int position_fix = NODE_OPERATOR_NOT;
    return unary_operator_lookup_table[node->id - position_fix];
  }
  
  translation_error(node,"Unknown error");
  return ";";
}


