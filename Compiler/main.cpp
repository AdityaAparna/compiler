#include "code_gen.h"
#include <cstdio>
#include <iostream>
using namespace std;
bool error = false;

int main ()
{	
	initializeRegisterMap();
	stmt_list ();
	return 0;
}
