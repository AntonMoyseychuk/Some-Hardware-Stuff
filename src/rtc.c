//��.024401-2 ���ᥩ�� �.�.
//��-5 "���� ॠ�쭮�� �६���"

#include <mem.h>
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>

typedef enum
{
	PORT_20 = 0x20, PORT_21, 
	PORT_A0 = 0xA0, PORT_A1,
	PORT_60 = 0x60, PORT_64 = 0x64,
	PORT_70 = 0x70, PORT_71
} PortsNumber;

typedef enum
{
	ISR_READ = 0x0B,
	RTC_SECOND_REGISTER = 0x00,
	RTC_MINUTE_REGISTER = 0x02,
	RTC_HOUR_REGISTER = 0x04,
	RTC_WEAK_DAY_REGISTER = 0x06,
	RTC_MONTH_DAY_REGISTER,
	RTC_MONTH_REGISTER,
	RTC_YEAR_REGISTER,
	RTC_STATE_A_REGISTER = 0x0A,
	RTC_STATE_B_REGISTER,
	RTC_STATE_C_REGISTER,
	RTC_STATE_D_REGISTER
} Register;

typedef enum
{
	KEY_BOARD_INT = 0x09,
	RTC_INT = 0x70,
	TIMER_INT = 0x1C
} InterruptVectors;

typedef enum
{
	INFO_TABLE_H = 4, SCREEN_H = 25, MENU_TABLE_H = 5,
	INFO_TABLE_LEN = 40, SCREEN_LEN = 80
} ScreenSize;

typedef enum
{
	SYMBOL_SIZE = 2, 
	SYMBOL_ESC = 0x01,
	SYMBOL_1, 
	SYMBOL_2,
	SYMBOL_3,
	SYMBOL_4,
	SYMBOL_5,
	SYMBOL_6,
	SYMBOL_7,
	SYMBOL_8,
	SYMBOL_9,
	SYMBOL_0,
	SYMBOL_BACKSPACE = 0x0E,
	SYMBOL_ENTER = 0x1C,
	
	ERROR_INP = -1
} Symbol;

typedef enum
{
	_BLACK = 0x0, _BLUE, _GREEN, _TURQUOISE,
	_RED, _PURPLE, _BROWN, _WHITE, _GRAY,
	_LIGHT_BLUE, _LIGHT_GREEN, _LIGHT_TURQUOISE,
	_PINK, _LIGHT_PURPLE, _YELLOW, _BRIGHT_WHITE
} Colours;

typedef enum
{
	INP_BUFF_SIZE = 6
} InputBufferConsts;

typedef struct
{
	char* message;
	PortsNumber controlPort;
	PortsNumber dataPort;
	Register registerWord;
} Port;

typedef struct
{
	unsigned char currInputPos;
	char buff[INP_BUFF_SIZE];
} InputBuffer;	


//�㭪�� �����頥� 1, �᫨ ��� ��᮪��� � 0 � ��⨢��� ��砥
unsigned char IsLeapYear(unsigned int year);

//�㭪�� ��������� ASCII ᨬ��� ����⮩ ������
unsigned char Getch(void);

//��楤�� ����� ���ଠ樨 �१ ����஫��� ���뢠���
unsigned int InputInt(unsigned char raw, unsigned char startCol, unsigned char colour);

//��楤�� ������� ����஬
void IndicateCursor(unsigned char raw, unsigned char col, unsigned char colour);

//��楤�� �뢮�� ��ப� str � ����樨 [raw][col]
//�१ ��אַ� ���饭�� � ����������
void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//��楤�� ���⪨ ��ப�
void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol);

//��楤�� ���⪨ �࠭�
void ClearScreen(void);

//��楤�� �뢮�� ��ਧ��⠫쭮� ࠬ�� �� �࠭
void PrintHorizontalBorder(unsigned char raw, unsigned char startCol,
	unsigned char endCol, unsigned char colour);

