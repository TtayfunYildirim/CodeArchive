#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
#include <iomanip>

using namespace std;

vector<vector<char> > ScreenBegin(vector<vector<char> >, int, int);
void ScreenDisplay(vector<vector<char> >);
vector<vector<char> > Update(vector<vector<char> >, vector<pair<int, int> > &, string);
vector<vector<char> > SnakeStart(vector<vector<char> >, vector<pair<int, int> > &);
bool IsGameOver(vector<vector<char> >, vector<pair<int, int> > & );
vector<vector<char> > FruitSpawn(vector<vector<char> >, pair<int, int> &, vector<pair<int, int> > &);
vector<vector<char> > FruitControl(vector<vector<char> >, pair<int, int> &, vector<pair<int, int> > & );

int score = 0;
int speed = 100;

int main() {

	srand(time(0));

	int entry;
	string Direction = "up";
	vector<vector<char> > screen;
	vector<pair<int, int> >* snake = new vector<pair<int, int> >();
	pair<int, int>* fruit = new pair<int, int>();

	
	screen = ScreenBegin(screen, 35, 20);
	screen = SnakeStart(screen, *snake);
	screen = FruitSpawn(screen, *fruit, *snake);

	while (1) {

		ScreenDisplay(screen);

		if (_kbhit()) {
			entry = _getch();

			if (entry == 224) {

				entry = _getch();

				switch (entry) {

				case 72:

					if(Direction != "down" && Direction != "up")
						Direction = "up";
					break;

				case 80:

					if(Direction != "up" && Direction != "down")
						Direction = "down";
					break;

				case 75:

					if(Direction != "right" && Direction != "left")
						Direction = "left";
					break;

				case 77:

					if(Direction != "left" && Direction != "right")
						Direction = "right";
					break;
				}

			}
		}

		screen = Update(screen, *snake, Direction);
		screen = FruitControl(screen, *fruit, *snake);
		if (IsGameOver(screen, *snake))
			break;
		Sleep(speed);
		system("cls");
	}

	system("pause");
	return 0;
}

vector<vector<char> > ScreenBegin(vector<vector<char> > Screen, int width, int length) {

	vector<char> temp, temp2;

	for (int i = 0; i < width; i++) {

		temp.push_back('#');

	}

	Screen.push_back(temp);
	temp2.push_back('#');

	for (int i = 0; i < width - 2; i++) {

		temp2.push_back(' ');

	}

	temp2.push_back('#');

	for (int i = 0; i < length - 2; i++) {

		Screen.push_back(temp2);

	}

	Screen.push_back(temp);

	return Screen;
}

void ScreenDisplay(vector<vector<char> > Screen) {

	for (int i = 0; i < Screen.size(); i++) {

		for (int j = 0; j < Screen[0].size(); j++)
			cout << Screen[i][j];

		cout << endl;

	}

	cout << setw(15) << "SCORE: " << score;

}

vector<vector<char> > Update(vector<vector<char> > Screen, vector<pair<int, int> > & Snake, string direction) {

	pair<int, int> temp = Snake.front(), temp2;

	Screen[Snake[0].first][Snake[0].second] = ' ';

	if (direction == "up") {

		Snake.front().first--;

	}

	else if (direction == "down") {

		Snake.front().first++;

	}

	else if (direction == "left") {

		Snake.front().second--;

	}

	else if (direction == "right") {

		Snake.front().second++;

	}

	Screen[Snake[0].first][Snake[0].second] = '0';

	for (int i = 1; i < Snake.size(); i++) {

		Screen[Snake[i].first][Snake[i].second] = ' ';

		temp2 = Snake[i];
		Snake[i] = temp;
		temp = temp2;

		Screen[Snake[i].first][Snake[i].second] = 'o';

	}

	return Screen;

}

bool IsGameOver(vector<vector<char> > Screen, vector<pair<int, int> > & Snake) {

	for (int i = 1; i < Snake.size(); i++) {

		if (Snake[0].first == Snake[i].first && Snake[0].second == Snake[i].second)
			return true;

	}

	if (Snake[0].first >= Screen.size() - 1 || Snake[0].first <= 0 || Snake[0].second >= Screen[0].size() - 1 || Snake[0].second <= 0)
		return true;

	else
		return false;

}

vector<vector<char> > FruitSpawn(vector<vector<char> > Screen, pair<int, int> & Fruit, vector<pair<int, int> > & Snake) {

	Fruit.first = rand() % (Screen.size() - 2) + 1;
	Fruit.second = rand() % (Screen[0].size() - 2) + 1;

	for (int i = 0; i < Snake.size(); i++) {

		if (Snake[i].first == Fruit.first && Snake[i].second == Fruit.second)
			Screen = FruitSpawn(Screen, Fruit, Snake);

	}

	Screen[Fruit.first][Fruit.second] = '*';

	return Screen;

}

vector<vector<char> > FruitControl(vector<vector<char> >Screen, pair<int, int> & Fruit, vector<pair<int, int> > & Snake) {

	if (Snake[0].first == Fruit.first && Snake[0].second == Fruit.second) {

		Screen[Fruit.first][Fruit.second] = ' ';
		Screen = FruitSpawn(Screen, Fruit, Snake);

		int y = Snake.back().first, x = Snake.back().second - 1;
		Snake.push_back({ y, x });
		score += 10;
		speed -= 10;

	}

	return Screen;

}

vector<vector<char> > SnakeStart(vector<vector<char> > Screen, vector<pair<int, int> >& Snake) {

	int Firstx = Screen[0].size() / 2;
	int Firsty = Screen.size() / 2;

	Screen[Firsty][Firstx] = '0';

	Snake.push_back({ Firsty, Firstx });

	for (int i = 0; i < 4; i++) {

		Snake.push_back({ Firsty, ++Firstx });

		Screen[Firsty][Firstx] = 'o';
		
	}

	return Screen;

}
