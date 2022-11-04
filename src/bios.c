//��.024401-2 ���ᥩ�� �.�.
//��-6 "�㭪樨 BIOS"
#include <conio.h>
#include <mem.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	SYSTEM_CONFIG_BIOS_INT = 0x11,
	RAM_SIZE_BIOS_INT,
	KEY_BOARD_BIOS_INT = 0x16,
	RTC_BIOS_INT = 0x1A
} InterruptVectors;

typedef enum
{
	INFO_TABLE_H = 4, SCREEN_H = 25, MENU_TABLE_H = 6,
	INFO_TABLE_LEN = 40, SCREEN_LEN = 80
} ScreenSize;

typedef enum
{
	READ_TIME_RTC = 0x02,
	SET_TIME_RTC,
	READ_DATE_RTC,
	SET_DATE_RTC
} RtcCommands;

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
	SYMBOL_SPACE = 0x39,
	
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
	INP_BUFF_SIZE = 100
} InputBufferConsts;

typedef struct
{
	unsigned char currInputPos;
	char buff[INP_BUFF_SIZE];
} InputBuffer;

typedef struct
{
	unsigned hddAvailability : 1; //0 - ����稥 ���⪮�� ��᪠
	unsigned fpuAvailability : 1; //1 - ��⥬���᪨� ᮯ�����
	unsigned ramBanksCount : 2; //2-3 - ������⢮ ������ ���
	unsigned videoMode : 2; //4-5 - ��⨢�� �����०�� 
	unsigned hddCount : 2; //6-7 - ��᫮ �����㦥���� ����
	unsigned dmaAvailability : 1; //8 - ����稥 ����஫��� DMA
	unsigned comCount : 3; //9-11 - ��᫮ �ᨭ�஭��� ��᫥����⥫��� ���⮢
	unsigned gameControllerAvailability : 1; //12 - ����稥 ��஢��� ����
	unsigned comPortJrPC: 1; //13 - ��᫥����⥫�� ���� (jrPC)
	unsigned lptCount : 2; //14-15 - ��᫮ LPT-���⮢
} PcConfiguration;

unsigned char IsLeapYear(unsigned int year);

//�㭪�� ������� ������⥫쭮�� �᫠
int Input(unsigned char raw, unsigned char column,
	unsigned char colour);

//�㭪�� �஢�ન �宦����� ���祭�� � �᫮��� ��������
unsigned char IsInRange(int value, int leftBorder, int rightBorder);

//�㭪�� ����� ᨢ��� ��� ��������, ��������� ASCII ��� � ᪠�-��� ����⮩ ������
union REGS Getch(void);

//�㭪�� ����� ᨢ��� � ���������, ��������� ASCII ��� � ᪠�-��� ����⮩ ������
union REGS Getchar(unsigned char raw, unsigned char column,
	unsigned char colour);

//�㭪�� ����� ��ப�
char* Getline(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//��楤�� �뢮�� ��ப� str � ����樨 [raw][col]
//�१ ��אַ� ���饭�� � ����������
void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//��楤�� ���⪨ ��ப�
void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol);

//��楤�� ���⪨ �࠭�
void ClearScreen(void);

//��楤�� ��⠭���� ���� � �६���
void SetDateAndTime(unsigned char sec, unsigned char min,
	unsigned char hour, unsigned char day, unsigned char month, 
		unsigned short year);

//��楤�� �뢮�� ⥯��譥�� �६��� �� �࠭
void PrintCurrentDateAndTime(unsigned char raw, unsigned char col, 
	unsigned char colour);

