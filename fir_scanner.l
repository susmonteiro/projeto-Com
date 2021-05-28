%option c++ prefix="fir_scanner_" outfile="fir_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "fir_parser.tab.h"

// don't change this
#define yyerror LexerError
%}

%x X_TILDE
%x X_STRING
%x X_COMMENT
%x X_IGNORE
%%

  /* === COMMENTS === */

                        /* { yydebug = 1; set_debug(true); } */
"!!".*                 ;

"(*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*)"        yy_pop_state();
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */

  /* === DATA TYPES === */

"int"                  return tTYPE_INT;
"float"                return tTYPE_FLOAT;
"string"               return tTYPE_STRING;
"void"                 return tTYPE_VOID;

"null"                 return tNULL;
"sizeof"               return tSIZEOF;

  /* === EXPRESSION OPERATORS === */

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE; 
"&&"                   return tAND;
"||"                   return tOR;

  /* === OTHER TOKENS === */

"if"                   return tIF; 
"then"                 return tTHEN;
"else"                 return tELSE; 

"while"                return tWHILE; 
"do"                   return tDO; 
"finally"              return tFINALLY; 

"leave"                return tLEAVE; 
"restart"              return tRESTART; 

"return"               return tRETURN; 
"write"                return tWRITE; 
"writeln"              return tWRITELN;

"->"                   return tDEFAULT_VALUE;
">>"                   return tEPILOGUE; 

  /* === INTEGERS === */


[1-9][0-9]*            {
                         try {
                           yylval.i = strtoul(yytext, NULL, 10); return tINTEGER;
                         } catch (std::out_of_range& e) {
                           yyerror("Integer out of range.");
                         }
                       }
                       
[0]+                   { yylval.i = 0; return tINTEGER; }

0[0-7]*[8-9]+          std::cerr << "WARNING: line " << lineno() << ": bad octal data!" << std::endl; exit(1);

[0]+[1-7][0-7]*        {
                         try {
                           yylval.i = strtoul(yytext, NULL, 8); return tINTEGER;
                         } catch (std::out_of_range& e) {
                           yyerror("Integer out of range.");
                         }
                       }





  /* === REALS === */

([0-9]*)([eE][-+]?[0-9]+)                         {
                                                    try {
                                                      yylval.d = strtod(yytext, NULL); return tREAL;
                                                    } catch (std::out_of_range& e) {
                                                      yyerror("Float out of range.");
                                                    }
                                                  }

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)? {
                                                    try {
                                                      yylval.d = strtod(yytext, NULL); return tREAL;
                                                    } catch (std::out_of_range& e) {
                                                      yyerror("Float out of range.");
                                                    }
                                                  }

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;


    /* === STRINGS === */

\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\~           yy_push_state(X_TILDE);
<X_STRING>\'           yy_pop_state(); return tSTRING;
<X_STRING>\n           yyerror("newline in string");
<X_STRING>.            *yylval.s += yytext;

<X_TILDE>n             *yylval.s += '\n'; yy_pop_state();
<X_TILDE>r             *yylval.s += '\r'; yy_pop_state();
<X_TILDE>t             *yylval.s += '\t'; yy_pop_state();
<X_TILDE>\~            *yylval.s += '~'; yy_pop_state();

<X_TILDE>[[:xdigit:]]{1,2} {
                            yy_pop_state();
                            if ((char)(unsigned char)strtoul(yytext, NULL, 16) == 0) {
                              yy_push_state(X_IGNORE);
                            } else {
                              *yylval.s += (char)(unsigned char)strtoul(yytext, NULL, 16);
                            }
                           }
<X_TILDE>\n               yyerror("newline in string");
<X_TILDE>.                *yylval.s += *yytext; yy_pop_state(); 

<X_IGNORE>\'              yy_pop_state(); yyless(yyleng-1);
<X_IGNORE>\n              yyerror("newline in string");
<X_IGNORE>.               ;



[-()<>=+*/%;{}\[\].,?!@]    return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
