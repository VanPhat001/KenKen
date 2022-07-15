#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

// hang so mau
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

// hang so thong tin tro choi
#define maxN 10
#define SPEED 100
#define WIDTH 4
#define HEIGHT 2

// hang so qui uoc
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define FIRST_COLOR 8
#define LAST_COLOR 14
#define TABLE_BACKROUND LIGHTGRAY
#define SCREEN_MAX_X 120
#define SCREEN_MAX_Y 43


int move[][2] = { {-1, 0}, {0, -1}, {0, 1}, {1, 0} };

int n = maxN ;
int map[maxN][maxN], block[maxN][maxN];
int hang[maxN][maxN], cot[maxN][maxN];

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

void print_char(char c, int size)
{
	int i;
	for (i = 0; i < size; i++)
		printf("%c", c);
}

void print_rectangle(int y, int x, int width, int height)
{
	gotoxy(x, y);
	print_char(197, 1);
	print_char(196, width);
	print_char(197, 1);

	int hang;
	for (hang = 1; hang <= height; hang++)
	{
		gotoxy(x, y + hang);
		print_char(179, 1);
		gotoxy(x + width + 1, y + hang);
		print_char(179, 1);
	}

	gotoxy(x, y + height + 1);
	print_char(197, 1);
	print_char(196, width);
	print_char(197, 1);
}

void print_border(int startX, int startY, int row, int col, int cell_width, int cell_height)
{
	// bat dau tai vi tri (startY, startX), voi bang row X col, kich thuoc o la cell_width X cell_height
	int hang, cot, y, x;
	for (hang = 1; hang <= row; hang++)
	{
		y = startY + (hang - 1)*(cell_height + 1);
		for (cot = 1; cot <= col; cot++)
		{
			x = startX + (cot - 1)*(cell_width + 1);
			print_rectangle(y, x, cell_width, cell_height);
			if (hang == 1)
			{
				gotoxy(x, y);
				print_char(194, 1);
			}
			else if (hang == row)
			{
				gotoxy(x, y + cell_height + 1);
				print_char(193, 1);
			}
		}

		if (hang > 1)
		{
			gotoxy(startX, y);
			print_char(195, 1);
			gotoxy(x + cell_width + 1, y);
			print_char(180, 1);
		}
	}

	y += cell_height + 1;
	x += cell_width + 1;

	gotoxy(startX, startY);
	print_char(218, 1);
	gotoxy(x, startY);
	print_char(191, 1);
	gotoxy(startX, y);
	print_char(192, 1);
	gotoxy(x, y);
	print_char(217, 1);
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
	int_str(s_num, SCREEN_MAX_X);
	strcat(cmd, s_num);
	int_str(s_num, SCREEN_MAX_Y);
	strcat(cmd, " LINES=");
	strcat(cmd, s_num);
	system(cmd);
}

int random(int first, int last)
{
	return first + rand() % (last - first + 1);
}

void hoanvi(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void print_arr1(int arr[])
{
	int i;
	printf(" ");
	for (i = 0; i < n; i++)
	{
		textcolor(arr[i]);
		printf("%3d  ", arr[i]);
	}
	printf("\n");
}

void print_arr2(int arr[][maxN])
{
	int i;
	for (i = 0; i < n; i++)
		print_arr1(arr[i]);
	printf("\n");
	Sleep(SPEED);
}

void chuyen_toado(int * x, int * y) // HEIGHT = 1
{
	*x = SCREEN_MAX_X/2 - (n*(WIDTH + 1) + 1)/2 + (*x * (WIDTH + 1) + 1);
	*y = SCREEN_MAX_Y/2 - (n*(HEIGHT + 1) + 1)/2 + (*y * (HEIGHT + 1) + 1);
}

void format_border_block()
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			int y = i, x = j, sametop = 0, sameleft = 0;
			setbackground(FIRST_COLOR + block[i][j]%(LAST_COLOR - FIRST_COLOR + 1));
			chuyen_toado(&x, &y);

			if (i - 1 >= 0 && block[i][j] == block[i - 1][j])
			{
				gotoxy(x, y - 1);
				print_char(' ', WIDTH);
				sametop = 1;
			}

			if (j - 1 >= 0 && block[i][j] == block[i][j - 1])
			{
				gotoxy(x - 1, y);
				printf(" ");
				gotoxy(x - 1, y + 1);
				printf(" ");
				sameleft = 1;
			}

			if (i - 1 >= 0 && j - 1 >= 0 && block[i][j] == block[i - 1][j - 1] && sametop == 1 && sameleft == 1)
			{
				gotoxy(x - 1, y - 1);
				printf(" ");
			}
		}
	}
}

void print_map()
{
	int i, j, y, x;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			y = i;
			x = j;
			setbackground(FIRST_COLOR + block[i][j]%(LAST_COLOR - FIRST_COLOR + 1));
			chuyen_toado(&x, &y);
			gotoxy(x, y);
			printf("    ");
			gotoxy(x, y + 1);
			printf(" %02d ", map[i][j]);
		}
	}
}

