// KeyboardTest.cpp : Defines the entry point for the console application.
//

#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <windows.h>

using namespace std;

#define KEY_UP      72
#define KEY_DOWN    80
#define KEY_LEFT    75
#define KEY_RIGHT   77

int KEY1;       //键盘输入值
int KEY2;

DWORD WINAPI Player1(LPVOID lpParam);//线程处理函数PLAYER1
DWORD WINAPI Player2(LPVOID lpParam);//线程处理函数FUN2

int main( void )
{

	HANDLE hThread1 = CreateThread(NULL, 0, Player1, NULL, 0, NULL);//线程启动函数fun
	HANDLE hThread2 = CreateThread(NULL, 0, Player2, NULL, 0, NULL);//线程启动函数fun2

	while(1)
	{
		if(_kbhit())  //用if避免按住键使方块卡住
		{
			int key=_getch();
			switch (key)
			{
			case KEY_UP:
				KEY1 = 1;
				break;
			case KEY_DOWN:
				KEY1 = 2;				
				break;
			case KEY_LEFT:
				KEY1 = 3;
				break;
			case KEY_RIGHT:
				KEY1 = 4;
				break;
			case 'W':
				KEY2 = 1;
				break;
			case 'S':
				KEY2 = 2;
				break;
			case 'A':
				KEY2 = 3;
				break;
			case 'D':
				KEY2 = 4;
				break;
			}
		}
	}

	CloseHandle(hThread1);
	CloseHandle(hThread2);

	return 0;
}

DWORD WINAPI Player1(LPVOID lpParam)
{
	while (1)
	{
		switch (KEY1)
		{
		case 1:
			KEY1 = 0;
			cout << "Player 1 pressed " << "UP KEY" << endl;
			break;
		case 2:
			KEY1 = 0;
			cout << "Player 1 pressed " << "DOWN KEY" << endl;				
			break;
		case 3:
			KEY1 = 0;
			cout << "Player 1 pressed " << "LEFT KEY" << endl;
			break;			
		case 4:
			KEY1 = 0;
			cout << "Player 1 pressed " << "RIGHT KEY" << endl;
			break;

		}
		

		Sleep(8);
	}


	return EXIT_SUCCESS;

}


DWORD WINAPI Player2(LPVOID lpParam)
{

	while (1)
	{
		switch (KEY2)
		{
		case 1:
			KEY2 = 0;
			cout << "Player 2 pressed " << "W KEY" << endl;
			break;
		case 2:
			KEY2 = 0;
			cout << "Player 2 pressed " << "S KEY" << endl;				
			break;
		case 3:
			KEY2 = 0;
			cout << "Player 2 pressed " << "A KEY" << endl;
			break;
		case 4:
			KEY2 = 0;
			cout << "Player 2 pressed " << "D KEY" << endl;
			break;

		}
		Sleep(8);
	}

	return EXIT_SUCCESS;

}