//��楤�� �뢮�� ���⨪��쭮� ࠬ�� �� �࠭
void PrintVerticalBorderBorder(unsigned char startRaw, unsigned char endRaw,
	unsigned char col, unsigned char colour);

//��楤�� �뢮�� ���ଠ樨 � ࠧࠡ��稪�
void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� �㭪⮢ �ணࠬ��
void PrintMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� ���ﭨ� ॣ���஢
void PrintStatusWord(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//��楤�� �뢮�� ॣ���� � ����筮� � ��⭠����筮� ���� �� �࠭
void PrintRegInfo(Port port, unsigned char startRaw, 
	unsigned char startCol, unsigned char colour);

//�㭪��, ��������� 1 �᫨ RTC ����㯭� ��� ���뢠���,
//0 �᫨ RTC ������ � ����� ������
unsigned char IsRTCFree(void);

//��楤�� �����/ࠧ�襭�� ���������� �ᮢ ॠ�쭮�� �६���
void SetClockUpdatePermission(unsigned char isPermitted);

//�㭪�� ����祭�� ����� ��� ������ �� �᫮���, �� ����ᥭ� = 1
unsigned char GetWeakDayNumber(unsigned char day, unsigned char month, unsigned char year);

//��楤�� ��⠭���� ������ �६��� � ����
void SetNewDateAndTime(unsigned char startRaw, unsigned char startCol, unsigned char colour);

//��楤�� �뢮�� ����প� �� �࠭
void PrintDelay(unsigned char startRaw, unsigned char startCol, unsigned char colour);

//��楤�� ��⠭���� ����প�
void Delay(unsigned int del, unsigned char raw, unsigned char col, unsigned char colour);

//�㭪�� �⥭�� ᮤ�ন���� ॣ���� �ᮢ ॠ�쭮�� �६���
char* ReadRTCField(unsigned short* value, unsigned char rtcRegister, char* buf, int bufSize);

//��楤�� ���樠����樨 ᮡ�⢥����� ��ࠡ��稪� ���뢠��� �ᮢ ॠ�쭮�� �६���
void InitUserRTCIntrHandler(void);

//��楤�� ���樠����樨 �����稪� ���뢠��� �ᮢ ॠ�쭮�� �६��� �� 㬮�砭��
void InitDefaultRTCIntrHandler(void);

//��楤�� �뢮�� ⥪�饣� �६��� �� �࠭
void PrintCurrentDateTime(unsigned char raw, unsigned char col, unsigned char colour);

//��楤�� ���४�஢�� �६���
void CorrectTime(void);

//�㭪�� ���������� ᨬ���� � ����� �����, ����� �����頥� 1, �᫨ ᨬ���
//�ᯥ譮 ��������, 0 - �᫨ ����� ��������
unsigned char PushSymbol(unsigned char symbol);

//��楤�� 㤠����� ��᫥����� ᨬ���� �� ����� �����
void PopSymbol(void);

//��楤�� ���⪨ ����� �����
void ClearInputBuffer(void);

//��楤�� �뢮�� ����� ����� �� �࠭
void PrintInputBuffer(unsigned char raw, unsigned char col, unsigned char colour);
	
//��楤�� ��ࠡ�⪨ ���뢠��� ����������
void interrupt far keyBoardHandler(void);

//��楤�� ��ࠡ�⪨ ���뢠��� RTC
void interrupt far rtcHandler(void);

//�㭪�� ��ॢ��� �� ����筮-�����筮� ��⥬� ��᫥���
//� �������� ��⥬� ��᫥���
unsigned int BcdToInt(unsigned int bcd);

//�㭪�� ��ॢ��� �� �����筮� ��⥬� ��᫥���
//� ����筮-�������� ��⥬� ��᫥���
unsigned int IntToBcd(unsigned int value);

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

//㪠��⥫� �� �㭪�� ��ࠡ�⪨ ���뢠��� ����������
void interrupt far(*oldKBHandler)(void);
//㪠��⥫� �� �㭪�� ��ࠡ�⪨ ���뢠��� RTC
void interrupt far(*oldRTCHandler)(void);


//䫠�� ���ﭨ� �ணࠬ��
volatile unsigned char cancelOperation = 0;

volatile unsigned int ms = 0;

volatile unsigned char isExit = 0;
volatile unsigned char isInputDataMode = 0;
volatile unsigned char setNewTimeFlag = 0;
volatile unsigned char setTimeDelayFlag = 0;

//����� �����
volatile InputBuffer inpBuffer;

Port rtcRegisters[] = 
{
	{"RTC_A(0Ah)", PORT_70, PORT_71, RTC_STATE_A_REGISTER },
	{"RTC_B(0Bh)", PORT_70, PORT_71, RTC_STATE_B_REGISTER },
	{"RTC_C(0Ch)", PORT_70, PORT_71, RTC_STATE_C_REGISTER },
	{"RTC_D(0Dh)", PORT_70, PORT_71, RTC_STATE_D_REGISTER }
};

int main()
{
	ClearScreen();
	ClearInputBuffer();

	disable();
	oldKBHandler = getvect(KEY_BOARD_INT);
	setvect(KEY_BOARD_INT, keyBoardHandler);
	enable();

	PrintDeveloperInfo(0, 0, _PINK);
	PrintMenu(INFO_TABLE_H, 0, _YELLOW);
	PrintStatusWord(0, INFO_TABLE_LEN, _LIGHT_BLUE);

	while(!isExit)
	{
		PrintCurrentDateTime(SCREEN_H - 4, SCREEN_LEN - 15, _YELLOW);
		PrintStatusWord(0, INFO_TABLE_LEN, _LIGHT_BLUE);
		if (setNewTimeFlag)
		{
			SetNewDateAndTime(INFO_TABLE_H + MENU_TABLE_H + 2, 2, _BRIGHT_WHITE);
		}
		else if (setTimeDelayFlag)
		{
			PrintDelay(INFO_TABLE_H + MENU_TABLE_H + 2, 2, _BRIGHT_WHITE);
		}
	}

	disable();
	setvect(KEY_BOARD_INT, oldKBHandler);
	enable();

	return 0;
}


unsigned char IsLeapYear(unsigned int year)
{
	return year % 4 == 0 || year % 100 == 0 && year % 400 == 0;
}

unsigned char GetWeakDayNumber(unsigned char day, unsigned char month, unsigned char year)
{
	unsigned char i, weakDayNumber = 7;
	unsigned char monthDayCount[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	for (i = 2000; i < year; i++) {
		weakDayNumber += IsLeapYear(i) ? 366 : 365;
	}

	monthDayCount[1] = (IsLeapYear(year) ? 29 : 28);
	for (i = 0; i < month - 1; i++) {
		weakDayNumber += monthDayCount[i];
	}

	weakDayNumber += day;

	return weakDayNumber % 7 + 1;
}

unsigned char Getch()
{
	unsigned char ch;

	setvect(KEY_BOARD_INT, oldKBHandler);
	ch = getch();
	setvect(KEY_BOARD_INT, keyBoardHandler);

	return ch;
}

unsigned int InputInt(unsigned char raw, unsigned char startCol, unsigned char colour)
{
	unsigned int val;
	isInputDataMode = 1;

	while (isInputDataMode)
	{
		ClearScreenRaw(raw, startCol + inpBuffer.currInputPos);
		IndicateCursor(raw, startCol + inpBuffer.currInputPos, _BRIGHT_WHITE);
		PrintInputBuffer(raw, startCol, colour);
	}
	ClearScreenRaw(raw, startCol + inpBuffer.currInputPos);
	val = inpBuffer.currInputPos > 0 ? atoi(inpBuffer.buff) : ERROR_INP;

	ClearInputBuffer();
	return val;
}

void IndicateCursor(unsigned char raw, unsigned char col, unsigned char colour)
{
	Print("_", raw, col, colour);
}

void Print(char* str, unsigned char raw, unsigned char column, unsigned char colour)
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
	char far* videoBuffer = (char far*)(0xb8000000 + (rawNumber * SCREEN_LEN + startCol) * SYMBOL_SIZE);
	_fmemset(videoBuffer, 0, (SCREEN_LEN - startCol) * SYMBOL_SIZE);
}

void ClearScreen(void)
{
	char far* videoBuffer = (char far*)0xb8000000;
	_fmemset(videoBuffer, 0, SCREEN_LEN * SCREEN_H * SYMBOL_SIZE);
}

void PrintHorizontalBorder(unsigned char raw, unsigned char startCol, unsigned char endCol, unsigned char colour)
{
	Print("+", raw, startCol++, colour);
	for (; startCol < endCol; startCol++)
	{
		Print("-", raw, startCol, colour);
	}
	Print("+", raw, endCol, colour);
}

void PrintVerticalBorder(unsigned char startRaw, unsigned char endRaw, unsigned char col, unsigned char colour)
{
	Print("+", startRaw++, col, colour);
	for (; startRaw < endRaw; startRaw++)
	{
		Print("|", startRaw, col, colour);
	}
	Print("+", endRaw, col, colour);
}

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Print("+--------------------------------------+", startRaw, startCol, colour);
	Print("|      ��.024401-2 ���ᥩ�� �.�.      |", startRaw + 1, startCol, colour);
	Print("|    ��-5 \"���� ॠ�쭮�� �६���\"     |", startRaw + 2, startCol, colour);
	Print("+--------------------------------------+", startRaw + 3, startCol, colour);
}

