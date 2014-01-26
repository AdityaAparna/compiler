#include "code_gen.h"
#include "label.h"
#include <iostream>
#include <sstream>
using namespace std;
#define PR(X) cout<< #X <<" = "<<X<<endl;
#define DBG 1
map<string, bool> idDef;
string datasec = "org 100h\nJMP start\n";
stringstream codesec;

void stmt_list()
{
    /*
    *  stmt_list -> stmt stmt_list | epsilon
    */
    
    while(!match(EOI))
    {
		
        stmt();
		//setLookaheadfalse();
    }
	if (!error)
	{
		cout << datasec << "\nstart:" <<endl;
		cout << codesec.str() << endl << "RET" << endl;
	}
}

void stmt()
{
    /*  stmt -> ID ASSIGN expr SEMI | IF expr THEN stmt_list | WHILE expr DO stmt_list | BEGIN stmt_list END | expr SEMI */

    if( !legal_lookahead( NUM, ID, IF, WHILE, BEGIN, LP, 0 ) )
        return;

    if (match(ID))
    {
		string idname = getCurrentToken();

		if (idDef[idname] == false){
			datasec += idname + " DD 0\n";
			idDef[idname] = true;
		}
        advance();
        if(match(ASSIGN)){
            advance();
			string tempreg = expr();
			//codesec << idname << "=" << tempreg << endl;
			codesec << "MOV " << idname << "," << tempreg << endl;
			freereg(tempreg);
            if(!match(SEMI)){
                fprintf(stderr, "Missing Semicolon at line no.%d\n",yylineno);
				error = true;
			}
			else{
				advance();
			}
        }else{
            fprintf(stderr, "Missing Assignment Operator at line no.%d\n",yylineno);
			error = true;
        }
    }
    else if(match(IF))
    {
        advance();
		freereg(expr());
        if(match(THEN)){
			int hereIndex = labelIndex-1;
            advance();
            stmt();
			codesec << "\n" << "Label" << hereIndex << ":" << endl;
        }else{
            fprintf(stderr, "Missing THEN at line no.%d\n",yylineno);
			error = true;
        }
    }
    else if(match(WHILE))
    {
        advance();
		genLabel();
		int hereIndex = labelIndex - 1;
		codesec << "\n" << "Label" << hereIndex  << ":" << endl;
        freereg(expr());
		
        if(match(DO)){
            advance();
            stmt();
			codesec << "JMP Label" << hereIndex << endl;
			codesec << "\n" << "Label" << hereIndex + 1 << ":" << endl;

        }else{
            fprintf(stderr, "Missing DO at line no.%d\n",yylineno);
			error = true;
        }
    }
    else if(match(BEGIN))
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
				fprintf(stderr, "Missing END at line no.%d\n", yylineno);
				error = true;
				break;
			}
			stmt();
		}
        
    }
    else 
    {
        expr();
        if(!match(SEMI)){
            fprintf(stderr, "Missing Semicolon at line no.%d\n", yylineno);
			error = true;
		}
		else{
			advance();
		}
    }    
}

string expr()
{
    /* expr -> equality expr'
     * expr' -> EQ equality expr' |  epsilon
     */

    string tempvar1, tempvar2;
	bool flag = true;
    tempvar1 = equality();

    while(match(EQ)){
        advance();
		tempvar2 = equality();
		//codesec << regNew << "= comparision " << tempvar1 << "==" << tempvar2 << endl;
		codesec << "CMP " << tempvar1 << " , " << tempvar2 << endl;
		if (flag)
			codesec << "JNZ " << genLabel() << endl;
		else
			codesec << "JNZ " << retLabel() << endl;
		
		freereg(tempvar1);
		tempvar1 = tempvar2;
    }

    return tempvar1;
}

string equality()
{
    /*
    * equality -> cmp equality'
    * equality'-> LESS cmp equality' | GREATER cmp equality' | epsilon
    */
	string tempvar1, tempvar2;
	bool flag = true;

    tempvar1 = cmp();
	int less = match(LESS);
	int greater = match(GREATER);
    while(less || greater){
        advance();
        tempvar2 = cmp();
		
		if (less){
			//codesec << regNew << " = result of " << tempvar1 << "<" << tempvar2 << endl;
			codesec << "CMP " << tempvar1 << " , " << tempvar2 << endl;
			/*
				mov var,4
				cmp var,3

				JNS FURTHER

        
				FURTHER:

				if(var<3)
				{}
			*/
			if (flag)
				codesec << "JNS "<<genLabel() << endl; 
			else
				codesec << "JNS " << retLabel() << endl;

		}
		else{
			//codesec << regNew << " = result of " << tempvar1 << ">" << tempvar2 << endl;
			codesec << "CMP " << tempvar1 << " , " << tempvar2 << endl;
			if (flag)
				codesec << "JS " << genLabel() << endl;
			else
				codesec << "JS " << retLabel() << endl;
		}
		freereg(tempvar1);
		tempvar1 = tempvar2;
		flag = false;
		less = match(LESS);
		greater = match(GREATER);
    }
	
    return tempvar1;
    
}

