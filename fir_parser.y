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
%type <node> instruction declaration vardec fundef argdec fundec leave restart
%type <sequence> exprs opt_exprs file declarations opt_vardecs vardecs argdecs opt_instructions instructions
%type <expression> expr integer real default_value opt_init
%type<type> data_type void_type
%type <lvalue> lvalue
%type <prologue> prologue
%type <block> block epilogue body

%nonassoc tIFX
%nonassoc tELSE

%nonassoc tWHILEX
%nonassoc tFINALLY

%right '='
%left tOR
%left tAND
%right '~'
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY tFUNDEC

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file           : declarations { compiler->ast($$ = $1); } 
               ;

declarations   :              declaration { $$ = new cdk::sequence_node(LINE, $1);     }
               | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

declaration    : vardec ';' { $$ = $1; }
               | fundec     { $$ = $1; }
               | fundef     { $$ = $1; }
               ;

opt_vardecs    : /* empty */ { $$ = nullptr; }         // TODO CHECK WHY NULL
               | vardecs     { $$ = $1; }
               ;

vardecs        : vardec ';'          { $$ = new cdk::sequence_node(LINE, $1);     }
               | vardecs vardec ';'  { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

vardec         : data_type     tIDENTIFIER opt_init   { $$ = new fir::variable_declaration_node(LINE, tPRIVATE, $1, *$2, $3 ); }
               | data_type '*' tIDENTIFIER opt_init   { $$ = new fir::variable_declaration_node(LINE, tPUBLIC, $1, *$3, $4 ); }
               | data_type '?' tIDENTIFIER            { $$ = new fir::variable_declaration_node(LINE, tPRIVATE, $1, *$3, nullptr ); }
               ;

opt_init       : /* empty */         { $$ = nullptr; }
               | '=' expr            { $$ = $2; }
               ;
             

fundec         : data_type     tIDENTIFIER '(' argdecs ')' tFUNDEC { $$ = new fir::function_declaration_node(LINE, *$2, tPRIVATE, $1, $4); }
               | data_type '*' tIDENTIFIER '(' argdecs ')' tFUNDEC { $$ = new fir::function_declaration_node(LINE, *$3, tPUBLIC, $1, $5); }
               | data_type '?' tIDENTIFIER '(' argdecs ')' tFUNDEC { $$ = new fir::function_declaration_node(LINE, *$3, tEXTERN, $1, $5); }
               | void_type     tIDENTIFIER '(' argdecs ')' tFUNDEC { $$ = new fir::function_declaration_node(LINE, *$2, tPRIVATE, $1, $4); }
               | void_type '*' tIDENTIFIER '(' argdecs ')' tFUNDEC { $$ = new fir::function_declaration_node(LINE, *$3, tPUBLIC, $1, $5); }
               | void_type '?' tIDENTIFIER '(' argdecs ')' tFUNDEC { $$ = new fir::function_declaration_node(LINE, *$3, tEXTERN, $1, $5); }
               ;

fundef         : data_type     tIDENTIFIER '(' argdecs ')' default_value prologue body epilogue { $$ = new fir::function_definition_node(LINE, *$2, tPRIVATE, $1, $4, $6, $7, $8, $9); }
               | data_type '*' tIDENTIFIER '(' argdecs ')' default_value prologue body epilogue { $$ = new fir::function_definition_node(LINE, *$3, tPUBLIC,  $1, $5, $7, $8, $9, $10); }
               | data_type '?' tIDENTIFIER '(' argdecs ')' default_value prologue body epilogue { $$ = new fir::function_definition_node(LINE, *$3, tEXTERN,  $1, $5, $7, $8, $9, $10); }
               | void_type     tIDENTIFIER '(' argdecs ')' default_value prologue body epilogue { $$ = new fir::function_definition_node(LINE, *$2, tPRIVATE, $1, $4, $6, $7, $8, $9); }
               | void_type '*' tIDENTIFIER '(' argdecs ')' default_value prologue body epilogue { $$ = new fir::function_definition_node(LINE, *$3, tPUBLIC,  $1, $5, $7, $8, $9, $10); }
               | void_type '?' tIDENTIFIER '(' argdecs ')' default_value prologue body epilogue { $$ = new fir::function_definition_node(LINE, *$3, tEXTERN,  $1, $5, $7, $8, $9, $10); }
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

prologue       : /* empty */                      { $$ = nullptr; }
               | '@' block                        { $$ = new fir::prologue_node(LINE, $2); }
               ;

epilogue       : /* empty */                      { $$ = nullptr; }
               | tEPILOGUE block                  { $$ = $2; }
               ;

body           : /* empty */                        { $$ = nullptr; }
               | block                              { $$ = $1; }

block          : '{' opt_vardecs opt_instructions '}'       { $$ = new fir::block_node(LINE, $2, $3); }
               ;

opt_instructions : /* empty */  { $$ = new cdk::sequence_node(LINE); }
                 | instructions { $$ = $1; }
                 ;

instructions   : instruction                { $$ = new cdk::sequence_node(LINE, $1);     }
               | instructions instruction   { $$ = new cdk::sequence_node(LINE, $2, $1); }
               ;

/* var_type       : tTYPE_INT                  { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
               | tTYPE_FLOAT                { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
               | tTYPE_STRING               { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
               | '<' data_type '>'          { $$ = cdk::reference_type::create(4, $2); }
               ;

data_type      : var_type                   { $$ = $1; }
               | tTYPE_VOID                 { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID); }
               ; */

data_type      : tTYPE_INT                  { $$ = cdk::primitive_type::create(4, cdk::TYPE_INT);     }
               | tTYPE_FLOAT                { $$ = cdk::primitive_type::create(8, cdk::TYPE_DOUBLE);  }
               | tTYPE_STRING               { $$ = cdk::primitive_type::create(4, cdk::TYPE_STRING);  }
               | '<' void_type '>'          { $$ = cdk::reference_type::create(4, $2); }
               | '<' data_type '>'          { $$ = cdk::reference_type::create(4, $2); }
               ;


void_type      : tTYPE_VOID                 { $$ = cdk::primitive_type::create(0, cdk::TYPE_VOID); }
               ;
        

        
instruction    : expr ';'                                         { $$ = new fir::evaluation_node(LINE, $1); }
               | tIF expr tTHEN instruction %prec tIFX            { $$ = new fir::if_node(LINE, $2, $4); }
               | tIF expr tTHEN instruction tELSE instruction     { $$ = new fir::if_else_node(LINE, $2, $4, $6); }
               | tWHILE expr tDO instruction %prec tWHILEX        { $$ = new fir::while_node(LINE, $2, $4, nullptr); } // TODO %prec?
               | tWHILE expr tDO instruction tFINALLY instruction { $$ = new fir::while_node(LINE, $2, $4, $6); } 
               | tWRITE exprs ';'                                 { $$ = new fir::print_node(LINE, $2, false); }
               | tWRITELN exprs ';'                               { $$ = new fir::print_node(LINE, $2, true); }
               | tRETURN                                          { $$ = new fir::return_node(LINE); }
               | leave                                            { $$ = $1; }
               | restart                                          { $$ = $1; }
               | block                                            { $$ = $1; }
               ;

leave          : tLEAVE ';'                { $$ = new fir::leave_node(LINE); }
               | tLEAVE tINTEGER ';'       { $$ = new fir::leave_node(LINE, $2); }
               ;

restart        : tRESTART ';'              { $$ = new fir::restart_node(LINE); }
               | tRESTART tINTEGER ';'     { $$ = new fir::restart_node(LINE, $2); }
               ;

expr           : integer                       { $$ = $1; }
               | real                          { $$ = $1; }
               | string                        { $$ = new cdk::string_node(LINE, $1); }
               | tNULL                         { $$ = new fir::null_node(LINE); }
               // left values
               | lvalue                        { $$ = new cdk::rvalue_node(LINE, $1); }
               // assignments
               | lvalue '=' expr               { $$ = new cdk::assignment_node(LINE, $1, $3); }
               // address of node
               | lvalue '?'                    { $$ = new fir::address_of_node(LINE, $1); }
               // arithmetic expressions
               | expr '+' expr	               { $$ = new cdk::add_node(LINE, $1, $3); }
               | expr '-' expr	               { $$ = new cdk::sub_node(LINE, $1, $3); }
               | expr '*' expr	               { $$ = new cdk::mul_node(LINE, $1, $3); }
               | expr '/' expr	               { $$ = new cdk::div_node(LINE, $1, $3); }
               | expr '%' expr	               { $$ = new cdk::mod_node(LINE, $1, $3); }
               // comparison expressions
               | expr '<' expr	               { $$ = new cdk::lt_node(LINE, $1, $3); }
               | expr '>' expr	               { $$ = new cdk::gt_node(LINE, $1, $3); }
               | expr tGE expr	               { $$ = new cdk::ge_node(LINE, $1, $3); }
               | expr tLE expr                 { $$ = new cdk::le_node(LINE, $1, $3); }
               | expr tNE expr	               { $$ = new cdk::ne_node(LINE, $1, $3); }
               | expr tEQ expr	               { $$ = new cdk::eq_node(LINE, $1, $3); }
               // logical expressions
               | expr tAND expr                { $$ = new cdk::and_node(LINE, $1, $3); }
               | expr tOR  expr                { $$ = new cdk::or_node (LINE, $1, $3); }
               // unary expressions  
               | '-' expr %prec tUNARY         { $$ = new cdk::neg_node(LINE, $2); }
               | '+' expr %prec tUNARY         { $$ = $2; }
               | '~' expr                      { $$ = new cdk::not_node(LINE, $2); }
               // read expression  
               | '@'                           { $$ = new fir::read_node(LINE); }
               // function call
               | tIDENTIFIER '(' opt_exprs ')' { $$ = new fir::function_call_node(LINE, *$1, $3); delete $1; }
               // other expressions
               | tSIZEOF '(' expr ')'          { $$ = new fir::sizeof_node(LINE, $3); }
               | '[' expr ']'                  { $$ = new fir::stack_alloc_node(LINE, $2); }
               | '(' expr ')'                  { $$ = $2; }
               ;

lvalue         : tIDENTIFIER                                 { $$ = new cdk::variable_node(LINE, *$1); delete $1; }
               | lvalue       '[' expr ']'                   { $$ = new fir::index_node(LINE, new cdk::rvalue_node(LINE, $1), $3); }
               | '(' expr ')' '[' expr ']'                   { $$ = new fir::index_node(LINE, $2, $5); }
               | tIDENTIFIER '(' opt_exprs ')' '[' expr ']'  { $$ = new fir::index_node(LINE, new fir::function_call_node(LINE, *$1, $3), $6); }
               ;


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