void PrintMenu(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Print("+--------------------------------------+", startRaw, startCol, colour);
	Print("| 1. ��⠭����� ���� � �६�           |", startRaw + 1, startCol, colour);
	Print("| 2. ��⠭����� ����প�               |", startRaw + 2, startCol, colour);
	Print("| 0. ��室                             |", startRaw + 3, startCol, colour);
	Print("+--------------------------------------+", startRaw + 4, startCol, colour);
}

void PrintStatusWord(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	unsigned char i;
	unsigned char rtcPortsCount = sizeof(rtcRegisters) / sizeof(Port);
	unsigned char width = 14;
	Port isrPort = {"ISR(20h)", PORT_20, PORT_20, ISR_READ };

	PrintHorizontalBorder(startRaw, startCol, SCREEN_LEN - 1, colour);
	PrintHorizontalBorder(startRaw + INFO_TABLE_H + MENU_TABLE_H - 1, startCol, SCREEN_LEN - 1, colour);

	PrintVerticalBorder(startRaw, startRaw + INFO_TABLE_H + MENU_TABLE_H - 1, startCol, colour);
	PrintVerticalBorder(startRaw++, startRaw + INFO_TABLE_H + MENU_TABLE_H - 1, SCREEN_LEN - 1, colour);


	Print("MASTER", startRaw, startCol + 16, colour);
	Print("SLAVE", startRaw++, startCol + 31, colour);

	Print(isrPort.message, startRaw, startCol + 2, colour);
	PrintRegInfo(isrPort, startRaw, startCol + width, _BRIGHT_WHITE);
	isrPort.dataPort += 0x80;
	isrPort.controlPort += 0x80;
	PrintRegInfo(isrPort, startRaw++, startCol + 2 * width, _BRIGHT_WHITE);

	PrintHorizontalBorder(startRaw++, startCol, SCREEN_LEN - 1, colour);

	for(i = 0; i < rtcPortsCount; i++)
	{
		Print(rtcRegisters[i].message, startRaw + i, startCol + 2, colour);
		PrintRegInfo(rtcRegisters[i], startRaw + i, startCol + 1.5 * width, _BRIGHT_WHITE);
	}
}

