#pragma once
#include "ReadData.h"
#include "ThisHead.h"

//这是S盒，从文件中读取S盒的内容，然后进行处理
class SBoxs
{
public:
	map<int, map<int, int>> sBoxs;//从文件中读取的S盒的内容
	ReadData* reader = new ReadData();
	/*
	无参构造函数，负责从txt文件中读取S盒，然后存放到类成员sBoxs中
	*/
	SBoxs()
	{
		this->reader =  new ReadData();
		sBoxs[1] = reader->getDataofnumInt("SBox1.txt");
		sBoxs[2] = reader->getDataofnumInt("SBox2.txt");
		sBoxs[3] = reader->getDataofnumInt("SBox3.txt");
		sBoxs[4] = reader->getDataofnumInt("SBox4.txt");
		sBoxs[5] = reader->getDataofnumInt("SBox5.txt");
		sBoxs[6] = reader->getDataofnumInt("SBox6.txt");
		sBoxs[7] = reader->getDataofnumInt("SBox7.txt");
		sBoxs[8] = reader->getDataofnumInt("SBox8.txt");

		//尝试输出sBoxs
		/*map<int, map<int, int>>::iterator it = sBoxs.begin();
		for (; it != sBoxs.end(); it++)
		{
			cout << "=====S盒的编号为：**" << it->first << "**======" << endl;
			map<int, int>::iterator it1 = it->second.begin();
			for (; it1 != it->second.end(); it1++)
			{
				cout << "KEY：--" << it1->first << "VAL--" << it1->second << endl;
			}
		}*/
	}
	/*
	S盒运算查表
	输入：--一个6比特的map<int,int> Bk,以及S盒的编号，注意Bk的编号是1到6
	输出：--一个来自于S盒的4位2进制map
	*/
	map<int,int> BinSCout(map<int,int> Bk,int SNum)
	{
		map<int, int> Ck;
		int sre = -1;//从S盒中找到的整数
		map<int, int> SB = this->sBoxs[SNum];//S盒
		int hangBiao = -1;	//行标
		int lieBiao = -1;	//列标
		//分解Bk，第一位和最后一位结合为行标，中间四位结合为列标
		int a = Bk[1];
		int b = Bk[2];
		int c = Bk[3];
		int d = Bk[4];
		int e = Bk[5];
		int f = Bk[6];
		//确定行标：
		if (a == 0 && f == 0)hangBiao = 0;		//00
		else if (a == 0 && f == 1)hangBiao = 1;	//01
		else if (a == 1 && f == 0)hangBiao = 2;	//10
		else if (a == 1 && f == 1)hangBiao = 3;	//11
		//确定列标
		if (b == 0 && c == 0 && d == 0 && e == 0)lieBiao = 0;		//0000
		else if (b == 0 && c == 0 && d == 0 && e == 0)lieBiao = 1;	//0001
		else if (b == 0 && c == 0 && d == 1 && e == 0)lieBiao = 2;	//0010
		else if (b == 0 && c == 0 && d == 1 && e == 1)lieBiao = 3;	//0011
		else if (b == 0 && c == 1 && d == 0 && e == 0)lieBiao = 4;	//0100
		else if (b == 0 && c == 1 && d == 0 && e == 1)lieBiao = 5;	//0101
		else if (b == 0 && c == 1 && d == 1 && e == 0)lieBiao = 6;	//0110
		else if (b == 0 && c == 1 && d == 1 && e == 1)lieBiao = 7;	//0111
		else if (b == 1 && c == 0 && d == 0 && e == 0)lieBiao = 8;	//1000
		else if (b == 1 && c == 0 && d == 0 && e == 1)lieBiao = 9;	//1001
		else if (b == 1 && c == 0 && d == 1 && e == 0)lieBiao = 10;	//1010
		else if (b == 1 && c == 0 && d == 1 && e == 1)lieBiao = 11;	//1011
		else if (b == 1 && c == 1 && d == 0 && e == 0)lieBiao = 12;	//1100
		else if (b == 1 && c == 1 && d == 0 && e == 1)lieBiao = 13;	//1101
		else if (b == 1 && c == 1 && d == 1 && e == 0)lieBiao = 14;	//1110
		else if (b == 1 && c == 1 && d == 1 && e == 1)lieBiao = 15;	//1111

		//按照行标和列标从S盒中取出对应的数字
		//我们的S盒按照1到64的序列存放，按16*行标+列标+1即可找到相对应的数字
		int numPosition = 16 * hangBiao + lieBiao + 1;
		sre = SB[numPosition];
		int a1 = -1, b1 = -1, c1 = -1, d1 = -1;//临时表示Ck的四个二进制数
		if (sre == 0)
		{
			a1 = 0, b1 = 0, c1 = 0, d1 = 0;//0000
		}else if (sre == 1)
		{
			a1 = 0, b1 = 0, c1 = 0, d1 = 1;//0001
		}
		else if (sre == 2)
		{
			a1 = 0, b1 = 0, c1 = 1, d1 = 0;//0010
		}
		else if (sre == 3)
		{
			a1 = 0, b1 = 0, c1 = 1, d1 = 1;//0011
		}
		else if (sre == 4)
		{
			a1 = 0, b1 = 1, c1 = 0, d1 = 0;//0100
		}
		else if (sre == 5)
		{
			a1 = 0, b1 = 1, c1 = 0, d1 = 1;//0101
		}
		else if (sre == 6)
		{
			a1 = 0, b1 = 1, c1 = 1, d1 = 0;//0110
		}
		else if (sre == 7)
		{
			a1 = 0, b1 = 1, c1 = 1, d1 = 1;//0111
		}
		else if (sre == 8)
		{
			a1 = 1, b1 = 0, c1 = 0, d1 = 0;//1000
		}
		else if (sre == 9)
		{
			a1 = 1, b1 = 0, c1 = 0, d1 = 1;//1001
		}
		else if (sre == 10)
		{
			a1 = 1, b1 = 0, c1 = 1, d1 = 0;//1010
		}
		else if (sre == 11)
		{
			a1 = 1, b1 = 0, c1 = 1, d1 = 1;//1011
		}
		else if (sre == 12)
		{
			a1 = 1, b1 = 1, c1 = 0, d1 = 0;//1100
		}
		else if (sre == 13)
		{
			a1 = 1, b1 = 1, c1 = 0, d1 = 1;//1101
		}
		else if (sre == 14)
		{
			a1 = 1, b1 = 1, c1 = 1, d1 = 0;//1110
		}
		else if (sre == 15)
		{
			a1 = 1, b1 = 1, c1 = 1, d1 = 1;//1111
		}
		//将四个二进制数按照顺序放入Ck
		Ck[1] = a1;
		Ck[2] = b1;
		Ck[3] = c1;
		Ck[4] = d1;
		return Ck;
	}
};