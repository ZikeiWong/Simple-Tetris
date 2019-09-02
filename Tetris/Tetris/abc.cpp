#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "colorConsole.h" 
#include <stdio.h>
#include <iostream>
#pragma comment (lib, "Winmm.lib")

#define KEY_UP      72
#define KEY_DOWN    80
#define KEY_LEFT    75
#define KEY_RIGHT   77
#define KEY_ESC     27
#define MAPW    12     //地图的宽度
#define MAPH    20     //地图的高度

void Turn1(int a[][4], int w, int h, int *x, int y);
void Turn2(int a[][4], int w, int h, int *x, int y);   //方块转动
bool IsAvailable1(int a[], int x, int y, int w, int h);
bool IsAvailable2(int a[], int x, int y, int w, int h);//判定是否能放下
void DrawBlocks1(int a[], int w, int h, int x, int y, WORD wColors[], int nColors);
void DrawBlocks2(int a[], int w, int h, int x, int y, WORD wColors[], int nColors);


HANDLE handle;
//7种不同形状的方块
int b[7][4][4] = { { { 1 },{ 1,1,1 } },
{ { 0,2 },{ 2,2,2 } },
{ { 3,3 },{ 0,3,3 } },
{ { 0,0,4 },{ 4,4,4 } },
{ { 0,5,5 },{ 5,5 } },
{ { 6,6,6,6 } },
{ { 7,7 },{ 7,7 } }
};
WORD SQUARE_COLOR[7] = { FOREGROUND_RED | FOREGROUND_INTENSITY,
FOREGROUND_GREEN | FOREGROUND_INTENSITY,
FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

int map1[MAPH][MAPW] = { 0 }; //保存工作区的区域
int map2[MAPH][MAPW] = { 0 };
int dx1 = 12; //初始化屏幕时起始坐标
int dy1 = 3;
int dx2 = 40;
int dy2 = 3;
int key1;       //键盘输入值
int key2;
bool exit1 = 1;
bool exit2 = 1;
HANDLE hThread1;
HANDLE hThread2;

void Init()//初始化
{
	handle = initiate();
	srand((unsigned)time(NULL));
	for (int i = 0; i<20; i++)  //初始化工作区1
	{
		map1[i][0] = -2; map1[i][11] = -2;
	}
	for (int i = 0; i<12; i++)
	{
		map1[0][i] = -1; map1[19][i] = -1;
	}
	map1[0][0] = -3;
	map1[0][11] = -3;
	map1[19][0] = -3;
	map1[19][11] = -3;

	for (int i = 0; i<20; i++)  //初始化工作区2
	{
		map2[i][0] = -2; map2[i][11] = -2;
	}
	for (int i = 0; i<12; i++)
	{
		map2[0][i] = -1; map2[19][i] = -1;
	}
	map2[0][0] = -3;
	map2[0][11] = -3;
	map2[19][0] = -3;
	map2[19][11] = -3;

	//初始化屏幕边的信息显示区
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, dx1 - 8, 3 + dy1, wColors, 1, "SCORE");
	textout(handle, dx1 - 8, 7 + dy1, wColors, 1, "LEVEL");
	textout(handle, dx1 - 8, 11 + dy1, wColors, 1, "NEXT");
	textout(handle, dx2 + 26, 3 + dy1, wColors, 1, "SCORE");
	textout(handle, dx2 + 26, 7 + dy1, wColors, 1, "LEVEL");
	textout(handle, dx2 + 26, 11 + dy1, wColors, 1, "NEXT");

	wColors[0] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	DrawBlocks1(&map1[0][0], 12, 20, 0, 0, wColors, 1);
	textout(handle, dx1, dy1, wColors, 1, "◇══════════◇");
	wColors[0] = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	DrawBlocks2(&map2[0][0], 12, 20, 0, 0, wColors, 1);
	textout(handle, dx2, dy1, wColors, 1, "◇══════════◇");

	wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	textout(handle, 30, 1, wColors, 1, "按任意键开始");

	int ch = _getch();
	textout(handle, 30, 1, wColors, 1, "              ");

}

