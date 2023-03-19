#include <iostream>
#include <thread>
#include <Windows.h>
#include <vector>

using namespace std;

int nBoxHeight = 18;
int nBoxWidth = 12;
wchar_t blockcharacter = L'0';

int nScreenWidth = 120;
int nScreenHeight = 40;

int GameSpeed = 15;

wstring* TetroBlock = new wstring[7];
bool* Control = new bool[nBoxWidth * nBoxHeight];
int Score = 0;

void CreateBox(wchar_t* &screen, int height, int width);
//wstring AddBlock(wchar_t* &screen, int selected);
int MoveBlock(wchar_t* &screen, int PosX, int PosY, wstring block);
bool ControlField(wchar_t* &screen, int heightlimit, int PosX, int PosY); // Gereksiz artik
int Rotate(wchar_t* &screen, int PosX, int PosY, int rotate);
wstring RotateBlock(wchar_t* &screen, int PosX, int PosY, int rotate, wstring block);
bool CanRotate(wchar_t*& screen, int PosX, int PosY, int rotate, wstring block);
void WriteOutControlField(wchar_t*& screen);
bool MovementControl(wchar_t*& screen, int PosX, int PosY, wstring block);
void DestroyLine(wchar_t*& screen, int PosY);
void CheckifScore(wchar_t*& screen);


int main() {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenHeight * nScreenWidth; i++) screen[i] = L' ';
	DWORD s = 0;
	SetConsoleActiveScreenBuffer(hConsole);

	bool GameOver = 0;
	srand(time(0));

	TetroBlock[0].append(L"....xxxx........");
	TetroBlock[1].append(L".....x...xxx....");
	TetroBlock[2].append(L".......x.xxx....");
	TetroBlock[3].append(L".....xx..xx.....");
	TetroBlock[4].append(L"......xx.xx.....");
	TetroBlock[5].append(L"......x..xxx....");
	TetroBlock[6].append(L"......xx.xx.....");

	wstring CurrentBlock;
	int YPos = 0;
	int XPos = nBoxWidth / 2;
	CurrentBlock = TetroBlock[rand() % 7];

	for (int i = 0; i < 16; i++)
		CurrentBlock[i] = (CurrentBlock[i] == L'x') ? blockcharacter : L' ';

	bool key[4];
	int rotation = 0;
	int nSpeedCount = 0;
	bool move = false;

	while (!GameOver) {
		
		this_thread::sleep_for(70ms);
		move = (nSpeedCount == GameSpeed);
		nSpeedCount++;


		for (int i = 0; i < 4; i++)								// R   L   D Z
			key[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[i]))) != 0;

		XPos += (key[0] && MovementControl(screen, XPos + 1, YPos, CurrentBlock)) ? 1 : 0;
		XPos -= (key[1] && MovementControl(screen, XPos - 1, YPos, CurrentBlock)) ? 1 : 0;
		YPos += (key[2] && MovementControl(screen, XPos, YPos + 1, CurrentBlock)) ? 1 : 0;

		if (key[3] && CanRotate(screen, XPos, YPos, rotation, CurrentBlock)) {

			rotation += 1;

			CurrentBlock = RotateBlock(screen, XPos, YPos, rotation, CurrentBlock);
			

		}

		for (int i = 0; i < nScreenHeight * nScreenWidth; i++) screen[i] = L' ';
		MoveBlock(screen, XPos, YPos, CurrentBlock);

		if (move) {
			YPos++;
			nSpeedCount = 0;
		}
			

		CreateBox(screen, nBoxHeight, nBoxWidth);

		if (!MovementControl(screen, XPos, YPos, CurrentBlock)) {

			for (int i = 0; i < 4; i++) {

				for (int j = 0; j < 4; j++) {

					if (CurrentBlock[i * 4 + j] == blockcharacter)
						Control[(i + YPos) * nBoxWidth + XPos + j] = 1;
				}
			}

			YPos = 0; XPos = nBoxWidth / 2; CurrentBlock = TetroBlock[rand() % 7];

			for (int i = 0; i < 16; i++)
				CurrentBlock[i] = (CurrentBlock[i] == L'x') ? blockcharacter : L'x';
		}

		for (int i = 0; i < nBoxWidth; i++) {

			if (Control[2 * nBoxWidth + i] == 1)
				GameOver = 1;
		}

		WriteOutControlField(screen);

		CheckifScore(screen);

		swprintf_s(&screen[2 * nScreenWidth + nBoxWidth + 6], 16, L"SCORE: %8d", Score);

		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &s);

		
	}
	system("cls");
	cout << "GAME OVER" << endl;
	cout << "TOTAL SCORE: " << Score << endl;
	system("pause");
}


