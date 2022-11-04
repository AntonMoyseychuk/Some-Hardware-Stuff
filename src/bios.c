//гр.024401-2 Мойсейчук А.О.
//ЛР-6 "Функции BIOS"
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
	unsigned hddAvailability : 1; //0 - наличие жёсткого диска
	unsigned fpuAvailability : 1; //1 - Математический сопроцессор
	unsigned ramBanksCount : 2; //2-3 - Количество банков ОЗУ
	unsigned videoMode : 2; //4-5 - Активный видеорежим 
	unsigned hddCount : 2; //6-7 - Число обнаруженных НГМД
	unsigned dmaAvailability : 1; //8 - Наличие контроллера DMA
	unsigned comCount : 3; //9-11 - Число асинхронных последовательных портов
	unsigned gameControllerAvailability : 1; //12 - Наличие игрового порта
	unsigned comPortJrPC: 1; //13 - последовательный порт (jrPC)
	unsigned lptCount : 2; //14-15 - Число LPT-портов
} PcConfiguration;

unsigned char IsLeapYear(unsigned int year);

//функция вводада положительного числа
int Input(unsigned char raw, unsigned char column,
	unsigned char colour);

//функция проверки вхождения значения в числовой диапозон
unsigned char IsInRange(int value, int leftBorder, int rightBorder);

//функия ввода сивола без ожидания, возвращающая ASCII код и скан-код нажатой клавиши
union REGS Getch(void);

//функия ввода сивола с ожиданием, возвращающая ASCII код и скан-код нажатой клавиши
union REGS Getchar(unsigned char raw, unsigned char column,
	unsigned char colour);

//функция ввода строки
char* Getline(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//процедура вывода строки str с позиции [raw][col]
//через прямое обращение к видеобуферу
void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//процедура очистки строки
void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol);

//процедура очистки экрана
void ClearScreen(void);

//процедура установки даты и времени
void SetDateAndTime(unsigned char sec, unsigned char min,
	unsigned char hour, unsigned char day, unsigned char month, 
		unsigned short year);

//процедура вывода теперешнего времени на экран
void PrintCurrentDateAndTime(unsigned char raw, unsigned char col, 
	unsigned char colour);