bool IsAvailable1(int a[], int x, int y, int w, int h)
{
	for (int i = y; i<y + h; i++)
		for (int j = x; j<x + w; j++)
			if (map1[i][j] && a[w*(i - y) + j - x])
				return 0;
	return 1;
}
bool IsAvailable2(int a[], int x, int y, int w, int h)
{
	for (int i = y; i<y + h; i++)
		for (int j = x; j<x + w; j++)
			if (map2[i][j] && a[w*(i - y) + j - x])
				return 0;
	return 1;
}
void DrawBlocks1(int a[], int w, int h, int x, int y, WORD wColors[], int nColors)
{
	int temp;

	for (int i = 0; i<h; i++)
		for (int j = 0; j<w; j++)
			if ((temp = a[i*w + j]) && y + i>0)
			{
				if (temp == -3)
					textout(handle, 2 * (x + j) + dx1, y + i + dy1, wColors, nColors, "◆");
				else if (temp == -2)
					textout(handle, 2 * (x + j) + dx1, y + i + dy1, wColors, nColors, "║");
				else if (temp == -1)
					textout(handle, 2 * (x + j) + dx1, y + i + dy1, wColors, nColors, "═");
				else if (temp >= 1)
					textout(handle, 2 * (x + j) + dx1, y + i + dy1, wColors, nColors, "■");
			}
}
void DrawBlocks2(int a[], int w, int h, int x, int y, WORD wColors[], int nColors)
{
	int temp;

	for (int i = 0; i<h; i++)
		for (int j = 0; j<w; j++)
			if ((temp = a[i*w + j]) && y + i>0)
			{
				if (temp == -3)
					textout(handle, 2 * (x + j) + dx2, y + i + dy2, wColors, nColors, "◆");
				else if (temp == -2)
					textout(handle, 2 * (x + j) + dx2, y + i + dy2, wColors, nColors, "║");
				else if (temp == -1)
					textout(handle, 2 * (x + j) + dx2, y + i + dy2, wColors, nColors, "═");
				else if (temp >= 1)
					textout(handle, 2 * (x + j) + dx2, y + i + dy2, wColors, nColors, "■");
			}
}
void Turn1(int a[][4], int w, int h, int *x, int y)
{
	int b[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
	int sign = 0, line = 0;
	for (int i = h - 1; i >= 0; i--)
	{
		for (int j = 0; j<w; j++)
			if (a[i][j])
			{
				b[j][line] = a[i][j];
				sign = 1;
			}
		if (sign)
		{
			line++;
			sign = 0;
		}
	}
	for (int i = 0; i<4; i++)
		if (IsAvailable1(b[0], *x - i, y, w, h))
		{
			*x -= i;
			for (int k = 0; k<h; k++)
				for (int j = 0; j<w; j++)
					a[k][j] = b[k][j];
			break;
		}
}
void Turn2(int a[][4], int w, int h, int *x, int y)
{
	int b[4][4] = { { 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } };
	int sign = 0, line = 0;
	for (int i = h - 1; i >= 0; i--)
	{
		for (int j = 0; j<w; j++)
			if (a[i][j])
			{
				b[j][line] = a[i][j];
				sign = 1;
			}
		if (sign)
		{
			line++;
			sign = 0;
		}
	}
	for (int i = 0; i<4; i++)
		if (IsAvailable2(b[0], *x - i, y, w, h))
		{
			*x -= i;
			for (int k = 0; k<h; k++)
				for (int j = 0; j<w; j++)
					a[k][j] = b[k][j];
			break;
		}
}
void ClearSquare1(int *a, int w, int h, int x, int y)
{
	WORD wColors[1] = { 0 };
	for (int i = 0; i<h; i++)
		for (int j = 0; j<w; j++)
		{
			if (a[i*w + j]>0 && (i + y>0))
			{
				textout(handle, 2 * (x + j) + dx1, y + i + dy1, wColors, 1, "  ");
			}
		}

}
void ClearSquare2(int *a, int w, int h, int x, int y)
{
	WORD wColors[1] = { 0 };
	for (int i = 0; i<h; i++)
		for (int j = 0; j<w; j++)
		{
			if (a[i*w + j]>0 && (i + y>0))
			{
				textout(handle, 2 * (x + j) + dx2, y + i + dy2, wColors, 1, "  ");
			}
		}

}
bool GameOver1()
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 7 + dx1, 10 + dy1, wColors, 1, "GAME OVER");
	//CloseHandle(hThread1);
	return 1;

}
bool GameOver2()
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 7 + dx2, 10 + dy2, wColors, 1, "GAME OVER");

	//CloseHandle(hThread2); 
	return 1;

}
void DeleteLine1(int m[][MAPW], int row)
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 2 + dx1, row + dy1, wColors, 1, "﹌﹌﹌﹌﹌﹌﹌﹌﹌﹌");
	Sleep(100);

	for (int i = row; i>1; i--)
	{
		ClearSquare1(&m[i][1], MAPW - 2, 1, 1, i);
		for (int j = 1; j<MAPW - 1; j++)
		{
			m[i][j] = m[i - 1][j];
			if (m[i][j] == 0)
				wColors[0] = 0;
			else
				wColors[0] = SQUARE_COLOR[m[i][j] - 1];
			DrawBlocks1(&m[i][j], 1, 1, j, i, wColors, 1);
		}
	}
	for (int i = 1; i<MAPW - 1; i++)
		m[1][i] = 0;
}
void DeleteLine2(int m[][MAPW], int row)
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 2 + dx2, row + dy2, wColors, 1, "﹌﹌﹌﹌﹌﹌﹌﹌﹌﹌");
	Sleep(100);

	for (int i = row; i>1; i--)
	{
		ClearSquare2(&m[i][1], MAPW - 2, 1, 1, i);
		for (int j = 1; j<MAPW - 1; j++)
		{
			m[i][j] = m[i - 1][j];
			if (m[i][j] == 0)
				wColors[0] = 0;
			else
				wColors[0] = SQUARE_COLOR[m[i][j] - 1];
			DrawBlocks2(&m[i][j], 1, 1, j, i, wColors, 1);
		}
	}
	for (int i = 1; i<MAPW - 1; i++)
		m[1][i] = 0;
}
void AddLine1(int m[][MAPW])
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	Sleep(100);
	for (int i = 1; i<MAPH - 1; i++)
	{
		ClearSquare1(&m[i][1], MAPW - 2, 1, 1, i);
		for (int j = 1; j<MAPW - 1; j++)
		{

			if (i == 18) {
				for (int f = 1; f < MAPW - 1; f++)
					m[18][f] = f % 2;
			}
			else
			m[i][j] = m[i + 1][j];
			if (m[i][j] == 0)
				wColors[0] = 0;
			else
				wColors[0] = SQUARE_COLOR[m[i][j] - 1];
			DrawBlocks1(&m[i][j], 1, 1, j, i, wColors, 1);
		}
	}
	for (int i = 1; i< MAPW - 1; i++)
		m[i][i] =0;
}
void AddLine2(int m[][MAPW])
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };

	Sleep(100);
	for (int i = 1; i<MAPH - 1; i++)
	{
		ClearSquare2(&m[i][1], MAPW - 2, 1, 1, i);
		for (int j = 1; j<MAPW - 1; j++)
		{

			if (i == 18) 
			{
				for (int f = 1; f < MAPW - 1; f++)
					m[18][f] = f % 2;
			}
			else

				m[i][j] = m[i + 1][j];
			if (m[i][j] == 0)
				wColors[0] = 0;
			else
				wColors[0] = SQUARE_COLOR[m[i][j] - 1];
			DrawBlocks2(&m[i][j], 1, 1, j, i, wColors, 1);
		}
	}
	for (int i = 1; i< MAPW - 1; i++)
		m[i][i] = 0;
}