void PrintRegInfo(Port port, unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	unsigned char state;
	char hex[3], bin[9];

	if (port.registerWord != NULL)
	{
		if (port.controlPort == PORT_70)
		{
			while(!IsRTCFree());
		}
		outp(port.controlPort, port.registerWord);
	}
	state = inp(port.dataPort);

	Print(Bin8toBinSym8(state, bin), startRaw, startCol, colour);
	Print(Bin8toHex2(state, hex), startRaw, startCol + 9, colour);
}

unsigned char IsRTCFree()
{
	outp(PORT_70, RTC_STATE_A_REGISTER);
	//0x80 = 1000 0000, �஢��塞 7 ��� ॣ���� A
	//�ᮢ ॠ�쭮�� �६���. �᫨ 1 - ��� ������, 0 - ᢮�����
	return !(inp(PORT_71) & 0x80);
}

void SetClockUpdatePermission(unsigned char isPermitted) //�㩭� ��९஢����
{
	unsigned char state;

	while(!IsRTCFree());
	outp(PORT_70, RTC_STATE_B_REGISTER);
	state = inp(PORT_71);

	while(!IsRTCFree());
	outp(PORT_70, RTC_STATE_B_REGISTER);
	outp(PORT_71, (isPermitted ? state & 0x7F : state | 0x80));
}

