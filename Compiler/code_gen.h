#include <stdio.h>
#include "lex.h"
#include <iostream>
#include <map>
#include <string>
#include "names.h"
using namespace std;

void stmt_list();
void stmt();
string  factor(void);
string  term(void);
string  expr(void);
string  equality(void);
string  cmp(void);

extern char *newname(void);
extern void freename(char *name);
int legal_lookahead(int  first_arg, ...);