DWORD WINAPI Player1(LPVOID lpParam)
{

	//cout << "玩家1";
	Init();

	int score = 0;  //初始化分数
	int level = 0;  //初始化游戏级别

	int Num = rand() % 7; //创建第一个方块编号
	int nextNum = Num;    //保存下一个方块编号

	int blank;  //记录每个方块起始位置
	int x = 0, y = 0;  //记录游戏开始的相对坐标

	int a[4][4] = { 0 }; //临时使用用来保存当前方块


	while (1)
	{
		for (int i = 0; i < 4; i++)          //复制方块
			for (int j = 0; j < 4; j++)
				if (a[i][j] = b[nextNum][i][j])
					blank = i;

		y = 1 - blank;
		x = 4;

		//创建下一个方块
		Num = nextNum;
		ClearSquare1(b[Num][0], 4, 4, -4, 13);
		nextNum = rand() % 7;
		WORD wColors[1] = { SQUARE_COLOR[nextNum] };
		DrawBlocks1(b[nextNum][0], 4, 4, -4, 13, wColors, 1);

		wColors[0] = SQUARE_COLOR[Num];
		DrawBlocks1(&a[0][0], 4, 4, x, y, wColors, 1);

		//显示分数信息
		char string[5];
		wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		textout(handle, 6, 5 + dy1, wColors, 1, _itoa(score, string, 10));
		textout(handle, 6, 9 + dy1, wColors, 1, _itoa(level, string, 10));

		int max_delay = 100 - 10 * level; //计算不同游戏级别的下落时间间隔

		while (1)
		{
			int delay = 0; //延迟量
			while (delay < max_delay)
			{

				switch (key1)
				{
				case 1:
				{
					key1 = 0;
					ClearSquare1(&a[0][0], 4, 4, x, y);
					Turn1(a, 4, 4, &x, y);
					wColors[0] = SQUARE_COLOR[Num];
					DrawBlocks1(&a[0][0], 4, 4, x, y, wColors, 1);
				}
				break;
				case 2:
					key1 = 0;
					delay = max_delay;
					break;
				case 3:
				{
					key1 = 0;
					if (IsAvailable1(&a[0][0], x - 1, y, 4, 4))
					{
						ClearSquare1(&a[0][0], 4, 4, x, y);
						x--;
						wColors[0] = SQUARE_COLOR[Num];
						DrawBlocks1(&a[0][0], 4, 4, x, y, wColors, 1);
					}
				}
				break;
				case 4:
				{
					key1 = 0;
					if (IsAvailable1(&a[0][0], x + 1, y, 4, 4))
					{
						ClearSquare1(&a[0][0], 4, 4, x, y);
						x++;
						wColors[0] = SQUARE_COLOR[Num];
						DrawBlocks1(&a[0][0], 4, 4, x, y, wColors, 1);
					}
				}
				break;
				case KEY_ESC:
					exit(EXIT_SUCCESS);
					break;
				}


				Sleep(8); delay++;
			}
			if (IsAvailable1(&a[0][0], x, y + 1, 4, 4)) //是否能下移
			{
				ClearSquare1(&a[0][0], 4, 4, x, y);
				y++;
				wColors[0] = SQUARE_COLOR[Num];
				DrawBlocks1(&a[0][0], 4, 4, x, y, wColors, 1);
			}
			else
			{
				if (y <= 1) {
					exit1 = GameOver1();
					if (exit1)
					{
						textout(handle, dx1, dy1 - 2, wColors, 1, "玩家2获胜");
						exit(EXIT_SUCCESS);
					}
				}   //是否结束
				for (int i = 0; i < 4; i++)     //放下方块，更新工作区
					for (int j = 0; j < 4; j++)
						if (a[i][j] && ((i + y) < MAPH - 1) && ((j + x) < MAPW - 1))
							map1[i + y][j + x] = a[i][j];

				int full, k = 0;
				for (int i = y; i < min(y + 4, MAPH - 1); i++)
				{
					full = 1;
					for (int j = 1; j < 11; j++)
						if (!map1[i][j]) full = 0;
					if (full)   //消掉一行
					{
						DeleteLine1(map1, i);
						AddLine2(map2);
						k++;
						score = score + k;
						level = min(score / 30, 9);
						max_delay = 100 - 10 * level;
					}
				}
				break;
			}
		}
	}
}
DWORD WINAPI Player2(LPVOID lpParam)
{

	//cout << "玩家2";
	Init();

	int score = 0;  //初始化分数
	int level = 0;  //初始化游戏级别

	int Num = rand() % 6; //创建第一个方块编号
	int nextNum = Num;    //保存下一个方块编号

	int blank;  //记录每个方块起始位置
	int x = 9, y = 9;  //记录游戏开始的相对坐标

	int a[4][4] = { 0 }; //临时使用用来保存当前方块


	while (1)
	{
		for (int i = 0; i<4; i++)          //复制方块
			for (int j = 0; j<4; j++)
				if (a[i][j] = b[nextNum][i][j])
					blank = i;

		y = 1 - blank;
		x = 4;

		//创建下一个方块
		Num = nextNum;
		ClearSquare2(b[Num][0], 4, 4, 13, 13);
		nextNum = rand() % 7;
		WORD wColors[1] = { SQUARE_COLOR[nextNum] };
		DrawBlocks2(b[nextNum][0], 4, 4, 13, 13, wColors, 1);

		wColors[0] = SQUARE_COLOR[Num];
		DrawBlocks2(&a[0][0], 4, 4, x, y, wColors, 1);

		//显示分数信息
		char string[5];
		wColors[0] = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		textout(handle, 26 + dx2, 5 + dy2, wColors, 1, _itoa(score, string, 10));
		textout(handle, 26 + dx2, 9 + dy2, wColors, 1, _itoa(level, string, 10));

		int max_delay = 100 - 10 * level; //计算不同游戏级别的下落时间间隔

		while (1)
		{
			int delay = 0; //延迟量
			while (delay<max_delay)
			{



				switch (key2)
				{
				case 1:
				{
					key2 = 0;
					ClearSquare2(&a[0][0], 4, 4, x, y);
					Turn2(a, 4, 4, &x, y);
					wColors[0] = SQUARE_COLOR[Num];
					DrawBlocks2(&a[0][0], 4, 4, x, y, wColors, 1);
				}
				break;
				case 2:
					key2 = 0;
					delay = max_delay;
					break;
				case 3:
				{
					key2 = 0;

					if (IsAvailable2(&a[0][0], x - 1, y, 4, 4))
					{
						ClearSquare2(&a[0][0], 4, 4, x, y);
						x--;
						wColors[0] = SQUARE_COLOR[Num];
						DrawBlocks2(&a[0][0], 4, 4, x, y, wColors, 1);
					}
				}
				break;
				case 4:
				{
					key2 = 0;
					if (IsAvailable2(&a[0][0], x + 1, y, 4, 4))
					{
						ClearSquare2(&a[0][0], 4, 4, x, y);
						x++;
						wColors[0] = SQUARE_COLOR[Num];
						DrawBlocks2(&a[0][0], 4, 4, x, y, wColors, 1);
					}
				}
				break;
				case KEY_ESC:
					exit(EXIT_SUCCESS);
					break;
				}


				Sleep(8); delay++;
			}
			if (IsAvailable2(&a[0][0], x, y + 1, 4, 4)) //是否能下移
			{
				ClearSquare2(&a[0][0], 4, 4, x, y);
				y++;
				wColors[0] = SQUARE_COLOR[Num];
				DrawBlocks2(&a[0][0], 4, 4, x, y, wColors, 1);
			}
			else
			{
				if (y <= 1) {
					exit2 = GameOver2();
					if (exit2)
					{
						textout(handle, dx1, dy1 - 2, wColors, 1, "玩家1获胜");
						exit(EXIT_SUCCESS);
					}
				}     //是否结束
				for (int i = 0; i<4; i++)     //放下方块，更新工作区
					for (int j = 0; j<4; j++)
						if (a[i][j] && ((i + y)<MAPH - 1) && ((j + x)<MAPW - 1))
							map2[i + y][j + x] = a[i][j];

				int full, k = 0;
				for (int i = y; i<min(y + 4, MAPH - 1); i++)
				{
					full = 1;
					for (int j = 1; j<11; j++)
						if (!map2[i][j]) full = 0;
					if (full)   //消掉一行
					{
						DeleteLine2(map2, i);
						AddLine1(map1);
						k++;
						score = score + k;
						level = min(score / 30, 9);
						max_delay = 100 - 10 * level;
					}
				}
				break;
			}
		}
	}


}
int main()
{

	hThread1 = CreateThread(NULL, 0, Player1, NULL, 0, NULL);//线程启动函数fun
	hThread2 = CreateThread(NULL, 0, Player2, NULL, 0, NULL);//线程启动函数fun2

	while (1)
	{
		if (_kbhit())  //用if避免按住键使方块卡住
		{
			int key = _getch();
			switch (key)
			{
			case KEY_UP:
				key1 = 1;
				break;
			case KEY_DOWN:
				key1 = 2;
				break;
			case KEY_LEFT:
				key1 = 3;
				break;
			case KEY_RIGHT:
				key1 = 4;
				break;
			case 'W':
				key2 = 1;
				break;
			case 'S':
				key2 = 2;
				break;
			case 'A':
				key2 = 3;
				break;
			case 'D':
				key2 = 4;
				break;
			}
		}
	}




	CloseHandle(hThread1);

	CloseHandle(hThread2);
	system("pause");
	return EXIT_SUCCESS;

}
