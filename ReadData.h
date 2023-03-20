#pragma once
#include "ThisHead.h"
#include <bitset>
using namespace std;
//从文件中读取数据的方法合集
class ReadData
{
public:
	/*
	=====d从文件中读取文字转化为单个字母，返回map<字母位置，字母>
	*/
	map<int, string> getDataofChar(string fileName)
	{
		ifstream ifile(fileName);
		map<int, string> keyMap;
		int count = 0;//编号，从1到64
		while (ifile.good())
		{
			string data;
			ifile >> data;
			int strSize = data.length();
			for (int i = 0; i < strSize; i++)
			{
				//只识别字母，其余丢弃
				if (data[i] >= 'a' && data[i] <= 'z' || data[i] >= 'A' && data[i] <= 'Z') 
				{
					count++;
					keyMap[count] = data[i];
				}
			}
			//int datai = stoi(data);

		}
		//输出内容
		/*map<int, string>::iterator mit = keyMap.begin();
		for (; mit != keyMap.end(); mit++)
		{
			cout << "键：" << mit->first << " 值：" << mit->second << endl;
		}*/
		return keyMap;
	}

	/*
	=====按照二进制读入，返回map
	*/
	map<int, int> getDataofnum2(string fileName)
	{
		ifstream ifile(fileName);
		map<int, int> keyMap;
		int count = 0;//编号，从1到64
		while (ifile.good())
		{
			string data;
			ifile >> data;
			int strSize = data.length();
			for (int i = 0; i < strSize; i++)
			{
				count++;
				int num = -1;
				if (data[i] == '0')num = 0;
				else if (data[i] == '1')num = 1;
				keyMap[count] = num;
			}
			//int datai = stoi(data);

		}
		//输出map的内容
		/*map<int, int>::iterator mit = keyMap.begin();
		for (; mit != keyMap.end(); mit++)
		{
			cout << "键：" << mit->first << " 值：" << mit->second << endl;
		}*/
		return keyMap;
	}

	/*
	=====读取文件中的字母，然后转化为二进制数字(八位二进制一个字母)，返回map<int,int>
	输入为字符文件，和保存二进制内容的文件
	*/
	map<int, int> getCharToNum2(string fileName,string fileNameNum2)
	{
		//先读入字母
		map<int, string> charMap = this->getDataofChar(fileName);
		//以此将每个字母转化为二进制，有序的保存在vector中
		vector<string> numVec;
		map<int, string>::iterator mit = charMap.begin();
		cout << "=====开始将字母转化为二进制=====" << endl;
		for (; mit != charMap.end(); mit++)
		{
			string binStr = this->translateStringToNum2(mit->second);
			cout << "原字母是：" << mit->second << " 二进制：" << binStr << endl;
			numVec.push_back(binStr);
		}
		cout << "-----字母转化结束-----" << endl;
		//写入目标文件
		ofstream writer;
		//写入文件之前先清空文件
		writer.open(fileNameNum2,ofstream::out);
		vector<string>::iterator vit = numVec.begin();
		string space = " ";
		//cout << "====开始写入文件====" << endl;
		for (; vit != numVec.end(); vit++)
		{
			writer << *vit<<space;
			//cout << "写入：" << *vit << endl;
		}
		writer.close();
		//cout << "----文件写入结束----" << endl;
		//cout << "====获取2进制文件开始====" << endl;
		map<int, int>re = this->getDataofnum2(fileNameNum2);
		//cout << "----获取2进制文件结束，返回结果----" << endl;
		return re;
	}

	/*
	=====将输入的字符串转化为对应的二进制
	*/
	string translateStringToNum2(string str1)
	{
		string tempStr = str1;
		int n = tempStr.length();
		string bin = "";	//存储二进制的字符串
		//for (int i = 0; i <= n; i++)
		//{
		//	int val = tempStr[i];
		//	while (val > 0)
		//	{//直接对val字符串进行ASCII码的十进制计算，每次除以二，按照结果向bin加上1或者0
		//		(val % 2) ? bin.push_back('1') : bin.push_back('0');
		//		val /= 2;
		//	}
		//	//按照二进制的计算，最后是反过来才对
		//	reverse(bin.begin(), bin.end());
		//	//cout << "2字母："<< tempStr[i] << "二进制：" << bin << " " << endl;
		//}

		//==更改，以保证8位2进制，不然只有7位
		for (int i = 0; i < n; i++)
		{
			bitset<8> bits = bitset<8>(tempStr[i]);
			//反转得到正确顺序的2进制
			for (int j = 7; j >= 0; j--)
			{
				(bits[j] == 1) ? bin.push_back('1') : bin.push_back('0');
			}
		}
		//==
		return bin;
	}

	/*
	=====按照十进制读入数字，返回map
	*/
	map<int, int> getDataofnumInt(string fileName)
	{
		ifstream ifile(fileName);
		map<int, int> keyMap;
		int count = 0;//编号,从1开始
		while (ifile.good())
		{
			string data;
			ifile >> data;
			stringstream ss;
			ss << data;
			int temp = 0;
			ss >> temp;
			int datai = stoi(data);
			count++;
			keyMap[count] = datai;
		}
		//输出map的内容
		/*map<int, int>::iterator mit = keyMap.begin();
		for (; mit != keyMap.end(); mit++)
		{
			cout << "键：" << mit->first << " 值：" << mit->second << endl;
		}*/
		return keyMap;
	}
};

