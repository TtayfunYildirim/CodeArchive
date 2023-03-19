#include <iostream>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

int main() {
	
	pair<int, int> yol;
	stack<pair<int, int> > track;

	vector<char> gecici;
	vector<vector<char> > maze;

	bool counter = 0; //backtracking için

	int yon[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

	int genislik, yukseklik;

	cout << "Genislik: "; cin >> genislik;
	cout << "Yukseklik: "; cin >> yukseklik;

	for (int j = 0; j < genislik; j++)
		gecici.push_back('#');

	for (int i = 0; i < yukseklik; i++) {

		maze.push_back(gecici);

	}

	srand( time( 0 ) );

	yol.first = rand() % (yukseklik - 1) + 1;
	yol.second = 1;

	maze[yol.first][0] = ' ';
	maze[yol.first][yol.second] = ' ';

	track.push(yol);

	while (1) {

		if (track.empty()) {
			break;
		}


		for (int i = 0; i < 4; i++) {

			int r = rand() % 4;
			swap(yon[r], yon[i]);

		}

		for (int i = 0; i < 4; i++) {

			pair<int, int> temp;
			temp.first = yol.first + yon[i][0];
			temp.second = yol.second + yon[i][1];


			int up = temp.first - 1;
			int down = temp.first + 1;
			int left = temp.second - 1;
			int right = temp.second + 1;


			counter = 1;

			if (temp.first < yukseklik - 1 && temp.second < genislik - 1 && temp.first > 0 && temp.second > 0) {

				maze[yol.first][yol.second] = '#';

				if (maze[up][temp.second] != ' ' && maze[down][temp.second] != ' ' && maze[temp.first][left] != ' ' && maze[temp.first][right] != ' ') {

					maze[yol.first][yol.second] = ' ';
					maze[temp.first][temp.second] = ' ';

					yol = temp;

					//maze[yol.first + 1][yol.second] = 'x'; maze[yol.first - 1][yol.second] = 'x'; maze[yol.first][yol.second + 1] = 'x'; maze[yol.first][yol.second - 1] = 'x';

					//maze[yol.first - yon[i][0]][yol.second - yon[i][1]] = ' ';

					track.push(yol);
					counter = 0;
					break;

				}
				maze[yol.first][yol.second] = ' ';
			}

		}

		if (counter) {

			yol = track.top();
			track.pop();
			counter = 0;

		}

	}
	while (1) {
		int cıkıs = rand() % yukseklik;
		if (maze[cıkıs][genislik - 2] != '#') {
			maze[cıkıs][genislik - 1] = 'y';
			break;
		}
	}

	ofstream yaz("maze.txt");

	for (int i = 0; i < yukseklik; i++) {

		for (int j = 0; j < genislik; j++) {
			cout << maze[i][j];
			yaz << maze[i][j];
		}
		cout << endl;
		yaz << endl;


	}

	
	yaz.close();
	

}