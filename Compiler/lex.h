#include <cstdio>
#include <cctype>
#include <iostream>
#include <cstring>
using namespace std;
#define EOI				0	/* End of input			*/
#define SEMI			1	/* ; 					*/
#define PLUS 			2	/* + 					*/
#define MINUS 			3	/* - 					*/
#define MUL				4	/* * 					*/
#define DIV				5	/* / 					*/
#define LP				6	/* (					*/
#define RP				7	/* )					*/
#define ASSIGN			8	/* =					*/
#define LESS			9	/* <					*/
#define GREATER			10	/* >					*/
#define EQ				11	/* ==					*/
#define NUM				12	/* Decimal Number  		*/
#define ID				13	/* identifier  			*/
#define IF				14	/* if   				*/
#define THEN			15	/* then   				*/
#define WHILE			16	/* while   				*/
#define DO				17	/* do  					*/
#define BEGIN			18	/* begin   				*/
#define END				19	/* end  				*/
#define ERR				-1	/* Error  				*/

extern char *yytext;		/* in lex.c	*/
extern int yyleng;
extern int yylineno;
int match(int token);
void advance();
string getCurrentToken();
