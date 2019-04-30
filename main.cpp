#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "conio2.h"
#include <stdlib.h>
#include <time.h>

using namespace std;


#define xPlansza 45
#define yPlansza 2
#define xLegenda 2
#define yLegenda 2

#define tabx (x - xPlansza - 1)
#define taby (y - yPlansza - 1)

#define RandomMax 50
#define RandomMin 30
#define MaxTries (n*n)

#define Up_Key 72
#define Down_Key 80
#define Left_Key 75
#define Right_Key 77
#define Uppercase -32
#define ESC 27
#define Enter 13
#define Space ' '
#define Make1 49
#define Make0 48
#define N_KEY 110
#define KROPKA 46
#define O_KEY 111
#define P_KEY 112
#define Q_KEY 113
#define R_KEY 114
#define K_KEY 107

struct plansza
{
	int able; //0-na stałe 1-mozna pisac 2-napisana przez gracza
	char value; //wartosc: '0' '1' '.'
};


void wypiszLegende(int x, int y, int zero, int zn);
void wczytajPlansze(plansza** tab, int N);
void wypiszRamke(int N);
void wypiszPlansze(int N, plansza** tab);
int Reg1(char wyraz, int x, int y, plansza** tab, int N);
int Reg2(char wyraz, int x, int y, plansza** tab, int N);
int Reg3(char wyraz, int x, int y, plansza** tab, int N);
int Reg3x(char wyraz, int x, int y, plansza** tab, int N);
int Reg3y(char wyraz, int x, int y, plansza** tab, int N);
void writeValue(plansza** tab, int N, int x, int y, char znak);
plansza** makeBoard(int n);
plansza** clearBoard(int n, plansza** tab);
plansza** randomBoard(int n, plansza** tab);
void hint(plansza** tab, int x, int y, int N);
void freeBoard(plansza** tab, int N);
plansza** newBoard(plansza** tab, int *N);
plansza** defaultBoard(plansza** mtab, int N);
void markError(plansza** tab, int N);


int main() {
	int N = 12;
	int x = (1 + xPlansza), y = (1 + yPlansza); // współrzędne kursora
	int zn = 0; // przycisk z klawiatury
	int	zero = 0; // dla znaków specjalnych jak strzałki
	srand(time_t(NULL));

	_setcursortype(0);//nie mryga kursor
	settitle("Adrian Szulc 171595");

	plansza** tab = makeBoard(N);
	wczytajPlansze(tab, N);
	
	int showHint = 0;
	int showError = 0;
	
	do {
		clrscr();
		textbackground(BLACK); // czarny kolor tła
		textcolor(7); // kolor tekstu na lightgray

		wypiszLegende(x,y,zero,zn);
		wypiszRamke(N);
		wypiszPlansze(N,tab);

		if (showHint == 1)
		{
			hint(tab, x, y, N);
			showHint = 0;
		}

		if (showError == 1)
		{
			markError(tab, N);
			showError = 0;
		}

		//rysowanie planszy
		

		gotoxy(x, y);
		textcolor(LIGHTGRAY);
		textbackground(YELLOW);
		if (tab[tabx][taby].able == 0)
			textcolor(DARKGRAY);
		else if (tab[tabx][taby].able == 1)
			textcolor(LIGHTGREEN);
		else textcolor(LIGHTCYAN);

		putch(tab[tabx][taby].value);//Kursor


		textbackground(BLACK);
		zero = 0;
		zn = getch();
		//Sterowanie
		if(zn == 0) {
			zero = 1;		
			zn = getch();
			if (zn == Up_Key) { if (y != (yPlansza + 1))y--; else y += (N - 1);}
			else if (zn == Down_Key) { if (y != (yPlansza + N))y++; else y -= (N - 1); }
			else if (zn == Left_Key) { if (x != (xPlansza + 1))x--; else x += (N - 1); }
			else if (zn == Right_Key) { if (x != (xPlansza + N))x++; else x -= (N - 1); }
		} 
		else if (zn == Make0)
			writeValue(tab, N, x, y, '0');
		else if (zn == Make1)
			writeValue(tab, N, x, y, '1');
		else if (zn == KROPKA)
		{
			if (tab[tabx][taby].able != 0)
			{
				tab[tabx][taby].value = '.';
				tab[tabx][taby].able = 1;
			}
		}
		else if (zn == N_KEY || zn == (N_KEY Uppercase))
		{
			x = (1 + xPlansza), y = (1 + yPlansza);
			tab = defaultBoard(tab, N);
			N = 12;
		}
		else if (zn == O_KEY || zn == (O_KEY Uppercase))
		{
			clearBoard(N, tab);
			randomBoard(N, tab);
		}
		else if (zn == P_KEY || zn == (P_KEY Uppercase))showHint = 1;
		else if (zn == R_KEY || zn == (R_KEY Uppercase))
		{
			x = (1 + xPlansza), y = (1 + yPlansza);
			tab = newBoard(tab, &N);
		}
		else if (zn == K_KEY || zn == (K_KEY Uppercase))showError = 1;
	} while (zn != ESC);
	freeBoard(tab,N);
	return 0;
}



