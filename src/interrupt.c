//��.024401-2 ���ᥩ�� �.�.
//��-4 "�����⥬� ���뢠���"

#include <dos.h>
#include <mem.h>
#include <stdlib.h>

enum PortsNumber
{
	PORT_20 = 0x20, PORT_21,
	PORT_A0 = 0xA0, PORT_A1,
	PORT_60 = 0x60, PORT_70 = 0x70, PORT_71
};

enum Register
{
	IRR_READ = 0x0A, ISR_READ = 0x0B,
	RTC_STATE_B_REGISTER = 0x0B
};

enum InterruptVectors
{
	ZERO_DIV_INT = 0x00, RTC_INT = 0x70,
	SYSTEM_TIMER_INT = 0x08, KEY_BOARD_INT = 0x09
};

enum InterruptIndicatorsPositions
{
	TIMER_INDICATOR_RAW = 6, RTC_INDICATOR_RAW,
	ZERO_INDICATOR_RAW, KBOARD_INDICATOR_RAW
};

enum ScreenSize
{
	INFO_TABLE_H = 4, SCREEN_H = 25,
	INFO_TABLE_LEN = 40, SCREEN_LEN = 80
};

enum Symbol
{
	SYMBOL_SIZE = 2, SYMBOL_1 = 0x02, SYMBOL_0 = 0x0B
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
	enum PortsNumber number;
	enum Register reg;
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

//��楤�� �뢮�� ���ﭨ� ॣ���஢ �� � ����������
void PrintControllerInfo(Port* ports, unsigned char portsCount, unsigned char startRaw,
	unsigned char startCol, unsigned char colour);

//��楤�� �뢮�� ���ଠ樨 �� ��ࠡ��뢠���� ���뢠����
void PrintInterruptionsInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� ॣ���� ������ ����������
void PrintPort60Info(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� ᪠�-����� �� �࠭ � ��⭠����筮� ����
void PrintScanCodes(char* str, unsigned char colour);

//��楤�� ��ࠡ�⪨ ���뢠��� ��⥬���� ⠩���
void interrupt far timerHandler(void);

//��楤�� ��ࠡ�⪨ ���뢠��� ����������
void interrupt far keyBoardHandler(void);

//��楤�� ��ࠡ�⪨ ���뢠��� RTC
void interrupt far rtcHandler(void);

//��楤�� ��ࠡ�⪨ ���뢠��� �� �訡�� ������� �� 0
void interrupt far zeroDivHandler(void);

//�㭪�� ������� ������ �᫠ �� ��㣮�
int Divide(int divisible, int divider);

//��楤��, �⮡ࠦ���� �� �࠭� ���� ��אַ㣮�쭨� - 
//�ਧ��� ������������� ���뢠���
void PrintInterruptIndicator(unsigned char raw, unsigned char col, unsigned char colour,
	unsigned short duration);

//bin -> hex (1 ᨬ���)
//������� 1 hex-ᨬ��� �� ������ 4 ��� Bin4Bit
char Bin4toHex1(unsigned int Bin4Bit);

//bin -> hex2 (2 ᨬ����)
//������� 2 hex-ᨬ���� �� ����襣� ���� Bin4Bit
//ࠧ��� BufOut >= 3
char* Bin8toHex2(unsigned int Bin8Bit, char* BufOut);

//bin -> hex4 (4 ᨬ����)
//������� 4 hex-ᨬ���� �� Bin16Bit
//������ BufOut >= 5
char* Bin16toHex4(unsigned int Bin16Bit, char* BufOut);

//������� 8 bin-ᨬ����� �� ����襣� ���� Bin8Bit
//������ BufOut >= 9
char* Bin8toBinSym8(unsigned int Bin8Bit, char* BufOut);


//㪠��⥫� �� �㭪�� ��ࠡ�⪨ ���뢠��� ��⥬���� ⠩���
void interrupt far(*oldTimerHandler)(void);
//㪠��⥫� �� �㭪�� ��ࠡ�⪨ ���뢠��� ����������
void interrupt far(*oldKBHandler)(void);
//㪠��⥫� �� �㭪�� ��ࠡ�⪨ ���뢠��� RTC
void interrupt far(*oldRTCHandler)(void);
//㪠��⥫� �� �㭪�� ��ࠡ�⪨ ���뢠��� �� ������� �� 0
void interrupt far(*oldZeroDivHandler)(void);

//䫠�� ���ﭨ�, ������� � ���祢� ᫮��� volatile,
//�� ������ ���������� �� ��������� ������ ��६�����
//�� ��஭��� ��⮪�� �ᯮ������ �ணࠬ��
volatile unsigned char isExit = 0;
volatile unsigned char kbInt = 0;
volatile unsigned char rtcInt = 0;
volatile unsigned char timerInt = 0;
volatile unsigned char zeroInt = 0;
volatile unsigned short ticks = 0;

int main(void)
{
	ClearScreen();
	PrintDeveloperInfo(0, 0, _RED);
	PrintMenu(0, INFO_TABLE_LEN, _LIGHT_TURQUOISE);
	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);

	//����� �맮��� ���뢠���
	disable();

	//��⠭���� ������ ��ࠡ��稪� ���뢠��� ����������
	oldKBHandler = getvect(KEY_BOARD_INT);
	setvect(KEY_BOARD_INT, keyBoardHandler);

	//��⠭���� ������ ��ࠡ��稪� ���뢠��� ⠩���
	oldTimerHandler = getvect(SYSTEM_TIMER_INT);
	setvect(SYSTEM_TIMER_INT, timerHandler);

	//��⠭���� ������ ��ࠡ��稪� ���뢠��� �� ������� �� 0
	oldZeroDivHandler = getvect(ZERO_DIV_INT);
	setvect(ZERO_DIV_INT, zeroDivHandler);

	//��⠭���� ������ ��ࠡ��稪� ���뢠��� �ᮢ ॠ�쭮�� �६���
	oldRTCHandler = getvect(RTC_INT);
	setvect(RTC_INT, rtcHandler);

	//����襭�� ���뢠��� IRQ8 �ᮢ ॠ�쭮�� �६���
	outp(PORT_70, RTC_STATE_B_REGISTER);
	//������ ������ ���祭�� � ����� ॣ����. 40h -> 0100 0000
	outp(PORT_71, inp(PORT_71) | 0x40);

	//ࠧ�襭�� �맮��� ���뢠���
	enable();

	while (!isExit)
	{
		if (timerInt) {
			PrintInterruptIndicator(TIMER_INDICATOR_RAW, SCREEN_LEN - 2,
				_RED << 4, 50);
			timerInt = 0;
		}

		if (rtcInt) {
			PrintInterruptIndicator(RTC_INDICATOR_RAW, SCREEN_LEN - 2,
				_RED << 4, 10);
			rtcInt = 0;
		}

		if (kbInt) {
			PrintInterruptIndicator(KBOARD_INDICATOR_RAW, SCREEN_LEN - 2,
				_RED << 4, 50);
			kbInt = 0;
		}

		if (zeroInt) {
			Divide(1, 0);
			Print("������� �� 0", 3 * INFO_TABLE_H - 1, 0, _LIGHT_BLUE);
			PrintInterruptIndicator(ZERO_INDICATOR_RAW, SCREEN_LEN - 2,
				_RED << 4, 350);
            ClearScreenRaw(3 * INFO_TABLE_H - 1, 0);
			zeroInt = 0;
		}
	}

	disable();

	//��⠭���� ����� ��ࠡ��稪�� ���뢠���
	setvect(KEY_BOARD_INT, oldKBHandler);
	setvect(SYSTEM_TIMER_INT, oldTimerHandler);
	setvect(ZERO_DIV_INT, oldZeroDivHandler);
	setvect(RTC_INT, oldRTCHandler);

	//����⠭������� ॣ���� ���ﭨ� B �ᮢ ॠ�쭮�� �६���
	outp(PORT_70, RTC_STATE_B_REGISTER);
	//0xBF -> 1011 1111
	outp(PORT_71, inp(PORT_71) & 0xBF);

	enable();

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

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Print("+--------------------------------------+", startRaw, startCol, colour);
	Print("|      ��.024401-2 ���ᥩ�� �.�.      |", startRaw + 1, startCol, colour);
	Print("|     ��-4 \"�����⥬� ���뢠���\"     |", startRaw + 2, startCol, colour);
	Print("+--------------------------------------+", startRaw + 3, startCol, colour);
}

void PrintMenu(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Print("+--------------------------------------+", startRaw, startCol, colour);
	Print("| 1. ������� �� 0                      |", startRaw + 1, startCol, colour);
	Print("| 0. ��室                             |", startRaw + 2, startCol, colour);
	Print("+--------------------------------------+", startRaw + 3, startCol, colour);
}

void PrintStatusWord(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Port ports[] =
	{
		{ "| IRR", PORT_20, IRR_READ },
		{ "| ISR", PORT_20, ISR_READ },
		{ "| IMR", PORT_21, NULL },
		{ "| 60h", PORT_60, NULL }
	};

	int i, portsCount = sizeof(ports) / sizeof(Port), infoCollumnLen = 12;

	Print("+------------------------------------------------------------------------------+",
		startRaw++, startCol, colour);

	Print("|", startRaw, startCol, colour);
	Print("MASTER", startRaw, startCol + infoCollumnLen + 7, colour);
	Print("SLAVE", startRaw, startCol + 2.5 * infoCollumnLen + 7, colour);
	Print("INTERRUPT INFO", startRaw, startCol + 4 * infoCollumnLen + 7, colour);
	Print("|", startRaw++, SCREEN_LEN - 1, colour);


	for (i = 0; i < portsCount; i++)
	{
		Print(ports[i].message, startRaw + i, startCol, colour);
		Print("|", startRaw + i, SCREEN_LEN - 1, colour);
	}

	//�뢮� ॣ���஢ ����饩 �����奬� ����஫��� ���뢠���
	PrintControllerInfo(ports, portsCount, startRaw, startCol + infoCollumnLen + 5, colour);
	for (i = 0; i < portsCount; i++)
	{
		//���室 � ���⠬ ������� �����奬� ����஫��� ���뢠���
		ports[i].number += ((ports[i].number == PORT_20 || ports[i].number == PORT_21) ? 0x80 : 0x0);
	}
	//�뢮� ॣ���஢ ������� �����奬� ����஫��� ���뢠���
	PrintControllerInfo(ports, portsCount, startRaw, startCol + 2.5 * infoCollumnLen + 5, colour);

	PrintInterruptionsInfo(startRaw, startCol + 4 * infoCollumnLen + 5, colour);

	//�뢮� ������ �� ���� ����������
	PrintPort60Info(startRaw + 3, startCol + infoCollumnLen + 5, colour);

	Print("+------------------------------------------------------------------------------+",
		startRaw + portsCount, startCol, colour);
}

void PrintControllerInfo(Port* ports, unsigned char portsCount, unsigned char startRaw,
	unsigned char startCol, unsigned char colour)
{
	int i;
	//ᨬ����� ���ᨢ� ��� ����筮�� ��⭠����筮��
	//�।�⠢����� ���ﭨ� ������� � ॣ���஢
	char bin[9], hex[3];
	unsigned char state;

	for (i = 0; i < portsCount; i++)
	{
		//�᫨ ��� �⥭�� ���� �ॡ���� ᯥ樠�쭠� �������,
		//� �믮������ ������ � ���� if
		if (ports[i].reg != NULL) {
			outp(ports[i].number, ports[i].reg);
			state = inp(ports[i].number);
		}
		else {
			state = inp(ports[i].number);
		}

		//����� ���� ���������� �뢮����� �⤥�쭮
		if (ports[i].number != PORT_60)
		{
			Bin8toBinSym8(state, bin);
			Bin8toHex2(state, hex);
			Print(bin, startRaw + i, startCol, colour);
			Print(hex, startRaw + i, startCol + 9, colour);
		}
	}
}

void PrintScanCodes(char* str, unsigned char colour)
{
	static unsigned char rawPos = 3 * INFO_TABLE_H;
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
			ScrollScreen(3 * INFO_TABLE_H);
		}
		else
		{
			rawPos++;
		}
	}

	Print(str, rawPos, colPos, colour);
	colPos += 3;
}