void fill_background(int startX, int startY, int width, int height, int color_background)
{
	int i;
	setbackground(color_background);
	for (i = 0; i < height; i++)
	{
		gotoxy(startX, startY + i);
		print_char(' ', width);
	}
}

void create_arr1(int arr[], int value)
{
	int i;
	for (i = 0; i < n; i++)
		arr[i] = value;
}

void create_arr2(int arr[][maxN], int value)
{
	int i;
	for (i = 0; i < n; i++)
		create_arr1(arr[i], value);
}

void create_random_arr(int arr[])
{
	int a[maxN], i;
	for (i = 0; i < n; i++)
		a[i] = i + 1;
	int a_size = n;

	for (i = 0; i < n; i++)
	{
		int pos = rand() % a_size;
		arr[i] = a[pos];
		hoanvi(&a[pos], &a[a_size - 1]);
		a_size--;
	}
}

void create_first_line(int arr[])
{
	create_random_arr(map[0]);
	int i;
	for (i = 0; i < n; i++)
	{
		hang[0][map[0][i] - 1] = 1; // in row 0 with value map[0][i] - 1
		cot[i][map[0][i] - 1] = 1;  // in col i with value map[0][i] - 1
	}
}

void create_first_col(int arr[])
{
	int i;
	for (i = 0; i < n; i++)
		if (map[0][0] == arr[i])
			hoanvi(&arr[0], &arr[i]);
	for (i = 0; i < n; i++)
	{
		map[i][0] = arr[i];
		hang[i][map[i][0] - 1] = 1; // in row i with value map[i][0] - 1
		cot[0][map[i][0] - 1] = 1;  // in col 0 with value map[i][0] - 1
	}
}

int exit_function = 0;
void create_map(int y, int x)
{
	if (x >= n)
	{
		x = 1;
		y++;
	}

	if (y >= n)
	{
		exit_function = 1;
		return;
	}

	int i;
	for (i = 0; i < n; i++)
	{
		if (hang[y][i] == 0 && cot[x][i] == 0)
		{
			// create new status
			hang[y][i] = 1; // in row y with value i
			cot[x][i] = 1;  // in col x with value i
			map[y][x] = i + 1;

			create_map(y, x + 1);
			if (exit_function) return;

			// destroy status
			hang[y][i] = 0; // in row y with value i
			cot[x][i] = 0;  // in col x with value i
		}
	}
}

void dequi_createBlock(int y, int x, int count, int value)
{
	print_map();
	if (y < 0 || x < 0 || y >= n || x >= n) return;
	if (count <= 0) return;
	if (block[y][x] != 0) return;

	block[y][x] = value;

	int status = rand() % 4; // huong di chuyen
	dequi_createBlock(y + move[status][0], x + move[status][1], count - 1, value);
}

int is_alone(int y, int x, int block[][maxN])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		int y2 = y + move[i][0];
		int x2 = x + move[i][1];
		if (block[y][x] == block[y2][x2]) return 0;
	}
	return 1;
}

void alone_process(int y, int x, int block[][maxN])
{
	int status, y2, x2;
	do
	{
		status = rand() % 4;
		y2 = y + move[status][0];
		x2 = x + move[status][1];
	}
	while (block[y2][x2] == 0 || y2 < 0 || x2 < 0 || y2 >= n || x2 >= n);
	block[y][x] = block[y2][x2];
	print_map();
}

void create_block()
{
	int i, j, d = 0;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (block[i][j] == 0)
			{
				// create 2-4 block same value
				d++;
				dequi_createBlock(i, j, random(3, 5), d);
			}

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (is_alone(i, j, block))
				alone_process(i, j, block);
}

void init()
{
	create_arr2(hang, 0);
	create_arr2(cot, 0);
	create_arr2(map, 0);
	create_arr2(block, 0);

	// create random first line
	create_arr2(map, 0);
	create_first_line(map[0]);

	// create random first col
	int arr[maxN];
	create_random_arr(arr);
	create_first_col(arr);

	// create map
	create_map(1, 1);
	create_block();

	// print
	setbackground(BLACK);
	textcolor(WHITE);
	textcolor(WHITE);
	print_map();
	format_border_block();
}

int main()
{
	srand(time(NULL));
	set_console_size(SCREEN_MAX_X, SCREEN_MAX_Y);
	
	fill_background(SCREEN_MAX_X/2 - (n*(WIDTH + 1) + 1)/2, SCREEN_MAX_Y/2 - (n*(HEIGHT + 1) + 1)/2, WIDTH*n + n + 1, HEIGHT*n + n + 1, TABLE_BACKROUND);
	textcolor(LIGHTRED);

//	print_border(0, 0, 1, 1, WIDTH*n + n - 1, HEIGHT*n + n - 1);

	textcolor(WHITE);
	init();

	gotoxy(0, SCREEN_MAX_Y - 3);

	setbackground(BLACK);
	textcolor(WHITE);
	return 0;
}
