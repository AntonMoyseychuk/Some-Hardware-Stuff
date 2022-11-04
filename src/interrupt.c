//гр.024401-2 Мойсейчук А.О.
//ЛР-4 "Подсистема прерываний"

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

//процедура вывода строки str с позиции [raw][col]
//через прямое обращение к видеобуферу
void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//процедура очистки строки
void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol);

//процедура очистки экрана
void ClearScreen(void);

//процедура прокрутки экрана
void ScrollScreen(unsigned char startRaw);

//процедура вывода информации о разработчике
void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода пунктов программы
void PrintMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода состояния регистров
void PrintStatusWord(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода состояния регистров КП и клавиатуры
void PrintControllerInfo(Port* ports, unsigned char portsCount, unsigned char startRaw,
	unsigned char startCol, unsigned char colour);

//процедура вывода информации об обрабатываемых прерываниях
void PrintInterruptionsInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода регистра данных клавиатуры
void PrintPort60Info(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура вывода скан-кодов на экран в шестнадцатеричном виде
void PrintScanCodes(char* str, unsigned char colour);

//процедура обработки прерываний системного таймера
void interrupt far timerHandler(void);

//процедура обработки прерываний клавиатуры
void interrupt far keyBoardHandler(void);

//процедура обработки прерываний RTC
void interrupt far rtcHandler(void);

//процедура обработки прерывания при ошибке деления на 0
void interrupt far zeroDivHandler(void);

//функция деления одного числа на другое
int Divide(int divisible, int divider);

//процедура, отображающая на экране красный прямоугольник - 
//признак возникновения прерывания
void PrintInterruptIndicator(unsigned char raw, unsigned char col, unsigned char colour,
	unsigned short duration);

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


//указатель на функцию обработки прерываний системного таймера
void interrupt far(*oldTimerHandler)(void);
//указатель на функцию обработки прерываний клавиатуры
void interrupt far(*oldKBHandler)(void);
//указатель на функцию обработки прерываний RTC
void interrupt far(*oldRTCHandler)(void);
//указатель на функцию обработки перывания при делении на 0
void interrupt far(*oldZeroDivHandler)(void);

//флаги состояния, объявленные с ключевым словом volatile,
//что говорит компилятору об изменении данных переменных
//из сторонних потоков исполнения программы
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

	//запрет вызовов прерываний
	disable();

	//установка нового обработчика прерываний клавиатуры
	oldKBHandler = getvect(KEY_BOARD_INT);
	setvect(KEY_BOARD_INT, keyBoardHandler);

	//установка нового обработчика прерываний таймера
	oldTimerHandler = getvect(SYSTEM_TIMER_INT);
	setvect(SYSTEM_TIMER_INT, timerHandler);

	//установка нового обработчика прерывания при делении на 0
	oldZeroDivHandler = getvect(ZERO_DIV_INT);
	setvect(ZERO_DIV_INT, zeroDivHandler);

	//установка нового обработчика прерывания часов реального времени
	oldRTCHandler = getvect(RTC_INT);
	setvect(RTC_INT, rtcHandler);

	//Разрешение прерывания IRQ8 часов реального времени
	outp(PORT_70, RTC_STATE_B_REGISTER);
	//Запись нового значения в данный регистр. 40h -> 0100 0000
	outp(PORT_71, inp(PORT_71) | 0x40);

	//разрешение вызовов прерываний
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
			Print("ДЕЛЕНИЕ НА 0", 3 * INFO_TABLE_H - 1, 0, _LIGHT_BLUE);
			PrintInterruptIndicator(ZERO_INDICATOR_RAW, SCREEN_LEN - 2,
				_RED << 4, 350);
            ClearScreenRaw(3 * INFO_TABLE_H - 1, 0);
			zeroInt = 0;
		}
	}

	disable();

	//установка старых обработчиков прерываний
	setvect(KEY_BOARD_INT, oldKBHandler);
	setvect(SYSTEM_TIMER_INT, oldTimerHandler);
	setvect(ZERO_DIV_INT, oldZeroDivHandler);
	setvect(RTC_INT, oldRTCHandler);

	//восстановление регистра состояния B часов реального времени
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

	//определение следующей после начала строки видеобуфера
	char far* buffSecondLineStart =
		(char far*)(vedeoBuffStart + SYMBOL_SIZE * SCREEN_LEN);

	_fmemmove(vedeoBuffStart, buffSecondLineStart,
		(SCREEN_H - 1) * SCREEN_LEN * SYMBOL_SIZE);

	ClearScreenRaw(SCREEN_H - 1, 0);
}

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Print("+--------------------------------------+", startRaw, startCol, colour);
	Print("|      гр.024401-2 Мойсейчук А.О.      |", startRaw + 1, startCol, colour);
	Print("|     ЛР-4 \"Подсистема прерываний\"     |", startRaw + 2, startCol, colour);
	Print("+--------------------------------------+", startRaw + 3, startCol, colour);
}

