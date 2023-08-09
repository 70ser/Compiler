%option noyywrap yylineno

%{

	#include <string>
	#include "../structure/ast.h"
	#include "parser.hpp"
	#include <cstdlib>
    #include <iostream>

	int get10(char *, int);
	int get8(char *, int);
	int get16(char *, int);

	extern int yyleng;

%}

%start COMMENT
%start LINECOMMENT

%%

<INITIAL>"//" {
	BEGIN LINECOMMENT;
}

<LINECOMMENT>\n {
	BEGIN INITIAL;
}

<LINECOMMENT>[^\n]* {}

<INITIAL>"/*" {
	BEGIN COMMENT;
}

<COMMENT>"*/" {
	BEGIN INITIAL;
}

<COMMENT>[^\n\*]* {}
<COMMENT>\* {}

return		{ return RETURN; }
if			{ return IF;}
else 		{ return ELSE; }
while		{ return WHILE; }
break		{ return BREAK; }
continue	{ return CONTINUE; }
void 		{ return VOIDT; }
int 		{ return INTT; }
float		{ return FLOATT; }
const		{ return CONST; }

starttime"("")"	{ return STARTTIME; }
stoptime"("")"	{ return STOPTIME; }

[a-z_A-Z][0-9a-z_A-Z]* {
	yylval.id = new std::string(yytext);
	return ID;
}


0 {
	yylval.token = 0;
	return INTNUMBER;
}

0[0-7]+ {
	yylval.token = get8(yytext, yyleng);
	return INTNUMBER;
}

0[xX][0-9a-fA-F]+ {
	yylval.token = get16(yytext, yyleng);
	return INTNUMBER;
}

[1-9][0-9]* {
	yylval.token = get10(yytext, yyleng);
	return INTNUMBER;
}

[ \n\t\r] {}

([0-9]*)\.[0-9]* {
	yylval.floatnumber = new std::string(yytext);
	return FLOATNUMBER;
}

[0-9]+[eE][\+\-]?[0-9]+ {
	yylval.floatnumber = new std::string(yytext);
	return FLOATNUMBER;
}

[0-9]*\.[0-9]*[eE][\+\-]?[0-9]+ {
	yylval.floatnumber = new std::string(yytext);
	return FLOATNUMBER;
}

0[xX][0-9a-fA-F]*[\.]?[0-9a-fA-F]*[pP][\+\-]?[0-9]+ {
	yylval.floatnumber = new std::string(yytext);
	return FLOATNUMBER;
}

"+"			{ return ADDOP; }
"-"			{ return SUBOP; }
"!"			{ return NOTOP; }
"*"			{ yylval.mul = AST::mul_t::MULT; return MULOP; }
"/"			{ yylval.mul = AST::mul_t::DIV; return MULOP; }
"%"			{ yylval.mul = AST::mul_t::REM; return MULOP; }
"<"			{ yylval.rel = AST::rel_t::LT; return RELOP; }
">"			{ yylval.rel = AST::rel_t::GT; return RELOP; }
"<="		{ yylval.rel = AST::rel_t::LE; return RELOP; }
">="		{ yylval.rel = AST::rel_t::GE; return RELOP; }
"=="		{ yylval.equal = AST::equal_t::EQ; return EQUALOP; }
"!="		{ yylval.equal = AST::equal_t::NE; return EQUALOP; }
"&&"		{ return LANDOP; }
"||"		{ return LOROP; }

"{" { return '{';}
"}" { return '}';}
"(" { return '(';}
")" { return ')';}
"[" { return '[';}
"]" { return ']';}
";" { return ';';}
"=" { return '=';}
"," { return ',';}


. {
	yylval.token = yytext[0];
	std::cerr << yytext << std::endl;
	exit(yytext[0]);
}


%%
int get10(char *s, int l)
{
	int ret = 0;
	for(int i = 0; i < l; i++) {
		ret = ret * 10 + s[i] - '0';
	}
	return ret;
}

int get8(char *s, int l)
{
	int ret = 0;
	for(int i = 0; i < l; i++) {
		ret = ret * 8 + s[i] - '0';
	}
	return ret;
}

int get16(char *s, int l)
{
	int ret = 0;
	for(int i = 0; i < l; i++) {
		ret = ret * 16;
		if (s[i] <= '9' && s[i] >= '0') ret += s[i] - '0';
		else if (s[i] <= 'f' && s[i] >= 'a') ret += 10 + s[i] - 'a';
		else if (s[i] <= 'F' && s[i] >= 'A') ret += 10 + s[i] - 'A';
	}
	return ret;
}