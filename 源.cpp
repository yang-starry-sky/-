#include<cstdio>
#include<iostream>
#include<fstream>
#include<cmath>
#define MAX_N 512//�洢������
#define MAX_M 32//ָ���Ϊһ���ֳ�
using namespace std;
static char Memory[MAX_N][MAX_M];//�洢��
static char Register[16][MAX_M];//ͨ�üĴ���
static char MAR[4];//��������ַ�Ĵ���
static char MDR[2][MAX_M];//�������Ĵ���
static char ACC[MAX_M];
static char Rx[MAX_M];//��ַ�Ĵ���
static char Rd[MAX_M];//��ַ�Ĵ���
static char IR[MAX_M];//ָ��Ĵ���
static char SP[MAX_M];//ջ����ַ�Ĵ���
static char CL[MAX_M];//��λ�Ĵ���
static int PC = 0;
static int address[2];
//ָ���ʽ��6 4 9 4 9
int Transform_1(char ch[], int length) //�����ƴ�תʮ������ֵ
{
	int decimal_number = 0;
	int i;
	int j = 0;
	for (i = length - 1; i >= 0; i--)
	{
		if (ch[i] == '1')
		{
			decimal_number = decimal_number + pow(2, j);
		}
		j++;
	}
	return decimal_number;
}

void Transform_2(int number, char ch[], int length)//ʮ������ת��Ϊ�����ƴ�
{
	length = length - 1;
	while (number != 0)
	{
		int remainder = number % 2;
		number = number / 2;
		if (remainder == 1)
		{
			ch[length] = '1';
		}
		length--;
	}
}

void Addressing(int distinguish)
{
	int addressing_mode = Transform_1(MAR, 4);
	int i, j;
	char temp[6];
	if (distinguish == 1)
	{
		for (i = 0; i < 6; i++)
		{
			temp[i] = Memory[PC][10 + i];
		}
	}
	else
	{
		for (i = 0; i < 6; i++)
		{
			temp[i] = Memory[PC][23 + i];
		}
	}
	switch (addressing_mode)
	{
	case 1://����Ѱַ
		for (i = 26; i < 32; i++)
		{
			MDR[distinguish - 1][i] = temp[i - 26];
		}
		break;
	case 2://ֱ��Ѱַ
		int value = Transform_1(temp, 6);
		address[distinguish - 1] = value;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Memory[value][i];
		}
		break;
	case 3://���Ѱַ
		int value_1 = Transform_1(temp, 6);
		int value_2 = Transform_1(Memory[value_1], 32);
		address[distinguish - 1] = value_2;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Memory[value_2][i];
		}
		break;
	case 4://�Ĵ���Ѱַ
		int value = Transform_1(temp, 6);
		address[distinguish - 1] = value;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Register[value][i];
		}
		break;
	case 5://�Ĵ������Ѱַ
		int value_1 = Transform_1(temp, 6);
		int value_2 = Transform_1(Register[value_1], 32);
		address[distinguish - 1] = value_2;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Memory[value_2][i];
		}
		break;
	case 6://����Ѱַ
		break;
	case 7://��ַѰַ
		int value_1 = Transform_1(Rd, 32);
		int value_2 = Transform_1(temp, 6);
		address[distinguish - 1] = value_2;
		value_2 = value_1 + value_2;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Memory[value_2][i];
		}
		break;
	case 8://��ַѰַ
		int value_1 = Transform_1(Rx, 32);
		int value_2 = Transform_1(temp, 6);
		address[distinguish - 1] = value_2;
		value_2 = value_1 + value_2;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Memory[value_2][i];
		}
		break;
	case 9://���Ѱַ
		int value_1 = Transform_1(PC, 32);
		int value_2 = Transform_1(temp, 6);
		address[distinguish - 1] = value_2;
		value_2 = value_1 + value_2;
		for (i = 0; i < 32; i++)
		{
			MDR[distinguish - 1][i] = Memory[value_2][i];
		}
		break;
	}
}

void ADD()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][6 + i];
	}
	Addressing(1);
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][19 + i];
	}
	Addressing(2);
	int addition_number_1 = Transform_1(MDR[0], 32);
	int addition_number_2 = Transform_1(MDR[1], 32);
	addition_number_2 = addition_number_1 + addition_number_2;
	Transform_2(addition_number_2, Memory[address[1]], 32);
}
void SUB()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][6 + i];
	}
	Addressing(1);
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][19 + i];
	}
	Addressing(2);
	int reduction = Transform_1(MDR[0], 32);
	int minuend = Transform_1(MDR[1], 32);
	minuend = minuend - reduction;
	Transform_2(minuend, Memory[address[1]], 32);
}
void MOV()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][6 + i];
	}
	Addressing(1);
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][19 + i];
	}
	Addressing(2);
	for (i = 0; i < 32; i++)
	{
		MDR[0][i] = MDR[1][i];
		Memory[address[0]][i] = Memory[address[1]][i];
	}
}
void XCHG()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][6 + i];
	}
	Addressing(1);
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][19 + i];
	}
	Addressing(2);
	int value_1 = Transform_1(MDR[0], 32);
	int value_2 = Transform_1(MDR[1], 32);
	int value_3 = value_1;
	value_1 = value_2;
	value_2 = value_3;
	Transform_2(value_1, Memory[address[0]], 32);
	Transform_2(value_2, Memory[address[1]], 32);
}
void POP()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][6 + i];
	}
	Addressing(1);
	int value = Transform_1(SP, 32);
	for (i = 0; i < 32; i++)
	{
		Memory[address[0]][i] = Memory[value][i];
	}
	value = value + 2;
	Transform_2(value, SP, 32);
}
void PUSH()
{
	int i;
	for (i = 0; i < 4; i++)
	{
		MAR[i] = Memory[PC][6 + i];
	}
	Addressing(1);
	int value = Transform_1(SP, 32);
	value = value - 2;
	for (i = 0; i < 32; i++)
	{
		Memory[value][i] = Memory[address[0]][i];
	}
	Transform_2(value, SP, 32);
}
int main() {
	ifstream file("data.txt");
	int datalen = 0, i = 0;
	while (!file.eof()) {
		file >> Memory[i][datalen++];
		if (datalen == MAX_M)
		{
			i++;
			datalen = 0;
		}
	}
	while (PC < i) {
		strcpy(IR, Memory[PC]);

		char ch[6];
		int ir;
		strncpy(ch, IR, 6);
		ir = Transform(ch);
		switch (ir)
		{
		case 0:ADD();
		default:
			break;
		}
		PC++;
	}
}