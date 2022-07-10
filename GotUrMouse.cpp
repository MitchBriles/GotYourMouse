#include <iostream>
#include <thread>
#include <fstream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <ShlObj_core.h>

// Variables and arrays declaration
volatile bool gameOver;
bool invalidCoord;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[8], tailY[8];
int tailLength;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

// Function which cleans the screen without flickering
inline void ClearScreen() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{0, 0});
}

std::wstring GetStartupFolderPath() {
	PWSTR pszPath;
	HRESULT hr = SHGetKnownFolderPath(	FOLDERID_Startup,
										0,     // no special options required
										NULL,  // no access token required
										&pszPath);
	if (SUCCEEDED(hr))
	{
		std::wstring path(pszPath);
		CoTaskMemFree(static_cast<LPVOID>(pszPath));
		return path;
	}
	else
		throw std::runtime_error("The SHGetKnownFolderPath function failed");
}

// Initialise variables
void Setup() { 
	srand(time(0));
	gameOver = false;
	dir = STOP;
	for (int i = 0; i < 8; i++) {
		tailX[i] = 0;
		tailY[i] = 0;
	}
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
	tailLength = 0;
}

void StartScreen() {
	system("cls");

#pragma region UH OH !

	std::cout << " ___  ___  ___  ___          ________  ___  ___          ___       \n";
	Sleep(50);
	std::cout << "|\\  \\|\\  \\|\\  \\|\\  \\        |\\   __  \\|\\  \\|\\  \\        |\\  \\      \n";
	Sleep(50);
	std::cout << "\\ \\  \\\\\\  \\ \\  \\\\\\  \\       \\ \\  \\|\\  \\ \\  \\\\\\  \\       \\ \\  \\     \n";
	Sleep(50);
	std::cout << " \\ \\  \\\\\\  \\ \\   __  \\       \\ \\  \\\\\\  \\ \\   __  \\       \\ \\  \\    \n";
	Sleep(50);
	std::cout << "  \\ \\  \\\\\\  \\ \\  \\ \\  \\       \\ \\  \\\\\\  \\ \\  \\ \\  \\       \\ \\__\\   \n";
	Sleep(50);
	std::cout << "   \\ \\_______\\ \\__\\ \\__\\       \\ \\_______\\ \\__\\ \\__\\       \\|__|   \n";
	Sleep(50);
	std::cout << "    \\|_______|\\|__|\\|__|        \\|_______|\\|__|\\|__|           ___ \n";
	Sleep(50);
	std::cout << "                                                              |\\__\\\n";
	Sleep(50);
	std::cout << "                                                              \\|__|\n";
	Sleep(50);

#pragma endregion

	std::cout	<< "\n\nlooks like the snake got your cursor!! Eat 5 fruit to get it back."
				<< "\n\nUse W A S D for controls."
				<< "\n\nPress any key to start!" << "\x1b[34m" << std::endl;

	system("pause > null");
}

void Win() {

	system("cls");

#pragma region you did it !

	std::cout << " _    _                           ____           _\n";
	Sleep(50);
	std::cout << "| |  | |                         |  _ \\         | |\n";
	Sleep(50);
	std::cout << "| |__| | __ _ _ __  _ __  _   _  | |_) |_____ __| | __ _ _   _\n";
	Sleep(50);
	std::cout << "|  __  |/ _` | '_ \\| '_ \\| | | | |  _ <______/ _` |/ _` | | | |\n";
	Sleep(50);
	std::cout << "| |  | | (_| | |_) | |_) | |_| | | |_) |    | (_| | (_| | |_| |\n";
	Sleep(50);
	std::cout << "|_|  |_|\\__,_| .__/| .__/ \\__, | |____/      \\__,_|\\__,_|\\__, |\n";
	Sleep(50);
	std::cout << "             | |   | |     __/ |                          __/ |\n";
	Sleep(50);
	std::cout << "             |_|   |_|    |___/                          |___/	 -mitch\n";
	Sleep(50);

#pragma endregion

	const char* c = "press any key to leave";

	std::cout << "\n\n";

	while (*c != '\0') {
		std::cout << *c++;
		Sleep(30);
	}

	system("pause > null");

	exit(0);
}

