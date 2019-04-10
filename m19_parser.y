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

  basic_type            *btype;

  m19::fun_body_node            *body;
  m19::fun_init_section_node    *init;
  m19::fun_final_section_node   *final;
  m19::block_node               *block;


};

%token <i> tINTEGER
%token <r> tREAL
%token <s> tIDENTIFIER tSTRING
%token tPRINTLN tREAD tPUNCH tCONT tBREAK tINSEC tOUTSEC

%nonassoc tIFEX tVDECX tFDECX

%right '='
%left tGE tLE tEQ tNE '>' '<'
%left tAND tOR
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY ':'

%type <i> qali
%type <s> strs
%type <node> stmt program decl var func sec
%type <sequence> decls exps args vars secs stmts
%type <expression> expr lit
%type <lvalue> lval
%type <btype> type
%type <body> body
%type <init> ini
%type <final> fin
%type <block> blck
%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

program : decls                              { compiler->ast($$ = $1); }
        ;

decls :       decl                           { $$ = new cdk::sequence_node(LINE, $1);     }
      | decls decl                           { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

decl : var ';'                               { $$ = $1; }
     | func                                  { $$ = $1; }
     ;

var  : type tIDENTIFIER qali %prec tVDECX    { $$ = new m19::var_decl_node(LINE, $3, $1, *$2);       }
     | type tIDENTIFIER qali '=' expr        { $$ = new m19::var_decl_node(LINE, $3, $1, *$2, $5);   }
     ;

func : type tIDENTIFIER qali '(' args ')' %prec tFDECX               { $$ = new m19::fun_decl_node(LINE, $1, *$2, $3, $5);        }
     | '!'  tIDENTIFIER qali '(' args ')' %prec tFDECX               { $$ = new m19::fun_decl_node(LINE, new basic_type(0, basic_type::TYPE_VOID), *$2, $3, $5);        }
     | type tIDENTIFIER qali '(' args ')' '=' lit %prec tFDECX       { $$ = new m19::fun_decl_node(LINE, $1, *$2, $3, $8, $5);    }
     | '!'  tIDENTIFIER qali '(' args ')' '=' lit %prec tFDECX       { $$ = new m19::fun_decl_node(LINE, new basic_type(0, basic_type::TYPE_VOID), *$2, $3, $8, $5);    }

     | type tIDENTIFIER qali '(' args ')'         body  { $$ = new m19::fun_def_node(LINE, $1, *$2, $3, $7, $5);     }
     | '!'  tIDENTIFIER qali '(' args ')'         body  { $$ = new m19::fun_def_node(LINE, new basic_type(0, basic_type::TYPE_VOID), *$2, $3, $7, $5);     }
     | type tIDENTIFIER qali '(' args ')' '=' lit body  { $$ = new m19::fun_def_node(LINE, $1, *$2, $3, $9, $8, $5); }
     | '!'  tIDENTIFIER qali '(' args ')' '=' lit body  { $$ = new m19::fun_def_node(LINE, new basic_type(0, basic_type::TYPE_VOID), *$2, $3, $9, $8, $5); }
     ;

lit  : tINTEGER                              { $$ = new cdk::integer_node(LINE, $1);           }
     | tREAL                                 { $$ = new cdk::double_node(LINE, $1);            }
	| strs                                  { $$ = new cdk::string_node(LINE, $1);            }
     ;


body : ini  secs fin                         { $$ = new m19::fun_body_node(LINE, $1,      $2,      $3);      }
     | ini  secs                             { $$ = new m19::fun_body_node(LINE, $1,      $2,      nullptr); }
     | ini       fin                         { $$ = new m19::fun_body_node(LINE, $1,      nullptr, $2);      }
     | ini                                   { $$ = new m19::fun_body_node(LINE, $1,      nullptr, nullptr); }
     |      secs fin                         { $$ = new m19::fun_body_node(LINE, nullptr, $1,      $2);      }
     |      secs                             { $$ = new m19::fun_body_node(LINE, nullptr, $1,      nullptr); }
     |           fin                         { $$ = new m19::fun_body_node(LINE, nullptr, nullptr, $1);      }
     ;
     

ini  : tINSEC blck                           { $$ = new m19::fun_init_section_node(LINE, $2);   }
     ;

fin  : tOUTSEC blck                          { $$ = new m19::fun_final_section_node(LINE, $2);  }
     ;

secs :      sec                              { $$ = new cdk::sequence_node(LINE, $1);     } 
     | secs sec                              { $$ = new cdk::sequence_node(LINE, $2, $1); } 
     ;

sec  : '[' expr ']' blck                     { $$ = new m19::fun_section_node(LINE, $4, true,  $2);  }
     | '(' expr ')' blck                     { $$ = new m19::fun_section_node(LINE, $4, false, $2);  }
     | '[' ']' blck                          { $$ = new m19::fun_section_node(LINE, $3, true);       }
     | '(' ')' blck                          { $$ = new m19::fun_section_node(LINE, $3, false);      }
     | blck                                  { $$ = new m19::fun_section_node(LINE, $1, false);      }
     ;

blck : '{' decls stmts '}'                   { $$ = new m19::block_node(LINE, $2,      $3);          }
     | '{' decls       '}'                   { $$ = new m19::block_node(LINE, $2,      nullptr);     }
     | '{'       stmts '}'                   { $$ = new m19::block_node(LINE, nullptr, $2);          }
     | '{'             '}'                   { $$ = new m19::block_node(LINE, nullptr, nullptr);     }
     ;

args : /* sem args */                        { $$ = nullptr; }
     | vars                                  { $$ = $1;      }
     ;

vars :          var                          { $$ = new cdk::sequence_node(LINE, $1);     }
     | vars ',' var                          { $$ = new cdk::sequence_node(LINE, $3, $1); }
     ;

qali : /* void */                            { $$ = 0; }
     | '!'                                   { $$ = 1; }
     | '?'                                   { $$ = 2; }
     ;

type : '#'                                   { $$ = new basic_type(4, basic_type::TYPE_INT);                        }
     | '%'                                   { $$ = new basic_type(8, basic_type::TYPE_DOUBLE);                     }
     | '$'                                   { $$ = new basic_type(4, basic_type::TYPE_STRING);                     }
     | '<' type '>'                          { $$ = new basic_type(4, basic_type::TYPE_POINTER); $$->_subtype = $2; }
     ;

stmt : expr ';'                              { $$ = new m19::evaluation_node(LINE, $1);        }
 	| expr '!'                              { $$ = new m19::print_node(LINE, $1, false);      }
 	| expr tPRINTLN                         { $$ = new m19::print_node(LINE, $1, true);       }

     | tCONT                                 { $$ = new m19::continue_node(LINE);              }
     | tBREAK                                { $$ = new m19::break_node(LINE);                 }
     | tPUNCH                                { $$ = new m19::return_node(LINE);                }

     | '[' expr ']' '#' stmt                 { $$ = new m19::if_node(LINE, $2, $5);            }
     | '[' expr ']' '?' stmt %prec tIFEX     { $$ = new m19::if_node(LINE, $2, $5);            }
     | '[' expr ']' '?' stmt ':' stmt        { $$ = new m19::if_else_node(LINE, $2, $5, $7);   }

     | '[' exps ';' exps ';' exps ']' stmt   { $$ = new m19::for_node(LINE, $2, $4, $6, $8);   }

     | blck                                  { $$ = $1;                                        }
     ;

stmts :       stmt                           { $$ = new cdk::sequence_node(LINE, $1);     }
      | stmts stmt                           { $$ = new cdk::sequence_node(LINE, $2, $1); }
      ;

exps :          expr                         { $$ = new cdk::sequence_node(LINE, $1);     }
	| exps ',' expr                         { $$ = new cdk::sequence_node(LINE, $3, $1); }
	;

strs :      tSTRING                          { $$ = $1;                                   }
     | strs tSTRING                          { $$ = new std::string(*$1 + *$2);                              }
     ;

expr : tINTEGER                              { $$ = new cdk::integer_node(LINE, $1);           }
     | tREAL                                 { $$ = new cdk::double_node(LINE, $1);            }
	| strs                                  { $$ = new cdk::string_node(LINE, $1);            }
     | '-' expr %prec tUNARY                 { $$ = new cdk::neg_node(LINE, $2);               }
     | '+' expr %prec tUNARY                 { $$ = new m19::id_node(LINE, $2);                }
     | '?' lval %prec tUNARY                 { $$ = new m19::ref_node(LINE, $2);               }
     | '~' expr %prec tUNARY                 { $$ = new cdk::not_node(LINE, $2);               }
     | '@'                                   { $$ = new m19::read_node(LINE);                  }
     | expr tAND expr	                    { $$ = new cdk::and_node(LINE, $1, $3);           } 
     | expr tOR expr	                    { $$ = new cdk::or_node(LINE, $1, $3);            } 
     | expr '+' expr	                    { $$ = new cdk::add_node(LINE, $1, $3);           } 
     | expr '-' expr	                    { $$ = new cdk::sub_node(LINE, $1, $3);           } 
     | expr '*' expr	                    { $$ = new cdk::mul_node(LINE, $1, $3);           } 
     | expr '/' expr	                    { $$ = new cdk::div_node(LINE, $1, $3);           } 
     | expr '%' expr	                    { $$ = new cdk::mod_node(LINE, $1, $3);           } 
     | expr '<' expr	                    { $$ = new cdk::lt_node(LINE, $1, $3);            } 
     | expr '>' expr	                    { $$ = new cdk::gt_node(LINE, $1, $3);            } 
     | expr tGE expr	                    { $$ = new cdk::ge_node(LINE, $1, $3);            } 
     | expr tLE expr                         { $$ = new cdk::le_node(LINE, $1, $3);            } 
     | expr tNE expr	                    { $$ = new cdk::ne_node(LINE, $1, $3);            } 
     | expr tEQ expr	                    { $$ = new cdk::eq_node(LINE, $1, $3);            } 
     | '(' expr ')'                          { $$ = $2;                                        } 
     | lval                                  { $$ = new cdk::rvalue_node(LINE, $1);            }
     | lval '=' expr                         { $$ = new cdk::assignment_node(LINE, $1, $3);    } 
     ;

lval : tIDENTIFIER                           { $$ = new cdk::variable_node(LINE, *$1);          }
     ;

%%
