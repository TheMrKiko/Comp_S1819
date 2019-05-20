%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"

static std::ostringstream strlit;

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_INVALSTRING X_COMMENT X_R_INI X_R_FIN
%x X_R_a X_R_f X_R_p X_R_n X_R_u X_R_m X_R_k X_R_M X_R_G X_R_T X_R_P X_R_E
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
"@>>" 	                return tCONT;
">>@"   	            return tBREAK;

"<<"                   	return tINSEC;
">>"                   	return tOUTSEC;

[A-Za-z][A-Za-z0-9_]*  	yylval.s = new std::string(yytext); return tIDENTIFIER;

"/*"               	   	yy_push_state(X_COMMENT);
<X_COMMENT>"/*"         yy_push_state(X_COMMENT);
<X_COMMENT>"*/"		   	yy_pop_state();
<X_COMMENT>.		   	;

"//".*             	   	; /* ignore comments */

\"                     	yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           	yy_pop_state(); return tSTRING;
<X_STRING>\\\"         	*yylval.s += yytext + 1;
<X_STRING>\\\\			*yylval.s += yytext + 1;
<X_STRING>\\\0         	yy_push_state(X_INVALSTRING);
<X_STRING>\\\[0-9a-fA-F]{1,2} 	{
									strlit << '\\' << 'x' << *(yytext + 1);
									*yylval.s += strlit.str().c_str();
									strlit.str("");
								}
<X_STRING>.            	*yylval.s += yytext;
<X_INVALSTRING>\"      	yy_pop_state(); yy_pop_state(); return tSTRING;
<X_INVALSTRING>\\\"    	;
<X_INVALSTRING>.       	;

 /*<X_STRING>\n           yyerror("newline in string");*/

0x[0-9A-Fa-f]+    	{
						yylval.i = strtoul(yytext, nullptr, 16);
						if (errno == ERANGE) yyerror("Overflow in int");
						return tINTEGER;
					}

[0-9]+\.[0-9]+		{
						yylval.r = strtod(yytext, nullptr);	
						if (errno == ERANGE) yyerror("Overflow in real");
						return tREAL;
					}


[0-9]+					strlit << *yytext; yy_push_state(X_R_INI);

<X_R_INI>a      		strlit << '.'; yy_push_state(X_R_a);
<X_R_a>[0-9]+			strlit << *yytext << "e-18"; yy_push_state(X_R_FIN);

<X_R_INI>f				strlit << '.'; yy_push_state(X_R_f);
<X_R_f>[0-9]+			strlit << *yytext << "e-15"; yy_push_state(X_R_FIN);

<X_R_INI>p				strlit << '.'; yy_push_state(X_R_p);
<X_R_p>[0-9]+			strlit << *yytext << "e-12"; yy_push_state(X_R_FIN);

<X_R_INI>n				strlit << '.'; yy_push_state(X_R_n);
<X_R_n>[0-9]+			strlit << *yytext << "e-9"; yy_push_state(X_R_FIN);

<X_R_INI>u				strlit << '.'; yy_push_state(X_R_u);
<X_R_u>[0-9]+			strlit << *yytext << "e-6"; yy_push_state(X_R_FIN);

<X_R_INI>m				strlit << '.'; yy_push_state(X_R_m);
<X_R_m>[0-9]+			strlit << *yytext << "e-3"; yy_push_state(X_R_FIN);

<X_R_INI>k				strlit << '.'; yy_push_state(X_R_k);
<X_R_k>[0-9]+			strlit << *yytext << "e3"; yy_push_state(X_R_FIN);

<X_R_INI>M				strlit << '.'; yy_push_state(X_R_M);
<X_R_M>[0-9]+			strlit << *yytext << "e6"; yy_push_state(X_R_FIN);

<X_R_INI>G				strlit << '.'; yy_push_state(X_R_G);
<X_R_G>[0-9]+			strlit << *yytext << "e9"; yy_push_state(X_R_FIN);

<X_R_INI>T				strlit << '.'; yy_push_state(X_R_T);
<X_R_T>[0-9]+			strlit << *yytext << "e12"; yy_push_state(X_R_FIN);

<X_R_INI>P				strlit << '.'; yy_push_state(X_R_P);
<X_R_P>[0-9]+			strlit << *yytext << "e15"; yy_push_state(X_R_FIN);

<X_R_INI>E				strlit << '.'; yy_push_state(X_R_E);
<X_R_E>[0-9]+			strlit << *yytext << "e18"; yy_push_state(X_R_FIN);

<X_R_INI>.|\n 		{
						yyless(yyleng - 1); yylval.i = strtoul(strlit.str().c_str(), nullptr, 10);
						if (errno == ERANGE) yyerror("Overflow in int");
						strlit.str(""); yy_pop_state(); return tINTEGER;
					}

<X_R_FIN>.|\n		{
						yyless(yyleng - 1); yylval.r = strtod(strlit.str().c_str(), nullptr);
						if (errno == ERANGE) yyerror("Overflow in real");
						strlit.str("");	yy_pop_state(); yy_pop_state(); yy_pop_state(); 
						return tREAL;
					}

[-[\]()<>=+*/%;{}$#!?:.,@]	return *yytext;

[ \t\n]+               	; /* ignore whitespace */

.                      	yyerror("Unknown character");

%%