void PrintInterruptionsInfo(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	char tickStr[4];

	Print("T�����(INT8):", startRaw, startCol, colour);
	Print("   ", startRaw, startCol + 15, colour);
	Print(itoa(ticks, tickStr, 10), startRaw++, startCol + 15, colour); //�뢮� ⨪�� ⠩���

	Print("RTC(INT70):", startRaw++, startCol, colour);

	Print("������� �� 0(INT0):", startRaw++, startCol, colour);

	Print("��������� (INT9):", startRaw++, startCol, colour);
}

void PrintPort60Info(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	unsigned char state = inp(PORT_60);
	char bin[9], hex[3];

	Print(Bin8toBinSym8(state, bin), startRaw, startCol, colour);
	Print(Bin8toHex2(state, hex), startRaw, startCol + 9, colour);
}

void interrupt far timerHandler(void)
{
	//����� �맮��� ���뢠���, ���� ��ࠡ��뢠���� ������ ���뢠���
	disable();
	timerInt = 1;

	ticks = ++ticks;
	ticks %= 1000;

	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);
	
	enable();
	//�맮� ��ண� ��ࠡ��稪� ���뢠��� ⠩���
	_chain_intr(oldTimerHandler);
}

void interrupt far keyBoardHandler(void)
{
	unsigned char scanCode;
	char hex[3];

	//����� �맮��� ���뢠���, ���� ��ࠡ��뢠���� ������ ���뢠���
	disable();
	kbInt = 1;

	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);

	//���뢠��� ᪠� ���� ����⮩ ������
	scanCode = inp(PORT_60);
	if (scanCode == SYMBOL_0) {
		isExit = 1;
	}
	else if (scanCode == SYMBOL_1) {
		zeroInt = 1;
	}

	PrintScanCodes(Bin8toHex2(scanCode, hex), _LIGHT_PURPLE);
	delay(25);
	
	enable();
	//�맮� ��ண� ��ࠡ��稪� ���렭�� ����������
	_chain_intr(oldKBHandler);
}

