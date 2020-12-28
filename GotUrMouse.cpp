#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <math.h>
#include <thread>

using namespace std;
bool gameover;
bool shutDown;
const int width = 28;
const int height = 16;
int x, y, fruitX, fruitY, score;
int tailX[400], tailY[400], nTail;
enum eDirection { STOP, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void StartScreen()
{
	system("cls");

	#pragma region UH OH !

	cout << " ___  ___  ___  ___          ________  ___  ___          ___       \n";
	Sleep(50);
	cout << "|\\  \\|\\  \\|\\  \\|\\  \\        |\\   __  \\|\\  \\|\\  \\        |\\  \\      \n";
	Sleep(50);
	cout << "\\ \\  \\\\\\  \\ \\  \\\\\\  \\       \\ \\  \\|\\  \\ \\  \\\\\\  \\       \\ \\  \\     \n";
	Sleep(50);
	cout << " \\ \\  \\\\\\  \\ \\   __  \\       \\ \\  \\\\\\  \\ \\   __  \\       \\ \\  \\    \n";
	Sleep(50);
	cout << "  \\ \\  \\\\\\  \\ \\  \\ \\  \\       \\ \\  \\\\\\  \\ \\  \\ \\  \\       \\ \\__\\   \n";
	Sleep(50);
	cout << "   \\ \\_______\\ \\__\\ \\__\\       \\ \\_______\\ \\__\\ \\__\\       \\|__|   \n";
	Sleep(50);
	cout << "    \\|_______|\\|__|\\|__|        \\|_______|\\|__|\\|__|           ___ \n";
	Sleep(50);
	cout << "                                                              |\\__\\\n";
	Sleep(50);
	cout << "                                                              \\|__|\n";
	Sleep(50);

	#pragma endregion

	cout << "\n\nlooks like the snake got your cursor!! Eat 5 fruit to get it back."
		<< "\n\nPress 's' to Start!"
		<< "\n\nPress 'i' for Instructions."
		<< "\n\nYour Previous Score: " << score << endl;
}
void Instructions()
{
	system("cls");
	cout << "\nUse the 'w' key to go UP."
		<< "\n\nUse the 'a' key to turn LEFT."
		<< "\n\nUse the 's' key to go DOWN."
		<< "\n\nUse the 'd' key to turn RIGHT."
		<< "\n\nDO NOT touch the borders or you will LOSE."
		<< "\nDO NOT eat your own tail or you will obviously also LOSE!"
		<< "\n\n\nNow press the 'm' key to go MENU.\n";
	string input;
	cin >> input;
	if (input == "m")
		StartScreen();
}
void FruitSpawn()
{
	fruitX = rand() % width;
	fruitY = rand() % height;
}
void Setup()
{
	gameover = false;
	shutDown = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	FruitSpawn();
	score = 0;
	nTail = 0;
}
void Draw()
{
	system("cls"); //clear screen
	for (int i = 0; i < width + 2; i++) {
		cout << "#";
	}
	cout << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0) 
				cout << "#";
			
			if (i == y && j == x)
				cout << "O";

			else if (i == fruitY && j == fruitX)
				cout << "+";

			else {
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}

			if (j == width - 1)
				cout << "#";
		}
		cout << endl;
	}
	for (int i = 0; i < width + 2; i++) {
		cout << "#";
	}
	cout << endl;
	cout << "Score: " << score << endl;
}
void Input()
{
	if (_kbhit()) {
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'd':
			dir = RIGHT;
			break;
		}
	}
}
void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;

	}

	switch (dir)
	{
	case LEFT:
		x -= 0.5;
		break;
	case RIGHT:
		x += 1;
		break;
	case UP:
		y -= 0.5;
		break;
	case DOWN:
		y += 1;
		break;
	}
	if (x > width || x < 0 || y > height || y < 0) {
		gameover = true;
		StartScreen();
	}

	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y)
			gameover = true;
	}

	if (x == fruitX && y == fruitY) {
		score++;
		nTail++;
		FruitSpawn();
	}
}

