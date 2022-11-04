//��.024401-2 ���ᥩ�� �.�.
//��-3 "����஫��� ����������"

#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

enum PortsNumber
{
	PORT_20 = 0x20, PORT_60 = 0x60, 
	PORT_61 = 0x61, PORT_64 = 0x64
};

//����� ����� ���뢠���, ��뢠����� 
//����⨥� ������ ����������
enum Interrupt
{
	INT9 = 0x9
};

//����⠭�� �ࠢ����� ᫮� ��� ���⮢ 60h � 20h 
enum ControlWords
{
	INDICATORS_CONTROL_WORD = 0xed, ISR_READ = 0x0b,
	INPUT_BUFFER = 0x02
};

enum ScreenSize
{
	INFO_TABLE_H = 4, SCREEN_H = 25, 
	INFO_TABLE_LEN = 37, SCREEN_LEN = 80
};

enum Symbol
{
	SYMBOL_SIZE = 2, SYMBOL_8 = 0x09, SYMBOL_9 = 0x0a
};

enum Colours
{
	_BLACK = 0x0, _BLUE, _GREEN, _TURQUOISE,
	_RED, _PURPLE, _BROWN, _WHITE, _GRAY,
	_LIGHT_BLUE, _LIGHT_GREEN, _LIGHT_TURQUOISE,
	_PINK, _LIGHT_PURPLE, _YELLOW, _BRIGHT_WHITE
};
 
typedef struct
{
	char* message;
	unsigned char number;
} Port;

//��楤�� �뢮�� ��ப� str � ����樨 [raw][col]
//�१ ��אַ� ���饭�� � ����������
void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//��楤�� ���⪨ ��ப�
void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol);

//��楤�� ���⪨ �࠭�
void ClearScreen(void);

//��楤�� �ப��⪨ �࠭�
void ScrollScreen(unsigned char startRaw);

//��楤�� �뢮�� ���ଠ樨 � ࠧࠡ��稪�
void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� �㭪⮢ �ணࠬ��
void PrintMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� ���ﭨ� ॣ���஢
void PrintStatusWord(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� ᪠�-����� �� �࠭ � ��⭠����筮� ����
void PrintScanCodes(char* str, unsigned char colour);

//ᮡ�⢥��� ��ࠡ��稪 ���뢠���
void interrupt far InterruptHandler(void);

//�㭪�� �஢�ન ������������ ����
unsigned char isBufferEmpty(unsigned char bufferMask);

//��楤�� ����祭��/�몫�祭�� �������஢ ����������
void changeIndicatorsState(unsigned char indicatorsMask);

//��楤�� ������� �������ࠬ� ����������
void Blink(int blinkCount);

//�㭪�� ��ॢ��� �� �����筮� � ������� ��⥬�
char* ToBinary(unsigned char value, char* buff, int bitCount);

//�㭪�� ��ॢ��� �� �����筮� � ��⭠������� ��⥬�
char* ToHex(unsigned char value, char* buff, int bitCount);

//�㭪�� ��ॢ��� ����襩 ��ࠤ� � ��⭠������� ��⥬�
char TetradaToHex(unsigned char bin);

//㪠��⥫� �� ���� ��ࠡ��稪 ���뢠��� INT9
void (interrupt far *oldInt9h)(void);

//䫠� ��室� �� �ணࠬ��
int isExit = 0;

int main(void)
{
	//��࠭���� ��ண� ��ࠡ��稪� ���뢠��� INT9
	oldInt9h = getvect(INT9);
	//������� ᮡ�⢥����� ��ࠡ��稪� ���뢠��� INT9
	setvect(INT9, InterruptHandler);

	ClearScreen();
	PrintDeveloperInfo(0, 0, _PINK);
	PrintMenu(INFO_TABLE_H, 0, _LIGHT_TURQUOISE);
	PrintStatusWord(0, INFO_TABLE_LEN, _LIGHT_GREEN);

	while (!isExit);

	//������� ��ண� ��ࠡ��稪� ���뢠��� INT9
	setvect(INT9, oldInt9h);
	return 0;
}

void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour)
{
	int i;
	//0xb8000000 - ���� ��砫� ���������
	char far* videoBuffer = (char far*)0xb8000000;
	//��।������ ����樨 ��砫� ����� ������ � �����
	videoBuffer += raw * SYMBOL_SIZE * SCREEN_LEN + column * SYMBOL_SIZE;

	for (i = 0; str[i] != '\0'; i++)
	{
		*videoBuffer = str[i];
		videoBuffer++;
		*videoBuffer = colour;
		videoBuffer++;
	}
}

