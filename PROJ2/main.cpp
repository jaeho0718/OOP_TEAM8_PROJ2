#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>

using namespace std;

int main()
{
	inf_int c("20003"); //321
	inf_int d("1"); //4321

	c = c * d;

	cout << c;

	return 0;
}