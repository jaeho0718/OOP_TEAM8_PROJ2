#include "inf_int.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;
W
int main()
{
	// 한자리수
	test_operator(1, 1, 0, EQUAL);
	test_operator(1, -2, 0, NOTEQUAL);
	test_operator(-2, 1, 0, SMALLER);
	test_operator(3, 1, 0, LARGER);
	test_operator(1, 2, 3, ADD);
	test_operator(2, 1, 1, SUBTRACT);
	test_operator(-1, 4, -5, SUBTRACT);
	test_operator(1, 2, -1, SUBTRACT);
	test_operator(3, 2, 6, MULTIPLY);
	test_operator(3, -2, -6, MULTIPLY);
	test_operator(3, 2, 1, DIVIDE);
	test_operator(3, -2, -1, DIVIDE);

	// 두자리수 and 한자리수
	test_operator(11, 11, 0, EQUAL);
	test_operator(11, 12, 0, NOTEQUAL);
	test_operator(1, 12, 0, SMALLER);
	test_operator(13, 1, 0, LARGER);
	test_operator(12, 2, 14, ADD);
	test_operator(12, 1, 11, SUBTRACT);
	test_operator(1, 12, -11, SUBTRACT);
	test_operator(13, 2, 26, MULTIPLY);
	test_operator(13, -2, -26, MULTIPLY);
	test_operator(13, 2, 6, DIVIDE);
	test_operator(13, -2, -6, DIVIDE);

	// 한자리 이상 수
	test_operator(inf_int("112311231231231"), inf_int("112311231231231"), 0, EQUAL);
	test_operator(inf_int("112311231231231"), inf_int("112311231231232"), 0, NOTEQUAL);
	test_operator(inf_int("112311231231231"), inf_int("112351231231231"), 0, SMALLER);
	test_operator(inf_int("112351231231231"), inf_int("112311231231231"), 0, LARGER);
	test_operator(inf_int("112351231231231"), inf_int("112311231231231"), inf_int("224662462462462"), ADD);
	test_operator(inf_int("12012"), inf_int("12334"), inf_int("-322"), SUBTRACT);
	test_operator(inf_int("59604"), inf_int("12345"), inf_int("47259"), SUBTRACT);
	test_operator(inf_int("12132931302198"), inf_int("224662462462465"), inf_int("-212529531160267"), SUBTRACT);
	test_operator(inf_int("12132931302198"), inf_int("12345"), inf_int("149781036925634310"), MULTIPLY);
	test_operator(inf_int("12132931302198"), inf_int("-12345"), inf_int("-149781036925634310"), MULTIPLY);
	test_operator(inf_int("12132931302198"), inf_int("12345"), inf_int("982821490"), DIVIDE);
	
	return 0;
}