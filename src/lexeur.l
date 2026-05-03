%{
#include <stdio.h>
#include "ast.h"
#include "grammar.h"
#include <string.h>
#include <stdlib.h>
int yyerror(char *msg);
int column = 1;
int lines = 1 ;
%}

%option noyywrap
%x COMMENT
%x COMMENT_LINE
%%


"/*" {
    BEGIN(COMMENT);
}
<COMMENT>{
    "*/" BEGIN(INITIAL);
    .   {column++;}
    \n  {lines++; column = 1;}
    <<EOF>> { BEGIN(INITIAL);}
}

"//" {BEGIN(COMMENT_LINE);}
<COMMENT_LINE>{
    .  {column++;}
    "\n" {BEGIN(INITIAL);lines++; column = 1;}
    <<EOF>>  {BEGIN(INITIAL);}

}
"\n"                            {lines++; column = 1;}
[\t\r ]+                    { column += strlen(yytext); }

"char"|"int"         {      column+= strlen(yytext);
                            yylval.identv = strdup(yytext);
                            return TYPE; }
"void"                                     {column+= strlen(yytext); return VOID;}
"else"                                     {column+= strlen(yytext); return ELSE;}
"while"                                 {column+= strlen(yytext); return WHILE;}
"return"                                 {column+= strlen(yytext); return RETURN;}
"struct"                                {column+= strlen(yytext); return STRUCT;}
"("|")"|"{"|"}"|","|";"|"="|"."             {column+= strlen(yytext); return yytext[0];}
"&&"                                     {column+= strlen(yytext); return AND;}
"||"                                     {column+= strlen(yytext); return OR;}
"=="|"!="                                 {column+= strlen(yytext); return EQ;}
">="|"<="|">"|"<"                        {column+= strlen(yytext); yylval.identv = strdup(yytext); return ORDER;}
"if"                                     {column+= strlen(yytext); return IF;}
[+-]                                     {column+= strlen(yytext);yylval.charv = yytext[0];return ADDSUB;}
"/"|"*"|"%"                             {column+= strlen(yytext);yylval.charv = yytext[0]; return DIVSTAR;}
[a-zA-Z_][a-zA-Z0-9_]*                    { column+= strlen(yytext); yylval.identv = strdup(yytext); return IDENT; }
[0-9]+                                   { column+= strlen(yytext); yylval.intv = atoi(yytext);  return NUM;}
'[^\\\n\t\r\v\f]'        { column+= strlen(yytext); yylval.charv = yytext[1]; return CHARACTER; }
'\\[abfnrtv\\\'"?0]' {column+= strlen(yytext);  yylval.charv = yytext[2]; return CHARACTER; }
<<EOF>> {return 0;}
. {
    column+= strlen(yytext);
     return yytext[0];
}


%%%%
