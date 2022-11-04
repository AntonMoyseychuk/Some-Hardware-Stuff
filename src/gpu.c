//гр.024401-2 Мойсейчук А.О.
//ЛР-7 "Работа видеоподсистемы"
#include <conio.h>
#include <dos.h>
#include <mem.h>
#include <math.h>

typedef enum
{
	SCREEN_H = 25, SCREEN_LEN = 80
} ScreenSize;

typedef enum
{
	VIDEOMODE_COUNT = 0x13,
	TEXT_MODE = 0x03, CGA_MODE = 0x04, EGA_MODE = 0x0D
} VideoMode;

typedef enum
{
	SET_VIDEOMODE = 0x00, 
	SET_COLOUR_AH = 0x0B, SET_BACKGROUND_BH = 0x00, SET_PALETTE_BH = 0x01,
	OUTPUT_PIXEL = 0x0C,
	GREEN_RED_YELLOW_PALETTE = 0x00, CYAN_PURPLE_WHITE_PALETTE = 0x01
} Settings;

typedef enum
{
	VIDEO_CONTROLLER_BIOS_INT = 0x10
} Interrupts;

typedef enum
{
	SYMBOL_SIZE = 2
} Symbol;

typedef enum
{
	_BLACK = 0x0, _BLUE, _GREEN, _TURQUOISE,
	_RED, _PURPLE, _BROWN, _WHITE, _GRAY,
	_LIGHT_BLUE, _LIGHT_GREEN, _LIGHT_TURQUOISE,
	_PINK, _LIGHT_PURPLE, _YELLOW, _BRIGHT_WHITE
} Colours;

unsigned char currentVideoPage = 0;

//процедура вывода строки str с позиции [raw][col]
//через прямое обращение к видеобуферу
void Print(char* str, unsigned char raw, unsigned char column,
	unsigned char colour);

//процедура очистки строки
void ClearScreenRaw(unsigned char rawNumber, unsigned char startCol);

//процедура очистки экрана
void ClearScreen(void);

//процедура вывода пунктов меню программы
void PrintStartMenu(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);
	
//процедура вывода информации о разработчике
void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol,
	unsigned char colour);

//процедура задния видеорежима
void SetVideomode(unsigned char mode);

//процедура рисования линии с позиции [x0, y0] до [xEnd, yEnd]
void LineBrasenhem(short x0, short y0, short xEnd, short yEnd, unsigned char colour);

//процедура рисования круга с центром в точке [xCent, yCent]
//и радиусом r
void CircleBrasenhem(short xCent, short yCent, short r, unsigned char colour);

void Square(short x0, short y0, short x1, short y1, unsigned char colour);

//процедура рисования пикселя в точке [x, y]
void SetPixel(short x, short y, unsigned char colour);

//процедура установки цвета фона
void SetBackgroundColour(unsigned char colour);

//процедура установки цветовой палитры
void SetPaletteColour(unsigned char paletteNumber);

//процедура установки видеостраницы
void SetVideoPage(unsigned char number);

//процедура вывода рисунка на экран в режиме CGA
void CGA(void);

//процедура вывода рисунка на экран в режиме EGA
void EGA(void);

int main()
{
	unsigned char isExit = 0;
	
	while(!isExit)
	{
		SetVideomode(TEXT_MODE);
		ClearScreen();
		PrintDeveloperInfo(0, 0, _LIGHT_PURPLE);
		PrintStartMenu(4, 0, _PINK);
		
		switch(getch())
		{
			case '1':
				CGA();
				break;
			case '2':
				EGA();
				break;
			case '0':
				isExit = 1;
				break;
		}
	}
	
	return 0;
}

