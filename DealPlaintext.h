#pragma once
#include "ReadData.h"
#include "SBox.h"


//处理明文的类
class DealPlaintext
{
public:
	map<int,map<int, int>> plainTexts;//原始明文,每一个都是64位
	map<int, map<int, int>> cipherTexts;//加密后的密文，每一个都是64位，与原始明文分别对应
	ReadData* reader;
	/*
	无参构造函数，通过明文所放置的文件，得到明文的二进制内容
	*/
	DealPlaintext()
	{
		this->reader = new ReadData();
		map<int,int> pts = reader->getCharToNum2("Plaintext.txt", "PlaintextNum2.txt");//明文的所有位，有多个64位，需要拆分，最后一组不足64位补零
		map<int, int>::iterator pIT1 = pts.begin();
		//先将明文长度补足到64的倍数
		cout << "=====将明文补足到64的整数倍" << endl;
		int PTEND = pts.size();
		while (1)
		{
			PTEND++;
			pts[PTEND] = 0;
			if (PTEND % 64 == 0)break;
		}
		cout << "=====将明文分割成若干个64位一组" << endl;
		for (; pIT1 != pts.end(); pIT1++)
		{
			int count = pIT1->first;
			int zushu = 0;//当前分组序号
			int zuNeiXu = 0;//当前分组的组内序号
			if (count % 64 != 0)
			{
				zushu = count / 64 + 1;
				zuNeiXu = count - (zushu-1) * 64;
			}
			else {
				zushu = count / 64;
				zuNeiXu = 64;
			}
			this->plainTexts[zushu][zuNeiXu] = pIT1->second;
		}

	}
	/*
	初始置换IP
	输入：	一个64位的原始明文
	输出：	L0和R0（都是32位的map）
	*/
	map<int,map<int, int>> IPChange(map<int,int> pt)
	{
		map<int,map<int, int>> LR0;//存放两个map的数组，0位是L0，1位是R0
		//读入置换选择规则IPChange
		map<int, int>IPChangeRule = reader->getDataofnumInt("IPChange.txt");
		cout << "=====读入IPChange成功=====" << endl;
		cout << "=====开始生成L0和R0=====" << endl;
		map<int, int>::iterator ipcrIT = IPChangeRule.begin();
		for (; ipcrIT != IPChangeRule.end(); ipcrIT++)
		{	
			int count = ipcrIT->first;			//规则计步器
			int bitPosition = ipcrIT->second;	//按照该规则在该位置应该有的二进制数在明文中的位置

			if (count <= 32)
			{//L0
				LR0[1][count] = pt[bitPosition];
				//cout << "L0--KEY--"<<count << "----" <<LR0[0][count]<< endl;
			}
			else {
				//R0
				LR0[2][count - 32] = pt[bitPosition];
				//cout << "R0--KEY--" << count-32 <<"----" << LR0[1][count - 32] << endl;
			}
		}
		cout << "-----L0和R0生成完成-----" << endl;
		return LR0;
	}

	/*
	E扩展功能，E扩展的表在文件EExtension.txt中
	输入一个32位的map
	输出为48位的map
	*/
	map<int, int> EExtension(map<int, int> input)
	{
		map<int, int>re;
		//读入置换选择规则EExtension
		map<int, int>EERule = reader->getDataofnumInt("EExtension.txt");
		cout << "=====读入EExtension成功=====" << endl;
		cout << "=====开始进行E扩展=====" << endl;
		map<int, int>::iterator ERIT = EERule.begin();
		for (; ERIT != EERule.end(); ERIT++)
		{
			int count = ERIT->first;		//EERule表中的位置
			int bitPosition = ERIT->second;	//规则对应位置的数字
			re[count] = input[bitPosition];	//生成E扩展的结果
		}
		return re;
	}

	/*
	实现两个map的异或运算（相同为0，不同为1）
	输入：--48位的E扩展结果，还有一个48位的子密钥
	输出：--48位异或运算结果B（一个map每六比特分成一组，分别进行S盒运算）
	*/
	map<int, int> yiHuoYunSuan(map<int, int> EERe, map<int, int>subK)
	{
		map<int, int>YHre;//异或运算的结果
		for (int i = 1; i <= 48; i++)
		{
			if (EERe[i] == subK[i])YHre[i] = 0;
			else YHre[i] = 1;
		}
		return YHre;
	}

	/*
	S盒变换
	输入：--B（一个map<int,int>）48位2进制
	输出：--C，32位2进制
	*/
	map<int, int> doSBoxChange(map<int, int> B)
	{
		SBoxs* sBoxs = new SBoxs();
		map<int, int> C;//总的C，8个Ck的整合版
		//一共有8个s盒所以循环8次，得到8个Ck
		for (int i = 1; i <= 8; i++)
		{
			//按照顺序获得Bk,一个B是6位
			map<int, int>Bk;
			int Bkbegin = 6 * (i - 1)+1;//Bk在B中的起点位置
			int Bkend = Bkbegin + 5;
			for (int j = Bkbegin,x = 1; j <= Bkend; j++,x++)
			{
				Bk[x] = B[j];
			}
			//调用S盒获得单个的Ck
			map<int, int> Ck;
			Ck = sBoxs->BinSCout(Bk,i);
			//将得到的Ck合并入C中
			int Ckbegin = 4 * (i - 1) + 1;
			int Ckend = Ckbegin + 3;
			for (int j = Ckbegin, x = 1; j <= Ckend; j++, x++)
			{
				C[j] = Ck[x];
			}
			return C;
		}
	}


};

