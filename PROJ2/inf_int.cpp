#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <cassert>
#include <cctype>
#include <deque>

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
		start_index = str.length()-1; //문자열이 모두 0일 때

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
	// 부호와 자릿수가 같지 않으면 바로 false를 반환
	if (a.thesign != b.thesign || a.digits.length() != b.digits.length()) {
		return false;
	}

	// 모든 자리수가 동일한지 확인
	for (int i = 0; i < a.digits.length(); i++) {
		if (a.digits[i] != b.digits[i]) {
			return false;
		}
	}

	return true; // 모든 조건을 만족하면 true 반환
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
	if (a==b) return inf_int(0); // 두 수가 같으면 0 반환

	inf_int subtrahend = a, minuend = b;
	subtrahend.thesign = true; minuend.thesign = true;

	if (a.thesign != b.thesign) // 부호가 다르면 더하기
	{
		inf_int result = subtrahend + minuend;
		result.thesign = a.thesign;
		return result;
	}
	
	if (subtrahend < minuend) // 작은값에서 큰값을 빼는 경우
	{
		inf_int result = minuend.karatsuba_subtract(subtrahend);
		result.thesign = !a.thesign;
		return result;
	}

    return subtrahend.karatsuba_subtract(minuend);
}

inf_int inf_int::karatsuba_subtract(const inf_int& other) const
{
    if (digits.length() <= 1 || other.digits.length() <= 1) return simple_subtract(other);

    int max_len = std::max(digits.length(), other.digits.length());
    std::string subtrahend_digits = this->digits; // 빼질 값
    subtrahend_digits.insert(0, max_len - subtrahend_digits.length(), '0'); // 자릿수 맞추기
    std::string minuend_digits = other.digits; // 뺄 값
    minuend_digits.insert(0, max_len - minuend_digits.length(), '0'); // 자릿수 맞추기

    int split_pos = max_len / 2;
    inf_int s_left(subtrahend_digits.substr(0, split_pos));
    inf_int s_right(subtrahend_digits.substr(split_pos));
    inf_int m_left(minuend_digits.substr(0, split_pos));
    inf_int m_right(minuend_digits.substr(split_pos));

    inf_int left_result = s_left.karatsuba_subtract(m_left);
	if (!left_result.thesign)
	{
		s_right = s_right.karatsuba_subtract(inf_int(1)); // 부호 처리 문제
		left_result.thesign = true;
	}
    inf_int right_result = s_right.karatsuba_subtract(m_right);

	int left_padding = m_left.digits.length()-left_result.digits.length();
	string result_digits = left_result.digits + string(left_padding, '0') + right_result.digits;
	reverse(result_digits.begin(), result_digits.end());
    inf_int result(result_digits);
	result.thesign = s_right.thesign ? right_result.thesign: false; // 부호처리 
    return result;
}

inf_int inf_int::simple_subtract(const inf_int& other) const
{
	if (*this == other) return inf_int(0); // 동일한 숫자이면 0반환

    inf_int subtrahend = *this;
    subtrahend.thesign = true;
    inf_int minuend = other;
    minuend.thesign = true;

    int s_digit, m_digit, temp;
    std::string digits;
    bool is_carry = false;

    for (size_t i = 0; i < subtrahend.digits.length(); i++)
    {
        s_digit = subtrahend.digits.at(i) - '0';
        m_digit = (i < minuend.digits.length() ? minuend.digits.at(i) - '0' : 0);

        if (is_carry) s_digit -= 1;

        temp = s_digit - m_digit;
        is_carry = temp < 0;
        if (is_carry) temp += 10;

        digits.push_back(temp + '0');
    } 
	while (digits.length() > 1 && digits.back() == '0') {
		digits.pop_back();
	}
    std::reverse(digits.begin(), digits.end());
    inf_int result(digits);
    result.thesign = !is_carry;
    return result;
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	if (a.digits.length() < 32 && b.digits.length() < 32) {
		return a.simple_multiply(b);
	}
	else {
		return a.karatsuba_multiply(b);
	}
}

inf_int inf_int::simple_multiply(const inf_int& other) const {

	unsigned char length = digits.length() + other.digits.length();
	inf_int c;
	c.digits.resize(length, '0'); // 결과의 자리수를 위한 배열 초기화

	// 두 숫자의 곱셈
	for (int i = 0; i < digits.length(); i++) {
		char multiplier = digits[i] - '0'; // char to int
		char carry = 0;

		for (int j = 0; j < other.digits.length(); j++) {
			char digit = (other.digits[j] - '0') * multiplier + carry + (c.digits[i + j] - '0');

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
			c.digits[i + other.digits.length()] += carry; // 결과의 다음 자리수에 추가
		}
	}

	// 부호 결정
	c.thesign = (thesign == other.thesign) ? true : false; // 두 수의 부호가 다르면 결과는 음수

	// 결과에서 불필요한 앞자리 0 제거
	while (c.digits.length() > 1 && c.digits.back() == '0') {
		c.digits.pop_back();
	}

	return c;
}

inf_int inf_int::karatsuba_multiply(const inf_int& other) const {

	if (digits.length() == 0 || other.digits.length() == 0) return inf_int("0");
	if (digits.length() == 1 || other.digits.length() == 1) return simple_multiply(other);

	int m = max(digits.length(), other.digits.length());
	int half = m / 2;

	inf_int a0(digits.substr(0, half));
	inf_int a1(digits.substr(half));
	inf_int b0(other.digits.substr(0, half));
	inf_int b1(other.digits.substr(half));

	inf_int z0 = a0.karatsuba_multiply(b0); // a0 * b0
	inf_int z1 = (a0 + a1).karatsuba_multiply(b0 + b1); // (a0 + a1) * (b0 + b1)
	inf_int z2 = a1.karatsuba_multiply(b1); // a1 * b1

	inf_int result = z0 + ((z1 - z0 - z2).digits.append(half, '0') + (z2.digits.append(2 * half, 0)));

	result.thesign = (thesign == other.thesign); // 부호 결정

	return result;
}

inf_int operator/(const inf_int& a, const inf_int& b)
{
	const inf_int zero;
	inf_int reminder = a;
	reminder.thesign = true;
	inf_int divisor = b;
	divisor.thesign = true;
	assert(divisor!=zero); //0으로 나누는 경우 에러
	
	if (a==zero) return zero; // 0/b 인경우 0 반환
	if (reminder < divisor) return zero; // 절댓값 비교시, 나누는값이 더 클 경우 몫은 0이므로 몫 반환

	reminder.digits = "";
	string quotient_digits;
	for (int i = a.digits.length()-1; i>=0; i--)
	{
		reminder.digits.insert(reminder.digits.begin(), a.digits.at(i)); // 나머지에 추가
		if (reminder<divisor) // 나누는 값이 나눠질 값보다 크면, 몫을 0으로 설정
		{
			quotient_digits.push_back('0');
		}
		else
		{
			int q = 0; // reminder/divisor시 몫 구하기
			while (reminder > divisor || reminder==divisor)
			{
				reminder = reminder-divisor;
				q++;
			}
			quotient_digits.push_back(q+'0'); // 몫 결과값에 몫 추가하기
		}
	}

	inf_int quotient = quotient_digits;
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