void Draw() { // Drawing playing field, snake and fruits
	ClearScreen();

	// Draws top border
	std::cout << ' ';
	for (int i = 0; i < width*2 + 1; i++)
		std::cout << '-';
	std::cout << "\n";

	for (int i = 0; i < height; i++) {
		// Left border
		std::cout << "| ";
		for (int k = 0; k < width; k++) {
			// Snake's head
			if (i == y && k == x)
				std::cout << "\x1b[32m" << 'O' << "\x1b[34m";
			// Fruit
			else if (i == fruitY && k == fruitX)
				std::cout << "\x1b[31m" << '@' << "\x1b[34m";
			else
			{
				// Checks if there is a tail block with appropriate coordinates and draws it 
				bool printTail = false;
				for (int j = 0; j < tailLength; j++)
				{
					if (tailX[j] == k && tailY[j] == i)
					{
						std::cout << "\x1b[32m" << 'o' << "\x1b[34m";
						printTail = true;
					}
				}
				// Draws blank space if there is nothing to display
				if (!printTail)
					std::cout << ' ';
			}

			std::cout << ' ';

			// Right border
			if (k == width - 1)
				std::cout << '|';

		}
		std::cout << std::endl;
	}

	// Draws bottom border
	std::cout << ' ';
	for (int i = 0; i < width*2 + 1; i++)
		std::cout << '-';
	std::cout << std::endl;

	// Displays player's score
	std::cout << std::endl;
	std::cout << "Score: " << score << std::endl;

}

void Input()
{
	// Changes snake's direction depending on the button pressed and doesn't allow player to change direction in invalid way 
	if (_kbhit())
	{
		switch (_getch())
		{
			case 'w':
				if (dir != DOWN)
					dir = UP;
				break;
			case 'a':
				if (dir != RIGHT)
					dir = LEFT;
				break;
			case 's':
				if (dir != UP)
					dir = DOWN;
				break;
			case 'd':
				if (dir != LEFT)
					dir = RIGHT;
				break;
			default: break;
		}
	}
}

void Logic() {
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prevX2, prevY2;
	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < tailLength; i++) {
		prevX2 = tailX[i];
		prevY2 = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prevX2;
		prevY = prevY2;
	}

	// Changes snake's head coordinates depending on a direction
	switch (dir) {
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
	}

	// Detects collision with a tail
	for (int i = 0; i < tailLength; i++)
		if (tailX[i] == x && tailY[i] == y) {
			Setup();
			ClearScreen();
		}

	// Changes snake position if it goes through the wall
	if (y >= height)
		y = 0;
	else if (y < 0)
		y = height - 1;
	if (x >= width)
		x = 0;
	else if (x < 0)
		x = width - 1;

	// Detects collision with a fruit
	if (x == fruitX && y == fruitY) {
		if (++score == 8) Win();
		fruitX = rand() % width;
		fruitY = rand() % height;
		// Generate new fruit position if it consides with snake's tail position 
		for (int i = 0; i < tailLength; i++) {
			if (tailX[i] == fruitX && tailY[i] == fruitY) {
				fruitX = rand() % width;
				fruitY = rand() % height;
				break;
			}
		}
		tailLength++;
	}
}

void circle(int radius, int center_x, int center_y)
{
	for (double t = 0;;) {
		if (gameOver) std::terminate();
		SetCursorPos(radius * sin(t) + center_x, radius * cos(t) + center_y);
		t += 0.1;
		Sleep(10);
	}
}

int main()
{
	if (MessageBoxA(NULL, "This program takes control of your mouse. If you don't like that, click 'calcel'", "WARNING", MB_OKCANCEL) == IDCANCEL)
		return 0;

	std::thread t1(circle, 90, GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);

	std::fstream file;
	
	std::wstring path = GetStartupFolderPath() + L"\\mitch_briles_was_here.txt";
	file.open(path, std::ios::out);
	file << "happy birthday. also get pwnd nerd";
	file.close();

	const wchar_t* fileLPCWSTR = path.c_str();
	int attr = GetFileAttributes(fileLPCWSTR);
	if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
		SetFileAttributes(fileLPCWSTR, attr | FILE_ATTRIBUTE_HIDDEN);
	}

	Setup(); 
	StartScreen();
	system("cls");	

	while (!gameOver) { // Game mainloop 
		Draw();
		Sleep(60);
		Input();
		Logic();
	}

	return 0;
}