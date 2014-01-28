#include "lex.h"
#include "code_gen.h"
using namespace std;

char* yytext = ""; /* Lexeme (not '\0'
                      terminated)              */
int yyleng   = 0;  /* Lexeme length.           */
int yylineno = 0;  /* Input line number        */

int forwardParse()
{
  
  char lexeme[1025];
  //copying the current lexeme into char array for comparison
  int i;
  for (i = 0; i < yyleng; ++i)
  {
    lexeme[i]=*(yytext+i);
  }
  lexeme[i]='\0';

  if(strcmp(lexeme,"if")==0) return IF;
  if(strcmp(lexeme,"then")==0) return THEN;
  if(strcmp(lexeme,"while")==0) return WHILE;
  if(strcmp(lexeme,"do")==0) return DO;
  if(strcmp(lexeme,"begin")==0) return BEGIN;
  if(strcmp(lexeme,"end")==0) return END;

  if(isalpha(*yytext)) return ID;

  //now first char must be a digit. Following loops verifies
  //whether all chars are digits or not.
  i=0;
  for (i = 0; i < yyleng; ++i)
  {
    if(!isdigit(lexeme[i]))
    {
      fprintf(stderr, "%s at line no:%d is not a valid identifier\n",lexeme, yylineno );
	  error = true;
      return ERR;
    }
  }
  return NUM;

} 
int lex(void){

  static char input_buffer[1024];
  char        *current;

  current = yytext + yyleng; /* Skip current
                                 lexeme        */

  while(1){       /* Get the next one         */
    while(!*current ){
         /* Get new lines, skipping any leading
         * white space on the line,
         * until a nonblank line is found.
         */

      current = input_buffer;
      if(!gets(input_buffer)){
        *current = '\0' ;

        return EOI;
      }
      ++yylineno;
      while(isspace(*current))
        ++current;
    }
    for(; *current; ++current){
       /* Get the next token */
      yytext = current;
      yyleng = 1;
      switch( *current ){
        case ';':
          return SEMI;
        case '+':
          return PLUS;
        case '-':
          return MINUS;
        case '*':
          return MUL;
        case '/':
          return DIV;
        case '(':
          return LP;
        case ')':
          return RP;
        case '<':
          return LESS;
        case '>':
          return GREATER;
        case '=':
			if ((current + 1) <= &input_buffer[1023] && *(current + 1) == '=')
			{
				yyleng = 2;
				return EQ;
			}
          else
            return ASSIGN;
		case '\n':
        case '\t':
        case ' ' :
          break;
// NUM, ID, IF, THEN, WHILE, DO, BEGIN, END remaining
        default:
			if (!isalnum(*current) && (*current) != '_')
			{
				fprintf(stderr, "Illegal character <%c> at line no.%d \n", *current, yylineno);
				error = true;
			}
          else{
            while(isalnum(*current) || (*current)=='_')
              ++current;
            yyleng = current - yytext;

            int tempRet = forwardParse();
            if (tempRet!=-1)
              return tempRet;
          }
        break;
      }
    }
  }
}


static int Lookahead = -1; /* Lookahead token  */

int match(int token){
   /* Return true if "token" matches the
      current lookahead symbol.                */


	if(Lookahead == -1)
  Lookahead = lex();

return token == Lookahead;
}

void advance(void){
/* Advance the lookahead to the next
   input symbol.                               */

  Lookahead = lex();
}

void clearThisLine(void){
	Lookahead = -1; 
	*yytext = '\0';
	yyleng = 0;
}

string getCurrentToken(){
	return string(yytext, yytext + yyleng);
}