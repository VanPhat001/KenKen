#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "graphics.h"

#define WIDTH 4
#define HEIGHT 2
#define SCREEN_MAX_X 100
#define SCREEN_MAX_Y 40
#define SCREEN_MENU_MAX_X 50
#define SCREEN_MENU_MAX_Y 8
#define true 1
#define false 0
#define check_const 0.3
#define maxN 9

typedef struct {
	int x, y;
} toado;

typedef struct {
	toado firstElement;
	int operation; // 0-1-2-3 | chia-nhan-tru-cong
	int result;
} thongtin;

typedef struct {
	thongtin group[maxN * maxN / 2];
	int map[maxN][maxN]; 
	int block[maxN][maxN];
	int block_size;
} trangthai_hoanthanh;

typedef struct {
	toado cursor;
	int map[maxN][maxN];
} thongtin_trochoi;

trangthai_hoanthanh finishStatus;
thongtin_trochoi gameInfor;
char pheptoan[] = { '/', 'x', '-', '+' };
int hang[maxN][maxN], cot[maxN][maxN];
int n = 5;

int move[][2] = {-1, 0, 0, -1, 0, 1, 1, 0};
const char MENU[][35] = { "Play game", "Setting", "Quit" };

void print_arr(int arr[][maxN])
{
//	gotoxy(0, 0);
	printf("\n");
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			textcolor(arr[i][j] % 9 + 7);
			printf("%3d ", arr[i][j]);
		}
		printf("\n");
	}
}

void chuyen_toado(int *x, int *y)
{
	int startX = get_consoleSize_cols()/2 - (n * (WIDTH + 1) + 1) / 2;
	int startY = get_consoleSize_rows()/2 - (n * (HEIGHT + 1) + 1 + 2) / 2;
	*x = startX + *x * (WIDTH + 1) + 1;
	*y = startY + *y * (HEIGHT + 1) + 1 ;
}

void print_result()
{
	int i;
	for (i = 1; i <= finishStatus.block_size; i++)
	{
		int y = finishStatus.group[i].firstElement.y;
		int x = finishStatus.group[i].firstElement.x;
		chuyen_toado(&x, &y);
//		print_number(x - 1, y, finishStatus.group[i].result, LIGHTBLUE);
		gotoxy(x, y);
		textcolor(LIGHTBLUE);
		setbackground(i % 5 + 9);
		printf("%d%c", finishStatus.group[i].result, pheptoan[finishStatus.group[i].operation]);
	}
}

void print_map(int map[][maxN], toado cursor)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			int	color = ((cursor.x == j && cursor.y == i) ? 1 : WHITE);
			setbackground(finishStatus.block[i][j] % 5 + 9);
			textcolor(color);
			int y = i, x = j;
			chuyen_toado(&x, &y);
			
			gotoxy(x, y);
			int k, left = 0, up = 0;
			for (k = 0; k < WIDTH; k++)
			{
				printf(" ");
			}
			gotoxy(x, y + 1);
			printf(" %2d ", map[i][j]);
			
			if (j > 0 && finishStatus.block[i][j] == finishStatus.block[i][j - 1])
			{
				gotoxy(x - 1, y);
				printf(" ");
				gotoxy(x - 1, y + 1);
				printf(" ");
				left = 1;
			}
			
			if (i > 0 && finishStatus.block[i][j] == finishStatus.block[i - 1][j])
			{
				gotoxy(x, y - 1);
				printf("    ");
				up = 1;
			}
			
			if (i > 0 && j > 0 && finishStatus.block[i][j] == finishStatus.block[i - 1][j - 1] & left == 1 && up == 1)
			{
				gotoxy(x - 1, y - 1);
				printf(" ");
			}
		}
	}
	print_result();
}

void fill_border_with_background()
{
	int i, j, x = 0, y = 0;
	chuyen_toado(&x, &y);
	setbackground(LIGHTGRAY);
	for (i = 0; i <= n * (HEIGHT + 1); i++)
	{
		gotoxy(x - 1, y - 1 + i);
		for (j = 0; j <= n * (WIDTH + 1) + 1; j++)
		{
			printf(" ");
		}
	}
}