void wypiszLegende(int x, int y, int zero, int zn)
{
	char txt[32];
	gotoxy(xLegenda, yLegenda);
	cputs("Adrian Szulc 171595");

	gotoxy(xLegenda, (yLegenda + 1));
	cputs("Zaimplementowane punkty:");

	gotoxy(xLegenda, (yLegenda + 2));
	cputs("a,b,c,d,e,f,g,h,i,j,k");

	gotoxy(xLegenda, (yLegenda + 3));
	cputs("Esc = wyjscie");

	gotoxy(xLegenda, (yLegenda + 4));
	cputs("Strzalki = poruszanie");

	gotoxy(xLegenda, (yLegenda + 5));
	cputs("N = nowa gra");

	gotoxy(xLegenda, (yLegenda + 6));
	cputs("1/0/. = Wpisywanie wartosci");

	gotoxy(xLegenda, (yLegenda + 7));
	cputs("O = Losowa plansza");

	gotoxy(xLegenda, (yLegenda + 8));
	cputs("P = Prosta podpowiedz");

	gotoxy(xLegenda, (yLegenda + 9));
	cputs("R = Zmiana rozmiaru planszy");

	gotoxy(xLegenda, (yLegenda + 10));
	cputs("K = Pole bez odpowiedzi");

	gotoxy(xLegenda, (yLegenda + 14));
	sprintf(txt, "Kursor na planszy: x:%02d y:%02d", (x - xPlansza - 1), (y - yPlansza - 1));
	cputs(txt);

	gotoxy(xLegenda, (yLegenda + 15));
	sprintf(txt, "Kursor na Konsoli: x:%02d y:%02d", x, y);
	cputs(txt);

}

void wczytajPlansze(plansza** tab, int N)
{
	int a = 0, b = 0;
	char znaczek;
	FILE *plik;
	plik = fopen("plansza12x12.txt", "r");
	if (plik == NULL) {};
	while (fscanf(plik, "%c", &znaczek) == 1)
	{
		if (znaczek == '.')tab[a][b].able = 1;
		else tab[a][b].able = 0;
		tab[a][b].value = znaczek;
		if (a == (N - 1))
		{
			b++;
			a = 0;
		}
		else a++;
	}


	fclose(plik);

}

void wypiszRamke(int N)
{
	textcolor(14);
	gotoxy(xPlansza, yPlansza);
	for (int i = 0; i < (N + 2); i++)
	{
		putch('#');
	}
	gotoxy(xPlansza, (1 + yPlansza + N));
	for (int i = 0; i < (N + 2); i++)
	{
		putch('#');
	}
	gotoxy(xPlansza, (1 + yPlansza));
	for (int i = 0; i < N; i++)
	{
		gotoxy(xPlansza, (1 + yPlansza + i));
		putch('#');
		gotoxy((1 + xPlansza + N), (1 + yPlansza + i));
		putch('#');
	}
	textcolor(7);
}