//��楤�� �뢮�� ���ଠ樨 � ࠧࠡ��稪�
void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� �㭪⮢ ���� �ணࠬ��
void PrintStartMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//��楤�� �뢮�� �㭪⮢ ���� �����
void PrintInputMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//��楤�� �뢮�� �㭪⮢ ���� ࠡ��� � �६����
void PrintTimeMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//�㭪�� �뢮�� ���䨣��樨 �������� �� �࠭
void PrintPCConfiguration(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//�㭪�� ���������� ᨬ���� � ����� �����, ����� �����頥� 1, �᫨ ᨬ���
//�ᯥ譮 ��������, 0 - �᫨ ����� ��������
unsigned char PushSymbol(unsigned char symbol);

//��楤�� 㤠����� ��᫥����� ᨬ���� �� ����� �����
void PopSymbol(void);

//��楤�� ���⪨ ����� �����
void ClearInputBuffer(void);

//��楤�� �뢮�� ����� ����� �� �࠭
void PrintInputBuffer(unsigned char raw, unsigned char col, unsigned char colour);

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

//����� �����
volatile InputBuffer inpBuffer;

volatile unsigned char isExit = 0;

int main()
{
	union REGS ch, rg;
	char scanCode[3], str[INP_BUFF_SIZE];
	unsigned char sec, min, hour, day, month, i;
	unsigned short year;
	unsigned char monthDayCount[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	while (!isExit)
	{
		ClearScreen();
		PrintDeveloperInfo(0, 0, _PINK);
		PrintStartMenu(INFO_TABLE_H, 0, _YELLOW);
		
		do
		{
			ch = Getch();
		} while (!IsInRange(ch.h.al, '0', '3'));
		
		switch(ch.h.al)
		{
			case '1':
				ClearScreen();
				PrintDeveloperInfo(0, 0, _PINK);
				PrintInputMenu(INFO_TABLE_H, 0, _YELLOW);
				
				do
				{
					ch = Getch();
				} while (!IsInRange(ch.h.al, '0', '3'));
				
				switch(ch.h.al)
				{
					case '1':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);
						
						Print("������ ���� �������:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						ch = Getch();
						
						PushSymbol(ch.h.al);
						PrintInputBuffer(INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);
						ClearInputBuffer();
						
						Print("����-���:", INFO_TABLE_H + 2, 0, _LIGHT_PURPLE);
						Print(Bin8toHex2(ch.h.ah, scanCode), INFO_TABLE_H + 2, 25, _BRIGHT_WHITE); 
						Print("�⮡� �த������, ������ ���� �������", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
						Getch();
						break;
					case '2':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);
						
						Print("������ ���� �������:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						ch = Getchar(INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);
						
						Print("����-���:", INFO_TABLE_H + 2, 0, _LIGHT_PURPLE);
						Print(Bin8toHex2(ch.h.ah, scanCode), INFO_TABLE_H + 2, 25, _BRIGHT_WHITE); 
						
						Print("�⮡� �த������, ������ ���� �������", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
						Getch();
						break;
					case '3':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);

						Print("������ ��ப�:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						Getline(str, INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);

						Print("��������� ��ப�:", INFO_TABLE_H + 2, 0, _LIGHT_PURPLE);
						Print(str, INFO_TABLE_H + 2, 25, _BRIGHT_WHITE);

						Print("�⮡� �த������, ������ ���� �������", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
						Getch();
						break;
					case '0':
						break;
				}
				break;
			case '2':
				ClearScreen();
				PrintDeveloperInfo(0, 0, _PINK);
				PrintPCConfiguration(INFO_TABLE_H + 1, 0, _BRIGHT_WHITE);
				break;
			case '3':
				ClearScreen();
				PrintDeveloperInfo(0, 0, _PINK);
				PrintTimeMenu(INFO_TABLE_H, 0, _YELLOW);

				do
				{
					ch = Getch();
				} while (!IsInRange(ch.h.al, '0', '3'));

				switch(ch.h.al)
				{
					case '1':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);

						Print("����騥 ��� � �६�:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						Print("�⮡� �த������, ������ ���� �������", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
						while (!kbhit())
						{
							PrintCurrentDateAndTime(INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);
						}
						Getch();
						break;
					case '2':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);

						i = INFO_TABLE_H + 1;
						do
						{
							ClearScreenRaw(i, 0);
							Print("������ ᥪ㭤�:", i, 0, _BRIGHT_WHITE);
							sec = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(sec, 0, 59));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("������ ������:", i, 0, _BRIGHT_WHITE);
							min = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(min, 0, 59));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("������ ���:", i, 0, _BRIGHT_WHITE);
							hour = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(hour, 0, 23));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("������ ���(2000 - 2030):", i, 0, _BRIGHT_WHITE);
							year = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(year, 2000, 2030));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("������ �����:", i, 0, _BRIGHT_WHITE);
							month = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(month, 1, 12));
						i++;

						if (IsLeapYear(year) && month == 2)
							monthDayCount[1] = 29;

						do
						{
							ClearScreenRaw(i, 0);
							Print("������ ����:", i, 0, _BRIGHT_WHITE);
							day = Input(i, 30, _BRIGHT_WHITE);

							if (!IsInRange(day, 1, monthDayCount[month - 1]))
								continue;
							else
								break;
						} while(1);
						i++;

						SetDateAndTime(sec, min, hour, day, month, year);

						Print("�⮡� �த������, ������ ���� �������", ++i, 0, _LIGHT_PURPLE);
						Getch();
						break;
					case '0':
						break;
				}
				break;
			case '0':
				isExit = 1;
				break;
		}
	}

	return 0;
}