string cmp()
{
    /*
    * cmp -> term cmp'
    * cmp'-> PLUS term cmp' | MINUS term cmp' | epsilon
    */
    string tempvar1,tempvar2;
    tempvar1 = term();
	int plus = match(PLUS); int minus = match(MINUS);
    while(plus || minus){
        advance();
        tempvar2 = term();
		//string regNew = newreg();
		if (plus){
			//codesec << regNew << " =" << tempvar1 << "+" << tempvar2 << endl;
			codesec << "ADD " << tempvar1 << " , " << tempvar2 << endl;
		}
		else{
			codesec << "SUB " << tempvar1 << " , " << tempvar2 << endl;
			//codesec << regNew << " =" << tempvar1 << "-" << tempvar2 << endl;
		}
		//freereg(tempvar1);
		freereg(tempvar2);
		//tempvar1 = regNew;

		plus = match(PLUS); 
		minus = match(MINUS);
    }
    
    return tempvar1;
    
}

string term()
{
    /*
    * term -> factor term'
    * term'-> MUL factor term' | DIV factor term' | epsilon
    */

    string tempvar1, tempvar2 ;
	tempvar1 = factor();
	int mul = match(MUL);
	int div = match(DIV);
    while( mul || div )
    {
        advance();
        tempvar2 = factor();
		if (mul){
			codesec << "MUL " << tempvar1 << " , " << tempvar2 << endl;
		}
		else{
			codesec << "MOV ax , " << tempvar1 << endl;
			codesec << "MOV dx , " << "0" << endl;
			codesec << "DIV " << tempvar2 << endl;
			codesec << "MOV " << tempvar1 << " , " << "ax" << endl;
		}
		freereg(tempvar2);

		mul = match(MUL);
		div = match(DIV);
    }

    return tempvar1;
}

string factor()
{
    /*
    * factor -> NUM | ID | LP expr RP
    */
    
    string tempvar = "";

    if( match(NUM) || match(ID) )
    {
	/* Print the assignment instruction. The %0.*s conversion is a form of
	 * %X.Ys, where X is the field width and Y is the maximum number of
	 * characters that will be printed (even if the string is longer). I'm
	 * using the %0.*s to print the string because it's not \0 terminated.
	 * The field has a default width of 0, but it will grow the size needed
	 * to print the string. The ".*" tells printf() to take the maximum-
	 * number-of-characters count from the next argument (yyleng).
	 */
		if (match(NUM)){
			string tok = getCurrentToken();
			string regi = newreg();
			
			//codesec << regi << " = " << tok << endl;
			codesec << "MOV " << regi << " , " << tok << endl;
			advance();
			return regi;
		}
		else{
			// match(ID) case
			string curId = getCurrentToken();

			if (idDef.find(curId) == idDef.end()){
				// Add to datasec about curId declaration
				datasec += curId+" DD 0 \n";

				idDef[curId] = true;
				
			}

			string reg = newreg();
			//codesec << reg << "=" << curId << endl;
			codesec << "MOV " << reg << " , " << curId << endl;
			advance();
			return reg;
			
		}

    }
    else if( match(LP) )
    {
        advance();
        tempvar = expr();
		if (match(RP))
		{
			advance();
			return tempvar;
		}
		else
		{
			fprintf(stderr, "Mismatched parenthesis at line no.%d\n", yylineno);
			error = true;
		}
            
    }
	else
	{
		fprintf(stderr, "Number or identifier expected at line no.%d\n", yylineno);
		error = true;
	}

    return tempvar;
}


#include <stdarg.h>

#define MAXFIRST 16
#define SYNCH    SEMI

int legal_lookahead(int  first_arg ,...)
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
    int     rval          = 0;

    va_start( args, first_arg );

    if( !first_arg )
    {
    if( match(EOI) )
        rval = 1;
    }
    else
    {
    *p++ = first_arg;
    while( (tok = va_arg(args, int)) && p < &lookaheads[MAXFIRST] )
        *p++ = tok;

    while( !match( SYNCH ) )
    {
        for( current = lookaheads; current < p ; ++current )
        if( match( *current ) )
        {
            rval = 1;
            goto exit;
        }

        if( !error_printed )
        {
			fprintf( stderr, "Syntax error at line no.%d\n", yylineno );
			error = true; error_printed = 1;
        }

        advance();
    }
    }

exit:
    va_end( args );
	if (error_printed != 1 && rval == 0)
	{
		fprintf(stderr, "Syntax error at line no.%d\n", yylineno);
		error = true;
		advance();
	}
    return rval;
}
