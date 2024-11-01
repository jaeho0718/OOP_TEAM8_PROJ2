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
	unsigned int start_index = 1; // 숫자 시작 인덱스

	// 부호 판별, 만약 첫번째 인덱스가 부호이거나 숫자가 아니면 에러를 발생시킵니다.
	if (str.at(0)=='+') thesign=true;
	else if (str.at(0)=='-') thesign=false;
	else if (isdigit(str.at(0)))
	{
		start_index = 0;
		thesign=true;
	}
	else assert(true);

	// 0으로 문자열이 시작하는 경우 첫번째로 0이 아닌 값이 나오는 인덱스를 계산합니다.
	size_t non_zero_start_pos = str.find_first_not_of('0', start_index);
	if (non_zero_start_pos != string::npos)
		start_index = non_zero_start_pos;
	else
		start_index = str.length()-1;

	digits = "";

	for (int i = str.length()-1; i >= start_index; i--) //역순으로 저장
	{

		if (i < 0) break;

		assert(isdigit(str.at(i)));
		digits.push_back(str.at(i));
	}
}

inf_int::inf_int(const inf_int& a): digits(a.digits), thesign(a.thesign) {}

inf_int::~inf_int() {
}

inf_int& inf_int::operator=(const inf_int& a)
{
	digits = a.digits;
	thesign = a.thesign;
	return *this;
}

bool operator==(const inf_int& a, const inf_int& b)
{
	if (a.thesign == b.thesign && a.digits.length() == b.digits.length()) {
		for (int i = 0; i < a.digits.length(); i++) {
			if (a.digits[i] != b.digits[i]) {
				return false;
			}
		}
	}
	return true;;
} 

bool operator!=(const inf_int& a, const inf_int& b)
{
	return !operator==(a, b);
}

bool operator>(const inf_int& a, const inf_int& b)
{
	// 부호가 같은 경우
	if (a.thesign == b.thesign) {
		// 자릿수가 같은 경우, 각 자리의 숫자를 비교
		if (a.digits.length() == b.digits.length()) {
			for (int i = a.digits.length() - 1; i >= 0; i--) { // 역순으로 비교
				if (a.digits[i] > b.digits[i]) return a.thesign;
				if (a.digits[i] < b.digits[i]) return !a.thesign;
			}
			return false; // 모든 자리가 같으면 두 수가 같음
		}
		else {
			// 자릿수가 다르면 양수일 경우 자릿수가 큰 쪽이 더 큼
			if (a.thesign) {
				return a.digits.length() > b.digits.length();
			}
			// 음수일 경우 자릿수가 작은 쪽이 더 큼
			else {
				return a.digits.length() < b.digits.length();
			}
		}
	}
	else {
		// 부호가 다르면 양수가 더 큼
		return a.thesign;
	}
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

	if (a.thesign != b.thesign) {
		return a - b;
	}

	inf_int c;
	unsigned int length;
	char carry = '0';

	// 최대 자리수만큼 초기화
	length = max(a.digits.length(), b.digits.length());
	c.digits.resize(length); // 필요한 길이만큼 문자열 배열 초기화
	c.thesign = a.thesign;

	for (int i = 0; i < length; i++) {
		// 각 자리의 숫자를 추출
		char a_digit = (i < a.digits.length()) ? a.digits[i] : '0';
		char b_digit = (i < b.digits.length()) ? b.digits[i] : '0';

		// 두 자릿수와 carry를 더해 sum을 계산
		char sum = (a_digit - '0') + (b_digit - '0') + (carry - '0') + '0';

		// 10 이상의 경우, carry 처리
		if (sum > '9') {
			sum -= 10;
			carry = '1';
		}
		else {
			carry = '0';
		}

		// 결과 자리수를 c.digits에 저장
		c.digits[i] = sum;
	}

	// 마지막 캐리가 남아 있으면 추가
	if (carry == '1') {
		c.digits.push_back('1');
	}

	return c;
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
	unsigned char length = a.digits.length() + b.digits.length();
	inf_int c;
	c.digits.resize(length, '0'); // 결과의 자리수를 위한 배열 초기화

	// 두 숫자의 곱셈
	for (int i = 0; i < a.digits.length(); i++) {
		char multiplier = a.digits[i] - '0'; // char to int
		char carry = 0;

		for (int j = 0; j < b.digits.length(); j++) {
			char digit = (b.digits[j] - '0') * multiplier + carry + (c.digits[i + j] - '0');

			// 자리수의 누적
			if (digit >= 10) {
				carry = digit / 10; // carry 계산
				digit = digit % 10;  // 현재 자리수
			}
			else {
				carry = 0; // carry 초기화
			}

			c.digits[i + j] = digit + '0'; // 결과에 현재 자리수 저장
		}

		// 남아있는 carry를 다음 자리수에 추가
		if (carry != 0) {
			c.digits[i + b.digits.length()] += carry; // 결과의 다음 자리수에 추가
		}
	}

	// 부호 결정
	c.thesign = (a.thesign == b.thesign) ? true : false; // 두 수의 부호가 다르면 결과는 음수

	// 결과에서 불필요한 앞자리 0 제거
	while (c.digits.length() > 1 && c.digits.back() == '0') {
		c.digits.pop_back();
	}

	return c;
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
	for (i = a.digits.length() - 1; i >= 0; i--) {
		out << a.digits[i];
	}

	return out;
}

void inf_int::Add(const char num, const unsigned int index)	// a�� index �ڸ����� n�� ���Ѵ�. 0<=n<=9, ex) a�� 391�϶�, Add(a, 2, 2)�� ����� 411
{
	/*
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
	}*/
}
