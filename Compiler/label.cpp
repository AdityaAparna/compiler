#include "label.h"

static int labelIndex = 1;

string genLabel()
{
	stringstream temp;
	temp << labelIndex++;
	return "Label" + temp.str();
}

string retLabel()
{
	stringstream temp;
	temp << labelIndex-1;
	return "Label" + temp.str();
}