void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol)
{
	int i;
	for (i = startCol; i < SCREEN_LEN; i++)
	{
		Print(" ", rawNumber, i, _BLACK);
	}
}

void ClearScreen(void)
{
	int i;
	for (i = 0; i < SCREEN_H; i++)
	{
	      ClearScreenRaw(i, 0);
	}
}

void ScrollScreen(unsigned char startRaw)
{
	int i, j;
	char far* vedeoBuffStart =
		(char far*)(0xb8000000 + startRaw * SYMBOL_SIZE * SCREEN_LEN);

	//��।������ ᫥���饩 ��᫥ ��砫� ��ப� ���������
	char far* buffSecondLineStart =
		(char far*)(vedeoBuffStart + SYMBOL_SIZE * SCREEN_LEN);

	_fmemmove(vedeoBuffStart, buffSecondLineStart, 
				(SCREEN_H - 1) * SCREEN_LEN * SYMBOL_SIZE);

	ClearScreenRaw(SCREEN_H - 1, 0);
}

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Print("+-----------------------------------+", startRaw, startCol, colour);
	Print("|     ��.024401-2 ���ᥩ�� �.�.    |", startRaw + 1, startCol, colour);
	Print("|    ��-3 \"����஫��� ����������\"   |", startRaw + 2, startCol, colour);
	Print("+-----------------------------------+", startRaw + 3, startCol, colour);
}


void PrintMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Print("+-----------------------------------+", startRaw, startCol, colour);
	Print("| 8. �������� �������ࠬ�          |", startRaw + 1, startCol, colour);
	Print("| 9. ��室                          |", startRaw + 2, startCol, colour);
	Print("+-----------------------------------+", startRaw + 3, startCol, colour);
}

void PrintStatusWord(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Port ports[] =
	{
		{ "| ������� ������(60h): ", PORT_60 },
		{ "| ������� ���ﭨ�(64h): ", PORT_64 },
		{ "| ������� ������(61h): ", PORT_61 },
		{ "| ������� ISR(20h): ", PORT_20 },
	};
	int i, maxLen = strlen(ports[1].message);

	//ᨬ����� ���ᨢ� ��� ����筮�� ��⭠����筮��
	//�।�⠢����� ���ﭨ� ������� � ॣ���஢
	char *bin = (char*)malloc(9), *hex = (char*)malloc(3);
	unsigned char state;

	Print("+--------------------------------------+", startRaw++, startCol, colour);
	ClearScreenRaw(startRaw, startCol);
	Print("|", startRaw , startCol, colour);
	Print("|", startRaw++, startCol + INFO_TABLE_LEN + 2, colour);

	for (i = 0; i < sizeof(ports) / sizeof(Port); i++, startRaw++)
	{
		if (ports[i].number == PORT_20)
		{
			//�� ���饭�� � ����� 20h �㦭� ᭠砫�
			//��᫠�� �ࠢ���饥 ᫮��, � ��⥬ ����� ᮤ�ন��� ॣ����
			outportb(PORT_20, ISR_READ);
		}

		state = inportb(ports[i].number);

		Print(ports[i].message, startRaw, startCol, colour);
		Print(ToBinary(state, bin, 8), startRaw, startCol + maxLen, colour);
		Print(ToHex(state, hex, 8), startRaw, startCol + maxLen + 9, colour);
		Print("|", startRaw, startCol + INFO_TABLE_LEN + 2, colour);
	}

	ClearScreenRaw(startRaw, startCol);
	Print("|", startRaw, startCol, colour);
	Print("|", startRaw++, startCol + INFO_TABLE_LEN + 2, colour);
	Print("+--------------------------------------+", startRaw, startCol, colour);

	//���⪠ �������᪨ �뤥������ �����
	free(bin);
	free(hex);
}

