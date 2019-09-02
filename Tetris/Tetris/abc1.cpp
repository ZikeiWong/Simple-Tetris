/****************系统数据部分***********/
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "colorConsole.h"     
#pragma warning(disable: 4996)

#define KEY_UP_1      72  //按键1的ASCII码：上
#define KEY_DOWN_1   80  //按键1的ASCII码：下
#define KEY_LEFT_1    75  //按键1的ASCII码：左
#define KEY_RIGHT_1     77  //按键1的ASCII码：右
#define KEY_UP_2      87  //按键2的ASCII码：上
#define KEY_DOWN_2   83  //按键2的ASCII码：下
#define KEY_LEFT_2    65  //按键2的ASCII码：左
#define KEY_RIGHT_2     68  //按键2的ASCII码：右
#define KEY_ESC     27  //按键ASCII码：“Esc”
#define MAPW    12     //地图的宽度
#define MAPH    20     //地图的高度



/***********多线程部分***************
DWORD WINAPI ThreadFunc(LPVOID lpParam);

HANDLE hThread = CreateThread(
NULL,                          // default security attributes
0,                                 // use default stack size
ThreadFunc,                // thread function
&dwThrdParam,         // argument to thread function
0,                               // use default creation flags
&dwThreadId);           // returns the thread identifier
void CloseHandle(hThread);
*************************************/

void Init();//初始化工作			
void Turn1(int a[][4],int w,int h,int *x,int y);    
void Turn2(int a[][4], int w, int h, int *x, int y);    //方块转动
bool IsAvailable1(int a[],int x,int y,int w,int h); 
bool IsAvailable2(int a[], int x, int y, int w, int h); //判定是否能放下
void DrawBlocks1(int a[],int w,int h,int x,int y,WORD wColors[],int nColors);
void DrawBlocks2(int a[], int w, int h, int x, int y, WORD wColors[], int nColors);
void ClearSquare1(int a[],int w,int h,int x,int y);
void ClearSquare2(int a[], int w, int h, int x, int y);
void GameOver1();
void GameOver2();
void DeleteLine(int m[][MAPW],int row);  //消除一行

HANDLE handle;                     //窗口句柄
int b[7][4][4]={	{{1},{1,1,1}},//7种不同形状的方块
					{{0,2},{2,2,2}},
					{{3,3},{0,3,3}},
					{{0,0,4},{4,4,4}},
					{{0,5,5},{5,5}},
                    {{6,6,6,6}},
                    {{7,7},{7,7}}};
//七种颜色
WORD SQUARE_COLOR[7] ={ FOREGROUND_RED|FOREGROUND_INTENSITY,
                        FOREGROUND_GREEN|FOREGROUND_INTENSITY,
						FOREGROUND_BLUE|FOREGROUND_INTENSITY,
						FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY,
						FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
						FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY,
						FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY };	