void PrintMenu(unsigned char startRaw, unsigned char startCol, unsigned char colour)
{
	Print("+--------------------------------------+", startRaw, startCol, colour);
	Print("| 1. Деление на 0                      |", startRaw + 1, startCol, colour);
	Print("| 0. Выход                             |", startRaw + 2, startCol, colour);
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

	//вывод регистров ведущей микросхемы контроллера прерываний
	PrintControllerInfo(ports, portsCount, startRaw, startCol + infoCollumnLen + 5, colour);
	for (i = 0; i < portsCount; i++)
	{
		//переход к портам ведомой микросхемы контроллера прерываний
		ports[i].number += ((ports[i].number == PORT_20 || ports[i].number == PORT_21) ? 0x80 : 0x0);
	}
	//вывод регистров ведомой микросхемы контроллера прерываний
	PrintControllerInfo(ports, portsCount, startRaw, startCol + 2.5 * infoCollumnLen + 5, colour);

	PrintInterruptionsInfo(startRaw, startCol + 4 * infoCollumnLen + 5, colour);

	//вывод данных из буфера клавиатуры
	PrintPort60Info(startRaw + 3, startCol + infoCollumnLen + 5, colour);

	Print("+------------------------------------------------------------------------------+",
		startRaw + portsCount, startCol, colour);
}

void PrintControllerInfo(Port* ports, unsigned char portsCount, unsigned char startRaw,
	unsigned char startCol, unsigned char colour)
{
	int i;
	//символьные массивы для двоичного шестнадцатиричного
	//представления состояния каналов и регистров
	char bin[9], hex[3];
	unsigned char state;

	for (i = 0; i < portsCount; i++)
	{
		//если для чтения порта требуется специальная команда,
		//то выполнение зайдёт в блок if
		if (ports[i].reg != NULL) {
			outp(ports[i].number, ports[i].reg);
			state = inp(ports[i].number);
		}
		else {
			state = inp(ports[i].number);
		}

		//данные буфера клавиатуры выводятся отдельно
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
		//если номер символа по горизонтали превышает
		//допустимое число символов в строке, тогда
		//перевод X позиции в начало строки
		colPos = 0;

		//если вывод достиг конца страницы, тогда
		//остаёмся в этой же строки и пролистываем страницу вниз,
		//иначе переход на новую строку
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

	Print("Tаймер(INT8):", startRaw, startCol, colour);
	Print("   ", startRaw, startCol + 15, colour);
	Print(itoa(ticks, tickStr, 10), startRaw++, startCol + 15, colour); //вывод тиков таймера

	Print("RTC(INT70):", startRaw++, startCol, colour);

	Print("Деление на 0(INT0):", startRaw++, startCol, colour);

	Print("Клавиатура (INT9):", startRaw++, startCol, colour);
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
	//запрет вызовов прерываний, пока обрабатывается данное перывание
	disable();
	timerInt = 1;

	ticks = ++ticks;
	ticks %= 1000;

	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);
	
	enable();
	//вызов старого обработчика прерывания таймера
	_chain_intr(oldTimerHandler);
}

void interrupt far keyBoardHandler(void)
{
	unsigned char scanCode;
	char hex[3];

	//запрет вызовов прерываний, пока обрабатывается данное перывание
	disable();
	kbInt = 1;

	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);

	//считывание скан кода нажатой клавиши
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
	//вызов старого обработчика прерыания клавиатуры
	_chain_intr(oldKBHandler);
}

void interrupt far rtcHandler(void)
{
	//запрет вызовов прерываний, пока обрабатывается данное перывание
	disable();
	rtcInt = 1;
	PrintStatusWord(INFO_TABLE_H, 0, _YELLOW);
	
	enable();
	//вызов старого обработчика прерыания часов реального времени
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
		//вызов программного прерывания деления на 0
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