void hoanvi(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void create_arr_2D(int arr[][maxN], int value)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			arr[i][j] = value;
		}
	}
}

int out_of_range(int x, int y)
{
	return (x < 0 || y < 0 || x >= n || y >= n);
}

int alone(int arr[][maxN], int x, int y)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (out_of_range(x + move[i][1], y + move[i][0])) continue;
		if (arr[y][x] == arr[y + move[i][0]][x + move[i][1]]) return false;
	}
	return true;
}

void create_block_dequi(int x, int y, int value, int count)
{
	if (count == 0) return;
	if (out_of_range(x, y)) return;
	if (finishStatus.block[y][x] != 0) return;

	finishStatus.block[y][x] = value;

	int status = rand() % 4;
	create_block_dequi(x + move[status][1], y + move[status][0], value, count - 1);
}

void create_block()
{
	finishStatus.block[0][0] = 1;
	int i, j, d = 0;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (finishStatus.block[i][j] == 0)
			{
				d++;
				create_block_dequi(j, i, d, rand() % 2 + 2);

				if (alone(finishStatus.block, j, i))
				{
					int y, x;
					do
					{
						int status = rand() % 4;
						y = i + move[status][0];
						x = j + move[status][1];
					}
					while (out_of_range(x, y) || finishStatus.block[y][x] == 0);
					finishStatus.block[i][j] = finishStatus.block[y][x];
					d--;
				}
			}
		}
	}
	finishStatus.block_size = d;
}

void create_random_arr_1D(int arr[maxN], int arr_size)
{
	int a[maxN], i;
	for (i = 0; i < arr_size; i++)
		a[i] = i + 1;
	int a_size = arr_size;
	for (i = 0; i < arr_size; i++)
	{
		int pos = rand() % a_size;
		arr[i] = a[pos];
		hoanvi(&a[pos], &a[a_size - 1]);
		a_size--;
	}
}

void create_first_col()
{
	int i, a[maxN];
	create_random_arr_1D(a, n);
	for (i = 0; i < n; i++)
		if (a[i] == finishStatus.map[0][0])
		{
			hoanvi(&a[i], &a[0]);
			break;
		}
		
	for (i = 0; i < n; i++)
	{
		finishStatus.map[i][0] = a[i];
		
		cot[0][finishStatus.map[i][0] - 1] = 1;
		hang[0][finishStatus.map[0][i] - 1] = 1;
		
		cot[i][finishStatus.map[0][i] - 1] = 1;
		hang[i][finishStatus.map[i][0] - 1] = 1;
	}
}

int exit_loop = 0;
void create_map_dequi(int x, int y)
{	
	if (x >= n)
	{
		x = 1;
		y++;
	}
	
	if (y >= n)
	{
		exit_loop = 1;
		return;
	}
	
	int i;
	for (i = 0; i < n; i++)
	{
		if (hang[y][i] == 0 && cot[x][i] == 0)
		{
			hang[y][i] = 1;
			cot[x][i] = 1;
			finishStatus.map[y][x] = i + 1;
			
			create_map_dequi(x + 1, y);
			if (exit_loop) return;
			
			hang[y][i] = 0;
			cot[x][i] = 0;
		}
	}
}

void create_map()
{
	create_random_arr_1D(finishStatus.map[0], n); // create first line
	create_first_col();
	exit_loop = 0;
	create_map_dequi(1, 1);
}

void create_group_dequi(int x, int y, int value, int arr[], int * arr_size)
{
	if (out_of_range(x, y)) return;
	if (hang[y][x] == 0) return;
	if (value != finishStatus.block[y][x]) return;

	hang[y][x] = 0;
	arr[*arr_size] = finishStatus.map[y][x];
	(*arr_size)++;
	
	int i;
	for (i = 0; i < 4; i++)
	{
		create_group_dequi(x + move[i][1], y + move[i][0], value, arr, arr_size);
	}
}