void circle(int radius, int center_x, int center_y)
{
	double t = 0;
	//  For best circle in the center
	//int mx_0 = GetSystemMetrics(SM_CXSCREEN) / 2;
	//int my_0 = GetSystemMetrics(SM_CYSCREEN) / 2;
	//int R = 200;
	int mx, my;
	for (int i = 0; i > -1; i++) {
		mx = radius * sin(t) + center_x;
		my = radius * cos(t) + center_y;
		t += 0.05;
		SetCursorPos(mx, my);
		Sleep(10);
	}
}

int main()
{	
	COORD a;
	a.X = 90;
	a.Y = 300;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = 65;
	rect.Right = 115;

	HANDLE consoleStyle = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND console = GetConsoleWindow();
	SetConsoleScreenBufferSize(consoleStyle, a);
	SetConsoleWindowInfo(consoleStyle, 1, &rect);
	SetWindowPos(console, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	SetConsoleTextAttribute(consoleStyle, 3);

	SetWindowTextA(console, "I got your cursor!!");

	int mx, my;
	do {
		StartScreen();
		string input;

		thread t1(circle, 200, GetSystemMetrics(SM_CXSCREEN) / 4, GetSystemMetrics(SM_CYSCREEN) / 2);
		//thread t2(circle, 100, GetSystemMetrics(SM_CXSCREEN) / 4, GetSystemMetrics(SM_CYSCREEN) / 2);
		//thread t3(circle, 10, GetSystemMetrics(SM_CXSCREEN) / 4, GetSystemMetrics(SM_CYSCREEN) / 2);
		
		cin >> input;
			if (input == "s")
			{	
				Setup();
				while (!gameover)
				{
					Draw();
					Input();
					Logic();

					mx = fruitX * 8.2 + 20;
					my = fruitY * 17 + 45;
					SetCursorPos(mx, my);

					if (score == 5) {

						system("cls");

						#pragma region you did it !

						cout << " _     _                    _         _        _       _ \n";
						Sleep(50);
						cout << "( )   ( )                  ( ) _     ( )    _ ( )_    ( )\n";
						Sleep(50);
						cout << "`\\`\\_/'/'_    _   _       _| |(_)   _| |   (_)| ,_)   | |\n";
						Sleep(50);
						cout << "  `\\ /'/'_`\\ ( ) ( )    /'_` || | /'_` |   | || |     | |\n";
						Sleep(50);
						cout << "   | |( (_) )| (_) |   ( (_| || |( (_| |   | || |_    |_|\n";
						Sleep(50);
						cout << "   (_)`\\___/'`\\___/'   `\\__,_)(_)`\\__,_)   (_)`\\__)    _ \n";
						Sleep(50);
						cout << "                                                      (_)\n\n";
						Sleep(50);

#pragma endregion

						string k = "You got away this time";
						string c = "press any key to leave";

						for (int i = 0; i < 23; i++) {
							cout << k[i];
							Sleep(30);
						}
						for (int i = 0; i < 10; i++) {
							cout << "!";
							Sleep(30);
						}
						cout << "\n\n";
						for (int i = 0; i < 23; i++) {
							cout << c[i];
							Sleep(30);
						}
						system("pause > null");

						exit(0);
					}

					if (GetAsyncKeyState(VK_END) & 1) exit(0);
					Sleep(15);
				}
			}
			else if (input == "i") Instructions();

	} while (shutDown == false);

	exit(0);


	cout << "\n___________________________________________\nlooks like i got your cursor\n___________________________________________\npress your end key to take it back...\n___________________________________________\n";

	cout << "you got away this time";
	for (int i = 0; i < 15; i++) {
		cout << "!";
		Sleep(100);
	}

	Sleep(1000);

	exit(0);
}