//процедура вывода информации о разработчике
void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода пунктов меню программы
void PrintStartMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//процедура вывода пунктов меню ввода
void PrintInputMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода пунктов меню работы с временем
void PrintTimeMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//функция вывода конфигурации компьютера на экран
void PrintPCConfiguration(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//функция добавления символа в буффер ввода, которая возвращает 1, если символ
//успешно добавлен, 0 - если буффер заполнен
unsigned char PushSymbol(unsigned char symbol);

//процедура удаления последнего символа из буффера ввода
void PopSymbol(void);

//процедура очистки буффера ввода
void ClearInputBuffer(void);

//процедура вывода буффера ввода на экран
void PrintInputBuffer(unsigned char raw, unsigned char col, unsigned char colour);

//функция перевода из двоично-десятичной системы счисления
//в десятичную систему счисления
unsigned int BcdToInt(unsigned int bcd);

//функция перевода из десятичной системы счисления
//в двоично-десятичную систему счисления
unsigned int IntToBcd(unsigned int value);

//bin -> hex (1 символ)
//получить 1 hex-символ из младших 4 бит Bin4Bit
char Bin4toHex1(unsigned int Bin4Bit);

//bin -> hex2 (2 символа)
//получить 2 hex-символа из младшего байта Bin4Bit
//размер BufOut >= 3
char* Bin8toHex2(unsigned int Bin8Bit, char* BufOut);

//bin -> hex4 (4 символа)
//получить 4 hex-символа из Bin16Bit
//Размер BufOut >= 5
char* Bin16toHex4(unsigned int Bin16Bit, char* BufOut);

//получить 8 bin-символов из младшего байта Bin8Bit
//Размер BufOut >= 9
char* Bin8toBinSym8(unsigned int Bin8Bit, char* BufOut);

//буффер ввода
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
						
						Print("Нажмите любую клавишу:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						ch = Getch();
						
						PushSymbol(ch.h.al);
						PrintInputBuffer(INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);
						ClearInputBuffer();
						
						Print("Скан-код:", INFO_TABLE_H + 2, 0, _LIGHT_PURPLE);
						Print(Bin8toHex2(ch.h.ah, scanCode), INFO_TABLE_H + 2, 25, _BRIGHT_WHITE); 
						Print("Чтобы продолжить, нажмите любую клавишу", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
						Getch();
						break;
					case '2':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);
						
						Print("Нажмите любую клавишу:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						ch = Getchar(INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);
						
						Print("Скан-код:", INFO_TABLE_H + 2, 0, _LIGHT_PURPLE);
						Print(Bin8toHex2(ch.h.ah, scanCode), INFO_TABLE_H + 2, 25, _BRIGHT_WHITE); 
						
						Print("Чтобы продолжить, нажмите любую клавишу", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
						Getch();
						break;
					case '3':
						ClearScreen();
						PrintDeveloperInfo(0, 0, _PINK);

						Print("Введите строку:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						Getline(str, INFO_TABLE_H + 1, 25, _BRIGHT_WHITE);

						Print("Введенная строка:", INFO_TABLE_H + 2, 0, _LIGHT_PURPLE);
						Print(str, INFO_TABLE_H + 2, 25, _BRIGHT_WHITE);

						Print("Чтобы продолжить, нажмите любую клавишу", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
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

						Print("Текущие дата и время:", INFO_TABLE_H + 1, 0, _LIGHT_PURPLE);
						Print("Чтобы продолжить, нажмите любую клавишу", INFO_TABLE_H + 4, 0, _LIGHT_PURPLE);
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
							Print("Введите секунды:", i, 0, _BRIGHT_WHITE);
							sec = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(sec, 0, 59));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("Введите минуты:", i, 0, _BRIGHT_WHITE);
							min = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(min, 0, 59));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("Введите часы:", i, 0, _BRIGHT_WHITE);
							hour = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(hour, 0, 23));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("Введите год(2000 - 2030):", i, 0, _BRIGHT_WHITE);
							year = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(year, 2000, 2030));
						i++;

						do
						{
							ClearScreenRaw(i, 0);
							Print("Введите месяц:", i, 0, _BRIGHT_WHITE);
							month = Input(i, 30, _BRIGHT_WHITE);
						} while(!IsInRange(month, 1, 12));
						i++;

						if (IsLeapYear(year) && month == 2)
							monthDayCount[1] = 29;

						do
						{
							ClearScreenRaw(i, 0);
							Print("Введите день:", i, 0, _BRIGHT_WHITE);
							day = Input(i, 30, _BRIGHT_WHITE);

							if (!IsInRange(day, 1, monthDayCount[month - 1]))
								continue;
							else
								break;
						} while(1);
						i++;

						SetDateAndTime(sec, min, hour, day, month, year);

						Print("Чтобы продолжить, нажмите любую клавишу", ++i, 0, _LIGHT_PURPLE);
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
	//0xb8000000 - адрес начала видеобуфера
	char far* videoBuffer = (char far*)0xb8000000;
	//определение позиции начала записи данных в памяти
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
	Print("|      гр.024401-2 Мойсейчук А.О.      |", startRaw++, startCol, colour);
	Print("|         ЛР-6 \"Функции BIOS\"          |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);
}

void PrintStartMenu(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. Ввод(INT16)                       |", startRaw++, startCol, colour);
	Print("| 2. Вывести конфигурацию компьютера   |", startRaw++, startCol, colour);
	Print("| 3. Дата и время(INT1A)               |", startRaw++, startCol, colour);
	Print("| 0. Выход                             |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);
}

void PrintInputMenu(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. Ввод символа без ожидания         |", startRaw++, startCol, colour);
	Print("| 2. Ввод символа с ожиданем           |", startRaw++, startCol, colour);
	Print("| 3. Ввод строки                       |", startRaw++, startCol, colour);
	Print("| 0. Отмена                            |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);
}

void PrintTimeMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Print("+--------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. Вывести текущее время             |", startRaw++, startCol, colour);
	Print("| 2. Установить дату и время           |", startRaw++, startCol, colour);
	Print("| 0. Отмена                            |", startRaw++, startCol, colour);
	Print("+--------------------------------------+", startRaw++, startCol, colour);

}

void PrintPCConfiguration(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	union REGS rg;
	PcConfiguration pcConf;
	char* videoModes[] = { "Не используется", "цветной(40x25)", "цветной(80x25)", "монохромный(80х25)" };
	char str[17];
	unsigned char dist = 60;
	
	rg.h.ah = 0;
	int86(SYSTEM_CONFIG_BIOS_INT, &rg, &rg);
	memcpy(&pcConf, &rg.x.ax, sizeof(rg.x.ax));
	
	Print("Слово конфигурации:", startRaw, startCol, _LIGHT_PURPLE);
	Print(Bin8toBinSym8(rg.h.ah, str), startRaw, startCol + 25, colour);
	Print(Bin8toBinSym8(rg.h.al, str), startRaw, startCol + 33, colour);
	Print(Bin16toHex4(rg.x.ax, str), startRaw++, startCol + 45, colour);
	
	startRaw++;
	Print("Прерывание INT11:", startRaw++, startCol, _LIGHT_PURPLE);
	
	Print("Наличие накопителя на магнитном диске:", startRaw, startCol, colour);
	Print(pcConf.hddAvailability ? "да" : "нет", startRaw++, startCol + dist, colour);
	
	Print("Наличие математического сопроцессора:", startRaw, startCol, colour);
	Print(pcConf.fpuAvailability ? "да" : "нет", startRaw++, startCol + dist, colour);

	Print("Количество банков ОЗУ:", startRaw, startCol, colour);
	Print(itoa(pcConf.ramBanksCount, str, 10), startRaw++, startCol + dist, colour);

	Print("Режим видеоадаптера:", startRaw, startCol, colour);
	Print(videoModes[pcConf.videoMode], startRaw++, startCol + dist, colour);
	
	Print("Количество установленных HDD:", startRaw, startCol, colour);
	Print(itoa(pcConf.hddCount + 1, str, 10), startRaw++, startCol + dist, colour);
	
	Print("Наличие контроллера прямого доступа к памяти:", startRaw, startCol, colour);
	Print(pcConf.dmaAvailability ? "да" : "нет", startRaw++, startCol + dist, colour);
	
	Print("Количество асинхронных последовательных портов:", startRaw, startCol, colour);
	Print(itoa(pcConf.comCount, str, 10), startRaw++, startCol + dist, colour);
	
	Print("Наличие игрового порта:", startRaw, startCol, colour);
	Print(pcConf.gameControllerAvailability ? "да" : "нет", startRaw++, startCol + dist, colour);
	
	Print("Наличие последовательного порта (для jrPC):", startRaw, startCol, colour);
	Print(pcConf.comPortJrPC ? "да" : "нет", startRaw++, startCol + dist, colour);
	
	Print("Количество параллельных портов:", startRaw, startCol, colour);
	Print(itoa(pcConf.lptCount, str, 10), startRaw++, startCol + dist, colour);
	
	startRaw++;
	Print("Прерывание INT12:", startRaw++, startCol, _LIGHT_PURPLE);
	Print("Объем основной оперативной памяти компьютера:", startRaw, startCol, colour);
	rg.h.ah = 0;
	int86(RAM_SIZE_BIOS_INT, &rg, &rg);
	Print(itoa(rg.x.ax, str, 10), startRaw, startCol + dist, colour);
	Print("КБ", startRaw++, startCol + dist + 5, colour);
	
	Print("Чтобы продолжить, нажмите любую клавишу", ++startRaw, startCol, _LIGHT_PURPLE);
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
