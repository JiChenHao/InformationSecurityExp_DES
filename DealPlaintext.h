#pragma once
#include "ReadData.h"
#include "SBox.h"
#include "Show16.h"


//处理明文的类
class DealPlaintext
{
public:
	map<int,map<int, int>> plainTexts;//原始明文,每一个都是64位,构造函数直接初始化
	SubKeyGenerator *subKeyGnt;//子密钥生成器
	map<int, map<int, int>> cipherTexts;//加密后的密文，每一个都是64位，与原始明文分别对应
	ReadData* reader;
	Show16 *show16 = new Show16();
	/*
	无参构造函数，通过明文所放置的文件，得到明文的二进制内容
	*/
	DealPlaintext()
	{
		this->subKeyGnt = new SubKeyGenerator();
		this->reader = new ReadData();
		map<int,int> pts = reader->getCharToNum2("Plaintext.txt", "PlaintextNum2.txt");//明文的所有位，有多个64位，需要拆分，最后一组不足64位补零
		map<int, int>::iterator pIT1 = pts.begin();
		//先将明文长度补足到64的倍数
		//cout << "=====将明文补足到64的整数倍" << endl;
		int PTEND = pts.size();
		while (1)
		{
			PTEND++;
			pts[PTEND] = 0;
			if (PTEND % 64 == 0)break;
		}
		//cout << "=====将明文分割成若干个64位一组" << endl;
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
		//使用16进制将各个分组的明文输出
		cout << "=====明文分组结果如下=====" << endl;
		map<int, map<int, int>>::iterator mingWenIt = this->plainTexts.begin();
		for (; mingWenIt != this->plainTexts.end(); mingWenIt++)
		{
			map<int, string> mingWen16 = show16->changeMap2ToMap16(mingWenIt->second);

			cout << "---分组编号--- " << mingWenIt->first <<"	--- 分组内容--- ";
			map<int, string>::iterator mingWen16It = mingWen16.begin();
			for (; mingWen16It != mingWen16.end(); mingWen16It++)
			{
				cout << mingWen16It->second;
			}
			cout << endl;
		}
		cout << "-----明文分组内容显示结束-----" << endl;
		

		//对明文进行加密，初始化密文字段
		this->DESall();


		//用16进制输出所有的密文
		cout << "=====加密结果如下==	";
		map<int, map<int, int>>::iterator CPTit= this->cipherTexts.begin();
		for (; CPTit != this->cipherTexts.end(); CPTit++)
		{
			map<int, string> CPT16 = this->show16->changeMap2ToMap16(CPTit->second);
			map<int, string>::iterator CPT16IT = CPT16.begin();
			int countCP = 0;
			for (; CPT16IT != CPT16.end(); CPT16IT++)
			{
				cout << CPT16IT->second;
				//每四位空一格
				countCP++;
				if (countCP == 4)
				{
					countCP = 0;
					cout << " ";
				}
			}
		}
		cout << " 加密结果输出结束==" << endl;



	}
	/*
	初始置换IP
	输入：	一个64位的原始明文
	输出：	L0和R0（都是32位的map）
	*/
	map<int,map<int, int>> IPChange(map<int,int> pt)
	{
		map<int,map<int, int>> LR0;//存放两个map的数组，1位是L0，2位是R0
		//读入置换选择规则IPChange
		map<int, int>IPChangeRule = reader->getDataofnumInt("IPChange.txt");
		//cout << "=====读入IPChange成功=====" << endl;
		
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
		//cout << "-----L0和R0生成完成-----" << endl;
		cout << "=====初始置换IP结果=====";
		//按照16进制展示输出
		for (int i = 1; i <= 2; i++)
		{
			map<int, string> LR016 = this->show16->changeMap2ToMap16(LR0[i]);
			map<int, string>::iterator LR016It = LR016.begin();
			for (; LR016It != LR016.end(); LR016It++)
			{
				cout << LR016It->second;
			}
		}
		cout << "-----初始置换IP结果结束-----" << endl;
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
		//cout << "=====读入EExtension成功=====" << endl;
		//cout << "=====开始进行E扩展=====" << endl;
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
		}
		return C;
	}

	/*
	P置换
	输入：--S盒变换产生的C（32位）
	输出：--P(C)   (32位)
	*/
	map<int, int> PZhiHuan(map<int,int>C)
	{
		map<int, int>re;
		//读入置换选择规则PZhiHuan
		map<int, int>PZHRule = reader->getDataofnumInt("PZhiHuan.txt");
		//cout << "=====读入PZhiHuan成功=====" << endl;
		//cout << "=====开始进行P置换=====" << endl;
		map<int, int>::iterator Pit = PZHRule.begin();
		for (; Pit != PZHRule.end(); Pit++)
		{
			int count = Pit->first;		//P置换表中的位置
			int bitPosition = Pit->second;	//规则对应位置的数字
			re[count] = C[bitPosition];	//生成E扩展的结果
		}
		return re;
	}



	/*
	F函数，由多个函数封装起来，简化操作
	输入：--32位Ri和48位子密钥Ki
	输出：--32位结果（与Li异或运算以前）
	*/
	map<int, int> FHanShu(map<int, int>R, map<int, int>subK)
	{
		map<int, int>re;
		//首先对R进行E扩展
		map<int, int>EER = this->EExtension(R);
		//然后E扩展结果与子密钥进行异或运算,得到B
		map<int, int>B = this->yiHuoYunSuan(EER, subK);
		//B进行S盒置换得到C
		map<int, int>C = this->doSBoxChange(B);
		//C进行P置换得到F函数的结果
		re = this->PZhiHuan(C);
		return re;
	}



	/*
	L和F函数结果的异或运算
	输入：--L0和F（R0，K1）  （两个32位2进制）
	输出：--R1
	*/
	map<int, int> LFYiHuo(map<int, int>L0, map<int, int>Fre)
	{
		map<int, int>R1;
		for (int i = 1; i <= 32; i++)
		{
			if (L0[i] == Fre[i])R1[i] = 0;
			else R1[i] = 1;
		}
		return R1;
	}

	/*
	定义一轮的加密运算
	由F函数和LF异或运算组成
	输入：--L0和R0（2个32位map，用一个map集成起来的，1是L0，2是R0）以及一个密钥subK
	输出：--L1和R1（2个32位map，用一个map集成起来的，1是L1，2是R2）
	*/
	map<int,map<int, int>> OneDES(map<int,map<int, int>>LR0, map<int, int>subK)
	{
		map<int,map<int, int>>LR1;
		map<int, int>R0 = LR0[2];
		map<int, int>L0 = LR0[1];
		map<int, int>FR0K = this->FHanShu(R0, subK);
		//异或运算的结果就是R1
		map<int, int>R1 = this->LFYiHuo(L0, FR0K);
		map<int, int>L1 = R0;//R0原封不动传递给L1
		LR1[1] = L1;
		LR1[2] = R1;
		return LR1;
	}
	/*
	IP的逆置换
	输入：--16轮加密之后的64位2进制密文LR16
	输出：--最终的64位密文(一个map，形式与分组后的明文相同)
	*/
	map<int, int> IPNiZhiHuan(map<int, map<int, int>> LR16)
	{
		map<int, int>re;
		//首先将LR16的L16和R16合并成一段64位的密文
		map<int, int>MIWEN16;
		int count = 1;
		map<int, int>::iterator Lit = LR16[1].begin();
		map<int, int>::iterator Rit = LR16[2].begin();
		for (; Lit != LR16[1].end(); Lit++, Rit++)
		{
			MIWEN16[count] = Lit->second;
			MIWEN16[count + 32] = Rit->second;
			count++;
		}
		//读入置换选择规则IPNiZhiHuan
		map<int, int>IPNiRule = reader->getDataofnumInt("IPNiZhiHuan.txt");
		//cout << "=====读入IPNiZhiHuan成功=====" << endl;
		//cout << "=====开始进行IP逆置换=====" << endl;
		map<int, int>::iterator Pit = IPNiRule.begin();
		for (; Pit != IPNiRule.end(); Pit++)
		{
			int count1 = Pit->first;		//P置换表中的位置
			int bitPosition = Pit->second;	//规则对应位置的数字
			re[count1] = MIWEN16[bitPosition];	//生成E扩展的结果
		}
		//以16进制输出IP逆置换的结果
		cout << "=====IP逆置换的结果：--";
		map<int, string> re16 = this->show16->changeMap2ToMap16(re);
		map<int, string>::iterator re16It = re16.begin();
		for (; re16It != re16.end(); re16It++)
		{
			cout << re16It->second;
		}
		cout << "--IP逆置换结束" << endl;
		return re;
	}




	/*
	定义一个64位分组明文的16轮加密运算
	输入：--一个64位的明文
	输出：--一个64位的密文
	*/
	map<int, int> DES16(map<int, int> plainText)
	{
		map<int, int>cipherText;//一组加密后的密文

		//第一步，IP置换，获取LR0
		map<int, map<int, int>> LR0 = this->IPChange(plainText);
		//获取16轮的子密钥
		map<int, map<int, int>> subKeys = subKeyGnt->subKeys;
		//开始16轮加密
		map<int, map<int, int>> tempL0R0 = LR0;//一轮的输入
		map<int, map<int, int>> tempL1R1;//一轮的结果
		for (int i = 1; i <= 16; i++)
		{
			int lunS = i;//当前正在加密的轮数
			tempL1R1 = OneDES(tempL0R0, subKeys[lunS]);
			//更新输入
			tempL0R0 = tempL1R1;
			//输出一轮迭代的结果
			cout << "==迭代轮数：--	" << i << "	--迭代结果==";
			map<int, map<int, int>>::iterator tempLRIt = tempL1R1.begin();
			for (; tempLRIt != tempL1R1.end(); tempLRIt++)
			{
				map<int, string> tempLR16 = this->show16->changeMap2ToMap16(tempLRIt->second);
				map<int, string>::iterator tempLR16It = tempLR16.begin();
				for (; tempLR16It != tempLR16.end(); tempLR16It++)
				{
					cout << tempLR16It->second;
				}
			}
			cout << "==本轮迭代结束==" << endl;
		}
		//循环结束后的tempLR就是LR反转的LR16，因为最后一轮的结果是L16R16,而IP逆置换的输入则是R16L16
		//因此要将16轮的结果前后32为调换位置才能输入到IP逆置换中得到正确的结果
		map<int, map<int, int>> reverseTempLR;
		reverseTempLR[2] = tempL1R1[1];
		reverseTempLR[1] = tempL1R1[2];

		cout << "=最后一轮LR反转之后的RL的16进制结果=";
		map<int, map<int, int>>::iterator ciptIT = reverseTempLR.begin();
		for (; ciptIT != reverseTempLR.end(); ciptIT++)
		{
			map<int, string> reverseTempLR16 = this->show16->changeMap2ToMap16(ciptIT->second);
			map<int, string>::iterator ititit = reverseTempLR16.begin();
			for (; ititit != reverseTempLR16.end(); ititit++)
			{
				cout << ititit->second;
			}
		}
		cout << "=反转LR输出结束" << endl;

		cipherText = this->IPNiZhiHuan(reverseTempLR);//IP逆置换
		
		
		return cipherText;
	}

	/*
	将所有的明文分组批量加密
	输入：--直接调用类中的分好组的明文
	输出：--直接将类中的密文赋值
	*/
	void DESall()
	{
		int i = 1;
		map<int, map<int, int>>::iterator it = this->plainTexts.begin();
		while (it != this->plainTexts.end())
		{
			this->cipherTexts[i] = this->DES16(it->second);
			it++;
		}
	}
};

