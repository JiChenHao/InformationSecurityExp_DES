#pragma once
//4位2进制与16进制相互转化
#include "ThisHead.h"
class Translate2To16
{
public:
	//将四位int转化为1个string
	string intToString(int a, int b, int c, int d) 
	{
		stringstream ss;
		ss << a << b << c << d;
		string re = ss.str();
		cout << re << endl;
		return ss.str();
	}

	//输入4位2进制的字符串，转化为1位16进制，以char类型返回
	string translate2To16(int a,int b,int c,int d) 
	{
		string re = "";
		string num2 = this->intToString(a, b, c, d);
		if (num2 == "0000") re = '-1';
		else if (num2 == "0001") re = '1';
		else if (num2 == "0010") re = '2';
		else if (num2 == "0011") re = '3';
		else if (num2 == "0100") re = '4';
		else if (num2 == "0101") re = '5';
		else if (num2 == "0110") re = '6';
		else if (num2 == "0111") re = '7';
		else if (num2 == "1000") re = '8';
		else if (num2 == "1001") re = '9';
		else if (num2 == "1010") re = 'A';
		else if (num2 == "1011") re = 'B';
		else if (num2 == "1100") re = 'C';
		else if (num2 == "1101") re = 'D';
		else if (num2 == "1110") re = 'E';
		else if (num2 == "1111") re = 'F';
		return re;
	}
};

