#include "names.h"
#include "code_gen.h"
#include "cstdlib"
using namespace std;

vector<string> registerNames;
map <string, bool> reginuse;


void initializeRegisterMap(){

	char temp[9][4] = { "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "r16" };
	for (int i = 0; i < 9; i++)
	{
		registerNames.push_back(temp[i]);
		reginuse[registerNames[i]] = false;
	}
}
string newreg(){
	string res;
	for (int i = 0; i < registerNames.size(); i++)
	{
		if (reginuse[registerNames[i]] == false){
			reginuse[registerNames[i]] = true;

		return registerNames[i];
		}
	}
	fprintf(stderr, "Expression too complex at line no.%d", yylineno);
	error = true;
	exit(EXIT_FAILURE);
}

void freereg(string regName){
	if (reginuse.find(regName) == reginuse.end()){

		cout << "Trying to free an unknown register named --" << regName << endl;
	}
	else{
		reginuse[regName] = false;
	}
}