void Square(short x0, short y0, short x1, short y1, unsigned char colour)
{
	short temp, i;
	
	if (y0 > y1)
	{
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	
	i = y0;
	for (; i <= y1; i++)
	{
		LineBrasenhem(x0, i, x1, i, colour);
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

void ClearScreen(void)
{
	char far* videoBuffer = (char far*)0xb8000000;
	_fmemset(videoBuffer, 0, SCREEN_LEN * SCREEN_H * SYMBOL_SIZE);
}

void PrintDeveloperInfo(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+----------------------------------------+", startRaw++, startCol, colour);
	Print("|      гр.024401-2 Мойсейчук А.О.        |", startRaw++, startCol, colour);
	Print("|     ЛР-7 \"Работа видеоподсистемы\"      |", startRaw++, startCol, colour);
	Print("+----------------------------------------+", startRaw++, startCol, colour);
}

void SetVideomode(unsigned char mode)
{	
	union REGS rg;
	rg.h.ah = SET_VIDEOMODE;
	rg.h.al = mode;
	
	int86(VIDEO_CONTROLLER_BIOS_INT, &rg, &rg);
}

void PrintStartMenu(unsigned char startRaw, unsigned char startCol, 
	unsigned char colour)
{
	Print("+----------------------------------------+", startRaw++, startCol, colour);
	Print("| 1. CGA (AL = 0x04, 320x200, 4 цвета)   |", startRaw++, startCol, colour);
	Print("| 2. EGA (AL = 0x0D, 320x200, 16 цветов) |", startRaw++, startCol, colour);
	Print("| 0. Выход                               |", startRaw++, startCol, colour);
	Print("+----------------------------------------+", startRaw++, startCol, colour);
}

void LineBrasenhem(short x0, short y0, short xEnd, short yEnd, unsigned char colour)
{
	short dx = abs(xEnd - x0), dy = abs(yEnd - y0);
	short p = dx > dy ? (2 * dy - dx) : (2 * dx - dy);
	short x = x0, y = y0;
	short xInc = (xEnd - x0) / dx;
	short yInc = (yEnd - y0) / dy;

	do
	{
		SetPixel(x, y, colour);
		dx > dy ? (x += xInc) : (y += yInc);
		if (p < 0)
			p += dx > dy ? 2 * dy : 2 * dx;
		else
		{
			if (dx > dy)
			{
				y += yInc;
				p += 2 * (dy - dx);
			}
			else
			{
				x += xInc;
				p += 2 * (dx - dy);
			} 
		}
	} while (dx > dy ? (x != xEnd) : (y != yEnd));
}

void CircleBrasenhem(short xCent, short yCent, short r, unsigned char colour)
{
	short x = 0, y = r, p = 1 - r;
	while (x <= y)
	{
		SetPixel(xCent + x, yCent + y, colour);
		SetPixel(xCent + x, yCent - y, colour);
		SetPixel(xCent - x, yCent + y, colour);
		SetPixel(xCent - x, yCent - y, colour);
		SetPixel(xCent + y, yCent + x, colour);
		SetPixel(xCent + y, yCent - x, colour);
		SetPixel(xCent - y, yCent + x, colour);
		SetPixel(xCent - y, yCent - x, colour);
		++x;
		if (p < 0)
			p += 2 * x + 3;
		else
		{
			--y;
			p += 2 * (x - y) + 4;
		}
	}
}

void SetPixel(short x, short y, unsigned char colour)
{
	union REGS rg;
	rg.h.ah = OUTPUT_PIXEL;
	rg.h.al = colour;
	rg.x.cx = x;
	rg.x.dx = y;
	
	int86(VIDEO_CONTROLLER_BIOS_INT, &rg, &rg);
}

void SetBackgroundColour(unsigned char colour)
{
	union REGS rg;
	rg.h.ah = SET_COLOUR_AH;
	rg.h.bh = SET_BACKGROUND_BH;
	rg.h.bl = colour;
	int86(VIDEO_CONTROLLER_BIOS_INT, &rg, &rg);
}

void SetPaletteColour(unsigned char paletteNumber)
{
	union REGS rg;
	rg.h.ah = SET_COLOUR_AH;
	rg.h.bh = SET_PALETTE_BH;
	rg.h.bl = paletteNumber;
	int86(VIDEO_CONTROLLER_BIOS_INT, &rg, &rg);
}

void SetVideoPage(unsigned char number)
{
	union REGS rg;
	rg.h.ah = 0x05;
	rg.h.al = number;
	int86(VIDEO_CONTROLLER_BIOS_INT, &rg, &rg);
}

void CGA()
{
	unsigned char colour = 1, i;
	SetVideomode(CGA_MODE);
	SetBackgroundColour(_BLUE);
	SetPaletteColour(GREEN_RED_YELLOW_PALETTE);
	
	while (!kbhit())
	{
		Square(20, 20, 100, 100, _YELLOW);
	}
	getch();
	
	SetVideomode(TEXT_MODE);
}

void EGA()
{
	unsigned char  i, j, startColour = _BLACK, currentColour;
	SetVideomode(EGA_MODE);
	SetVideoPage(0);
	
	while (!kbhit())
	{
		for (i = 0; i < 16; i++)
		{
			currentColour = (startColour + i) % 15 + 1;
			for (j = 0; j < 200; j++)
			{
				SetPixel(60 + j, 65 + i * 3, currentColour);
				SetPixel(60 + j, 66 + i * 3, currentColour);
			}
		}
		
		startColour = startColour % 15 + 1;
		delay(250);
	}
	getch();
	
	SetVideomode(TEXT_MODE);
}
