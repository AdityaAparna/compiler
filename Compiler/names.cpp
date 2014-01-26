#include "names.h"

using namespace std;

vector<string> registerNames = { "r1" , "r2", "r3" , "r4" , "r4" , "r5" , "r6" , "r7" , "r8" };
map <string, bool> reginuse;


void initializeRegisterMap(){
	for (int i = 0; i < registerNames.size(); i++)
	{
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
	
	//exit(EXIT_FAILURE);
}

void freereg(string regName){
	if (reginuse.find(regName) == reginuse.end()){

		cout << "Trying to free an unknown register named --" << regName << endl;
	}
	else{
		reginuse[regName] = false;
	}
}


