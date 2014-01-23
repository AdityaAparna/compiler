#include <stdio.h>
#include "lex.h"

char    *factor(void);
char    *term(void);
char    *expr(void);
char * equality(void);
char * cmp(void);

extern char *newname(void);
extern void freename(char *name);
int legal_lookahead(int  first_arg, ...);
stmt_list()
{
	/*
	*  stmt_list -> stmt stmt_list | epsilon
	*/

	while (!match(EOI))
	{

		stmt();
		//setLookaheadfalse();
	}
}

stmt()
{
	/*  stmt -> ID ASSIGN expr SEMI | IF expr THEN stmt_list | WHILE expr DO stmt_list | BEGIN stmt_list END | expr SEMI */

	char *tempvar;

	if (!legal_lookahead(NUM, ID, IF, WHILE, BEGIN, LP, 0))
		return;

	if (match(ID))
	{
		advance();
		if (match(ASSIGN)){
			advance();
			expr();
			if (!match(SEMI)){
				fprintf(stderr, "Missing Semicolon at line no. %d\n", yylineno);
			}
			else{
				advance();
			}
		}
		else{
			fprintf(stderr, " Missing Assignment Operator at line no. %d\n", yylineno);

		}
	}
	else if (match(IF))
	{
		advance();
		expr();
		if (match(THEN)){
			advance();
			stmt();
		}
		else{
			fprintf(stderr, " Missing THEN at line no. %d\n", yylineno);
		}
	}
	else if (match(WHILE))
	{
		advance();
		expr();
		if (match(DO)){
			advance();
			stmt();
		}
		else{
			fprintf(stderr, " Missing DO at line no %d\n", yylineno);
		}
	}
	else if (match(BEGIN))
	{
		advance();
		while (1)
		{
			if (match(END))
			{
				advance();
				break;
			}
			if (match(EOI))
			{
				fprintf(stderr, " Missing END at line no %d\n", yylineno);
				break;
			}
			stmt();
		}
		//stmt_list();

		/*if(match(END)){
		advance();
		}else{
		fprintf(stderr, " Missing END at line no %d\n",yylineno);
		}*/
	}
	else
	{
		expr();
		if (!match(SEMI)){
			fprintf(stderr, "Missing Semicolon at line no. %d\n", yylineno);
		}
		else{
			advance();
		}
	}
}

char    *expr()
{
	/* expr -> equality expr'
	* expr' -> EQ equality expr' |  epsilon
	*/

	char  *tempvar = NULL, *tempvar2;

	equality();

	while (match(EQ)){
		advance();
		equality();
	}


	return tempvar;
}

char *equality()
{
	/*
	* equality -> cmp equality'
	* equality'-> LESS cmp equality' | GREATER cmp equality' | epsilon
	*/
	char *tempvar = NULL
		;
	cmp();

	while (match(LESS) || match(GREATER)){
		advance();
		cmp();
	}

	return tempvar;

}

char *cmp()
{
	/*
	* cmp -> term cmp'
	* cmp'-> PLUS term cmp' | MINUS term cmp' | epsilon
	*/
	char *tempvar = NULL;
	term();

	while (match(PLUS) || match(MINUS)){
		advance();
		term();
	}

	return tempvar;

}

char    *term()
{
	/*
	* term -> factor term'
	* term'-> MUL factor term' | DIV factor term' | epsilon
	*/

	char  *tempvar = NULL, *tempvar2;

	tempvar = factor();
	while (match(MUL) || match(DIV))
	{
		advance();
		factor();
	}

	return tempvar;
}

char    *factor()
{
	/*
	* factor -> NUM | ID | LP expr RP
	*/

	char *tempvar = NULL;

	if (match(NUM) || match(ID))
	{
		/* Print the assignment instruction. The %0.*s conversion is a form of
		* %X.Ys, where X is the field width and Y is the maximum number of
		* characters that will be printed (even if the string is longer). I'm
		* using the %0.*s to print the string because it's not \0 terminated.
		* The field has a default width of 0, but it will grow the size needed
		* to print the string. The ".*" tells printf() to take the maximum-
		* number-of-characters count from the next argument (yyleng).
		*/

		// printf("    %s = %0.*s\n", tempvar = newname(), yyleng, yytext );
		advance();
	}
	else if (match(LP))
	{
		advance();
		tempvar = expr();
		if (match(RP))
			advance();
		else
			fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno);
	}
	else
		fprintf(stderr, "%d: Number or identifier expected\n", yylineno);

	return tempvar;
}


#include <stdarg.h>

#define MAXFIRST 16
#define SYNCH    SEMI

int legal_lookahead(int  first_arg, ...)
{
	/* Simple error detection and recovery. Arguments are a 0-terminated list of
	* those tokens that can legitimately come next in the input. If the list is
	* empty, the end of file must come next. Print an error message if
	* necessary. Error recovery is performed by discarding all input symbols
	* until one that's in the input list is found
	*
	* Return true if there's no error or if we recovered from the error,
	* false if we can't recover.
	*/

	va_list     args;
	int     tok;
	int     lookaheads[MAXFIRST], *p = lookaheads, *current;
	int     error_printed = 0;
	int     rval = 0;

	va_start(args, first_arg);

	if (!first_arg)
	{
		if (match(EOI))
			rval = 1;
	}
	else
	{
		*p++ = first_arg;
		while ((tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST])
			*p++ = tok;

		while (!match(SYNCH))
		{
			for (current = lookaheads; current < p; ++current)
			if (match(*current))
			{
				rval = 1;
				goto exit;
			}

			if (!error_printed)
			{
				fprintf(stderr, "Line %d: Syntax error\n", yylineno);
				error_printed = 1;
			}

			advance();
		}
	}

exit:
	va_end(args);
	if (error_printed != 1 && rval == 0)
	{
		fprintf(stderr, "Line %d: Syntax error\n", yylineno);
		advance();
	}
	return rval;
}
