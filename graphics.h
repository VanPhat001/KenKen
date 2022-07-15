#ifndef graphics_h 
#define graphics_h

#include <Windows.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

int get_consoleSize_cols()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int get_consoleSize_rows()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void setbackground(int BackC)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BackC & 0x0F) << 4);
}

void textcolor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h,c);
}

void int_str(char *s, int n)
{
	if (n == 0) 
	{
		s[0] = '\0';
		return;
	}
	char c[2] = {n % 10 + '0'};
	
	int_str(s, n / 10);
	strcat(s, c);
}

void set_console_size(int col, int line)
{
	char s_num[4];
	char cmd[30] = "MODE CON COLS=";
	int_str(s_num, col);
	strcat(cmd, s_num);
	int_str(s_num, line);
	strcat(cmd, " LINES=");
	strcat(cmd, s_num);
	system(cmd);
}
#endif