void SetNewDateAndTime(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	unsigned char i, del = 30, startRawCopy = startRaw, isLeapYear = 0, weakDayNumber;
	int sec, min, hour, day, month, year;

	setNewTimeFlag = 0;
	cancelOperation = 0;

	do
	{
		Print("������ ��� (2000 - 2030):", startRaw, startCol, colour);
		year = InputInt(startRaw, startCol + del, colour);
	} while(year < 2000 || year > 2030 || year == ERROR_INP);
	startRaw++;

	isLeapYear = IsLeapYear(year);

	do
	{
		Print("������ ����� (1 - 12):", startRaw, startCol, colour);
		month = InputInt(startRaw, startCol + del, colour);
	} while(month < 1 || month > 12 || month == ERROR_INP);
	startRaw++;

	do
	{
		Print("������ ���� (1 - 31):", startRaw, startCol, colour);
		day = InputInt(startRaw, startCol + del, colour);

		if (month == 2)
		{
			if (isLeapYear && day > 29)
			{
				ClearScreenRaw(startRaw, startCol);
				Print("� 䥢ࠫ� ��᮪�᭮�� ���� �� ����� ���� ����� 29 ����.", startRaw, startCol, _PINK);
				Getch();
				ClearScreenRaw(startRaw, startCol);

				day = ERROR_INP;
			}
			else if (!isLeapYear && day > 28)
			{
				ClearScreenRaw(startRaw, startCol);
				Print("� 䥢ࠫ� �� ��᮪�᭮�� ���� �� ����� ���� ����� 28 ����.", startRaw, startCol, _PINK);
				Getch();
				ClearScreenRaw(startRaw, startCol);

				day = ERROR_INP;
			}
		}
	} while(day < 1 || day > 31 || day == ERROR_INP);
	startRaw++;

	weakDayNumber = GetWeakDayNumber(day, month, year);

	do
	{
		Print("������ ᥪ㭤� (0 - 59):", startRaw, startCol, colour);
		sec = InputInt(startRaw, startCol + del, colour);
	} while(sec > 59 || sec == ERROR_INP);
	startRaw++;

	do
	{
		Print("������ ������ (0 - 59):", startRaw, startCol, colour);
		min = InputInt(startRaw, startCol + del, colour);
	} while(min > 59 || min == ERROR_INP);
	startRaw++;

	do
	{
		Print("������ ��� (0 - 23):", startRaw, startCol, colour);
		hour = InputInt(startRaw, startCol + del, colour);
	} while(hour > 23 || hour == ERROR_INP);
	startRaw++;

	Print("�⮡� �த������ ������ ���� �������...", startRaw++, startCol, _PINK);
	Getch();

	SetClockUpdatePermission(0);
	while(!IsRTCFree());
	outp(PORT_70, RTC_SECOND_REGISTER);
	outp(PORT_71, IntToBcd(sec));

	while(!IsRTCFree());
	outp(PORT_70, RTC_MINUTE_REGISTER);
	outp(PORT_71, IntToBcd(min));

	while(!IsRTCFree());
	outp(PORT_70, RTC_HOUR_REGISTER);
	outp(PORT_71, IntToBcd(hour));

	while(!IsRTCFree());
	outp(PORT_70, RTC_MONTH_DAY_REGISTER);
	outp(PORT_71, IntToBcd(day));

	while(!IsRTCFree());
	outp(PORT_70, RTC_WEAK_DAY_REGISTER);
	outp(PORT_71, IntToBcd(weakDayNumber));

	while(!IsRTCFree());
	outp(PORT_70, RTC_MONTH_REGISTER);
	outp(PORT_71, IntToBcd(month));

	while(!IsRTCFree());
	outp(PORT_70, RTC_YEAR_REGISTER);
	outp(PORT_71, IntToBcd(year % 100));
	SetClockUpdatePermission(1);

	for (; startRawCopy < startRaw; startRawCopy++)
		ClearScreenRaw(startRawCopy, startCol);

	cancelOperation = 0;
}

