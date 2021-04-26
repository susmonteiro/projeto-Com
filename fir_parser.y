%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <algorithm>
#include <memory>
#include <cstring>
#include <cdk/compiler.h>
#include <cdk/types/types.h>
#include "ast/all.h"
#define LINE                         compiler->scanner()->lineno()
#define yylex()                      compiler->scanner()->scan()
#define yyerror(compiler, s)         compiler->scanner()->error(s)
//-- don't change *any* of these --- END!
%}

%parse-param {std::shared_ptr<cdk::compiler> compiler}

%union {
  //--- don't change *any* of these: if you do, you'll break the compiler.
  YYSTYPE() : type(cdk::primitive_type::create(0, cdk::TYPE_VOID)) {}
  ~YYSTYPE() {}
  YYSTYPE(const YYSTYPE &other) { *this = other; }
  YYSTYPE& operator=(const YYSTYPE &other) { type = other.type; return *this; }

  std::shared_ptr<cdk::basic_type> type;        /* expression type */
  //-- don't change *any* of these --- END!

  int                   i;	     /* integer value */
  double                d;         /* double value */
  std::string           *s;	     /* symbol name or string literal */

  cdk::basic_node       *node;	/* node pointer */
  cdk::sequence_node    *sequence;
  cdk::expression_node  *expression; /* expression nodes */
  cdk::lvalue_node      *lvalue;

  fir::prologue_node    *prologue;
  fir::block_node       *block;
};

%token <i> tINTEGER
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING
%token <expression> tNULL


%token tWRITE tWRITELN
%token tTYPE_INT tTYPE_FLOAT tTYPE_STRING tTYPE_VOID
%token tPUBLIC tPRIVATE tEXTERN
%token tIF tTHEN tELSE
%token tWHILE tDO tFINALLY
%token tLEAVE tRESTART tRETURN
%token tDEFAULT_VALUE tEPILOGUE
%token tSIZEOF

%type <s> string
%type <node> stmt declaration vardec fundef argdec instruction
%type <sequence> list exprs opt_exprs file declarations opt_vardecs vardecs argdecs opt_instructions instructions
%type <expression> expr integer real default_value
%type<type> data_type
// %type <lvalue> lval
%type <prologue> prologue
%type <block> block epilogue

%nonassoc tIFX
%nonassoc tELSE

%right '='
%left tOR
%left tAND
%right '!'
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file           : /* empty */  { compiler->ast($$ = new cdk::sequence_node(LINE)); }
               | declarations { compiler->ast($$ = $1); }
               ;

declarations   :              declaration { $$ = new cdk::sequence_node(LINE, $1);     }
               | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

declaration    : vardec ';' { $$ = $1; }
             /* | fundec     { $$ = $1; } */
               | fundef     { $$ = $1; }
               ;

opt_vardecs    : /* empty */ { $$ = nullptr; }         // TODO CHECK WHY NULL
               | vardecs     { $$ = $1; }
               ;

