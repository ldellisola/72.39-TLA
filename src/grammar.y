
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>

  #include "../include/node.h"
  #include "../include/types.h"
  #include "../include/print.h"
  #include "../include/variables.h"
  #include "../include/translator.h"
  #include "../include/errors.h"

  void yyerror(const char *s);
	int yylex(void);

  Node * root;  

%}

%union {
  char text[1024];
  char * unrestricted_text;
  Node * nodePtr;
}

%locations
%define parse.error detailed


/* ****************************************************** */ 
/*                       TOKENS                           */
/* ****************************************************** */ 

%token STATEMENT_IF STATEMENT_THEN STATEMENT_END  STATEMENT_WHILE STATEMENT_DO STATEMENT_ELSE
%token OPERATOR_EQUALS OPERATOR_NOT_EQUALS OPERATOR_GREATER_THAN OPERATOR_LESS_THAN OPERATOR_LOGIC_AND OPERATOR_LOGIC_OR OPERATOR_NOT OPERATOR_OPEN_VIDEO OPERATOR_GET_VIDEO_STREAM OPERATOR_APPEND_STREAM OPERATOR_GET_AUDIO_STREAM
%token OPERATOR_ADDITION OPERATOR_SUBSTRACTION OPERATOR_DIVISION OPERATOR_MULTIPLICATION
%token VARIABLE_FLOAT VARIABLE_INT VARIABLE_STRING VARIABLE_BOOL VARIABLE_VIDEO VARIABLE_AUDIO_STREAM VARIABLE_VIDEO_STREAM
%token FUNCTION_PRINT FUNCTION_READ
%token KEYWORD_STREAM_CODEC KEYWORD_STREAM_SIZE KEYWORD_STREAM_SPEED KEYWORD_STREAM_ROTATE KEYWORD_STREAM_FRAMERATE KEYWORD_STREAM_BITRATE KEYWORD_STREAM_CHANNELS

// according to https://en.wikipedia.org/wiki/Order_of_operations
%left ','
%left '=' OPERATOR_APPEND_STREAM OPERATOR_GET_VIDEO_STREAM OPERATOR_GET_AUDIO_STREAM
%left OPERATOR_LOGIC_OR
%left  OPERATOR_LOGIC_AND 

%left OPERATOR_EQUALS OPERATOR_NOT_EQUALS

%left OPERATOR_GREATER_THAN OPERATOR_LESS_THAN

%left OPERATOR_ADDITION OPERATOR_SUBSTRACTION

%left OPERATOR_MULTIPLICATION OPERATOR_DIVISION

%left OPERATOR_NOT OPERATOR_OPEN_VIDEO  

%nonassoc STATEMENT_THEN STATEMENT_ELSE ':' 

%token <text> VARIABLE_NAME VALUE_STRING VALUE_FLOAT VALUE_INT VALUE_BOOL
%token <unrestricted_text> C_SHARP_CODE

/* ****************************************************** */ 
/*                        TYPES                           */
/* ****************************************************** */ 

%type <nodePtr> program lines line 

/* Control flow */
%type <nodePtr> statement statement_if statement_while statement_if_else

/* assingment */
%type <nodePtr> assignment declaration

/* functions */
%type <nodePtr> functions function_print function_read function_append_stream function_save_video function_transcode_video  functions_return functios_no_return function_transcode_audio function_open_video function_parse_string

/* operations */
%type <nodePtr> operation operation_aritmetic  operation_boolean  

/* expressions */
%type <nodePtr> expression_boolean expression_boolean_parentheses expression_aritmetic expression_aritmetic_prarentheses

/* values */
%type <nodePtr> value_string value_float value_int value_bool

/* variables */
%type <nodePtr> variable_name variable_type

/* transcode */
%type <nodePtr> video_transcode_parameter_type video_transcode_parameters video_transcode_parameter_list audio_transcode_parameter_list audio_transcode_parameters audio_transcode_parameter_type

%type <nodePtr> c_sharp_code

  


/* ****************************************************** */ 
/*                      GRAMMAR                           */
/* ****************************************************** */ 

%start program

%%
program : STATEMENT_DO ':' lines 
        { 
          root = $3;
        }
        ;
lines :  line lines 
      { 
        $$ = node(NODE_NEXT_LINE,$1,$2,@1.first_line);
      }
      | 
      {
        $$ = NULL;
      }
      ;
  
