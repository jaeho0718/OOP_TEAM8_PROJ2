#define _CRT_SECURE_NO_WARNINGS
#include "inf_int.h"
#include <cassert>
#include <cctype>
#include <deque>
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
		start_index = str.length()-1; //문자열이 모두 0일 때

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
    inf_int result;
    if (a < b)
    {
        result = b.karatsuba_subtract(a);
        result.thesign = false;
    }
    else
    {
        result = a.karatsuba_subtract(b);
        result.thesign = true;
    }
    return result;
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
    inf_int right_result = s_right.karatsuba_subtract(m_right);

    if (!right_result.thesign) // carry 발생시
    {
        left_result = left_result.karatsuba_subtract(inf_int(1));
        right_result.thesign = true;
    }

    inf_int result;
    result.digits = left_result.digits + std::string(max_len - split_pos, '0') + right_result.digits;
    return result;
}

inf_int inf_int::simple_subtract(const inf_int& other) const
{
    inf_int subtrahend = digits; // 빼질 값
    inf_int minuend = other.digits; // 뺄 값

    if (subtrahend < minuend) // 작은값 - 큰값
    {
        inf_int result = minuend.simple_subtract(subtrahend);
        result.thesign = false;
        return result;
    }

    int s_digit, m_digit, temp;
    std::string digits;
    bool is_carry = false; // carry 초기화
    for (size_t i = 0; i < subtrahend.digits.length(); i++)
    {
        s_digit = subtrahend.digits.at(i) - '0';
        m_digit = (i < minuend.digits.length() ? minuend.digits.at(i) - '0' : 0);

        if (is_carry) s_digit -= 1;

        temp = s_digit - m_digit;
        is_carry = temp < 0;
        if (is_carry)
        {
            temp += 10;
        }

        digits.push_back(temp + '0');
    }

    // 결과 앞의 불필요한 0 제거
    while (digits.length() > 1 && digits.back() == '0') {
        digits.pop_back();
    }

    std::reverse(digits.begin(), digits.end());
    return inf_int(digits);
}

inf_int operator*(const inf_int& a, const inf_int& b)
{
	// to be filled
}

inf_int operator/(const inf_int& a, const inf_int& b)
{
	const inf_int zero;
	inf_int reminder;
	inf_int divisor = b.digits;

	assert(divisor!=zero); //0으로 나누는 경우 에러
	
	if (a==zero) return zero; // 0을 나누는 경우 0을 반환
	if (inf_int(a.digits) < inf_int(b.digits)) return zero; // 절댓값 비교시, 나누는값이 더 클 경우 몫은 0이므로 몫 반환

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