void wypiszPlansze(int N, plansza** tab)
{
	for (int j = 0; j < N; j++)
	{
		gotoxy((1 + xPlansza), (j + 1 + yPlansza));
		for (int i = 0; i < N; i++)
		{
			if (tab[i][j].able == 1)textcolor(LIGHTGREEN); // modyfikowalne na zielono, jako zachęta
			else if (tab[i][j].able == 0)textcolor(DARKGRAY);//nie modyfikowalne na szaro
			else if (tab[i][j].able == 2)textcolor(LIGHTCYAN);//zmodyfikowan na niebiesko
			putch(tab[i][j].value);
			textcolor(7);
		}
	}
	int count = 0;
	char txt[10];
	gotoxy((xPlansza - 1), (yPlansza + N + 3));
	cputs("0:");
	gotoxy((xPlansza - 1), (yPlansza + N + 5));
	cputs("1:");
	gotoxy((xPlansza + N + 3), (yPlansza));
	cputs("0:");
	gotoxy((xPlansza + N + 6), (yPlansza));
	cputs("1:");
	textbackground(CYAN);
	//kolumny
	gotoxy((xPlansza + 1), (yPlansza + N + 3));
	for (int i = 0; i < N; i++)
	{
		count = 0;
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j].value == '0')count++;

		}
		sprintf(txt,"%d",count);
		cputs(txt);
	}
	gotoxy((xPlansza + 1), (yPlansza + N + 5));
	for (int i = 0; i < N; i++)
	{
		count = 0;
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j].value == '1')count++;

		}
		sprintf(txt, "%d", count);
		cputs(txt);
	}

	//wiersze
	for (int i = 0; i < N; i++)
	{
		count = 0;
		for (int j = 0; j < N; j++)
		{
			if (tab[j][i].value == '0')count++;

		}
		gotoxy((xPlansza + N + 4), (yPlansza + i + 1));
		sprintf(txt, "%d", count);
		cputs(txt);
	}


	for (int i = 0; i < N; i++)
	{
		count = 0;
		for (int j = 0; j < N; j++)
		{
			if (tab[j][i].value == '1')count++;

		}
		gotoxy((xPlansza + N + 7), (yPlansza + i + 1));
		sprintf(txt, "%d", count);
		cputs(txt);
	}
	textbackground(BLACK);
}

int Reg1(char wyraz, int x, int y, plansza** tab, int N)
{
	//kolumny
	if ((x != xPlansza + N) && (x != xPlansza + N - 1))
	{
		if ((wyraz == tab[tabx + 1][taby].value) && (wyraz == tab[tabx + 2][taby].value))return 0;
	}
	if ((x != xPlansza + 1) && (x != xPlansza + N))
	{
		if ((wyraz == tab[tabx - 1][taby].value) && (wyraz == tab[tabx + 1][taby].value))return 0;
	}
	if ((x != xPlansza + 1) && (x != xPlansza + 2))
	{
		if ((wyraz == tab[tabx - 1][taby].value) && (wyraz == tab[tabx - 2][taby].value))return 0;
	}

	//wiersze
	if ((y != yPlansza + N) && (y != yPlansza + N - 1))
	{
		if ((wyraz == tab[tabx][taby + 1].value) && (wyraz == tab[tabx][taby + 2].value))return 0;
	}
	if ((y != yPlansza + 1) && (y != yPlansza + N))
	{
		if ((wyraz == tab[tabx][taby - 1].value) && (wyraz == tab[tabx][taby + 1].value))return 0;
	}
	if ((y != yPlansza + 1) && (y != yPlansza + 2))
	{
		if ((wyraz == tab[tabx][taby - 1].value) && (wyraz == tab[tabx][taby - 2].value))return 0;
	}

	return 1;//1 dla dobrze 0 dla źle
}

int Reg2(char wyraz, int x, int y, plansza** tab, int N)
{
	//wierz 
	int liczba = ((N / 2));
	for (int i = 0; i < N; i++)
	{
		if (i == taby)continue;
		if (tab[tabx][i].value == wyraz)liczba -= 1;
	}
	if (liczba <= 0)return 0;

	//kolumna
	liczba = ((N / 2));
	for (int i = 0; i < N; i++)
	{
		if (i == tabx)continue;
		if (tab[i][taby].value == wyraz)liczba -= 1;
	}
	if (liczba <= 0)return 0;
	else return 1;
}

int Reg3(char wyraz, int x, int y, plansza** tab, int N)
{
	int licznik = 0;
	//kolumny
	for (int licznik = 0; licznik < N; licznik++)
	{
		if (licznik == tabx)
			continue;

		for (int i = 0; i <= N; i++)
		{
			if (i == N)return 0;
			else if (i == (taby))continue;
			else if (tab[tabx][i].value == '.')break;
			else if (tab[tabx][i].value != tab[licznik][i].value)break;
		}
	}

	//wiersze
	for (int licznik = 0; licznik < N; licznik++)
	{
		if (licznik == taby)
			continue;

		for (int i = 0; i <= N; i++)
		{
			if (i == N)return 0;
			else if (i == tabx)continue;
			else if (tab[i][taby].value == '.')break;
			else if (tab[i][taby].value != tab[i][licznik].value)break;
		}
	}
	return 1;
}

