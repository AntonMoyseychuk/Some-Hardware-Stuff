//гр.024401-2 Мойсейчук А.О.
//ЛР-3 "Контроллер клавиатуры"

#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

enum PortsNumber
{
	PORT_20 = 0x20, PORT_60 = 0x60, 
	PORT_61 = 0x61, PORT_64 = 0x64
};

//номер вектора прерывания, вызываемого 
//нажатием клавиши клавиатуры
enum Interrupt
{
	INT9 = 0x9
};

//константы управляющих слов для портов 60h и 20h 
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

//процедура вывода скан-кодов на экран в шестнадцатеричном виде
void PrintScanCodes(char* str, unsigned char colour);

//собственный обработчик прерываний
void interrupt far InterruptHandler(void);

//функция проверки заполненности буфера
unsigned char isBufferEmpty(unsigned char bufferMask);

//процедура включения/выключения индикаторов клавиатуры
void changeIndicatorsState(unsigned char indicatorsMask);

//процедура мигания индикаторами клавиатуры
void Blink(int blinkCount);

//функция перевода из десятичной в двоичную систему
char* ToBinary(unsigned char value, char* buff, int bitCount);

//функция перевода из десятичной в шестнадцатиричную систему
char* ToHex(unsigned char value, char* buff, int bitCount);

//функция перевода младшей тетрады в шестнадцатиричную систему
char TetradaToHex(unsigned char bin);

//указатель на старый обработчик прерывания INT9
void (interrupt far *oldInt9h)(void);

//флаг выхода из программы
int isExit = 0;

int main(void)
{
	//сохранение старого обработчика прерывания INT9
	oldInt9h = getvect(INT9);
	//задание собственного обработчика прерывания INT9
	setvect(INT9, InterruptHandler);

	ClearScreen();
	PrintDeveloperInfo(0, 0, _PINK);
	PrintMenu(INFO_TABLE_H, 0, _LIGHT_TURQUOISE);
	PrintStatusWord(0, INFO_TABLE_LEN, _LIGHT_GREEN);

	while (!isExit);

	//задание старого обработчика прерывания INT9
	setvect(INT9, oldInt9h);
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

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Print("+-----------------------------------+", startRaw, startCol, colour);
	Print("|     гр.024401-2 Мойсейчук А.О.    |", startRaw + 1, startCol, colour);
	Print("|    ЛР-3 \"Контроллер клавиатуры\"   |", startRaw + 2, startCol, colour);
	Print("+-----------------------------------+", startRaw + 3, startCol, colour);
}


void PrintMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Print("+-----------------------------------+", startRaw, startCol, colour);
	Print("| 8. Помигать индикаторами          |", startRaw + 1, startCol, colour);
	Print("| 9. Выход                          |", startRaw + 2, startCol, colour);
	Print("+-----------------------------------+", startRaw + 3, startCol, colour);
}

void PrintStatusWord(unsigned char startRaw, unsigned char startCol,
	unsigned char colour)
{
	Port ports[] =
	{
		{ "| Регистр данных(60h): ", PORT_60 },
		{ "| Регистр состояния(64h): ", PORT_64 },
		{ "| Регистр команд(61h): ", PORT_61 },
		{ "| Регистр ISR(20h): ", PORT_20 },
	};
	int i, maxLen = strlen(ports[1].message);

	//символьные массивы для двоичного шестнадцатиричного
	//представления состояния каналов и регистров
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
			//при обращении к порту 20h нужно сначала
			//послать управляющее слово, а затем считать содержимое регистра
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

	//очистка динамически выделенной памяти
	free(bin);
	free(hex);
}

void PrintScanCodes(char* str, unsigned char colour)
{
	static unsigned char rawPos = 2 * INFO_TABLE_H + 1;
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

//процедура вызывается только при возникновении прерывания INT9,
//вызванного нажатием клавиши клавиатуры
void interrupt far InterruptHandler(void)
{
	unsigned char scanCode;
	char* hex = malloc(3);
	PrintStatusWord(0, INFO_TABLE_LEN, _LIGHT_GREEN);
	
	//считывание скан кода нажатой клавиши
	scanCode = inport(PORT_60);
	if (scanCode == SYMBOL_9)
	{
		isExit = 1;
	}
	else if (scanCode == SYMBOL_8)
	{
		Blink(10); //мигаем 10 раз
	}

	PrintScanCodes(ToHex(scanCode, hex, 8), _LIGHT_PURPLE);
	free(hex);

	//запись в порт 20h управляющего слова 0010 0000
	//оно сообщает об обычном конце прерывания
	outportb(PORT_20, 0x20);
}

unsigned char isBufferEmpty(unsigned char buffer)
{
	//в зависимости от переданной маски
	//проверяется наличие данных во входном/выходном буфере
	return ((inportb(PORT_64) & buffer) == 0x0);
}

void changeIndicatorsState(unsigned char indicatorsMask)
{
	//проверка входного буфера на пустоту
	while (!isBufferEmpty(INPUT_BUFFER));
	//запись управляющего слова на включения/выключения индикаторов
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
		//при наложении маски 0x01 результатом может быть
		//только 1 или 0
		buff[i] = (value & 0x01 ? '1' : '0');

		//переход к следующему разряду через побитовый
		//сдвиг вправо на 1 бит
		value = value >> 1;
	}
	buff[bitCount] = '\0';
	return buff;
}

char* ToHex(unsigned char value, char* buff, int bitCount)
{
	//определяем количество тетрад
	//в зависимости от разрядности числа
	unsigned char symbCount = bitCount / 4;
	int i;
	
	//перевод каждой из тетерад в шестнадцатиричный формат
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
	//при наложении маски 1111 на младшую тетраду
	//получается число от 0 до 15 включительно
	return (bin < 10) ? '0' + bin : 'A' + bin - 10;
}
