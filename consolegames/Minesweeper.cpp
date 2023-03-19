#include <iostream>
#include <thread>
#include <Windows.h>
#include <chrono>
#include <ctime>
#include <cstdlib>

int nGameSizeWidth = 40;
int nGameSizeHeight = 20;

int nGameStartX = 19;
int nGameStartY = 10;

int nScreenWidth = 75;
int nScreenHeight = 30;

int FontSizeW = 12;
int FontSizeH = 12;

int mouse_x_pos = 0;
int mouse_y_pos = 0;

int MineAmount = 250;
int FlagAmount = MineAmount;

typedef struct cell {

	int value = 0;
	bool IsOpened = false;
	bool IsMine = false;
	bool IsFlagged = false;
} cell;

cell* GameField = new cell[nGameSizeHeight * nGameSizeWidth];

CHAR_INFO* screen = new CHAR_INFO[nScreenHeight * nScreenWidth];

short DecimalToUnicode(int decimal) {

	short temp = 0x0030;
	return temp + (short)decimal;
}

void DrawField(int x = 8, int y = 6, int nFieldWidth = 540, int nFieldHeight = 240, bool ShowMines = false, short color = 0x0008, short pixel = 0x2588);
void GenerateField(int MineAmount = 20);
void OpenField(int x, int y);
void PutFlag(int mouse_x_pos, int mouse_y_pos);
bool IsGameOver(int x, int y);
bool YouWin();