int Reg3x(char wyraz, int x, int y, plansza** tab, int N)
{
	int licznik = 0;
	//kolumny
	for (int licznik = 0; licznik < N; licznik++)
	{
		if (licznik == tabx)
			continue;

		for (int i = 0; i <= N; i++)
		{
			if (i == N)return (licznik + 1);
			else if (i == (taby))
			{
				if (wyraz != tab[licznik][i].value)break;
			}
			else if (tab[tabx][i].value == '.')break;
			else if (tab[tabx][i].value != tab[licznik][i].value)break;
		}
	}
	return 0;
}

int Reg3y(char wyraz, int x, int y, plansza** tab, int N)
{
	int licznik = 0;

	//wiersze
	for (int licznik = 0; licznik < N; licznik++)
	{
		if (licznik == taby)
			continue;

		for (int i = 0; i <= N; i++)
		{
			if (i == N)return (licznik + 1);
			else if (i == tabx)
			{
				if (wyraz != tab[i][licznik].value)break;
			}
			else if (tab[i][taby].value == '.')break;
			else if (tab[i][taby].value != tab[i][licznik].value)break;
		}
	}
	return 0;
}

void writeValue(plansza** tab, int N, int x, int y, char znak)
{
	if (tab[tabx][taby].able != 0)
	{
		if (Reg1(znak, x, y, tab, N) && Reg2(znak, x, y, tab, N) && Reg3(znak, x, y, tab, N))
		{
			tab[tabx][taby].able = 2; // zostało zmodyfikoawne
			tab[tabx][taby].value = znak;
		}
	}
}

plansza** makeBoard(int n)
{
	plansza** ppp = (plansza**)malloc(n * sizeof(plansza));
	for (int i = 0; i < n; i++)
	{
		ppp[i] = (plansza*)malloc(n * sizeof(plansza));
	}
	return  ppp;
}

plansza** clearBoard(int n, plansza** tab)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			tab[i][j].value = '.';
			tab[i][j].able = 1;
		}
	}
	return tab;
}

plansza** randomBoard(int n, plansza** tab)
{
	int randAmount = (rand() % (RandomMax - RandomMin + 1)) + RandomMin;
	randAmount = randAmount * n  * n / 100;
	int currentAmount = 0;

	int x;
	int y;
	int randomValue;
	char randomChar;

	for (int i = 0; i < MaxTries; i++)//górne ograniczenie prób
	{

		x = (rand() % n);
		y = (rand() % n);
		randomValue = (rand() % 2);
		if (randomValue == 0)randomChar = '0';
		else randomChar = '1';




		if (tab[x][y].able != 0)
		{
			x += xPlansza + 1;
			y += yPlansza + 1;
			if (Reg1(randomChar, x, y, tab, n) && Reg2(randomChar, x, y, tab, n) && Reg3(randomChar, x, y, tab, n))
			{
				x -= (xPlansza + 1);
				y -= (yPlansza + 1);
				tab[x][y].able = 0;
				tab[x][y].value = randomChar;
				currentAmount++;
			}
		}


		if (currentAmount >= randAmount)break;
	}


	return tab;
}