void interrupt far rtcHandler(void)
{
	//����� �맮��� ���뢠���, ���� ��ࠡ��뢠���� ������ ���뢠���
	disable();
	rtcInt = 1;
	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);
	
	enable();
	//�맮� ��ண� ��ࠡ��稪� ���렭�� �ᮢ ॠ�쭮�� �६���
	_chain_intr(oldRTCHandler);
}

void interrupt far zeroDivHandler(void)
{
	zeroInt = 1;
	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);
}

int Divide(int divisible, int divider)
{
	if (divider == 0) {
		//�맮� �ணࠬ����� ���뢠��� ������� �� 0
		zeroDivHandler();
		return 0;
	}

	return divisible / divider;
}

void PrintInterruptIndicator(unsigned char raw, unsigned char col, unsigned char colour,
	unsigned short duration)
{
	Print(" ", raw, col, colour);
	delay(duration);
	Print(" ", raw, col, _BLACK);
}

char Bin4toHex1(unsigned int Bin4Bit)
{
	const char* HexSym = "0123456789ABCDEF";
	Bin4Bit &= 0x0F;
	return HexSym[Bin4Bit];
}

char* Bin8toHex2(unsigned int Bin8Bit, char* BufOut)
{
	BufOut[0] = Bin4toHex1(Bin8Bit >> 4);
	BufOut[1] = Bin4toHex1(Bin8Bit);
	BufOut[2] = '\0';
	return BufOut;
}

char* Bin16toHex4(unsigned int Bin16Bit, char* BufOut)
{
	Bin8toHex2(Bin16Bit >> 8, BufOut);
	Bin8toHex2(Bin16Bit, &BufOut[2]);
	return BufOut;
}

char* Bin8toBinSym8(unsigned int Bin8Bit, char* BufOut)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		BufOut[i] = (Bin8Bit & 0x80) ? '1' : '0';
		Bin8Bit <<= 1;
	}
	BufOut[8] = '\0';
	return BufOut;
}