vardecs        : vardec ';'          { $$ = new cdk::sequence_node(LINE, $1);     }
               | vardecs vardec ';'  { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

vardec         : /* empty */         { $$ = nullptr; }
               ;
             

fundef         : data_type tIDENTIFIER '(' argdecs ')' default_value prologue block epilogue { $$ = new fir::function_definition_node(LINE, *$2, tPRIVATE, $1, $4, $6, $7, $8, $9); }
               | data_type '*' tIDENTIFIER '(' argdecs ')' default_value prologue block epilogue { $$ = new fir::function_definition_node(LINE, *$3, tPUBLIC, $1, $5, $7, $8, $9, $10); }
               | data_type '?' tIDENTIFIER '(' argdecs ')' default_value prologue block epilogue { $$ = new fir::function_definition_node(LINE, *$3, tEXTERN, $1, $5, $7, $8, $9, $10); }
               ;


argdecs        : /* empty */         { $$ = new cdk::sequence_node(LINE);  }
               |             argdec  { $$ = new cdk::sequence_node(LINE, $1);     }
               | argdecs ',' argdec  { $$ = new cdk::sequence_node(LINE, $3, $1); }
               ;

argdec         : data_type tIDENTIFIER { $$ = new fir::variable_declaration_node(LINE, tPRIVATE, $1, *$2, nullptr); }
               ;


default_value  : /* empty */                      { $$ = nullptr; }
               | tDEFAULT_VALUE integer           { $$ = $2; }
               | tDEFAULT_VALUE real              { $$ = $2; }
               | tDEFAULT_VALUE string            { $$ = new cdk::string_node(LINE, $2); }
               | tDEFAULT_VALUE tNULL             { $$ = new fir::null_node(LINE); }
               ;

prologue       : /* empty */                      { $$ = new fir::prologue_node(LINE, nullptr); }
               | '@' block                        { $$ = new fir::prologue_node(LINE, $2); }
               ;

epilogue       : /* empty */                      { $$ = nullptr; }
               | tEPILOGUE block                  { $$ = $2; }
               ;

block          : '{' opt_vardecs opt_instructions '}'       { $$ = new fir::block_node(LINE, $2, $3); }
               ;

opt_instructions: /* empty */  { $$ = new cdk::sequence_node(LINE); }
                | instructions { $$ = $1; }
                ;

instructions    : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
                | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
                ;

instruction     : /* empty */ { $$ = nullptr; }   // TODO
                ;


data_type      : tTYPE_INT                        { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
               | tTYPE_FLOAT                      { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
               | tTYPE_STRING                     { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
               | tTYPE_VOID                       { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID);    }
               | '<' data_type '>'                { $$ = cdk::reference_type::create(4, $2); }
               ;

list           : stmt	                         { $$ = new cdk::sequence_node(LINE, $1); }
               | list stmt                        { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

        
stmt           : expr ';'                            { $$ = new fir::evaluation_node(LINE, $1); }
/*                | tWRITE exprs ';'                     { $$ = new fir::print_node(LINE, $2, false); }*/
               | tWRITELN exprs ';'                   { $$ = new fir::print_node(LINE, $2, true); } /*
               | tSIZEOF '(' expr ')' ';'            { $$ = new fir::sizeof_node(LINE, $3); }
               | leave                               { $$ = $1; }
               | restart                             { $$ = $1; }
               | tRETURN ';'                         { $$ = new fir::return_node(LINE); }
               | tWHILE expr tDO stmt                { $$ = new fir::while_node(LINE, $2, $4); }
               | tWHILE expr tDO stmt tFINALLY stmt  { $$ = new fir::while_node(LINE, $2, $4, $6); } */     
               | tIF '(' expr ')' stmt %prec tIFX    { $$ = new fir::if_node(LINE, $3, $5); }
               | tIF '(' expr ')' stmt tELSE stmt    { $$ = new fir::if_else_node(LINE, $3, $5, $7); }
               | '{' list '}'                        { $$ = $2; }
               ;

expr           : tINTEGER                   { $$ = new cdk::integer_node(LINE, $1); }
               /* | tREAL                      { $$ = new cdk::double_node(LINE, $1); } */ /* TODO check */
               | tSTRING                    { $$ = new cdk::string_node(LINE, $1); }
               | '-' expr %prec tUNARY      { $$ = new cdk::neg_node(LINE, $2); }
               | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); }
               | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); }
               | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); }
               | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); }
               | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); }
               | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); }
               | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); }
               | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); }
               | expr tLE expr              { $$ = new cdk::le_node(LINE, $1, $3); }
               | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); }
               | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); }
               | '(' expr ')'               { $$ = $2; }
/*                | lval                       { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
               | lval '=' expr              { $$ = new cdk::assignment_node(LINE, $1, $3); }*/     
               | '@'                        { $$ = new fir::read_node(LINE); }
               ;

/* lval : tIDENTIFIER                 { $$ = new cdk::variable_node(LINE, $1); }
     ; */ // TODO

/* leave : tLEAVE ';'                { $$ = new fir::leave_node(LINE); }
      | tLEAVE tINTEGER ';'       { $$ = new fir::leave_node(LINE, $2); }
      ;

restart   : tRESTART ';'          { $$ = new fir::restart_node(LINE); }
          | tRESTART tINTEGER ';' { $$ = new fir::restart_node(LINE, $2); }
          ; */ // TODO

exprs          : expr                  { $$ = new cdk::sequence_node(LINE, $1);     }
               | exprs ',' expr        { $$ = new cdk::sequence_node(LINE, $3, $1); }
               ;

opt_exprs      : /* empty */           { $$ = new cdk::sequence_node(LINE); }
               | exprs                 { $$ = $1; }
               ;

integer        : tINTEGER              { $$ = new cdk::integer_node(LINE, $1); };
real           : tREAL                 { $$ = new cdk::double_node(LINE, $1); };
string         : tSTRING               { $$ = $1; }
               | string tSTRING        { $$ = $1; $$->append(*$2); delete $2; }
               ;
%%