void hint(plansza** tab, int x, int y, int N)
{
	char txt[64];
	textcolor(3);
	int i = 20;
	gotoxy(xLegenda, (yLegenda + i));
	if (tab[tabx][taby].able == 0)
		cputs("To pole jest niemodyfikowalne");
	else
	{
		if (Reg1('0', x, y, tab, N) && Reg2('0', x, y, tab, N) && Reg3('0', x, y, tab, N))
		{
			if (Reg1('1', x, y, tab, N) && Reg2('1', x, y, tab, N) && Reg3('1', x, y, tab, N))
				cputs("Mozliwe wartosci: 0 i 1");
			else
				cputs("Mozliwa wartosc: 0");
		}
		else if (Reg1('1', x, y, tab, N) && Reg2('1', x, y, tab, N) && Reg3('1', x, y, tab, N))
			cputs("Mozliwa wartosc: 1");
		else cputs("Brak mozliwych wartosci!");

		textcolor(4);

		i++;
		gotoxy(xLegenda, (yLegenda + i));

		if (Reg1('0', x, y, tab, N) == 0)
		{
			cputs("Wpisanie '0' bedzie sprzeczne z 1 regula");
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg2('0', x, y, tab, N) == 0)
		{
			cputs("Wpisanie '0' bedzie sprzeczne z 2 regula");
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg3x('0', x, y, tab, N) != 0)
		{
			sprintf(txt, "Wpisani 0 koliduje z %2d kolumna", Reg3x('0', x, y, tab, N));
			cputs(txt);
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg3y('0', x, y, tab, N) != 0)
		{
			sprintf(txt, "Wpisani 0 koliduje z %2d wierszem", Reg3y('0', x, y, tab, N));
			cputs(txt);
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg1('1', x, y, tab, N) == 0)
		{
			cputs("Wpisanie '1' bedzie sprzeczne z 1 regula");
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg2('1', x, y, tab, N) == 0)
		{
			cputs("Wpisanie '1' bedzie sprzeczne z 2 regula");
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg3x('1', x, y, tab, N) != 0)
		{
			sprintf(txt, "Wpisani 1 koliduje z %2d kolumna", Reg3x('1', x, y, tab, N));
			cputs(txt);
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}
		if (Reg3y('1', x, y, tab, N) != 0)
		{
			sprintf(txt, "Wpisani 1 koliduje z %2d wierszem", Reg3y('1', x, y, tab, N));
			cputs(txt);
			i++;
			gotoxy(xLegenda, (yLegenda + i));
		}

	}
}

void freeBoard(plansza** tab, int N)
{
	for (int i = 0; i < N; i++)
		free(tab[i]);
	free(tab);
}

plansza** newBoard(plansza** mtab, int *new_n)
{
	char txt[32];
	int zero;
	int zn;
	freeBoard(mtab, *new_n);
	textcolor(WHITE);
	while (true)
	{
		clrscr();
		gotoxy(xLegenda, yLegenda);
		sprintf(txt, "Nowe wymiary planszy: %d x %d", *new_n, *new_n);
		cputs(txt);
		gotoxy(xLegenda, (yLegenda + 2));
		cputs("Strzalki = zmiana rozmiaru");
		gotoxy(xLegenda, (yLegenda + 3));
		cputs("Enter = Akceptuj");
		gotoxy(xLegenda, (yLegenda + 4));
		cputs("Q = wyjscie");

		zero = 0;
		zn = getch();
		if (zn == 0) {
			zero = 1;
			zn = getch();
			if ((zn == Up_Key) || (zn == Right_Key))
			{
				if (*new_n < 26)*new_n += 2;
			}
			else if ((zn == Down_Key) || (zn == Left_Key))
			{
				if (*new_n > 2)*new_n -= 2;
			}
		}
		else if (zn == Enter)break;
	}
	plansza** tab = makeBoard(*new_n);
	tab = clearBoard(*new_n, tab);
	tab = randomBoard(*new_n, tab);
	return tab;
}

plansza** defaultBoard(plansza** mtab, int N)
{
	freeBoard(mtab, N);
	plansza** tab = makeBoard(12);
	wczytajPlansze(tab, 12);
	return tab;
}

void markError(plansza** tab, int N)
{
	textbackground(RED);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (tab[i][j].value == '.')
			{
				if (Reg1('0', (i + xPlansza + 1), (j + yPlansza + 1), tab, N)==0 || Reg2('0', (i + xPlansza + 1), (j + xPlansza + 1), tab, N)==0 || Reg3('0', (i + xPlansza + 1), (j + yPlansza + 1), tab, N)==0)
				{
					if (Reg1('1', (i + xPlansza + 1), (j + yPlansza + 1), tab, N) == 0 || Reg2('1', (i + xPlansza + 1), (j + yPlansza + 1), tab, N) == 0 || Reg3('1', (i + xPlansza + 1), (j + yPlansza + 1), tab, N) == 0)
					{
						gotoxy((i + xPlansza + 1), (j + yPlansza + 1));
						putch('.');
					}
				}
			}
		}
	}
	textbackground(BLACK);
}
