%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                    i;	/* integer value */
  double                 r; /*double value*/
  std::string           *s;	/* symbol name or string literal */
  cdk::basic_node       *node;	/* node pointer */
  cdk::sequence_node    *sequence;
  cdk::expression_node  *expression; /* expression nodes */
  cdk::lvalue_node      *lvalue;
};

%token <i> tINTEGER
%token <r> tREAL
%token <s> tIDENTIFIER tSTRING
%token tIF tPRINT tREAD

%nonassoc tIFEX

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY

%type <node> stmt program
%type <sequence> list exps
%type <expression> expr
%type <lvalue> lval
%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program	: tBEGIN list tEND { compiler->ast(new m19::program_node(LINE, $2)); }
	      ;

list : stmt	     { $$ = new cdk::sequence_node(LINE, $1); }
	   | list stmt { $$ = new cdk::sequence_node(LINE, $2, $1); }
	   ;


stmt : expr ';'                             { $$ = new m19::evaluation_node(LINE, $1); } //Fica
 	   | tPRINT expr ';'                      { $$ = new m19::print_node(LINE, $2); } //TOBECHANGED
     | tREAD lval ';'                       { $$ = new m19::read_node(LINE, $2); } //TOBECHANGED
     | '[' expr ']' '#' stmt                { $$ = new m19::if_node(LINE, $2, $5); }
     | '[' expr ']' '?' stmt %prec tIFEX    { $$ = new m19::if_node(LINE, $2, $5); }
     | '[' expr ']' '?' stmt ':' stmt       { $$ = new m19::if_else_node(LINE, $2, $5, $7); }
     | '[' exps ';' exps ';' exps ']' stmt  { $$ = new m19::for_node(LINE, $2, $4, $6, $8); }
     | '{' list '}'                         { $$ = $2; }
     ;

expr : tINTEGER                { $$ = new cdk::integer_node(LINE, $1); }
     | tREAL                   { $$ = new cdk::double_node(LINE, $1); }
	   | tSTRING                 { $$ = new cdk::string_node(LINE, $1); } //Maybe
     | '-' expr %prec tUNARY   { $$ = new cdk::neg_node(LINE, $2); } // Fica
     | '+' expr %prec tUNARY   { $$ = $2; }
     | '?' lval %prec tUNARY   { $$ = new m19::ref_node(LINE, $2); } //done
     | '~' expr %prec tUNARY   { $$ = new cdk::not_node(LINE, $2); } //FROM CDK
     | expr '&&' expr	         { $$ = new cdk::and_node(LINE, $1, $3); } //FROM CDK
     | expr '||' expr	         { $$ = new cdk::or_node(LINE, $1, $3); } //FROM CDK
     | expr '+' expr	         { $$ = new cdk::add_node(LINE, $1, $3); } //Fica
     | expr '-' expr	         { $$ = new cdk::sub_node(LINE, $1, $3); } //Fica
     | expr '*' expr	         { $$ = new cdk::mul_node(LINE, $1, $3); } //Fica
     | expr '/' expr	         { $$ = new cdk::div_node(LINE, $1, $3); } //Fica
     | expr '%' expr	         { $$ = new cdk::mod_node(LINE, $1, $3); } //Fica
     | expr '<' expr	         { $$ = new cdk::lt_node(LINE, $1, $3); } //Fica
     | expr '>' expr	         { $$ = new cdk::gt_node(LINE, $1, $3); } //Fica
     | expr tGE expr	         { $$ = new cdk::ge_node(LINE, $1, $3); } //Fica
     | expr tLE expr           { $$ = new cdk::le_node(LINE, $1, $3); } //Fica
     | expr tNE expr	         { $$ = new cdk::ne_node(LINE, $1, $3); } //Fica
     | expr tEQ expr	         { $$ = new cdk::eq_node(LINE, $1, $3); } //Fica
     | '(' expr ')'            { $$ = $2; } //Fica
     | lval                    { $$ = new cdk::rvalue_node(LINE, $1); }  //FIXME
     | lval '=' expr           { $$ = new cdk::assignment_node(LINE, $1, $3); } //Fica
     //PENSAR EM []
     ;

exps : expr          { $$ = new cdk::sequence_node(LINE, $1); }
	   | exps ',' expr { $$ = new cdk::sequence_node(LINE, $3, $1); }
	   ;

lval : tIDENTIFIER             { $$ = new cdk::variable_node(LINE, $1); }
     ;

%%
