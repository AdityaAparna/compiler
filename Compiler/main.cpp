#include "code_gen.h"
#include <cstdio>
#include <iostream>
#include "names.h"
using namespace std;
bool error = false;

int main ()
{	
	initializeRegisterMap();
	stmt_list ();
	return !error;
}