line : statement  
      | assignment ';' 
      | functions ';'
      | declaration ';'
      | c_sharp_code 
      ;

c_sharp_code  : C_SHARP_CODE 
              {
                $$ = leaf(NODE_C_SHARP_CODE,$1,@1.first_line); 
                free($1);
              }
              ;


/* ****************************************************** */ 
/*                 DECLARATIONS                           */
/* ****************************************************** */ 

declaration : variable_type variable_name
            {
              $$ = node(NODE_DECLARATION, $1, $2,@1.first_line);
            }
            | variable_type assignment
            {
              $$ = node(NODE_DECLARATION_ASSIGNMENT, $1, $2,@1.first_line);
            }
            ;


/* ****************************************************** */ 
/*                    FUNCTIONS                           */
/* ****************************************************** */ 

functions : functios_no_return | functions_return;

functios_no_return  : function_print
                    | function_read
                    | function_save_video
                    ;

functions_return  : function_transcode_video 
                  | function_transcode_audio
                  | function_open_video
                  | function_parse_string
                  ;

function_print : FUNCTION_PRINT operation 
                {
                  $$  = node(NODE_FUNCTION_PRINT,$2,NULL,@1.first_line);
                }
                ;

function_read : FUNCTION_READ variable_name
              {
                $$ = node(NODE_FUNCTION_READ, $2, NULL, @1.first_line);
              }
              ;

function_parse_string : variable_type '[' operation ']'
                      {
                        $$ = node(NODE_FUNCTION_PARSE_STRING,$1,$3,@2.first_line);
                      }

function_save_video : '|' operation '|' function_append_stream
                    {
                       $$ = node(NODE_FUNCTION_SAVE_VIDEO, $2,$4,@1.first_line);
                    } 
                    | OPERATOR_NOT '|' operation '|' function_append_stream
                    {
                       $$ = node(NODE_FUNCTION_SAVE_OVERWRITE_VIDEO, $3,$5,@1.first_line);
                    } 
                    ;

function_append_stream  : OPERATOR_APPEND_STREAM operation
                        {
                          $$ = node(NODE_FUNCTION_APPEND_STREAM,$2,NULL,@1.first_line);
                        }
                        | OPERATOR_APPEND_STREAM operation function_append_stream
                        {
                          $$ = node(NODE_FUNCTION_APPEND_STREAM,$2,$3,@1.first_line);
                        }
                        ;

function_transcode_video : OPERATOR_GET_VIDEO_STREAM video_transcode_parameter_list OPERATOR_GET_VIDEO_STREAM operation
            {
              $$ = node(NODE_FUNCTION_TRANSCODE_VIDEO,$4,$2,@4.first_line);
            }
            | OPERATOR_GET_VIDEO_STREAM operation
            {
              $$ = node(NODE_FUNCTION_TRANSCODE_VIDEO,$2,NULL,@1.first_line);
            }
            ;

function_transcode_audio : OPERATOR_GET_AUDIO_STREAM audio_transcode_parameter_list OPERATOR_GET_AUDIO_STREAM operation
            {
              $$ = node(NODE_FUNCTION_TRANSCODE_AUDIO,$4,$2,@4.first_line);
            }
            | OPERATOR_GET_AUDIO_STREAM operation
            {
              $$ = node(NODE_FUNCTION_TRANSCODE_AUDIO,$2,NULL,@1.first_line);
            }
            ;

function_open_video : OPERATOR_OPEN_VIDEO operation
                    {
                      $$ = node(NODE_FUNCTION_OPEN_VIDEO, $2,NULL,@1.first_line);
                    }


/* ****************************************************** */ 
/*              TRANSCODE SPECIFIC RULES                  */
/* ****************************************************** */

audio_transcode_parameter_list  : audio_transcode_parameters ',' audio_transcode_parameter_list
                                {
                                  $$ = node(NODE_TRANSCODE_PARAMETER_LIST, $1, $3,@2.first_line);
                                }
                                | audio_transcode_parameters
                                ;
audio_transcode_parameters  : audio_transcode_parameter_type ':' operation
                            {
                              $$ = node(NODE_TRANSCODE_PARAMETER, $1,$3,@2.first_line);
                            }
                            ; 