int map1[MAPH][MAPW] = { 0 }; //保存工作区的区域
int dx1 = 12 ; //初始化屏幕时起始x坐标
int dy1 = 3;
int map2[MAPH][MAPW] = { 0 }; //保存工作区的区域
int dx2 = 40; //初始化屏幕时起始x坐标
int dy2 = 3;

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
	int temp;//-1左右边界；-2上下边界；-3四个角；1~7代表不同颜色的方块
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
	int temp;//-1左右边界；-2上下边界；-3四个角；1~7代表不同颜色的方块
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
void GameOver1()
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 7 + dx1, 10 + dy1, wColors, 1, "GAME OVER");

	exit(EXIT_SUCCESS);
}
void GameOver2()
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 7 + dx1, 10 + dy1, wColors, 1, "GAME OVER");

	exit(EXIT_SUCCESS);
}
void DeleteLine(int m[][MAPW], int row)
{
	WORD wColors[1] = { FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
	textout(handle, 2 + dx1, row + dy1, wColors, 1, "﹌﹌﹌﹌﹌﹌﹌﹌﹌﹌");
	textout(handle, 2 + dx2, row + dy2, wColors, 1, "﹌﹌﹌﹌﹌﹌﹌﹌﹌﹌");
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

int main()
{
	
	Init();

	int score1 = 0; 
	int score2 = 0; //初始化分数
	int level1 = 0; 
	int level2 = 0;  //初始化游戏级别
	int Num1 = rand() % 7 ; 
	int Num2 = rand() % 7;//创建第一个方块编号
	int nextNum1 = Num1;   
	int nextNum2 = Num2;  //保存下一个方块编号
	int blank1,blank2;  //记录每个方块起始位置
	int x1 = 0,y1 = 0, x2 = 0, y2 = 0;  //记录游戏开始的相对坐标
	int a1[4][4] = { 0 };
	int a2[4][4] = { 0 }; //临时使用用来保存当前方块


	while(1)
	{
		for(int i=0;i<4;i++)          //复制方块
			for(int j=0;j<4;j++)
				if(a1[i][j] = b[nextNum1][i][j]) 
					blank1=i;
				
		y1 = 1- blank1;
		x1 = 4;

		for (int i = 0; i<4; i++)          //复制方块
			for (int j = 0; j<4; j++)
				if (a1[i][j] = b[nextNum1][i][j])
					blank2 = i;

		y2 = 1 - blank2;
		x2 = 4;

		//创建下一个方块
		Num1 = nextNum1; Num2 = nextNum2;
		ClearSquare1(b[Num1][0],4,4,-4,13);
		ClearSquare2(b[Num2][0], 4, 4, 66, 13);
		nextNum1 = rand() % 7 ;
		nextNum2 = rand() % 7;
		WORD wColors[1]= { SQUARE_COLOR[nextNum1] };
		DrawBlocks1(b[nextNum1][0], 4, 4, -4, 13, wColors, 1);
		DrawBlocks2(b[nextNum2][0], 4, 4, 66, 13, wColors, 1);
		wColors[0] = SQUARE_COLOR[Num1] ;
		DrawBlocks1(&a1[0][0], 4, 4, x1, y1, wColors,1);
		DrawBlocks2(&a2[0][0], 4, 4, x2, y2, wColors, 1);
		
		//显示分数信息
		char string[5];
		wColors[0]=FOREGROUND_RED| FOREGROUND_GREEN|FOREGROUND_INTENSITY;
		textout(handle, dx1 - 8,5+dy1,wColors,1,itoa(score1,string,10));
		textout(handle, dx1 - 8,9+dy1,wColors,1,itoa(level1,string,10));
		textout(handle, dx2 + 26, 5 + dy1, wColors, 1, itoa(score2, string, 10));
		textout(handle, dx2 + 26, 9 + dy1, wColors, 1, itoa(level2, string, 10));
		int max_delay1 = 100 - 10 * level1; 
		int max_delay2 = 100 - 10 * level2; //计算不同游戏级别的下落时间间隔

		while(1)
		{
			int delay=0; //延迟量
			while(delay<max_delay1)
			{
				if(_kbhit())  //用if避免按住键使方块卡住
				{
					int key=_getch();
					switch (key)
					{
					case KEY_UP_1:
						{
							ClearSquare1(&a1[0][0],4,4,x1,y1);
							Turn1(a1,4,4,&x1,y1);
							wColors[0]=SQUARE_COLOR[Num1];
							DrawBlocks1(&a2[0][0],4,4,x1,y1,wColors,1);
						}
						break;
					case KEY_UP_2:
					{
						ClearSquare2(&a2[0][0], 4, 4, x2, y2);
						Turn1(a2, 4, 4, &x2, y2);
						wColors[0] = SQUARE_COLOR[Num1];
						DrawBlocks2(&a2[0][0], 4, 4, x2, y2, wColors, 1);
					}
					break;
					case KEY_DOWN_1:
						delay=max_delay1;
						break;
					case KEY_LEFT_1:
						{
							if(IsAvailable1(&a1[0][0],x1-1,y1,4,4))
							{
								ClearSquare1(&a1[0][0],4,4,x1,y1);
								x1--;
								wColors[0]=SQUARE_COLOR[Num1];
								DrawBlocks1(&a1[0][0],4,4,x1,y1,wColors,1);
							}	
						}
						break;
					case KEY_LEFT_2:
					{
						if (IsAvailable2(&a2[0][0], x2 - 1, y2, 4, 4))
						{
							ClearSquare2(&a2[0][0], 4, 4, x2, y2);
							x2--;
							wColors[0] = SQUARE_COLOR[Num1];
							DrawBlocks2(&a2[0][0], 4, 4, x2, y2, wColors, 1);
						}
					}
					break;
					case KEY_RIGHT_1:
						{
							if(IsAvailable1(&a1[0][0],x1+1,y1,4,4))
							{
								ClearSquare1(&a1[0][0],4,4,x1,y1);
								x1++;
								wColors[0]=SQUARE_COLOR[Num1];
								DrawBlocks1(&a1[0][0],4,4,x1,y1,wColors,1);
							}
						}
						break;
					case KEY_RIGHT_2:
					{
						if (IsAvailable2(&a2[0][0], x2 + 1, y2, 4, 4))
						{
							ClearSquare2(&a2[0][0], 4, 4, x2, y2);
							x2++;
							wColors[0] = SQUARE_COLOR[Num1];
							DrawBlocks2(&a2[0][0], 4, 4, x2, y2, wColors, 1);
						}
					}
					break;
					case KEY_ESC:
						exit(EXIT_SUCCESS);
						break;
					}

				}
				Sleep(8);delay++;
			}
			if(IsAvailable1(&a1[0][0],x1,y1+1,4,4)) //是否能下移
			{
				ClearSquare1(&a1[0][0],4,4,x1,y1);
				y1++;
				wColors[0]=SQUARE_COLOR[Num1];
				DrawBlocks1(&a1[0][0],4,4,x1,y1,wColors,1);
			}
			else
			{
				if(y1<=1) GameOver1();     //是否结束
				for(int i=0;i<4;i++)     //放下方块，更新工作区
					for(int j=0;j<4;j++)
						if(a1[i][j]&&((i+y1)<MAPH-1)&&((j+x1)<MAPW-1)) 
							map1[i+y1][j+x1]=a1[i][j];

				int full,k=0;
				for(int i=y1;i<min(y1+4,MAPH-1);i++)
				{
					full=1;
					for(int j=1;j<11;j++)
						if(!map1[i][j]) full=0;
					if(full)   //消掉一行
					{
						DeleteLine(map1,i);
						k++;
						score1=score1+k;
						level1=min(score1/30,9);
						max_delay1=100-10*level1;
					}
				}
				break; 
			}
		}
	}
	return EXIT_SUCCESS;
}



