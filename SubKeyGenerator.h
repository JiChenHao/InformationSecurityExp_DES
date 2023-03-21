#pragma once
//子密钥生成器
#include "ThisHead.h"
#include "ReadData.h"
class SubKeyGenerator
{
public:
	map<int,int> sourceKey;		//原始密钥
	map<int, int> C0, D0;		//原始密钥生成的原始子密钥
	map<int,map<int, int>> CDk;	//存放16轮的中间Ck和Dk
	map<int,map<int,int>> subKeys;	//存放子密钥
	string sourceKey_PC1;	//经过pc1IP置换之后的密钥
	ReadData *reader;		//执行文件操作的对象

	/*
	位置变换器（IP置换）PC-1
	*/	
	void changePosition1() 
	{
		
		//读入置换选择规则PC1
		map<int, int>pc1 = reader->getDataofnumInt("PC1.txt");
		cout << "=====读入PC1成功=====" << endl;
		cout << "=====开始生成C0和D0=====" << endl;
		//按照PC1移位生成C0和D0
		map<int, int>::iterator pc1It = pc1.begin();
		for (; pc1It != pc1.end(); pc1It++)
		{
			int count = pc1It->first;
			int bitPosition = pc1It->second;
			if (count <= 28)
			{
				this->C0[count] = this->sourceKey[bitPosition];
				cout << "C0--" << count << "--" << this->sourceKey[bitPosition] << endl;
			}
			else {
				int countD0 = count - 28;
				this->D0[countD0] = this->sourceKey[bitPosition];
				cout << "D0--" << countD0 << "--" << this->sourceKey[bitPosition] << endl;
			}
		}
		cout << "-----C0和D0生成结束-----" << endl;
	}


	/*
	通过将Ck和Dk进行PC2的置换选择得到每一轮的内容
	*/
	void generateSubKey() 
	{
		for (int i = 1; i <= 16; i++)
		{
			map<int, int>::iterator C0It = this->C0.begin();
			int temp = -1;
		}
	}

	/*
	根据位移次数表实现循环位移的函数
	输入是位移次数表的文件名以及当前轮数
	将第k轮的Ck和Dk放在一个map中，存放到CDk中
	前28位时Ck，后28位是Dk
	*/
	void changePositionOfSubKey(string ruleOfPositionChange)
	{
		//为了实现循环左移，将C0和D0复制一遍，加到原来的后面，然后要循环左移时只需将需求字段向右移动即可
		map<int, int>::iterator C0It = C0.begin();
		map<int, int>::iterator D0It = D0.begin();
		cout << "======将C0和D0翻倍=====" << endl;
		for (int i = 1;i<=28;i++)
		{
			C0[i + 28] = C0[i];
			D0[i + 28] = D0[i];
		}
		cout << "-----C0和D0翻倍结束-----" << endl;
		/*for (; C0It != C0.end(); C0It++, D0It++)
		{
			cout << "----C0----" << C0It->first << "----val----" << C0It->second<<endl;
			cout << "----D0----" << D0It->first << "****val****" << D0It->second<<endl;
		}*/
		//读入循环左移的规则表
		map<int,int> rulePC = reader->getDataofnumInt(ruleOfPositionChange);
		//计算当前轮数应该进行的左移的位数
		for (int lunS = 1; lunS <= 16; lunS++)
		{
			int sum = 0;//当前轮数下，应该左移的位数
			for (int i = 1; i <= lunS; i++)
			{
				sum += rulePC[i];
			}
			//得到Ck(re的前28位)和Dk（re的后28位）
			map<int, int>re;
			for (int i = 1; i <= 28; i++)
			{
				re[i] = C0[i + sum];
				re[i + 28] = D0[i + sum];
			}
			//存放re
			this->CDk[lunS] = re;
			//输出re
			cout << "====第" << lunS << "轮的Ck和Dk====" << endl;
			map<int, int>::iterator reIt = re.begin();
			for (; reIt != re.end(); reIt++)
			{
				cout << "编号：" << reIt->first << " 内容：" << reIt->second << endl;
			}
			cout << "-------re结束--------" << endl;
		}
	}


	/*
	位置变换器（IP置换）PC-2，生成16轮的子密钥并且存放在subK当中
	*/
	void changePosition2()
	{
		//读入置换选择规则PC2
		map<int, int>pc2 = reader->getDataofnumInt("PC2.txt");
		
		for (int lunS = 1; lunS <= 16; lunS++)
		{
			cout << "=====读入PC2成功=====" << endl;
			cout << "=====第" << lunS << "轮subK生成开始=====" << endl;
			map<int, int> tempCDK = this->CDk[lunS];//取出当前轮数的Ck和Dk
			map<int, int>subK;//临时存放一轮的子密钥
			//按照PC2移位生成subK
			map<int, int>::iterator pc2It = pc2.begin();
			for (; pc2It != pc2.end(); pc2It++)
			{
				int count = pc2It->first;
				int bitPosition = pc2It->second;

				subK[count] = tempCDK[bitPosition];
				//cout << "subK--" << count << "--" << subK[count] << endl;
			}
			this->subKeys[lunS] = subK;
			cout << "-----第" << lunS << "轮subK生成结束-----" << endl;
		}
	}


	/*
	构造函数
	*/
	SubKeyGenerator(map<int,int> sourceKey) 
	{
		this->sourceKey = sourceKey;
		this->reader = new ReadData();
		this->changePosition1();
		this->changePositionOfSubKey("ruleOfPositionChange.txt");
		this->changePosition2();
		//generateSubKey();
	}
};