void CreateBox(wchar_t*& screen, int height, int width) {

	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {
			
			if (j == 0 || j == width - 1 || i == height - 1)
				screen[i * nScreenWidth + j] = L'#';
		}
	}
}

int MoveBlock(wchar_t*& screen, int PosX, int PosY, wstring block) {

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {
			//screen[(i + PosY + 1) * nScreenWidth + PosX + j] = L'.';
			if (block[i * 4 + j] == blockcharacter) {

				screen[(i + PosY + 1) * nScreenWidth + PosX + j] = blockcharacter;
			}
		}
	}

	return PosY + 1;
}

/*wstring AddBlock(wchar_t*& screen, int selected) {

	wstring temp;

	for (int i = 0; i < 16; i++) {

		temp.push_back(TetroBlock[selected][i]);
	}

	return temp;
} */

int Rotate(wchar_t*& screen, int PosX, int PosY, int rotate) {

	/*switch (rotate % 2) {

	case 0: //0
		return PosY * 4 + PosX;*/

	//case 1: // 90
		return (PosX * 4 + 4 - PosY);
		
	/*case 2: // 180
		return ((4 - PosY) * 4 + 4 - PosX);
		
	case 3: // 270
		return ((4 - PosX) * 4 + PosY);
		
	}

	return 0;*/
}

bool CanRotate(wchar_t*& screen, int PosX, int PosY, int rotate, wstring block) {

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			if (block[i * 4 + j] == blockcharacter) {

				int pi = Rotate(screen, j, i, rotate);

				if (Control[(PosY + (pi / 4))*nBoxWidth + PosX + (pi % 4)] == 1 || PosX + (pi % 4) >= nBoxWidth - 1 || PosX + (pi % 4) <= 1)
					return false;
			}
		}
	}
	return true;
}

wstring RotateBlock(wchar_t*& screen, int PosX, int PosY, int rotate, wstring block) {

	wstring temp = block;

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			int pi = Rotate(screen, j, i, rotate);
			temp[i * 4 + j] = block[pi - 1];
			
			
		}
	}
	return temp;
}

bool ControlField(wchar_t*& screen, int heightlimit, int PosX, int PosY) {

	bool Hit = false;

	for (int i = 0; i < 4; i++) {

		for (int j = 0; j < 4; j++) {

			if (screen[(i + PosY) * nScreenWidth + PosX + j] == blockcharacter) {
				if (screen[(i + PosY) * nScreenWidth + PosX + j] == '#' || Control[(i + PosY + 1) * nBoxWidth + PosX + j] == 1 || PosY + j >= nBoxHeight - 1) {

					Hit = true;
				}
			}
		}
	}

	if (Hit) {
		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 4; j++) {

				if (screen[(i + PosY) * nScreenWidth + PosX + j] == blockcharacter)
					Control[(i + PosY) * nBoxWidth + PosX + j] = 1;
			}
		}
	}

	return Hit;
}

void WriteOutControlField(wchar_t*& screen) {

	for (int i = 0; i < nBoxHeight; i++) {

		for (int j = 0; j < nBoxWidth; j++) {

			if (Control[i * nBoxWidth + j] == 1)
				screen[i * nScreenWidth + j] = blockcharacter;
		}
	}
}

bool MovementControl(wchar_t*& screen, int PosX, int PosY, wstring block) {


		for(int i = 0; i < 4; i++){
		
			for (int j = 0; j < 4; j++) {

				if (block[i * 4 + j] == blockcharacter) {
					
					if (PosX <= -2 || PosX + 3 > nBoxWidth || Control[(i + PosY + 1) * nBoxWidth + PosX + j] == 1)
						return false;
					
						if (screen[(i + PosY + 1) * nScreenWidth + PosX + j] == L'#')
							return false;

					
				}
			}
		}

		return true;
	
}

void DestroyLine(wchar_t*& screen, int PosY) {

	for (int i = 0; i < nBoxWidth; i++) {

		screen[(PosY)*nScreenWidth + i] = L'=';
		Control[PosY * nBoxWidth + i] = 0;
	}
	this_thread::sleep_for(100ms);
	
	for (int i = PosY; i > 0; i--) {

		for (int j = 0; j < nBoxWidth; j++) {

			swap(Control[i * nBoxWidth + j], Control[(i - 1) * nBoxWidth + j]);
		}
	}
}

void CheckifScore(wchar_t*& screen) {

	int control = 0;

	for (int i = 0; i < nBoxHeight; i++) {

		for (int j = 0; j < nBoxWidth; j++) {

			if (Control[i * nBoxWidth + j] == 1)
				control++;

		}

		if (control == nBoxWidth - 2) {

			DestroyLine(screen, i);
			Score += 10;
		}
		control = 0;
	}
}