unsigned char IsLeapYear(unsigned int year)
{
	return year % 4 == 0 || year % 100 == 0 && year % 400 == 0;
}

int Input(unsigned char raw, unsigned char column,
	unsigned char colour)
{
	unsigned char i = 0;
	union REGS rg;
	char value[INP_BUFF_SIZE];
	memset(value, 0, INP_BUFF_SIZE);

	while(1)
	{
		rg = Getch();

		if(rg.h.ah == SYMBOL_ENTER)
			break;

		if (rg.h.ah == SYMBOL_BACKSPACE)
		{
			if (i > 0)
			{
				value[--i] = ' ';
				Print(value, raw, column, colour);
				value[i] = '\0';
			}
		}
		else
		{
			value[i] = rg.h.al;
			Print(value, raw, column, colour);
			i++;
		}
	}
	return atoi(value);
}

unsigned char IsInRange(int value, int leftBorder, int rightBorder)
{
	return value >= leftBorder && value <= rightBorder;
}

union REGS Getch()
{
	union REGS rg;
	rg.h.ah = 0;
	int86(KEY_BOARD_BIOS_INT, &rg, &rg);
	return rg;
}

union REGS Getchar(unsigned char raw, unsigned char column,
	unsigned char colour)
{
	union REGS rg, prev;
	ClearInputBuffer();
	
	while(1)
	{
		rg = Getch();
		switch(rg.h.ah)
		{
			case SYMBOL_ENTER:
				if (inpBuffer.currInputPos > 0)
					return prev;
			case SYMBOL_BACKSPACE:
				PopSymbol();
				PrintInputBuffer(raw, column, colour);
				break;
			default:
				if (inpBuffer.currInputPos == 0 
					&& (rg.h.al >= '0' && rg.h.al <= '9' 
						|| rg.h.al >= 'a' && rg.h.al <= 'z' 
						|| rg.h.al >= 'A' && rg.h.al <= 'Z'))
				{
					PushSymbol(rg.h.al);
					PrintInputBuffer(raw, column, colour);
					prev = rg;
				}
				break;
		}
	}
}

char* Getline(char* str, unsigned char raw, unsigned char column,
	unsigned char colour)
{
	union REGS rg;
	ClearInputBuffer();
	
	while(1)
	{
		rg = Getch();
		switch(rg.h.ah)
		{
			case SYMBOL_ENTER:
				if (inpBuffer.currInputPos > 0)
				{
					strcpy(str, inpBuffer.buff);
					ClearInputBuffer();
					return str;
				}
			case SYMBOL_BACKSPACE:
				PopSymbol();
				PrintInputBuffer(raw, column, colour);
				break;
			default:
				if ((rg.h.al >= '0' && rg.h.al <= '9' 
						|| rg.h.al >= 'a' && rg.h.al <= 'z' 
						|| rg.h.al >= 'A' && rg.h.al <= 'Z' || rg.h.al == ' '))
				{
					PushSymbol(rg.h.al);
					PrintInputBuffer(raw, column, colour);
				}
				break;
		}
	}
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
	char far* videoBuffer = (char far*)(0xb8000000 
		+ (rawNumber * SCREEN_LEN + startCol) * SYMBOL_SIZE);
	_fmemset(videoBuffer, 0, (SCREEN_LEN - startCol) * SYMBOL_SIZE);
}

