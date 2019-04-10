%option c++ prefix="m19_scanner_" outfile="m19_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "m19_parser.tab.h"

static char* realint,* realsci;
static char* ponto = strdup("."),* e_let = strdup("e");

inline char* sci(char* scichar) {
	const char* ret;
	char c = scichar[0];
	switch (c) {
		case 'a':
		ret = "-18";
		break;
		case 'f':
		ret = "-15";
		break;
		case 'p':
		ret = "-12";
		break;
		case 'n':
		ret = "-9";
		break;
		case 'u':
		ret = "-6";
		break;
		case 'm':
		ret = "-3";
		break;
		case 'k':
		ret = "3";
		break;
		case 'M':
		ret = "6";
		break;
		case 'G':
		ret = "9";
		break;
		case 'T':
		ret = "12";
		break;
		case 'P':
		ret = "15";
		break;
		case 'E':
		ret = "18";
		break;
	}
	return strdup(ret);
}

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_INVALSTRING X_COMMENT X_R_INI X_R_SCI
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

0x[0-9A-Fa-f]+          yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;

[0-9]+\.[0-9]+			yylval.r = strtod(yytext, nullptr); return tREAL;


[0-9]+					realint = strdup(yytext); yy_push_state(X_R_INI);
<X_R_INI>[fpnumkMGTPE] 	realsci = strdup(yytext); yy_push_state(X_R_SCI);
<X_R_SCI>[0-9]+ 		{
							char* str = strcat(realint, strcat(ponto, strcat(yytext, strcat(e_let, sci(realsci)))));
							yylval.r = strtod(str, nullptr);
                			yy_pop_state(); yy_pop_state(); return tREAL;
						}

[-[\]()<>=+*/%;{}#?:.,]	return *yytext;

[ \t\n]+               	; /* ignore whitespace */

.                      	yyerror("Unknown character");

%%
