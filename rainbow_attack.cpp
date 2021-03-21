#include <bangtal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int startX = 537, startY = 220;

int whiteX = 5, whiteY = 5;
ObjectID startButton;
SceneID scene1, scene2;
SoundID bgm;

struct obejct {
	ObjectID object;
	int oriX, oriY; //original x & y
	int nowX, nowY;
	bool whiteFlag = false;
}Table[6][6];

bool checkComplete() {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (Table[i][j].oriX != i || Table[i][j].oriY != j) {
				return false;
			}
		}
	}

	return true;
}

void swap(int X1, int Y1, int X2, int Y2) {
	if (X1 == X2 && Y1 == Y2) return;
	
	swap(Table[X1][Y1], Table[X2][Y2]);
	swap(Table[X1][Y1].nowX, Table[X2][Y2].nowX);
	swap(Table[X1][Y1].nowY, Table[X2][Y2].nowY);

	if (whiteX == X1 && whiteY == Y1) {
		whiteX = Table[whiteX][whiteY].nowX;
		whiteY = Table[whiteX][whiteY].nowY;
	}
	else if (whiteX == X2 && whiteY == Y2) {
		whiteX = Table[whiteX][whiteY].nowX;
		whiteY = Table[whiteX][whiteY].nowY;
	}

	locateObject(Table[X1][Y1].object, scene2, X2 * 104 + 331, Y2 * 104 + 48);
	locateObject(Table[X2][Y2].object, scene2, X1 * 104 + 331, Y1 * 104 + 48);
}

void shuffle() {
	for (int n = 0; n < 5; n++) {
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				swap(i, j, rand() % 6, rand() % 6);
			}
		}
	}
}

bool nearWhite(int x, int y) {
	if (-1 <= whiteX - x && whiteX - x <= 1 && -1 <= whiteY - y && whiteY - y <= 1) return true;
	else return false;
}

int xtoX(int n) { return (int)((n - 331) / 104); }
int ytoY(int n) { return (int)((n - 48) / 104); }

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	int X = xtoX(x), Y = ytoY(y);
	
	if (object == startButton) {
		enterScene(scene2);
		playSound(bgm, true);
		shuffle();
	}
	else if (0 < X && X < 6 && 0 < Y && Y < 6) {
		if (nearWhite(X, Y));///////////////////////////////////////////////////
		swap(Table[X][Y], Table[whiteX][whiteY]);
		swap(Table[X][Y].nowX, Table[whiteX][whiteY].nowX);
		swap(Table[X][Y].nowY, Table[whiteX][whiteY].nowY);
		whiteX = Table[whiteX][whiteY].nowX;
		whiteY = Table[whiteX][whiteY].nowY;
		locateObject(Table[X][Y].object, scene2, X * 104 + 331, Y * 104 + 48);
		locateObject(Table[whiteX][whiteY].object, scene2, whiteX * 104 + 331, whiteY * 104 + 48);

		if (checkComplete()) {
			enterScene(scene1);
			stopSound(bgm);
			showMessage("Congratulations! You cleard this Game!");
		}
	}
}

ObjectID createObject(const char* image, SceneID scene, int x, int y) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	showObject(object);
	
	return object;
}

void createTableArray() {
	char name[50][50] = {
		"Images/1.png", "Images/2.png", "Images/3.png", "Images/4.png", "Images/5.png", "Images/6.png", "Images/7.png", "Images/8.png", "Images/9.png", "Images/10.png",
		"Images/11.png", "Images/12.png", "Images/13.png", "Images/14.png", "Images/15.png", "Images/16.png", "Images/17.png", "Images/18.png", "Images/19.png", "Images/20.png",
		"Images/21.png", "Images/22.png", "Images/23.png", "Images/24.png", "Images/25.png", "Images/26.png", "Images/27.png", "Images/28.png", "Images/29.png", "Images/30.png",
		"Images/31.png", "Images/32.png", "Images/33.png", "Images/34.png", "Images/35.png", "Images/36.png"
	};

	int x = 0;

	for (int j = 5; j >= 0; j--) {
		for (int i = 0; i < 6; i++) {
			Table[i][j].object = createObject(name[x++], scene2, i*104+331, j*104+48);
		}
	}

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			Table[i][j].oriX = i, Table[i][j].oriY = j;
			Table[i][j].nowX = i, Table[i][j].nowY = j;
		}
	}

	Table[5][5].whiteFlag = true;
}

int main() {
	srand((unsigned int)time(0));

	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setMouseCallback(mouseCallback);

	bgm = createSound("Sounds/토끼가 깡총.mp3");

	scene1 = createScene("원본", "Images/원본.png");
	scene2 = createScene("배경", "Images/배경.png");
	
	createTableArray();
	
	startButton = createObject("Images/start.png", scene1, startX, startY);
	scaleObject(startButton, 2.0f);

	startGame(scene1);
}