void ClearScreen(void)
{
	char far* videoBuffer = (char far*)0xb8000000;
	_fmemset(videoBuffer, 0, SCREEN_LEN * SCREEN_H * SYMBOL_SIZE);
}

void SetDateAndTime(unsigned char sec, unsigned char min,
	unsigned char hour, unsigned char day, unsigned char month, 
		unsigned short year)
{
	union REGS rg;
	
	rg.h.ah = SET_TIME_RTC;
	rg.h.ch = IntToBcd(hour);
	rg.h.cl = IntToBcd(min);
	rg.h.dh = IntToBcd(sec);
	int86(RTC_BIOS_INT, &rg, &rg);
	
	rg.h.ah = SET_DATE_RTC;
	rg.h.dl = IntToBcd(day);
	rg.h.dh = IntToBcd(month);
	rg.h.cl = IntToBcd(year % 100);
	int86(RTC_BIOS_INT, &rg, &rg);
}

void PrintCurrentDateAndTime(unsigned char raw, unsigned char col, 
	unsigned char colour)
{
	union REGS rg;
	char time[9], date[11], temp[5];
	unsigned char sec, min, hour, day, month, i = 0;
	unsigned short year;
	
	rg.h.ah = READ_TIME_RTC;
	int86(RTC_BIOS_INT, &rg, &rg);
	
	hour = BcdToInt(rg.h.ch);
	if (hour < 10)
	{
		time[i++] = '0';
	}
	strcpy(&time[i], itoa(hour, temp, 10));
	i = 2;
	time[i++] = ':';
	
	min = BcdToInt(rg.h.cl);
	if (min < 10)
	{
		time[i++] = '0';
	}
	strcpy(&time[i], itoa(min, temp, 10));
	i = 5;
	time[i++] = ':';
	
	sec = BcdToInt(rg.h.dh);
	if (sec < 10)
	{
		time[i++] = '0';
	}
	strcpy(&time[i], itoa(sec, temp, 10));	
	
	rg.h.ah = READ_DATE_RTC;
	int86(RTC_BIOS_INT, &rg, &rg);
	i = 0;
	
	day = BcdToInt(rg.h.dl);
	if (day < 10)
	{
		date[i++] = '0';
	}
	strcpy(&date[i], itoa(day, temp, 10));
	i = 2;
	date[i++] = ':';
	
	month = BcdToInt(rg.h.dh);
	if (month < 10)
	{
		date[i++] = '0';
	}
	strcpy(&date[i], itoa(month, temp, 10));
	i = 5;
	date[i++] = ':';
	
	year = BcdToInt(rg.h.cl) + 2000;
	strcpy(&date[i], itoa(year, temp, 10));

	Print(date, raw, col, colour);
	Print(time, raw + 1, col + 1, colour);
}

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("|      ��.024401-2 ���ᥩ�� �.�.      |", startRaw++, startCol, colour);
	Print("|         ��-6 \"�㭪樨 BIOS\"          |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);
}

