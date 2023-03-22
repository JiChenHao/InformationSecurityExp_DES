#pragma once

//过程显示类，对于加密过程的中间输出，转化成16进制显示到控制台
class Show16
{
public:
	/*
	map<int,int>型的2进制转化为map<int,string>型的16进制
	输入：--一个map2进制
	输出：--相应的16进制map
	*/
	map<int, string> changeMap2ToMap16(map<int, int> map2)
	{
		map<int, string> re;
		string map16Ele = "";	//16进制map的一个元素
		int count16 = 0;		//16进制map的计步器 = count2 / 4
		map<int, int>::iterator it2 = map2.begin();
		for (; it2 != map2.end();)
		{
			
			count16++;
			int a=-1, b=-1, c=-1, d=-1;//用来存放每四位的2进制
			//获得连续的4位2进制
			a = it2->second;
			it2++;
			b = it2->second;
			it2++;
			c = it2->second;
			it2++;
			d = it2->second;
			it2++;
			//判断四位2进制对应的16进制数
			if (a == 0 && b == 0 && c == 0 && d == 0)map16Ele = "0";		//0000
			else if (a == 0 && b == 0 && c == 0 && d == 1)map16Ele = "1";	//0001
			else if (a == 0 && b == 0 && c == 1 && d == 0)map16Ele = "2";	//0010
			else if (a == 0 && b == 0 && c == 1 && d == 1)map16Ele = "3";	//0011
			else if (a == 0 && b == 1 && c == 0 && d == 0)map16Ele = "4";	//0100
			else if (a == 0 && b == 1 && c == 0 && d == 1)map16Ele = "5";	//0101
			else if (a == 0 && b == 1 && c == 1 && d == 0)map16Ele = "6";	//0110
			else if (a == 0 && b == 1 && c == 1 && d == 1)map16Ele = "7";	//0111
			else if (a == 1 && b == 0 && c == 0 && d == 0)map16Ele = "8";	//1000
			else if (a == 1 && b == 0 && c == 0 && d == 1)map16Ele = "9";	//1001
			else if (a == 1 && b == 0 && c == 1 && d == 0)map16Ele = "A";	//1010
			else if (a == 1 && b == 0 && c == 1 && d == 1)map16Ele = "B";	//1011
			else if (a == 1 && b == 1 && c == 0 && d == 0)map16Ele = "C";	//1100
			else if (a == 1 && b == 1 && c == 0 && d == 1)map16Ele = "D";	//1101
			else if (a == 1 && b == 1 && c == 1 && d == 0)map16Ele = "E";	//1110
			else if (a == 1 && b == 1 && c == 1 && d == 1)map16Ele = "F";	//1111

			//将16进制元素放入map16中
			re[count16] = map16Ele;
		}
		return re;

	}
};