void PrintScanCodes(char* str, unsigned char colour)
{
	static unsigned char rawPos = 2 * INFO_TABLE_H + 1;
	static unsigned char colPos = 0;

	if (colPos >= SCREEN_LEN - 1)
	{
		//�᫨ ����� ᨬ���� �� ��ਧ��⠫� �ॢ�蠥�
		//�����⨬�� �᫮ ᨬ����� � ��ப�, ⮣��
		//��ॢ�� X ����樨 � ��砫� ��ப�
		colPos = 0;

		//�᫨ �뢮� ���⨣ ���� ��࠭���, ⮣��
		//������ � �⮩ �� ��ப� � �஫���뢠�� ��࠭��� ����,
		//���� ���室 �� ����� ��ப�
		if (rawPos == SCREEN_H - 1)
		{
			ScrollScreen(2 * INFO_TABLE_H + 1);
		}
		else
		{
			rawPos++;
		}
	}

	Print(str, rawPos, colPos, colour);
	colPos += 3;
}

//��楤�� ��뢠���� ⮫쪮 �� ������������� ���뢠��� INT9,
//�맢������ ����⨥� ������ ����������
void interrupt far InterruptHandler(void)
{
	unsigned char scanCode;
	char* hex = malloc(3);
	PrintStatusWord(0, INFO_TABLE_LEN, _LIGHT_GREEN);
	
	//���뢠��� ᪠� ���� ����⮩ ������
	scanCode = inport(PORT_60);
	if (scanCode == SYMBOL_9)
	{
		isExit = 1;
	}
	else if (scanCode == SYMBOL_8)
	{
		Blink(10); //������ 10 ࠧ
	}

	PrintScanCodes(ToHex(scanCode, hex, 8), _LIGHT_PURPLE);
	free(hex);

	//������ � ���� 20h �ࠢ���饣� ᫮�� 0010 0000
	//��� ᮮ�頥� �� ���筮� ���� ���뢠���
	outportb(PORT_20, 0x20);
}

unsigned char isBufferEmpty(unsigned char buffer)
{
	//� ����ᨬ��� �� ��।����� ��᪨
	//�஢������ ����稥 ������ �� �室���/��室��� ����
	return ((inportb(PORT_64) & buffer) == 0x0);
}

void changeIndicatorsState(unsigned char indicatorsMask)
{
	//�஢�ઠ �室���� ���� �� ������
	while (!isBufferEmpty(INPUT_BUFFER));
	//������ �ࠢ���饣� ᫮�� �� ����祭��/�몫�祭�� �������஢
	outportb(PORT_60, INDICATORS_CONTROL_WORD);

	while (!isBufferEmpty(INPUT_BUFFER));
	outportb(PORT_60, indicatorsMask);
}

void Blink(int blinkCount)
{
	int i;
	for (i = 0; i < blinkCount; i++)
	{
		changeIndicatorsState(0x07);
		delay(200);

		changeIndicatorsState(0x00);
		delay(200);
	}
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
	//��।��塞 ������⢮ ��ࠤ
	//� ����ᨬ��� �� ࠧ�來��� �᫠
	unsigned char symbCount = bitCount / 4;
	int i;
	
	//��ॢ�� ������ �� ��ࠤ � ��⭠������ �ଠ�
	for (i = 0; i < symbCount; i++)
	{
		buff[i] = TetradaToHex(value >> (symbCount - 1 - i) * 4);
	}
	buff[symbCount] = '\0';

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