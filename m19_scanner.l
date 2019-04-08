%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_INVALSTRING X_COMMENT
%%
						{ set_debug(1); }

">="                   	return tGE; 
"<="                   	return tLE; 
"=="                   	return tEQ; 
"!="                   	return tNE; 

"&&"                    return tAND;
"||"                    return tOR;


"!!"                    return tPRINTLN;

">@<"                  	return tPUNCH;
"@<<" 	                return tCONT;
">>@"   	            return tBREAK;

"<<"                   	return tINSEC;
">>"                   	return tOUTSEC;

[A-Za-z][A-Za-z0-9_]*  	yylval.s = new std::string(yytext); return tIDENTIFIER;

"/*"               	   	yy_push_state(X_COMMENT);
<X_COMMENT>"*/"		   	yy_pop_state();
<X_COMMENT>.		   	;

"//".*             	   	; /* ignore comments */

\"                     	yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           	yy_pop_state(); return tSTRING;
<X_STRING>\\\"         	*yylval.s += yytext + 1;
<X_STRING>\\\0         	yy_push_state(X_INVALSTRING);
<X_STRING>.            	*yylval.s += yytext;

<X_INVALSTRING>\"      	yy_pop_state(); yy_pop_state(); return tSTRING;
<X_INVALSTRING>\\\"    	;
<X_INVALSTRING>.       	;

 /*<X_STRING>\n           yyerror("newline in string");*/

[0-9]+                 	yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

[-[\]()<>=+*/%;{}#?:.,]	return *yytext;

[ \t\n]+               	; /* ignore whitespace */

.                      	yyerror("Unknown character");

%%
