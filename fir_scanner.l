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
%x X_OCTAL_INT
%%

  /* === COMMENTS === */
                      // { set_debug(true); }

"!!".*                 ; /* ignore comments */
"(*"[.\n]*"*)"         ; /* ignore comments */

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

"0"[0-7]               { yy_push_state(X_OCTAL_INT); return tINTEGER; } // TODO todo

<X_OCTAL_INT>[0-7]+    { yylval.i = strtoul(yytext, NULL, 8); yy_pop_state(); return tINTEGER; }
<X_OCTAL_INT>.|\n      std::cerr << "WARNING: line " << lineno() << ": bad octal data!" << std::endl; exit(1);

[0-9]+                  yylval.i = strtoul(yytext, NULL, 10); return tINTEGER;

  /* === REALS === */

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)? { yylval.d = strtod(yytext, NULL); return tREAL; }


[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;


    /* === STRINGS === */

\'                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\~           yy_push_state(X_TILDE);
<X_STRING>\'           yy_pop_state(); return tSTRING;
<X_STRING>\\\'         *yylval.s += yytext + 1;
<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");

<X_TILDE>n             *yylval.s += '\n'; yy_pop_state();
<X_TILDE>r             *yylval.s += '\r'; yy_pop_state();
<X_TILDE>t             *yylval.s += '\t'; yy_pop_state();
<X_TILDE>\~            *yylval.s += '~'; yy_pop_state();

<X_TILDE>[[:xdigit:]]{1,2} {
                            *yylval.s += (char)(unsigned char)strtoul(yytext, NULL, 16);
                            yy_pop_state();
                           }
<X_TILDE>.              *yylval.s += *yytext; yy_pop_state();


[-()<>=+*/%;{}.?!@]    return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