void PrintDelay(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	unsigned char i, startRawCopy = startRaw;
	int del;
	char strDel[6];

	setTimeDelayFlag = 0;
	cancelOperation = 0;

	Print ("->��� �⬥�� ������ ESC<-", startRaw++, startCol, _PINK);
	do
	{
		Print("������ ����稭� ����প� (1500 - 10000 ��):", startRaw, startCol, colour);
		del = InputInt(startRaw, startCol + 45, colour);
	} while(del < 1500 || del > 10000 || del == ERROR_INP);

	if (!cancelOperation)
	{
		Print("��⠫��� �����ᥪ㭤:", ++startRaw, startCol, colour);

		Print("��砫�� ��� � �६�:", startRaw + 2, startCol, colour);
		PrintCurrentDateTime(startRaw + 3, startCol, colour);

		Print("������ ��� � �६�:", startRaw + 2, startCol + 35, colour);

		Delay(del, startRaw, startCol + 23, colour);

		PrintCurrentDateTime(startRaw + 3, startCol + 35, colour);

		startRaw += 4;
	}

	Print("�⮡� �த������ ������ ���� �������...", ++startRaw, startCol, _PINK);
	Getch();

	for (i = startRawCopy; i <= startRaw; i++) {
		ClearScreenRaw(i, startCol);
	}

	cancelOperation = 0;
}

void Delay(unsigned int del, unsigned char raw, unsigned char col, unsigned char colour)
{
	char strDel[6];
	unsigned char i, oldA1h, old21h, hour;

	//���뢠��� ��ன ��᪨ ���뢠��� �������� ����஫���
	oldA1h = inp(PORT_A1);
	//�����஢�� ��� ���뢠��� �஬� IRQ8
	outp(PORT_A1, 0xFE);

	//���뢠��� ��ன ��᪨ ���뢠��� ����饣� ����஫���
	old21h = inp(PORT_21);
	//�����஢�� ��� ���뢠��� �஬� IRQ2
	outp(PORT_21, 0xFB);

	InitUserRTCIntrHandler();

	ms = 0;
	while(ms <= del)
	{
		itoa(del - ms, strDel, 10);
		ClearScreenRaw(raw, col);
		Print(strDel, raw, col, colour);
		delay(1);
	}

	InitDefaultRTCIntrHandler();

	CorrectTime();

	//����⠭������� ��ன ��᪨ �������� ����஫���
	outp(PORT_A1, oldA1h);
	//����⠭������� ��ன ��᪨ ����饣� ����஫���
	outp(PORT_21, old21h);
}