void sort(int arr[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
		for (j = i + 1; j < n; j++)
			if (arr[i] < arr[j])
				hoanvi(&arr[i], &arr[j]);
}

int calculate(int a, int b, char operation, int * s)
{
	float kq;
	switch (operation)
	{
		case 0: // chia
			kq = 1.0 * a / b;
			if (kq != (int)kq) return 0; // phep tinh sai
			break;
		case 1: // nhan
			kq = a * b;
			if (kq > 50) return 0;
			break;
		case 2: // tru
			kq = a - b;
			if (kq < -10) return 0;
			break;
		case 3: // cong
			kq = a + b;
			break;
	}
	
	*s = (int)kq;
	return 1;
}

void complete_group(int arr[], int arr_size, int * operation, int * result)
{
	sort(arr, arr_size);
	int pheptoan;
	for (pheptoan = 0; pheptoan < 4; pheptoan++)
	{
		int i;
		new_loop:;
		*result = arr[0];
		for (i = 1; i < arr_size; i++)
		{
			if (calculate(*result, arr[i], pheptoan, result) == 0)
			{
				pheptoan++;
				goto new_loop;
			}
		}
		*operation = pheptoan;
		break;
	}
}

void create_group()
{	
	int i, j;
	
	// tai su dung mang hang nhu mang trace, hang[i][j] = 0 --> da chon | hang[i][j] =  1 --> chua chon
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (hang[i][j] == 1)
			{
				int arr[maxN*3];
				int arr_size = 0;
				create_group_dequi(j, i, finishStatus.block[i][j], arr, &arr_size);
				
				complete_group(arr, arr_size, &finishStatus.group[finishStatus.block[i][j]].operation, &finishStatus.group[finishStatus.block[i][j]].result);
				finishStatus.group[finishStatus.block[i][j]].firstElement.x = j;
				finishStatus.group[finishStatus.block[i][j]].firstElement.y = i;
			}
		}
	}
	
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			printf("%3d ", finishStatus.group[finishStatus.block[i][j]].result);
//		}
//		printf("\n");
//	}
//	printf("\n");
//	printf("\n");
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			printf("%3d ", finishStatus.group[finishStatus.block[i][j]].operation);
//		}
//		printf("\n");
//	}
}

void init()
{
	// create finish status
	create_arr_2D(hang, 0);
	create_arr_2D(cot, 0);
	create_arr_2D(finishStatus.map, 0);
	
	create_map();

	do {
		create_arr_2D(finishStatus.block, 0);
		create_block();
	} while(finishStatus.block_size < n * n * check_const);
	
	print_arr(finishStatus.map);
//	print_arr(finishStatus.block);
	
	create_group();
	
	// create gameInfor
	create_arr_2D(cot, 0);
	create_arr_2D(gameInfor.map, 0);
	gameInfor.cursor.x = 0;
	gameInfor.cursor.y = 0;
	
	fill_border_with_background();
}

int cmp_arr(int a[][maxN], int b[][maxN])
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (a[i][j] != b[i][j])
				return false;
	return true;
}

void format_range(int *x, int *y)
{
	if (*x < 0) *x = n - 1;
	if (*y < 0) *y = n - 1;
	if (*x > n - 1) *x = 0;
	if (*y > n - 1) *y = 0;
}

