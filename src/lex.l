%option yylineno

%{
    #include "../include/node.h"
	#include "../target/y.tab.h"
    
    void yyerror(char *s);
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

%}

%%

"if"      return STATEMENT_IF;
"then"    return STATEMENT_THEN;
"else"      return STATEMENT_ELSE;
"end"     return STATEMENT_END;
"while"  return STATEMENT_WHILE;
"do"      return STATEMENT_DO;

":!"        return FUNCTION_PRINT;
":?"        return FUNCTION_READ;

"=="      return OPERATOR_EQUALS;
"!="      return OPERATOR_NOT_EQUALS;
">"       return OPERATOR_GREATER_THAN;
"<"       return OPERATOR_LESS_THAN;
"||"      return OPERATOR_LOGIC_OR;
"&&"      return OPERATOR_LOGIC_AND;
"!"       return OPERATOR_NOT;

"{"       return OPERATOR_OPEN_VIDEO;
"<-"      return OPERATOR_GET_VIDEO_STREAM;
"<~"      return OPERATOR_GET_AUDIO_STREAM;
"<<"      return OPERATOR_APPEND_STREAM;

"channels"   return KEYWORD_STREAM_CHANNELS;

"codec"   return KEYWORD_STREAM_CODEC;
"size"   return KEYWORD_STREAM_SIZE;
"speed"   return KEYWORD_STREAM_SPEED;
"bitrate"   return KEYWORD_STREAM_BITRATE;

"framerate"   return KEYWORD_STREAM_FRAMERATE;

"+"       return OPERATOR_ADDITION;
"-"       return OPERATOR_SUBSTRACTION;
"/"       return OPERATOR_DIVISION;
"*"       return OPERATOR_MULTIPLICATION;

"Video"       return VARIABLE_VIDEO;
"VideoStream" return VARIABLE_VIDEO_STREAM;
"AudioStream" return VARIABLE_AUDIO_STREAM; 
"String"     return VARIABLE_STRING;
"Int"     return VARIABLE_INT;
"Float"     return VARIABLE_FLOAT;
"Bool"       return VARIABLE_BOOL;

[();=:,|\[\]]      return yytext[0];

[0-9]+          {strcpy(yylval.text,yytext); return VALUE_INT;}
"true"|"false"  {strcpy(yylval.text,yytext); return VALUE_BOOL;}
[0-9]+\.[0-9]+  {strcpy(yylval.text,yytext); return VALUE_FLOAT;}
\".*\"    {strcpy(yylval.text,yytext); return VALUE_STRING;}

[A-Za-z_\-][A-Za-z0-9_\-]+        {strcpy(yylval.text,yytext); return VARIABLE_NAME;}

[\n \t]   ; 

^^(.|\n)*^^      {
                    int length = strlen(yytext)-2;
                    yylval.unrestricted_text = calloc(length,sizeof(char));
                    yytext[length-2] = 0;
                    strcpy(yylval.unrestricted_text,2+yytext);
                    return C_SHARP_CODE;
                }



\/\/.*\n        ;

. yyerror("Unkown Character");
%%