int main() {
	
	SetConsoleTitle(L" Minesweeper");
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	
	INPUT_RECORD in[32];
	DWORD events = 0;

	SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 0;

	SMALL_RECT s = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &s);
	COORD buffer = { nScreenWidth, nScreenHeight };
	SetConsoleActiveScreenBuffer(hConsole);

	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);
	font.dwFontSize = { (short)FontSizeW, (short)FontSizeH };
	font.nFont = 0;
	font.FontFamily = FF_DONTCARE;
	font.FontWeight = FF_MODERN;

	SetCurrentConsoleFontEx(hConsole, FALSE, &font);
	SetConsoleScreenBufferSize(hConsole, buffer);
	SetConsoleCursorInfo(hConsole, &cursor);

	s = { 0, 0, short(nScreenWidth - 1), short(nScreenHeight - 1) };
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) { screen[i].Char.UnicodeChar = 0x2588;
	screen[i].Attributes = 0x0000;
	}
	SetConsoleWindowInfo(hConsole, TRUE, &s);

	GenerateField(MineAmount);

	bool GameOver = false;
	bool Win = false;

	while (!GameOver) {

		//Field Drawing
		DrawField(nGameStartX, nGameStartY, nGameSizeWidth, nGameSizeHeight);

		GameOver = Win = YouWin();

		//mouse inputs
		GetNumberOfConsoleInputEvents(hConsoleIn, &events);
		if (events > 0)
			ReadConsoleInput(hConsoleIn, in, events, &events);

		for (DWORD i = 0; i < events; i++) {
			
			if (in[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {

				mouse_x_pos = in[i].Event.MouseEvent.dwMousePosition.X;
				mouse_y_pos = in[i].Event.MouseEvent.dwMousePosition.Y;
			}

			if (in[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {

				if (!GameField[(mouse_y_pos - nGameStartY) * nGameSizeWidth + (mouse_x_pos - nGameStartX)].IsFlagged) {

					if (IsGameOver(mouse_x_pos, mouse_y_pos)) {

						DrawField(nGameStartX, nGameStartY, nGameSizeWidth, nGameSizeHeight, true);
						GameOver = true;
						break;
					}

					OpenField(mouse_x_pos, mouse_y_pos);
				}
			}

			else if (in[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {

				PutFlag(mouse_x_pos, mouse_y_pos);
			}

		}
		
		if (mouse_x_pos >= nGameStartX && mouse_x_pos < nGameSizeWidth + nGameStartX && mouse_y_pos <= nGameSizeHeight + nGameStartY && mouse_y_pos >= nGameStartY) {

			if (!GameField[(mouse_y_pos - nGameStartY) * nGameSizeWidth + (mouse_x_pos - nGameStartX)].IsOpened)
				screen[mouse_y_pos * nScreenWidth + mouse_x_pos].Attributes = 0x000F;
		}
		//mouse input end

		WriteConsoleOutput(hConsole, screen, { (short)nScreenWidth, (short)nScreenHeight }, { 0 , 0 }, &s);
	}

	//system("cls");
	std::cout << "Game Over!" << std::endl;

	if (Win)
		std::cout << "You Win!" << std::endl;
	else
		std::cout << "You Lose!" << std::endl;

	system("pause");

	return 0;
}

void DrawField(int x, int y, int nFieldWidth, int nFieldHeight, bool ShowMines, short color, short pixel) {

	for (int i = 0; i < nFieldHeight; i++) {

		for (int j = 0; j < nFieldWidth; j++) {

			if (!GameField[i * nGameSizeWidth + j].IsOpened) {

				if (GameField[i * nGameSizeWidth + j].IsFlagged) {

					screen[(y + i) * nScreenWidth + j + x].Char.UnicodeChar = L'X';
					screen[(y + i) * nScreenWidth + j + x].Attributes = 0x0080;
				}

				else {

					screen[(y + i) * nScreenWidth + j + x].Char.UnicodeChar = pixel;
					screen[(y + i) * nScreenWidth + j + x].Attributes = color;
				}

				if (ShowMines && GameField[i * nGameSizeWidth + j].IsMine) {

					screen[(y + i) * nScreenWidth + j + x].Char.UnicodeChar = L'*';
					screen[(y + i) * nScreenWidth + j + x].Attributes = 0x0070;
				}

			}
			else {

				
				screen[(y + i) * nScreenWidth + j + x].Char.UnicodeChar = DecimalToUnicode(GameField[i * nGameSizeWidth + j].value);
				screen[(y + i) * nScreenWidth + j + x].Attributes = 0x0070;

				if (GameField[i * nGameSizeWidth + j].value == 0) {

					screen[(y + i) * nScreenWidth + j + x].Char.UnicodeChar = 0x2588;
					screen[(y + i) * nScreenWidth + j + x].Attributes = 0x0007;
				}
					
			}
				
		}
	}
}

void GenerateField(int MineAmount) {

	srand(time(0));

	int pos;

	for (int i = 0; i < MineAmount; i++) {

		pos = rand() % (nGameSizeHeight * nGameSizeWidth);

		while(GameField[pos].IsMine)
			pos = rand() % (nGameSizeHeight * nGameSizeWidth);

		GameField[pos].IsMine = true;
	}

	for (int i = 0; i < nGameSizeHeight; i++) {

		for (int j = 0; j < nGameSizeWidth; j++) {

			if (GameField[i * nGameSizeWidth + j].IsMine) {

				GameField[(i - 1) * nGameSizeWidth + (j - 1)].value += (i - 1 >= 0 && j - 1 >= 0) ? 1 : 0; // left up
				GameField[(i - 1) * nGameSizeWidth + (j)].value += (i - 1 >= 0) ? 1 : 0; // up
				GameField[(i - 1) * nGameSizeWidth + (j + 1)].value += (i - 1 >= 0 && j + 1 < nGameSizeWidth) ? 1 : 0; // right up
				GameField[(i) * nGameSizeWidth + (j - 1)].value += (j - 1 >= 0) ? 1 : 0; // left
				GameField[(i) * nGameSizeWidth + (j + 1)].value += (j + 1 < nGameSizeWidth) ? 1 : 0; // right
				GameField[(i + 1) * nGameSizeWidth + (j - 1)].value += (i + 1 <= nGameSizeHeight && j - 1 >= 0) ? 1 : 0; // left down
				GameField[(i + 1) * nGameSizeWidth + (j)].value += (i + 1 <= nGameSizeHeight) ? 1 : 0; // down
				GameField[(i + 1) * nGameSizeWidth + (j + 1)].value += (i + 1 <= nGameSizeHeight && j + 1 < nGameSizeWidth) ? 1 : 0; // right down

			}
		}
	}
}

void OpenField(int x, int y) {

	if (x >= nGameStartX && x <= nGameSizeWidth + nGameStartX && y <= nGameSizeHeight+ nGameStartY && y >= nGameStartY)
		GameField[(y - nGameStartY) * nGameSizeWidth + (x - nGameStartX)].IsOpened = true;

	

	for (int k = 0; k < 20; k++) {

		for (int i = 0; i < nGameSizeHeight; i++) {

			for (int j = 0; j < nGameSizeWidth; j++) {

				if (GameField[i * nGameSizeWidth + j].IsOpened && GameField[i * nGameSizeWidth + j].value == 0 && !GameField[i * nGameSizeWidth + j].IsMine) {

					GameField[(i - 1) * nGameSizeWidth + (j - 1)].IsOpened = (GameField[(i - 1) * nGameSizeWidth + (j - 1)].IsMine) ? false : true; // left up
					GameField[(i - 1) * nGameSizeWidth + (j)].IsOpened = (GameField[(i - 1) * nGameSizeWidth + (j)].IsMine) ? false : true; // up
					GameField[(i - 1) * nGameSizeWidth + (j + 1)].IsOpened = (GameField[(i - 1) * nGameSizeWidth + (j + 1)].IsMine) ? false : true; // right up
					GameField[(i)*nGameSizeWidth + (j - 1)].IsOpened = (GameField[(i) * nGameSizeWidth + (j - 1)].IsMine) ? false : true; // left
					GameField[(i)*nGameSizeWidth + (j + 1)].IsOpened = (GameField[(i) * nGameSizeWidth + (j + 1)].IsMine) ? false : true; // right
					GameField[(i + 1) * nGameSizeWidth + (j - 1)].IsOpened = (GameField[(i + 1) * nGameSizeWidth + (j - 1)].IsMine) ? false : true; // left down
					GameField[(i + 1) * nGameSizeWidth + (j)].IsOpened = (GameField[(i + 1) * nGameSizeWidth + (j)].IsMine) ? false : true; // down
					GameField[(i + 1) * nGameSizeWidth + (j + 1)].IsOpened = (GameField[(i + 1) * nGameSizeWidth + (j + 1)].IsMine) ? false : true; // right down
					
				}
			}
		}

	}
}

void PutFlag(int mouse_x_pos, int mouse_y_pos) {

	if (mouse_x_pos >= nGameStartX && mouse_x_pos <= nGameSizeWidth + nGameStartX && mouse_y_pos <= nGameSizeHeight + nGameStartY && mouse_y_pos >= nGameStartY - 1) {

		FlagAmount += (GameField[(mouse_y_pos % (nGameSizeHeight)+1) * nGameSizeWidth + (mouse_x_pos % (nGameSizeWidth)+1)].IsFlagged) ? 1 : -1;
		GameField[(mouse_y_pos - nGameStartY) * nGameSizeWidth + (mouse_x_pos - nGameStartX)].IsFlagged = (GameField[(mouse_y_pos - nGameStartY) * nGameSizeWidth + (mouse_x_pos - nGameStartX)].IsOpened || GameField[(mouse_y_pos - nGameStartY) * nGameSizeWidth + (mouse_x_pos - nGameStartX)].IsFlagged) ? false : true;

	}

	
}

bool IsGameOver(int x, int y) {

	if (x >= nGameStartX && x <= nGameSizeWidth + nGameStartX && y <= nGameSizeHeight + nGameStartY && y >= nGameStartY)
		if (GameField[(y - nGameStartY) * nGameSizeWidth + (x - nGameStartX)].IsMine)
			return true;

	return false;
}

bool YouWin() {

	int count = 0;

	for (int i = 0; i < nGameSizeHeight; i++) {

		for (int j = 0; j < nGameSizeWidth; j++) {

			if (!GameField[i * nGameSizeWidth + j].IsOpened)
				count++;
		}
	}

	if (count == MineAmount)
		return true;

	return false;
}