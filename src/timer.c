//гр.024401-2
//Мойсейчук А.О.
//л.р. №2, "Системный таймер персонального компьютера"

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define TIMER_FREQ 1193182
#define DYNAMIC_ON 0x03
#define DYNAMIC_OFF 0xfc

//частоты нот
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
} Note; //структура, содержащая частоту и продолжительность ноты

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

void PrintDeveloperInfo(void);//функция вывода информации о разработчике
void PrintMenu(void);//функция вывода пунктов программы
void SetCount(int frequency, int duration);//функция вывода звука
void SetDynamicState(int state);//функция включения/выключения динамика
void PrintStatusWord(void);//функция вывода состояния каналов и регистров

char* ToBinary(unsigned char value, char* buff, int bitCount);//функция перевода из десятичной в двоичную систему
char* ToHex(unsigned char value, char* buff, int bitCount);//функция перевода из десятичной в шестнадцатиричную систему
char TetradaToHex(unsigned char bin);//функция перевода младшей тетрады в шестнадцатиричную систему

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
				PrintStatusWord();//вывод состояния порта 61h, каналов и регистров до включения динамика и проигрывания мелодии
				system("pause");

				SetDynamicState(1);//включение динамика
				for (i = 0; i < sizeof(notes) / sizeof(notes[0]); i++)
				{
					SetCount(notes[i].freq, notes[i].dur);//проигрывание i-ой ноты
					if (kbhit())
					{
						getch();
						break;
					}
				}
				printf("\n");
				PrintStatusWord();//вывод состояния порта 61h, каналов и регистров после проигрывания мелодии

				SetDynamicState(0);//выключение динамика
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
	printf("|                      гр.024401-2                    |\n");
	printf("|                     Мойсейчук А.О.                  |\n");
	printf("| л.р. №2,\"Системный таймер персонального компьютера\" |\n");
	printf("+-----------------------------------------------------+\n\n");
}

void SetCount(int frequency, int duration)
{
	short value;
	if (frequency <= 0)
		return;

	outport(0x43, 0xb6);

	value = TIMER_FREQ / frequency;//определение значения для записи в регистр счетчика
	outport(0x42, (char)value);//запись младшего байта полученной частоты
	outport(0x42, (char)(value >> 8));//запись старшего байта путем побитового сдвига влево на 8 бит
	delay(duration);//задержка, в течении которой динамик выдаст звук частоты value
}

void SetDynamicState(int state)
{
	if (state == 0)
	{
		//отключаем динамик
		//DYNAMIC_OFF = 1111 1100, т.е. при побитовом "И"
		//младших 2 бита будут установлены в 0
		//(остальные останутся нетронутыми) и динамик выключится
		outport(0x61, inport(0x61) & DYNAMIC_OFF);
	}
	else
	{
		//включаем динамик
		//DYNAMIC_ON = 0000 0011, т.е. при побитовом "ИЛИ"
		//младших 2 бита будут установлены в 1
		//(остальные останутся нетронутыми) и динамик включится
		outport(0x61, inport(0x61) | DYNAMIC_ON);
	}
}

void PrintMenu()
{
	printf("1. Регистры каналов таймера\n");
	printf("2. Проигрывание мелодии\n");
	printf("3. Выход\n");
}

void PrintStatusWord()
{
	//символьные массивы для двоичного шестнадцатиричного
	//представления состояния каналов и регистров
	char *bin = (char*)malloc(17), *hex = (char*)malloc(7);
	unsigned char state, controlWord = 0xe2;
	int channelsCount = 3, i;

	printf("\t\t\t\t\tБинарный\tШестнадцатиричный\n");

	state = inport(0x61);//чтение слова состояния порта 61h
	printf("\nПорт 61h\t\t\t\t%s\t\t%s\n", ToBinary(state, bin, 8), ToHex(state, hex, 8));

	for (i = 0; i < channelsCount; i++)
	{
		controlWord += i * 2;
		//запись управляющего слова в порт 43h на считывание
		//слова состояния канала i
		//0xe2 = 1110 0010  0xe4 = 1110 0100  0xe8 = 1110 1000
		//6, 7 биты установлены в 1, значит будет считываться слово состояния
		//5 бит фиксирует значение счётчика при значении 0
		//4 бит указывает на считывание состояния при значении 0
		//биты 3-1 номер канала
		//бит 0 всегда равен 0
		outport(0x43, controlWord);
		state = inport(0x40 + i);//чтение слова состояния канала i (порт (40 + i)h)
		printf("Канал %d(Порт 4%dh)\t\t\t%s\t\t%s\n", 
			i, i, ToBinary(state, bin, 8), ToHex(state, hex, 8));
	
		//запись управляющего слова в порт 43h на считывание
		//значения регистра счётчика CE
		//0x00 - 0000 0000  0x40 = 0100 0000  0x80 = 1000 0000
		//биты 7-6 - канал
		//биты 5-4 - код команды CLC (запомнить регистр счетчика CE)
		//биты 3-0 игнорируются
		outport(0x43, 0x00 + 0x40 * i);
		//считывание младшего и старшего байтов регистра счетчика CE
		state = inp(0x40 + i) | (inp(0x40 + i) << 8);
		printf("  Регистр счетчика CE\t\t%s\t\t%s\n", ToBinary(state, bin, 16), ToHex(state, hex, 16));
	}

	//очистка динамически выделенной памяти
	free(bin);
	free(hex);
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
	buff[0] = '0';
	buff[1] = 'x';
	if (bitCount == 16)
	{
		//перевод каждой из тетерад в шестнадцатиричный формат
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
	//при наложении маски 1111 на младшую тетраду
	//получается число от 0 до 15 включительно
	return (bin < 10) ? '0' + bin : 'A' + bin - 10;
}