char* ReadRTCField(unsigned short* value, unsigned char rtcRegister, char* buf, int bufSize)
{
	unsigned short data;
	memset(buf, '\0', bufSize);

	while(!IsRTCFree());
	outp(PORT_70, rtcRegister);
	data = BcdToInt(inp(PORT_71));

	if (rtcRegister == RTC_YEAR_REGISTER)
		data += 2000;

	if (value != NULL)
		*value = data;

	buf[0] = '0';
	itoa(data, (data < 10 ? buf + 1 : buf), 10);

	return buf;
}

void InitUserRTCIntrHandler(void)
{
	unsigned char state;

	oldRTCHandler = getvect(RTC_INT);
	setvect(RTC_INT, rtcHandler);

	//����襭�� ���뢠��� IRQ8 �ᮢ ॠ�쭮�� �६���
	while(!IsRTCFree());
	outp(PORT_70, RTC_STATE_B_REGISTER);
	state = inp(PORT_71);

	while(!IsRTCFree());
	outp(PORT_70, RTC_STATE_B_REGISTER);
	//������ ������ ���祭�� � ����� ॣ����. 40h -> 0100 0000
	outp(PORT_71, state | 0x40);
}

void InitDefaultRTCIntrHandler(void)
{
	unsigned char state;

	//����⠭������� ॣ���� ���ﭨ� B �ᮢ ॠ�쭮�� �६���
	while(!IsRTCFree());
	outp(PORT_70, RTC_STATE_B_REGISTER);
	state = inp(PORT_71);

	while(!IsRTCFree());
	outp(PORT_70, RTC_STATE_B_REGISTER);
	//0xBF -> 1011 1111
	outp(PORT_71, state & 0xBF);

	setvect(RTC_INT, oldRTCHandler);
}

void PrintCurrentDateTime(unsigned char raw, unsigned char col, unsigned char colour)
{
	char time[9], date[14], temp[5];
	unsigned char hour;
	char* weakDays[] = { "��", "��", "��", "��", "��", "��", "��" };
	memset(time, '\0', sizeof(time));
	memset(date, '\0', sizeof(date));

	SetClockUpdatePermission(0);

	memcpy(time, ReadRTCField(&hour, RTC_HOUR_REGISTER, temp, 3), 2);
	time[2] = ':';
	memcpy(&time[3], ReadRTCField(NULL, RTC_MINUTE_REGISTER, temp, 3), 2);
	time[5] = ':';
	memcpy(&time[6], ReadRTCField(NULL, RTC_SECOND_REGISTER, temp, 3), 2);

	memcpy(date, ReadRTCField(NULL, RTC_MONTH_DAY_REGISTER, temp, 3), 2);
	date[2] = ':';
	memcpy(&date[3], ReadRTCField(NULL, RTC_MONTH_REGISTER, temp, 3), 2);
	date[5] = ':';
	memcpy(&date[6], ReadRTCField(NULL, RTC_YEAR_REGISTER, temp, 5), 4);
	date[10] = ' ';

	while(!IsRTCFree());
	outp(PORT_70, RTC_WEAK_DAY_REGISTER);
	memcpy(&date[11], weakDays[BcdToInt(inp(PORT_71)) - 1], 2);

	SetClockUpdatePermission(1);

	Print(time, raw, col, colour);
	Print(date, raw + 1, col, colour);
}

