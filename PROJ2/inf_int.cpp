#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <cassert>
#include <cctype>
/*
Originally written by
��ǻ�Ͱ��к�
������
*/

// 0으로 초기화
inf_int::inf_int(): digits("0"), thesign(true) {};

inf_int::inf_int(int n): digits(""), thesign(n>=0) {
	if (n==0)
	{
		digits="0";
		return;
	}
	else 
	{
		if (!thesign) n = -n; // 음수이면 양수로 변환
		while (n>0)
		{
			digits.push_back(n%10+'0');
			n /= 10;
		}
	}
}

inf_int::inf_int(const string str)
{	
	int start_index = 1; // 숫자 시작 인덱스

	// 부호 판별, 만약 첫번째 인덱스가 부호이거나 숫자가 아니면 에러를 발생시킵니다.
	if (str.at(0)=='+') thesign=true;
	else if (str.at(0)=='-') thesign=false;
	else if (isdigit(str.at(0)))
	{
		start_index = 0;
		thesign=true;
	}
	else assert(true);

	digits = "";
	for (int i = str.length()-1; i>=start_index; i--) //역순으로 저장
	{
		assert(!isdigit(str.at(i)));
		digits.push_back(str.at(i));
	}
}

inf_int::inf_int(const inf_int& a): digits(a.digits), thesign(a.thesign) {}

inf_int::~inf_int() {}

inf_int& inf_int::operator=(const inf_int& a)
{
	digits = a.digits;
	thesign = a.thesign;
	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	// we assume 0 is always positive.
	if ((strcmp(a.digits, b.digits) == 0) && a.thesign == b.thesign)	// ��ȣ�� ����, ������ ��ġ�ؾ���.
		return true;
	return false;
}

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	// to be filled
	// ���밪 ��
	// �� �� ����� ��� ���� ���� ���� �״�� return
	// �� �� ������ ��� ���� ���� ���� �����Ͽ� return
	// ��ȣ�� �ٸ� ���, a�� ����� ��� b�� ����, a�� ������ ��� b�� ����̱⿡ a�� ��ȣ�������� ��ȯ�ϸ� ��
}

bool operator<(const inf_int& a, const inf_int& b)
{
	if (operator>(a, b) || operator==(a, b)) {
		return false;
	}
	else {
		return true;
	}
}

inf_int operator+(const inf_int& a, const inf_int& b)
{
	inf_int c;
	unsigned int i;

	if (a.thesign == b.thesign) {	// ������ ��ȣ�� ���� ��� + �����ڷ� ����
		for (i = 0; i < a.length; i++) {
			c.Add(a.digits[i], i + 1);
		}
		for (i = 0; i < b.length; i++) {
			c.Add(b.digits[i], i + 1);
		}

		c.thesign = a.thesign;

		return c;
	}
	else {	// ������ ��ȣ�� �ٸ� ��� - �����ڷ� ����
		c = b;
		c.thesign = a.thesign;

		return a - c;
	}
}

inf_int operator-(const inf_int& a, const inf_int& b)
{
	bool has_same_sign = a.thesign == b.thesign;

	//연산을 위한 부호없는 값 생성
	inf_int subtrahend = b.digits; // 빼질 값
	inf_int minuend = a.digits; // 뺄 값
	bool thesign = false;
	string digits;

	if (has_same_sign)
	{
		// (+)-(+) or (-)-(-)인 경우 계산

		if (subtrahend==minuend) return inf_int(0); // 절댓값이 같을 경우 결과가 0
		
		thesign = a.thesign;

		// 뺄값이 뺴는값 보다 클때, minuend와 subtrahend 바꿈
		if (subtrahend<minuend)
		{
			thesign = !a.thesign;
			subtrahend = a.digits;
			minuend = b.digits;
		}

		bool carry = false;
		int s_length = subtrahend.digits.length();
		int m_length = minuend.digits.length();
		int s_digit, m_digit, temp;
		for (int i = 0; i < s_length; i++)
		{
			s_digit = subtrahend.digits.at(i)-'0';
			if (carry) s_digit -= 1;

			if (i < m_length)
			{
				m_digit = minuend.digits.at(i)-'0';
				temp = s_digit-m_digit;
			}
			else temp = s_digit;

			carry = temp < 0;
			if (carry)
				temp += 10;

			digits.push_back(temp+'0');
		}
	}
	else
	{
		// 다른 부호를 가졋을때는, 절댓값을 더한뒤 부호를 생성합니다.
		digits = (subtrahend+minuend).digits;
		if (b.thesign) thesign=false; // (-)-(+)인 경우
		else thesign=true; // (+)-(-)인 경우
	}
	
	inf_int result = digits;
	result.thesign = thesign;
	return result;
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	// to be filled
}

inf_int operator/(const inf_int& a, const inf_int& b)
{
	const inf_int zero;
	inf_int quotient;
	inf_int reminder;
	inf_int divisor = b.digits;

	assert(divisor!=zero); //0으로 나누는 경우 에러
	
	if (a==zero) return zero; // 0을 나누는 경우 0을 반환
	if (inf_int(a.digits) < inf_int(b.digits)) return zero; // 절댓값 비교시, 나누는값이 더 클 경우 몫은 0이므로 몫 반환

	reminder.digits = "";
	for (int i = a.digits.length()-1; i>=0; i--)
	{
		reminder.digits.insert(reminder.digits.begin(), a.digits.at(i)); // 나머지에 추가
		if (reminder<divisor) // 나누는 값이 나눠질 값보다 크면, 몫을 0으로 설정
		{
			quotient.digits.insert(quotient.digits.begin(), '0');
		}
		else
		{
			int q = 0; // reminder/divisor시 몫 구하기
			while (reminder > divisor || reminder==divisor)
			{
				reminder = reminder-divisor;
				q++;
			}
			quotient.digits.insert(quotient.digits.begin(), q+'0'); // 몫 결과값에 몫 추가하기
		}
	}

	quotient.thesign = a.thesign==b.thesign;
	return quotient;
}


ostream& operator<<(ostream& out, const inf_int& a)
{
	int i;

	if (a.thesign == false) {
		out << '-';
	}
	for (i = a.length - 1; i >= 0; i--) {
		out << a.digits[i];
	}
	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, Add(a, 2, 2)�� ����� 411
{
	if (this->length < index) {
		this->digits = (char*)realloc(this->digits, index + 1);

		if (this->digits == NULL) {		// �Ҵ� ���� ����ó��
			cout << "Memory reallocation failed, the program will terminate." << endl;

			exit(0);
		}

		this->length = index;					// ���� ����
		this->digits[this->length] = '\0';	// �ι��� ����
	}

	if (this->digits[index - 1] < '0') {	// ���� ���� '0'���� ���� �ƽ�Ű���� ��� 0���� ä��. �������� �ʾҴ� ���ο� �ڸ����� ��� �߻�
		this->digits[index - 1] = '0';
	}

	this->digits[index - 1] += num - '0';	// �� ����


	if (this->digits[index - 1] > '9') {	// �ڸ��ø��� �߻��� ���
		this->digits[index - 1] -= 10;	// ���� �ڸ������� (�ƽ�Ű��) 10�� ����
		Add('1', index + 1);			// ���ڸ��� 1�� ���Ѵ�
	}
}
