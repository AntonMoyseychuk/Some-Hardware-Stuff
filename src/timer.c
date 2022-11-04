//��.024401-2
//���ᥩ�� �.�.
//�.�. �2, "���⥬�� ⠩��� ���ᮭ��쭮�� ��������"

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define TIMER_FREQ 1193182
#define DYNAMIC_ON 0x03
#define DYNAMIC_OFF 0xfc

//����� ���
#define G4 392
#define F4 349
#define D5 587
#define C5 523
#define A4 440
#define A_4 466
#define A5 880
#define A_5 932
#define F5 699
#define G5 784

typedef struct
{
	int freq;
	int dur;
} Note; //�������, ᮤ�ঠ�� ����� � �த����⥫쭮��� ����

Note notes[] =
{
	{ G4, 250 }, { G4, 250 }, { D5, 250 }, { C5, 250 },
	{ A_4, 250 }, { A4, 200 }, { A4, 200 }, { A4, 200 },
	{ C5, 200 }, { A_4, 200 }, { A4, 200 }, { G4, 250 },
	{ G4, 250 },{ A_5, 175 }, { A5, 175 }, { A_5, 175 },
	{ A5, 175 }, { A_5, 175 }, { G4, 250 }, { G4, 250 },
	{ A_5, 175 }, { A5, 175 }, { A_5, 175 }, { A5, 175 },
	{ A_5, 175 }, { G4, 250 }, { G4, 250 }, { D5, 250 },
	{ C5, 200 }, { A_4, 200 }, { A4, 200 }, { A4, 200 },
	{ A4, 200 }, { C5, 200 }, { A_4, 200 }, { A4, 200 },
	{ G4, 250 }, { G4, 250 }, { A_5, 175 }, { A5, 175 },
	{ A_5, 175 }, { A5, 175 }, { A_5, 175 }, { G4, 250 },
	{ G4, 250 }, { A_5, 175 }, { A5, 175 }, { A_5, 175 },
	{ A5, 175 }, { A_5, 175 }, { A_4, 175 }, { A_4, 175 },
	{ A_4, 175 }, { A4, 175 }, { D5, 175 }, { D5, 175 },
	{ D5, 175 }, { D5, 175 }, { C5, 175 }, { C5, 175 },
	{ C5, 175 }, { C5, 175 }, { F5, 175 }, { F5, 175 },
	{ F5, 175 }, { F5, 175 }, { G5, 175 }, { G5, 175 },
	{ G5, 175 }, { G5, 175 }, { G5, 175 }, { G5, 175 },
	{ G5, 175 }, { G5, 175 }, { G5, 275 }, { D5, 250 },
	{ C5, 225 }, { A_4, 200 }, { A4, 200 }, { F4, 200 },
	{ G4, 200 }, { G4, 200 }, { D5, 200 }, { C5, 200 },
	{ A_4, 200 }, { A4, 200 }, { A4, 200 }, { A4, 200 },
	{ C5, 200 }, { A_4, 200 }, { A4, 200 }, { G4, 250 },
	{ G4, 250 }, { A_5, 175 }, { A5, 175 }, { A_5, 175 },
	{ A5, 175 }, { A_5, 175 }, { G4, 250 }, { G4, 250 },
	{ A_5, 175 }, { A5, 175 }, { A_5, 175 }, { A5, 175 },
	{ A_5, 175 }, { G4, 250 }, { G4, 250 },	{ D5, 225 },
	{ C5, 200 }, { A_4, 200 }, { A4, 200 }, { A4, 200 },
	{ A4, 200 }, { C5, 200 }, { A_4, 200 }, { A4, 200 },
	{ G4, 250 }, { G4, 250 }, { A_5, 175 }, { A5, 175 },
	{ A_5, 175 }, { A5, 175 }, { A_5, 175 }, { G4, 250 },
	{ G4, 250 }, { A_5, 175 }, { A5, 175 }, { A_5, 175 },
	{ A5, 175 }, { A_5, 175 }, { A_4, 175 }, { A_4, 175 },
	{ A_4, 175 }, { A_4, 175 }, { D5, 175 }, { D5, 175 },
	{ D5, 175 }, { D5, 175 }, { C5, 175 }, { C5, 175 },
	{ C5, 175 }, { C5, 175 }, { F5, 175 }, { F5, 175 },
	{ F5, 175 }, { F5, 175 }, { G5, 175 }, { G5, 175 },
	{ G5, 175 }, { G5, 175 }, { G5, 175 }, { G5, 175 },
	{ G5, 175 }, { G5, 175 }, { G5, 200 }
};