void CorrectTime()
{
	int monthDayCount[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	char strDel[6];
	unsigned char hour;
	unsigned char weekDay;
	unsigned char monthDay;
	unsigned char month;
	unsigned int year;
	
	SetClockUpdatePermission(0);
	ReadRTCField(&hour, RTC_HOUR_REGISTER, strDel, 6);
	SetClockUpdatePermission(1);
	
	if (hour > 23)
	{
		hour = 0;
		ReadRTCField(&year, RTC_YEAR_REGISTER, strDel, 6);
		if (IsLeapYear(year))
			monthDayCount[1] = 29;
		ReadRTCField(&month, RTC_MONTH_REGISTER, strDel, 6);
		ReadRTCField(&monthDay, RTC_MONTH_DAY_REGISTER, strDel, 6);
		ReadRTCField(&weekDay, RTC_WEAK_DAY_REGISTER, strDel, 6);
		
		weekDay++;
		if (weekDay > 7)
			weekDay = 1;
		
		monthDay++;
		if (monthDay > monthDayCount[month - 1]) 
		{
			monthDay = 1;
			month++;
			if (month > 12) {
				month = 1;
				year++;
			}
		}
		
		SetClockUpdatePermission(0);
	
		outp(PORT_70, RTC_HOUR_REGISTER);
		outp(PORT_71, IntToBcd(hour));
	
		outp(PORT_70, RTC_WEAK_DAY_REGISTER);
		outp(PORT_71, IntToBcd(weekDay));
	
		outp(PORT_70, RTC_MONTH_DAY_REGISTER);
		outp(PORT_71, IntToBcd(monthDay));
	
		outp(PORT_70, RTC_MONTH_REGISTER);
		outp(PORT_71, IntToBcd(month));
	
		outp(PORT_70, RTC_YEAR_REGISTER);
		outp(PORT_71, IntToBcd(year % 100));
	
		SetClockUpdatePermission(1);
	}
}

unsigned char PushSymbol(unsigned char symbol)
{
	if (inpBuffer.currInputPos == INP_BUFF_SIZE - 1)
	{
		return 0;
	}

	inpBuffer.buff[inpBuffer.currInputPos++] = symbol;
	return 1;
}

void PopSymbol()
{
	if (inpBuffer.currInputPos > 0)
	{
		inpBuffer.currInputPos--;
	}
	inpBuffer.buff[inpBuffer.currInputPos] = ' ';
}

void ClearInputBuffer()
{
	memset(inpBuffer.buff, '\0', INP_BUFF_SIZE);
	inpBuffer.currInputPos = 0;
}

void PrintInputBuffer(unsigned char raw, unsigned char col, unsigned char colour)
{
	Print(inpBuffer.buff, raw, col, colour);
}

void interrupt far keyBoardHandler(void)
{
	unsigned char state = inp(PORT_60);

	PrintStatusWord(0, INFO_TABLE_LEN, _BLUE);

	if (isInputDataMode)
	{
		switch(state)
		{
			case SYMBOL_0:
				PushSymbol('0');
				break;
			case SYMBOL_1:
			case SYMBOL_2:
			case SYMBOL_3:
			case SYMBOL_4:
			case SYMBOL_5:
			case SYMBOL_6:
			case SYMBOL_7:
			case SYMBOL_8:
			case SYMBOL_9:
				PushSymbol('0' + state - SYMBOL_1 + 1);
				break;
			case SYMBOL_BACKSPACE:
				PopSymbol();
				break;
			case SYMBOL_ESC:
				cancelOperation = 1;
				break;
			case SYMBOL_ENTER:
				isInputDataMode = 0;
				break;
		}
	}
	else
	{
		if (state == SYMBOL_0)
		{
			isExit = 1;
		}
		else if (state == SYMBOL_1)
		{
			setNewTimeFlag = 1;
		}
		else if (state == SYMBOL_2)
		{
			setTimeDelayFlag = 1;
		}
	}

	outp(PORT_20, 0x20);
	delay(20);
}

void interrupt far rtcHandler(void)
{
	ms++;

	outp(PORT_70, 0x0C);
	inp(PORT_71);

	outp(PORT_20, 0x20);
	outp(PORT_A0, 0x20);
}

unsigned int BcdToInt(unsigned int bcd)
{
	return bcd % 16 + bcd / 16 * 10;
}

unsigned int IntToBcd(unsigned int value)
{
	return (unsigned char)((value / 10) << 4) | (value % 10);
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