int play_game() // return 1 if win || return 0 if lose
{
	set_console_size(SCREEN_MAX_X, SCREEN_MAX_Y);
	init();
	
	print_map(gameInfor.map, gameInfor.cursor);
	int timeStart = time(NULL);
	int count_fillCell = 0;
	while (1)
	{
		if (kbhit())
		{
			char c = getch();
			switch(c)
			{
				case '0':
					return 0; // lose
				case 72:
					(gameInfor.cursor.y)--;
					break;
				case 75:
					(gameInfor.cursor.x)--;
					break;
				case 77:
					(gameInfor.cursor.x)++;
					break;
				case 80:
					(gameInfor.cursor.y)++;
					break;
				default:
					if ('0' <= c && c <= '9')
					{
						if (gameInfor.map[gameInfor.cursor.y][gameInfor.cursor.x] == 0) // is empty cell 
							count_fillCell++;
						gameInfor.map[gameInfor.cursor.y][gameInfor.cursor.x] = c - '0'; // push number into cell
					}
					break;
			}
			format_range(&gameInfor.cursor.x, &gameInfor.cursor.y);
			print_map(gameInfor.map, gameInfor.cursor);
			
			if (count_fillCell == n*n)
			{
				if (cmp_arr(gameInfor.map, finishStatus.map)) // check win
					return 1;
			}
		}
		
		int timePlay = time(NULL) - timeStart;
		gotoxy(get_consoleSize_cols()/2 - 6, get_consoleSize_rows()/2 + ((n - 1) * (HEIGHT + 1) + 1 + 2) / 2);
		setbackground(BLACK);
		textcolor(WHITE);
		printf("Time: %02d:%02d", timePlay/60, timePlay%60);
	}
	return 1; // win
}

void print_menu(int key, const char menu[][35], int menu_size)
{
	int i;
	gotoxy(0, 0);
	for (i = 0; i < menu_size; i++)
	{
		if (i == key) 
		{
			textcolor(YELLOW);
			printf(" >> ");
		}
		else 
		{
			textcolor(WHITE);
			printf("    ");
		}
		puts(menu[i]);
	}
}

int menu_process(const char menu[][35], int menu_size)
{
	setbackground(BLACK);
	system("cls");
	set_console_size(SCREEN_MENU_MAX_X, SCREEN_MENU_MAX_Y);
	int key = 0;
	while (1)
	{
		print_menu(key, menu, menu_size);
		char c = getch();
		switch(c)
		{
			case 13:
				return key;
			case 72:
				key--;
				break;
			case 80:
				key++;
				break;
		}
		if (key < 0) key = menu_size - 1;
		if (key > menu_size - 1) key = 0;
	}
}

int play_again()
{
	const char PLAY_AGAIN[][35] = { "Play again", "Solve", "Back to menu", "Quit" };
	return menu_process(PLAY_AGAIN, sizeof(PLAY_AGAIN) / sizeof(PLAY_AGAIN[0]));
}

void solve_map()
{
	system("cls");
	set_console_size(SCREEN_MAX_X, SCREEN_MAX_Y);
	fill_border_with_background();
	
	create_arr_2D(gameInfor.map, 0);
	gameInfor.cursor.x = -1;
	print_map(gameInfor.map, gameInfor.cursor);
	
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			int y = i;
			int x = j;
			chuyen_toado(&x, &y);
			gotoxy(x, y + 1);
			setbackground(finishStatus.block[i][j] % 5 + 9);
			textcolor(BLUE);
			printf(" %2d ", finishStatus.map[i][j]);
			Sleep(300);
		}
	}
}

void setting()
{
	system("cls");
	set_console_size(50, 4);
	while (1)
	{
		gotoxy(0, 0);
		printf("Map %dx%d                  >> n = %d <<", n, n, n);
		if (kbhit())
		{
			char c = getch();
			switch(c)
			{
				case 13:
					return;
				case 75:
					n--;
					break;
				case 77:
					n++;
					break;
			}
			if (n < 3) n = 3;
			if (n > maxN) n = maxN;
		}
	}
}

int main()
{
	srand(time(NULL));
	
	home:;
	switch(menu_process(MENU, sizeof(MENU) / sizeof(MENU[0])))
	{
		case 0: // play game;
			again:;
			if (play_game() == 1)
			{
				printf("\nyou win");
			}
			else
			{
				printf("\nyou lose");
			}
			switch(play_again())
			{
				case 0: // again
					goto again;
					break;
				case 1:
					solve_map();
					getch();
					goto home;
					break;
				case 2: // back to menu
					goto home;
					break;
				case 3: // quit
					break;
			}
			break;
		case 1: // setting
			setting();
			goto home;
			break;
		case 2: // quit
			break;
	}
	

	return 0;
}