void PrintDeveloperInfo(void);//�㭪�� �뢮�� ���ଠ樨 � ࠧࠡ��稪�
void PrintMenu(void);//�㭪�� �뢮�� �㭪⮢ �ணࠬ��
void SetCount(int frequency, int duration);//�㭪�� �뢮�� ��㪠
void SetDynamicState(int state);//�㭪�� ����祭��/�몫�祭�� ��������
void PrintStatusWord(void);//�㭪�� �뢮�� ���ﭨ� ������� � ॣ���஢

char* ToBinary(unsigned char value, char* buff, int bitCount);//�㭪�� ��ॢ��� �� �����筮� � ������� ��⥬�
char* ToHex(unsigned char value, char* buff, int bitCount);//�㭪�� ��ॢ��� �� �����筮� � ��⭠������� ��⥬�
char TetradaToHex(unsigned char bin);//�㭪�� ��ॢ��� ����襩 ��ࠤ� � ��⭠������� ��⥬�

int main(void)
{
	int i;
	system("cls");
	while(1)
	{
		PrintDeveloperInfo();
		PrintMenu();
		switch(getch())
		{
			case '1':
				PrintDeveloperInfo();
				PrintStatusWord();
				system("pause");
				break;
			case '2':
				PrintDeveloperInfo();
				PrintStatusWord();//�뢮� ���ﭨ� ���� 61h, ������� � ॣ���஢ �� ����祭�� �������� � �ந��뢠��� �������
				system("pause");

				SetDynamicState(1);//����祭�� ��������
				for (i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
				{
					SetCount(notes[i].freq, notes[i].dur);//�ந��뢠��� i-�� ����
					if (kbhit())
					{
						getch();
						break;
					}
				}
				printf("\n");
				PrintStatusWord();//�뢮� ���ﭨ� ���� 61h, ������� � ॣ���஢ ��᫥ �ந��뢠��� �������

				SetDynamicState(0);//�몫�祭�� ��������
				system("pause");
				break;
			case '3':
				system("cls");
				return 0;
		}
	}
}

void PrintDeveloperInfo()
{
	system("cls");
	printf("+-----------------------------------------------------+\n");
	printf("|                      ��.024401-2                    |\n");
	printf("|                     ���ᥩ�� �.�.                  |\n");
	printf("| �.�. �2,\"���⥬�� ⠩��� ���ᮭ��쭮�� ��������\" |\n");
	printf("+-----------------------------------------------------+\n\n");
}

void SetCount(int frequency, int duration)
{
	short value;
	if (frequency <= 0)
		return;

	outport(0x43, 0xb6);

	value = TIMER_FREQ / frequency;//��।������ ���祭�� ��� ����� � ॣ���� ���稪�
	outport(0x42, (char)value);//������ ����襣� ���� ����祭��� �����
	outport(0x42, (char)(value >> 8));//������ ���襣� ���� ��⥬ ����⮢��� ᤢ��� ����� �� 8 ���
	delay(duration);//����প�, � �祭�� ���ன ������� �뤠�� ��� ����� value
}

void SetDynamicState(int state)
{
	if (state == 0)
	{
		//�⪫�砥� �������
		//DYNAMIC_OFF = 1111 1100, �.�. �� ����⮢�� "�"
		//������ 2 ��� ���� ��⠭������ � 0
		//(��⠫�� ��⠭���� ���஭��묨) � ������� �몫�����
		outport(0x61, inport(0x61) & DYNAMIC_OFF);
	}
	else
	{
		//����砥� �������
		//DYNAMIC_ON = 0000 0011, �.�. �� ����⮢�� "���"
		//������ 2 ��� ���� ��⠭������ � 1
		//(��⠫�� ��⠭���� ���஭��묨) � ������� ��������
		outport(0x61, inport(0x61) | DYNAMIC_ON);
	}
}

void PrintMenu()
{
	printf("1. �������� ������� ⠩���\n");
	printf("2. �ந��뢠��� �������\n");
	printf("3. ��室\n");
}

void PrintStatusWord()
{
	//ᨬ����� ���ᨢ� ��� ����筮�� ��⭠����筮��
	//�।�⠢����� ���ﭨ� ������� � ॣ���஢
	char *bin = (char*)malloc(17), *hex = (char*)malloc(7);
	unsigned char state, controlWord = 0xe2;
	int channelsCount = 3, i;

	printf("\t\t\t\t\t������\t���⭠������\n");

	state = inport(0x61);//�⥭�� ᫮�� ���ﭨ� ���� 61h
	printf("\n���� 61h\t\t\t\t%s\t\t%s\n", ToBinary(state, bin, 8), ToHex(state, hex, 8));

	for (i = 0; i < channelsCount; i++)
	{
		controlWord += i * 2;
		//������ �ࠢ���饣� ᫮�� � ���� 43h �� ���뢠���
		//᫮�� ���ﭨ� ������ i
		//0xe2 = 1110 0010  0xe4 = 1110 0100  0xe8 = 1110 1000
		//6, 7 ���� ��⠭������ � 1, ����� �㤥� ���뢠���� ᫮�� ���ﭨ�
		//5 ��� 䨪���� ���祭�� ����稪� �� ���祭�� 0
		//4 ��� 㪠�뢠�� �� ���뢠��� ���ﭨ� �� ���祭�� 0
		//���� 3-1 ����� ������
		//��� 0 �ᥣ�� ࠢ�� 0
		outport(0x43, controlWord);
		state = inport(0x40 + i);//�⥭�� ᫮�� ���ﭨ� ������ i (���� (40 + i)h)
		printf("����� %d(���� 4%dh)\t\t\t%s\t\t%s\n", 
			i, i, ToBinary(state, bin, 8), ToHex(state, hex, 8));
	
		//������ �ࠢ���饣� ᫮�� � ���� 43h �� ���뢠���
		//���祭�� ॣ���� ����稪� CE
		//0x00 - 0000 0000  0x40 = 0100 0000  0x80 = 1000 0000
		//���� 7-6 - �����
		//���� 5-4 - ��� ������� CLC (��������� ॣ���� ���稪� CE)
		//���� 3-0 �����������
		outport(0x43, 0x00 + 0x40 * i);
		//���뢠��� ����襣� � ���襣� ���⮢ ॣ���� ���稪� CE
		state = inp(0x40 + i) | (inp(0x40 + i) << 8);
		printf("  ������� ���稪� CE\t\t%s\t\t%s\n", ToBinary(state, bin, 16), ToHex(state, hex, 16));
	}

	//���⪠ �������᪨ �뤥������ �����
	free(bin);
	free(hex);
}

char* ToBinary(unsigned char value, char* buff, int bitCount)
{
	int i;
	for (i = bitCount - 1; i >= 0; i--)
	{
		//0x01 = 0000 0001
		//�� ��������� ��᪨ 0x01 १���⮬ ����� ����
		//⮫쪮 1 ��� 0
		buff[i] = (value & 0x01 ? '1' : '0');

		//���室 � ᫥���饬� ࠧ��� �१ ����⮢�
		//ᤢ�� ��ࠢ� �� 1 ���
		value = value >> 1;
	}
	buff[bitCount] = '\0';
	return buff;
}

char* ToHex(unsigned char value, char* buff, int bitCount)
{
	buff[0] = '0';
	buff[1] = 'x';
	if (bitCount == 16)
	{
		//��ॢ�� ������ �� ��ࠤ � ��⭠������ �ଠ�
		buff[2] = TetradaToHex(value >> 12);
		buff[3] = TetradaToHex(value >> 8);
		buff[4] = TetradaToHex(value >> 4);
		buff[5] = TetradaToHex(value);
		buff[6] = '\0';
	}
	else if (bitCount == 8)
	{
		buff[2] = TetradaToHex(value >> 4);
		buff[3] = TetradaToHex(value);
		buff[4] = '\0';
	}

	return buff;
}

char TetradaToHex(unsigned char bin)
{
	bin &= 0xf;
	//0xf = 1111
	//�� ��������� ��᪨ 1111 �� ������� ��ࠤ�
	//����砥��� �᫮ �� 0 �� 15 �����⥫쭮
	return (bin < 10) ? '0' + bin : 'A' + bin - 10;
}