void PrintStartMenu(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. ����(INT16)                       |", startRaw++, startCol, colour);
	Print("| 2. �뢥�� ���䨣���� ��������   |", startRaw++, startCol, colour);
	Print("| 3. ��� � �६�(INT1A)               |", startRaw++, startCol, colour);
	Print("| 0. ��室                             |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);
}

void PrintInputMenu(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. ���� ᨬ���� ��� ��������         |", startRaw++, startCol, colour);
	Print("| 2. ���� ᨬ���� � ��������           |", startRaw++, startCol, colour);
	Print("| 3. ���� ��ப�                       |", startRaw++, startCol, colour);
	Print("| 0. �⬥��                            |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);
}

void PrintTimeMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. �뢥�� ⥪�饥 �६�             |", startRaw++, startCol, colour);
	Print("| 2. ��⠭����� ���� � �६�           |", startRaw++, startCol, colour);
	Print("| 0. �⬥��                            |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);

}

void PrintPCConfiguration(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	union REGS rg;
	PcConfiguration pcConf;
	char* videoModes[] = { "�� �ᯮ������", "梥⭮�(40x25)", "梥⭮�(80x25)", "�����஬��(80�25)" };
	char str[17];
	unsigned char dist = 60;
	
	rg.h.ah = 0;
	int86(SYSTEM_CONFIG_BIOS_INT, &rg, &rg);
	memcpy(&pcConf, &rg.x.ax, sizeof(rg.x.ax));
	
	Print("����� ���䨣��樨:", startRaw, startCol, _LIGHT_PURPLE);
	Print(Bin8toBinSym8(rg.h.ah, str), startRaw, startCol + 25, colour);
	Print(Bin8toBinSym8(rg.h.al, str), startRaw, startCol + 33, colour);
	Print(Bin16toHex4(rg.x.ax, str), startRaw++, startCol + 45, colour);
	
	startRaw++;
	Print("���뢠��� INT11:", startRaw++, startCol, _LIGHT_PURPLE);
	
	Print("����稥 ������⥫� �� �����⭮� ��᪥:", startRaw, startCol, colour);
	Print(pcConf.hddAvailability ? "��" : "���", startRaw++, startCol + dist, colour);
	
	Print("����稥 ��⥬���᪮�� ᮯ�����:", startRaw, startCol, colour);
	Print(pcConf.fpuAvailability ? "��" : "���", startRaw++, startCol + dist, colour);

	Print("������⢮ ������ ���:", startRaw, startCol, colour);
	Print(itoa(pcConf.ramBanksCount, str, 10), startRaw++, startCol + dist, colour);

	Print("����� �����������:", startRaw, startCol, colour);
	Print(videoModes[pcConf.videoMode], startRaw++, startCol + dist, colour);
	
	Print("������⢮ ��⠭�������� HDD:", startRaw, startCol, colour);
	Print(itoa(pcConf.hddCount + 1, str, 10), startRaw++, startCol + dist, colour);
	
	Print("����稥 ����஫��� ��אַ�� ����㯠 � �����:", startRaw, startCol, colour);
	Print(pcConf.dmaAvailability ? "��" : "���", startRaw++, startCol + dist, colour);
	
	Print("������⢮ �ᨭ�஭��� ��᫥����⥫��� ���⮢:", startRaw, startCol, colour);
	Print(itoa(pcConf.comCount, str, 10), startRaw++, startCol + dist, colour);
	
	Print("����稥 ��஢��� ����:", startRaw, startCol, colour);
	Print(pcConf.gameControllerAvailability ? "��" : "���", startRaw++, startCol + dist, colour);
	
	Print("����稥 ��᫥����⥫쭮�� ���� (��� jrPC):", startRaw, startCol, colour);
	Print(pcConf.comPortJrPC ? "��" : "���", startRaw++, startCol + dist, colour);
	
	Print("������⢮ ��ࠫ������ ���⮢:", startRaw, startCol, colour);
	Print(itoa(pcConf.lptCount, str, 10), startRaw++, startCol + dist, colour);
	
	startRaw++;
	Print("���뢠��� INT12:", startRaw++, startCol, _LIGHT_PURPLE);
	Print("��ꥬ �᭮���� ����⨢��� ����� ��������:", startRaw, startCol, colour);
	rg.h.ah = 0;
	int86(RAM_SIZE_BIOS_INT, &rg, &rg);
	Print(itoa(rg.x.ax, str, 10), startRaw, startCol + dist, colour);
	Print("��", startRaw++, startCol + dist + 5, colour);
	
	Print("�⮡� �த������, ������ ���� �������", ++startRaw, startCol, _LIGHT_PURPLE);
	Getch();
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
	BufOut[i] = '\0';
	return BufOut;
}