audio_transcode_parameter_type  : KEYWORD_STREAM_CODEC
                                {
                                  $$ = leaf(NODE_STREAM_CODEC,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_CHANNELS
                                {
                                  $$ = leaf(NODE_STREAM_CHANNELS,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_SPEED
                                {
                                  $$ = leaf(NODE_STREAM_SPEED,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_BITRATE
                                {
                                  $$ = leaf(NODE_STREAM_BITRATE,NULL,@1.first_line);
                                }
                                ;


video_transcode_parameter_list  : video_transcode_parameters ',' video_transcode_parameter_list
                                {
                                  $$ = node(NODE_TRANSCODE_PARAMETER_LIST, $1, $3,@2.first_line);
                                }
                                | video_transcode_parameters
                                ;
video_transcode_parameters  : video_transcode_parameter_type ':' operation
                            {
                              $$ = node(NODE_TRANSCODE_PARAMETER, $1,$3,@2.first_line);
                            }
                            ; 

video_transcode_parameter_type  : KEYWORD_STREAM_CODEC
                                {
                                  $$ = leaf(NODE_STREAM_CODEC,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_SIZE
                                {
                                  $$ = leaf(NODE_STREAM_SIZE,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_SPEED
                                {
                                  $$ = leaf(NODE_STREAM_SPEED,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_FRAMERATE
                                {
                                  $$ = leaf(NODE_STREAM_FRAMERATE,NULL,@1.first_line);
                                }
                                | KEYWORD_STREAM_BITRATE
                                {
                                  $$ = leaf(NODE_STREAM_BITRATE,NULL,@1.first_line);
                                }
                                ;

/* ****************************************************** */ 
/*                    STATEMENT                           */
/* ****************************************************** */ 

statement : statement_if
          | statement_while
          | statement_if_else
          ;

statement_if : STATEMENT_IF expression_boolean_parentheses STATEMENT_THEN lines STATEMENT_END
          {
            $$ = node(NODE_STATEMENT_IF, $2,$4,@1.first_line);
          }
          ;

statement_if_else : STATEMENT_IF expression_boolean_parentheses STATEMENT_THEN lines STATEMENT_ELSE lines STATEMENT_END
          {
            Node * ptr = node(NODE_STATEMENT_ELSE, $4,$6,@4.first_line);
            $$ = node(NODE_STATEMENT_IF, $2,ptr,@1.first_line);
          }
          ;

statement_while : STATEMENT_WHILE expression_boolean_parentheses STATEMENT_DO lines STATEMENT_END
          {
            $$ = node(NODE_STATEMENT_WHILE, $2, $4,@1.first_line);
          }
          ;



/* ****************************************************** */ 
/*                    ASSINGMENT                          */
/* ****************************************************** */ 


assignment : variable_name '=' operation 
            {
              $$ = node(NODE_ASSIGNMENT, $1, $3,@2.first_line);
            }
            | variable_name function_open_video
            {
              $$ = node(NODE_ASSIGNMENT_OPEN_VIDEO, $1,$2,@1.first_line);
            }
            | variable_name function_transcode_video
            {
              $$ = node(NODE_ASSIGNMENT_TRANSCODE,$1, $2, @1.first_line);
            }
            | variable_name function_transcode_audio
            {
              $$ = node(NODE_ASSIGNMENT_TRANSCODE,$1, $2, @1.first_line);
            }
            ;


/* ****************************************************** */ 
/*                    EXPRESSIONs                         */
/* ****************************************************** */ 

expression_boolean : expression_boolean_parentheses
                   | operation_boolean
                   | value_bool 
                   ;

expression_boolean_parentheses : '(' expression_boolean ')'
                               {
                                 $$ = $2;
                               }
                               ;

expression_aritmetic : expression_aritmetic_prarentheses 
                      | operation_aritmetic 
                      | value_float 
                      | value_int 
                      | value_string
                      ;

expression_aritmetic_prarentheses : '(' expression_aritmetic ')'
                                  {
                                    $$ = $2;
                                  }
                                  ;
                                  

/* ****************************************************** */ 
/*                    OPERATIONS                          */
/* ****************************************************** */ 

operation : expression_aritmetic
          | expression_boolean 
          | variable_name
          | functions_return
          ;

operation_aritmetic : 
                  operation OPERATOR_ADDITION operation 
                  {
                    $$ = node(NODE_OPERATOR_ADDITION,$1,$3,@2.first_line); 
                  }
                  | operation OPERATOR_SUBSTRACTION operation 
                  {
                    $$ = node(NODE_OPERATOR_SUBSTRACTION,$1,$3,@2.first_line); 
                  }
                  | operation OPERATOR_DIVISION operation 
                  {
                    $$ = node(NODE_OPERATOR_DIVISION,$1,$3,@2.first_line); 
                  }
                  | operation OPERATOR_MULTIPLICATION operation 
                  {
                    $$ = node(NODE_OPERATOR_MULTIPLICATION,$1,$3,@2.first_line); 
                  }
                  ;


operation_boolean : operation  OPERATOR_EQUALS operation 
                  {
                    $$ = node(NODE_OPERATOR_EQUALS, $1, $3,@2.first_line);
                  }
                  |  operation  OPERATOR_NOT_EQUALS operation 
                  {
                    $$ = node(NODE_OPERATOR_NOT_EQUALS, $1, $3 ,@2.first_line);
                  }
                  | operation  OPERATOR_GREATER_THAN operation
                  {
                    $$ = node(NODE_OPERATOR_GREATER_THAN, $1, $3 ,@2.first_line);
                  }
                  | operation  OPERATOR_LESS_THAN operation
                  {
                    $$ = node(NODE_OPERATOR_LESS_THAN, $1, $3 ,@2.first_line);
                  }
                  | operation OPERATOR_LOGIC_AND operation
                  {
                    $$ = node(NODE_OPERATOR_LOGIC_AND, $1,$3,@2.first_line);
                  }
                  | operation OPERATOR_LOGIC_OR operation
                  {
                    $$ = node(NODE_OPERATOR_LOGIC_OR, $1,$3,@2.first_line);
                  }
                  | OPERATOR_NOT operation
                  {
                    $$ = node(NODE_OPERATOR_NOT, $2,NULL,@1.first_line);
                  }
                  ;


/* ****************************************************** */ 
/*                        VALUES                          */
/* ****************************************************** */ 



value_string : VALUE_STRING 
              { 
                $$ = leaf(NODE_VALUE_STRING,$1,@1.first_line);
              }
              ;

value_float : VALUE_FLOAT 
            { 
              $$ = leaf(NODE_VALUE_FLOAT,$1,@1.first_line);
            }
            ;
value_int : VALUE_INT 
          { 
            $$ = leaf(NODE_VALUE_INT,$1,@1.first_line);
          }
          ;

value_bool : VALUE_BOOL
           {
             $$ = leaf(NODE_VALUE_BOOL,$1,@1.first_line);
           }
           ;

/* ****************************************************** */ 
/*                     VARIABLES                          */
/* ****************************************************** */ 

variable_name : VARIABLE_NAME 
              { 
                $$ = leaf(NODE_VARIABLE_NAME,$1,@1.first_line); 
              }
              ;
              
variable_type : VARIABLE_BOOL
              {
                $$ = leaf(NODE_VARIABLE_TYPE_BOOL,NULL, @1.first_line);
              }
              | VARIABLE_INT
              {
                $$ = leaf(NODE_VARIABLE_TYPE_INT,NULL, @1.first_line);
              }
              | VARIABLE_FLOAT
              {
                $$ = leaf(NODE_VARIABLE_TYPE_FLOAT,NULL, @1.first_line);
              }
              | VARIABLE_STRING
              {
                $$ = leaf(NODE_VARIABLE_TYPE_STRING,NULL, @1.first_line);
              }
              | VARIABLE_VIDEO
              {
                $$ = leaf(NODE_VARIABLE_TYPE_VIDEO,NULL, @1.first_line);
              }
              | VARIABLE_AUDIO_STREAM 
              {
                $$ = leaf(NODE_VARIABLE_TYPE_AUDIO_STREAM,NULL, @1.first_line);
              }
              | VARIABLE_VIDEO_STREAM
              {
                $$ = leaf(NODE_VARIABLE_TYPE_VIDEO_STREAM,NULL, @1.first_line);
              }
              ;


%%



int yywrap()
{
  return 1;
}

int main(int argc, char** argv) {

  yyparse();
  validate_variables(root);
  infer_types(root);

  if(has_compilation_errors()){
    return -1;
  }

  char * path = "./Program.cs";

  if(argc==2){
    path = argv[1];
  }

  translate_to_file(root,path);

  free_node(root);
  dispose_variables();
}

void yyerror(const char *msg)
{
	extern int yylineno;
	parse_error(yylineno,"%s",msg);
	free_node(root);
}