#ifndef _INF_INT_H_
#define _INF_INT_H_

#include <iostream>
#include <string>

using namespace std;

class inf_int
{
private:
    // 숫자를 역순으로 바꿔 저장합니다. 12345 -> 54321
    string digits;
    bool thesign;   // true if positive , false if negative.
    // ex) 15311111111111111 -> digits="11111111111111351", length=17, thesign=true;
    // ex) -12345555555555 -> digits="55555555554321", length=14, thesign=false
    // You may modify or add private members of inf_int class. So, it is OK to insert Add() private member function in inf_int class. However, You may not modify public members of inf_int class.

public:
    inf_int(); // assign 0 as a default value
    inf_int(int);
    inf_int(const string);
    inf_int(const inf_int&);
    ~inf_int(); // destructor

    inf_int& operator=(const inf_int&); // assignment operator

    friend bool operator==(const inf_int&, const inf_int&);
    friend bool operator!=(const inf_int&, const inf_int&);
    friend bool operator>(const inf_int&, const inf_int&);
    friend bool operator<(const inf_int&, const inf_int&);

    friend inf_int operator+(const inf_int&, const inf_int&);
    friend inf_int operator-(const inf_int&, const inf_int&); // 재호형
    friend inf_int operator*(const inf_int&, const inf_int&);
    friend inf_int operator/(const inf_int& , const inf_int&); // 재호형

    friend ostream& operator<<(ostream&, const inf_int&);
    // friend istream& operator>>(istream& , inf_int&);    // not required

    void Add(const char num, const unsigned int index);

private:
    // 간단한 빼기 연산입니다.
    inf_int simple_subtract(const inf_int&) const;
    //  두개의 절댓값을 뺍니다.
    inf_int karatsuba_subtract(const inf_int&) const;
};

#endif
