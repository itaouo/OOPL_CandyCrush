#include <stdafx.h>
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include<iostream>
#include <cstdlib>
#include <ctime>
#include<sstream>
#include <vector>
#include<math.h>
#include<utility>
#include <fstream>

using namespace std;
using namespace game_framework;

int idx0 = 0, idx1 = 0;
int idy0 = 0, idy1 = 0;
bool which_mou = 0;

vector<vector<int>> LoadMap(string map_name, int *row, int *column) {
	ifstream in;
	in.open("Resources/map/" + map_name + ".txt");
	in >> *row >> *column;
	vector<vector<int>> map(10);
	for (int i = 0; i < *row; i++) {
		for (int j = 0; j < *column; j++) {
			int x = 0;
			in >> x;
			map[i].push_back(x);
		}
	}
	in.close();

	return map;
}

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{

}

int w = 5, h = 5;
int which_candy[9][9] = { 0 };
vector<vector<int>> mp;

int rnd_number(int start, int end) {
	int min = start;
	int max = end;
	int x = rand() % (max - min + 1) + min;
	return x;
}

bool CheckInitCandy(int arr[9][9], int w, int h) {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (arr[i][j] >= 0) {
				if (i >= 2) {
					if (arr[i][j] % 10 == arr[i - 1][j] % 10 && arr[i][j] % 10 == arr[i - 2][j] % 10) {
						return true;
					}
				}
				if (j >= 2) {
					if (arr[i][j] % 10 == arr[i][j - 1] % 10 && arr[i][j] % 10 == arr[i][j - 2] % 10) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void delay(int ms) {
	int set_clock = clock() + ms;
	int now = clock();
	while (now < set_clock) {
		now = clock();
	}
}

bool LTypeCandy(int mp[9][9], int now_h,int now_w) {
	if (now_w >= 2 && now_h < h - 2) {
		if (mp[now_h][now_w] == mp[now_h][now_w - 1]
			&& mp[now_h][now_w] == mp[now_h][now_w - 2]
			&& mp[now_h][now_w] == mp[now_h + 1][now_w]
			&& mp[now_h][now_w] == mp[now_h + 2][now_w]) {
			return true;
		}
	}
	if (now_w < w - 2 && now_h < h - 2) {
		if (mp[now_h][now_w] == mp[now_h][now_w + 1]
			&& mp[now_h][now_w] == mp[now_h][now_w + 2]
			&& mp[now_h][now_w] == mp[now_h + 1][now_w]
			&& mp[now_h][now_w] == mp[now_h + 2][now_w]) {
			return true;
		}
	}
	if (now_w >= 2 && now_h >= 2) {
		if (mp[now_h][now_w] == mp[now_h][now_w - 1]
			&& mp[now_h][now_w] == mp[now_h][now_w - 2]
			&& mp[now_h][now_w] == mp[now_h - 1][now_w]
			&& mp[now_h][now_w] == mp[now_h - 2][now_w]) {
			return true;
		}
	}
	if (now_w < w - 2 && now_h >= 2) {
		if (mp[now_h][now_w] == mp[now_h][now_w + 1]
			&& mp[now_h][now_w] == mp[now_h][now_w + 2]
			&& mp[now_h][now_w] == mp[now_h - 1][now_w]
			&& mp[now_h][now_w] == mp[now_h - 2][now_w]) {
			return true;
		}
	}
	return false;
}

bool ITypeCandy(int mp[9][9], int now_h, int now_w) {
	if (now_h >= 3) {
		if (mp[now_h][now_w] == mp[now_h - 1][now_w]
			&& mp[now_h][now_w] == mp[now_h - 2][now_w]
			&& mp[now_h][now_w] == mp[now_h - 3][now_w]) {
			return true;
		}
	}
	return false;
}

bool TTypeCandy(int mp[9][9], int now_h, int now_w) {
	if (now_w < w - 2 && now_h >= 1 && now_h < h - 1) {
		if (mp[now_h][now_w] == mp[now_h - 1][now_w]
			&& mp[now_h][now_w] == mp[now_h + 1][now_w]
			&& mp[now_h][now_w] == mp[now_h][now_w + 1]
			&& mp[now_h][now_w] == mp[now_h][now_w + 2]) {
			return true;
		}
	}
	if (now_w >= 2 && now_h >= 1 && now_h < h - 1) {
		if (mp[now_h][now_w] == mp[now_h - 1][now_w]
			&& mp[now_h][now_w] == mp[now_h + 1][now_w]
			&& mp[now_h][now_w] == mp[now_h][now_w - 1]
			&& mp[now_h][now_w] == mp[now_h][now_w - 2]) {
			return true;
		}
	}
	if (now_h >= 2 && now_w >= 1 && now_w < w - 1) {
		if (mp[now_h][now_w] == mp[now_h][now_w - 1]
			&& mp[now_h][now_w] == mp[now_h][now_w + 1]
			&& mp[now_h][now_w] == mp[now_h - 1][now_w]
			&& mp[now_h][now_w] == mp[now_h - 2][now_w]) {
			return true;
		}
	}
	if (now_h < h - 2 && now_w >= 1 && now_w < w - 1) {
		if (mp[now_h][now_w] == mp[now_h][now_w - 1]
			&& mp[now_h][now_w] == mp[now_h][now_w + 1]
			&& mp[now_h][now_w] == mp[now_h + 1][now_w]
			&& mp[now_h][now_w] == mp[now_h + 2][now_w]) {
			return true;
		}
	}
	return false;
}

bool ETypeCandy(int mp[9][9], int now_h, int now_w) {
	if (now_w > 0 && now_w < w - 2) {
		if (mp[now_h][now_w] % 10 == mp[now_h][now_w - 1] % 10
			&& mp[now_h][now_w] % 10 == mp[now_h][now_w + 1] % 10
			&& mp[now_h][now_w] % 10 == mp[now_h][now_w + 2] % 10) {
			return true;
		}
	}
	if (now_w > 1 && now_w < w - 1) {
		if (mp[now_h][now_w] % 10 == mp[now_h][now_w - 1] % 10
			&& mp[now_h][now_w] % 10 == mp[now_h][now_w + 1] % 10
			&& mp[now_h][now_w] % 10 == mp[now_h][now_w - 2] % 10) {
			return true;
		}
	}
	return false;
}
void CGameStateRun::update_candy() {
	for (int i = h - 1; i >= 0; i--) {
		for (int j = w - 1; j >= 0; j--) {
			if (which_candy[i][j] == -1) {
				candy[i][j].SetFrameIndexOfBitmap(26);
			}
			else if (which_candy[i][j] <= -60) {
				candy[i][j].SetFrameIndexOfBitmap(31);
			}
			else if (which_candy[i][j] <= -10) {
				candy[i][j].SetFrameIndexOfBitmap(std::abs(which_candy[i][j]) + 17);
			}
			else {
				candy[i][j].SetFrameIndexOfBitmap(which_candy[i][j] / 10 * 6 + which_candy[i][j] % 10);
			}
		}
	}
}
bool disapear = 0;
vector<vector<int>> CGameStateRun::CheckMapStatus(int mp[9][9], int w, int h) {
	vector<vector<int>> status(9);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			status[i].push_back(1);
			if (which_candy[i][j] == -10) {
				status[i][j] = 2;
				disapear = 1;
			}
			if (i >= 2 && which_candy[i][j] >= 0) {
				if (mp[i][j]%10 == mp[i - 1][j]%10 && mp[i][j]%10 == mp[i - 2][j]%10) {
					status[i][j] = 0;
					status[i - 1][j] = 0;
					status[i - 2][j] = 0;
				}
			}
			if (j >= 2 && which_candy[i][j] >= 0) {
				if (mp[i][j]%10 == mp[i][j - 1]%10 && mp[i][j]%10 == mp[i][j - 2]%10) {
					status[i][j] = 0;
					status[i][j - 1] = 0;
					status[i][j - 2] = 0;
				}
			}
			
		}
	}

	int ii = (idy0 - (400 - 25 * h)) / 50;
	int jj = (idx0 - (400 - 25 * w)) / 50;
	if (ETypeCandy(mp, ii, jj) ){
		TRACE("Hello");
		status[ii][jj] = 1;
		candy[ii][jj].SetFrameIndexOfBitmap(which_candy[ii][jj] + 20);
		which_candy[ii][jj] %= 10;
		which_candy[ii][jj] += 30;
		disapear = 1;
		return status;
	}
	ii= (idy1 - (400 - 25 * h)) / 50;
	jj= (idx1 - (400 - 25 * w)) / 50;
	if (ETypeCandy(mp, ii, jj)) {
		TRACE("Hello");
		status[ii][jj] = 1;
		candy[ii][jj].SetFrameIndexOfBitmap(which_candy[ii][jj] + 20);
		which_candy[ii][jj] %= 10;
		which_candy[ii][jj] += 30;
		disapear = 1;
		return status;
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (LTypeCandy(mp, i, j)||TTypeCandy(mp, i, j)) {
				status[i][j] = 1;
				candy[i][j].SetFrameIndexOfBitmap(which_candy[i][j] + 6);
				which_candy[i][j] %= 10;
				which_candy[i][j] += 10;
				disapear = 1;
			}
			if (ITypeCandy(mp, i, j)) {
				status[i][j] = 1;
				candy[i][j].SetFrameIndexOfBitmap(which_candy[i][j] + 12);
				which_candy[i][j] %= 10;
				which_candy[i][j] += 20;
				disapear = 1;
			}
			if (ETypeCandy(mp, i, j)) {
				status[i][j] = 1;
				candy[i][j].SetFrameIndexOfBitmap(which_candy[i][j] + 20);
				which_candy[i][j] %= 10;
				which_candy[i][j] += 30;
				disapear = 1;
			}
		}
	}
	return status;
}

void CGameStateRun::DropOneSquare() {

	for (int i = 0; i < 5; i++) {
		character.SetTopLeft(0, i * 20);
		int set_clock = clock() + 50;
		int now = clock();
		while (now < set_clock) {
			now = clock();
		}
	}


}


vector<vector<int>> UpdateMap(vector<vector<int>> mp, int i, int j) {
	for (int k = i; k > 0; k--) {
		mp[k][j] = mp[k - 1][j];
	}
	mp[0][j] = rnd_number(0, 5);
	return mp;
}

void CGameStateRun::vertical_fall_candy(int i,int j) {
	if (i == 0) { //potential bug (fall candy probably not start from i=0)
		//which_candy[0][j] = -10;
		return;
	}
	if (which_candy[i - 1][j] >= 0) {
		which_candy[i][j] = which_candy[i - 1][j];
		vertical_fall_candy(i - 1, j);
		return;
	}
	if (j >= 0){
		if (which_candy[i - 1][j - 1] >= 0) {
			which_candy[i][j] = which_candy[i - 1][j - 1];
			vertical_fall_candy(i - 1, j - 1);
			return;
		}
	}
	if (j <= w - 1) {
		if (which_candy[i - 1][j + 1] >= 0) {
			which_candy[i][j] = which_candy[i - 1][j + 1];
			vertical_fall_candy(i - 1, j + 1);
			return;
		}
	}
	which_candy[i][j] = -99;
}

void CGameStateRun::remove_obstacle_layer(int i, int j){
	if (i > 0) {
		if (which_candy[i - 1][j] < 0 && which_candy[i - 1][j] != -10) {
			which_candy[i - 1][j] += 1;
		}
	}
	if (j > 0) {
		if (which_candy[i][j - 1] < 0 && which_candy[i][j - 1] != -10) {
			which_candy[i][j - 1] += 1;
		}
	}
	if (i < h-1) {
		if (which_candy[i + 1][j] < 0 && which_candy[i + 1][j] != -10) {
			which_candy[i + 1][j] += 1;
		}
	}
	if (j < w-1) {
		if (which_candy[i][j + 1] < 0 && which_candy[i][j + 1] != -10) {
			which_candy[i][j + 1] += 1;
		}
	}
}
void CGameStateRun::OnMove()							// ç§»ï¿½???????ï¿½ï¿½??ï¿??
{
	if (character.IsOverlap(character, chest_and_key)) {
		chest_and_key.SetFrameIndexOfBitmap(1);

	}
	for (int i = 0; i < 3; i++) {
		if (character.IsOverlap(character, door[i])) {
			door[i].SetFrameIndexOfBitmap(1);
		}
	}
	vector<vector<int>> status = CheckMapStatus(which_candy, w, h);
	if (CheckInitCandy(which_candy, 5, 5)||disapear) {
		disapear = 0;
		for (int i = h - 1; i >= 0; i--) {
			for (int j = 0; j < w; j++) {
				if (status[i][j] == 0 && which_candy[i][j] != -10) {
					character.SetTopLeft(600, 600);
					for (int k = i - 1; k >= 0; k--) {
						for (int i = 0; i < 5; i++) {
							//candy[k][j].SetTopLeft(candy[k][j].GetLeft(), candy[k][j].GetTop() + 10);
						} //candy[k][j]
					}
					remove_obstacle_layer(i, j);
					vertical_fall_candy(i, j);
					which_candy[0][j] = rnd_number(0, 3);
					update_candy();
				}
				else if (status[i][j] == 2){
					vertical_fall_candy(i, j);
					which_candy[0][j] = rnd_number(0, 3);
					update_candy();
					disapear = 0;
				}
			}
		}
		disapear = 0;
		idx0 = 0, idx1 = 0;
		idy0 = 0, idy1 = 0;
		which_mou = 0;
		delay(1500);
	}

}

void CGameStateRun::OnInit()
{

	background.LoadBitmapByString({
		"resources/texture_pack_original/bg_screens/3.bmp",
		"resources/texture_pack_original/bg_screens/2.bmp",
		"resources/phase12_background.bmp",
		"resources/phase21_background.bmp",
		"resources/phase22_background.bmp",
		"resources/phase31_background.bmp",
		"resources/phase32_background.bmp",
		"resources/phase41_background.bmp",
		"resources/phase42_background.bmp",
		"resources/phase51_background.bmp",
		"resources/phase52_background.bmp",
		"resources/phase61_background.bmp",
		"resources/phase62_background.bmp",
		});
	background.SetTopLeft(0, 0);
	vector<vector<int>> mp = LoadMap("1", &h, &w);

	chest_and_key.LoadBitmapByString({ "resources/chest.bmp", "resources/chest_ignore.bmp" }, RGB(255, 255, 255));
	chest_and_key.SetTopLeft(150, 430);

	bee.LoadBitmapByString({ "resources/bee_1.bmp", "resources/bee_2.bmp" });
	bee.SetTopLeft(462, 265);
	bee.SetAnimation(10, false);

	ball.LoadBitmapByString({ "resources/ball-3.bmp", "resources/ball-3.bmp", "resources/ball-2.bmp", "resources/ball-1.bmp", "resources/ball-ok.bmp" });
	ball.SetTopLeft(150, 430);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {

			candy[i][j].LoadBitmapByString({
				"Resources/texture_pack_original/candy/0.bmp",
				"Resources/texture_pack_original/candy/1.bmp",
				"Resources/texture_pack_original/candy/2.bmp",
				"Resources/texture_pack_original/candy/3.bmp",
				"Resources/texture_pack_original/candy/4.bmp",
				"Resources/texture_pack_original/candy/5.bmp",
				"Resources/texture_pack_original/candy/10.bmp",
				"Resources/texture_pack_original/candy/11.bmp",
				"Resources/texture_pack_original/candy/12.bmp",
				"Resources/texture_pack_original/candy/13.bmp",
				"Resources/texture_pack_original/candy/14.bmp",
				"Resources/texture_pack_original/candy/15.bmp",
				"Resources/texture_pack_original/candy/20.bmp",
				"Resources/texture_pack_original/candy/21.bmp",
				"Resources/texture_pack_original/candy/22.bmp",
				"Resources/texture_pack_original/candy/23.bmp",
				"Resources/texture_pack_original/candy/24.bmp",
				"Resources/texture_pack_original/candy/25.bmp",
				"Resources/texture_pack_original/candy/30.bmp",
				"Resources/texture_pack_original/candy/31.bmp",
				"Resources/texture_pack_original/candy/32.bmp",
				"Resources/texture_pack_original/candy/33.bmp",
				"Resources/texture_pack_original/candy/34.bmp",
				"Resources/texture_pack_original/candy/35.bmp",
				"Resources/texture_pack_original/candy/40.bmp",
				"Resources/texture_pack_original/candy/50.bmp",
				"Resources/texture_pack_original/candy/-1.bmp", //frame: 26
				"Resources/texture_pack_original/candy/-10.bmp",
				"Resources/texture_pack_original/candy/-11.bmp",
				"Resources/texture_pack_original/candy/-12.bmp",
				"Resources/texture_pack_original/candy/-13.bmp",
				"Resources/texture_pack_original/candy/-99.bmp"
				});
			candy[i][j].SetTopLeft((400 - 25 * w) + j * 50, (400 - 25 * h) + i * 50);
			which_candy[i][j] = mp[i][j];
		}
	}
	update_candy();
	for (int i = 0; i < 3; i++) {
		door[i].LoadBitmapByString({ "resources/door_close.bmp", "resources/door_open.bmp" }, RGB(255, 255, 255));
		door[i].SetTopLeft(462 - 100 * i, 265);
	}
	character.LoadBitmapByString({ "resources/giraffe.bmp" });
	character.SetTopLeft(150, 265);
	cursor.LoadBitmapByString({ "Resources/texture_pack_original/cursor.bmp" }, RGB(255, 255, 255));
	cursor.SetTopLeft((400 - 25 * w), (400 - 25 * h));
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int tp = character.GetTop(), lft = character.GetLeft();
	if (nChar == VK_LEFT) {
		character.SetTopLeft(lft - 10, tp);
	}
	if (nChar == VK_RIGHT) {
		character.SetTopLeft(lft + 10, tp);
	}
	if (nChar == VK_UP) {
		character.SetTopLeft(lft, tp - 10);
	}
	if (nChar == VK_DOWN) {
		character.SetTopLeft(lft, tp + 10);
	}

	if (nChar == VK_RETURN) {
		if (phase == 1) {
			if (sub_phase == 1) {
				sub_phase += validate_phase_1();
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
			}
		}
		else if (phase == 2) {
			if (sub_phase == 1) {
				sub_phase += validate_phase_2();
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
			}
		}
		else if (phase == 3) {
			if (sub_phase == 1) {
				sub_phase += validate_phase_3();
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
			}
		}
		else if (phase == 4) {
			if (sub_phase == 1) {
				sub_phase += validate_phase_4();
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
			}
		}
		else if (phase == 5) {
			if (sub_phase == 1) {
				sub_phase += validate_phase_5();
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
			}
		}
		else if (phase == 6) {
			if (sub_phase == 1) {
				sub_phase += validate_phase_6();
			}
			else if (sub_phase == 2) {
				sub_phase = 1;
				phase += 1;
				GotoGameState(GAME_STATE_OVER);
			}
		}
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void swtch(int now_x, int now_y, int pr_x, int pr_y) {
	int xx = which_candy[now_y][now_x];
	which_candy[now_y][now_x] = which_candy[pr_y][pr_x];
	which_candy[pr_y][pr_x] = xx;
}
bool CGameStateRun::CanDelete() {
	int row0 = (idy0 - (400 - 25 * h)) / 50;
	int column0 = (idx0 - (400 - 25 * w)) / 50;
	int row1 = (idy1 - (400 - 25 * h)) / 50;
	int column1 = (idx1 - (400 - 25 * w)) / 50;
	if (!(std::abs(row0 - row1) == 1 && std::abs(column0 - column1) == 0) && !(std::abs(row0 - row1) == 0 && std::abs(column0 - column1) == 1)) {
		return false;
	}
	if (which_candy[row0][column0] < -10 || which_candy[row1][column1] < -10) {
		return false;
	}
	swtch(column0, row0
		, column1, row1);
	if (CheckInitCandy(which_candy, w, h)) {
		return true;
	}
	swtch(column0, row0
		, column1, row1);
	return false;
}
bool inSquare() {
	int potx = (400 - 25 * w);
	int poty = (400 - 25 * h);
	if (idx0 < potx || idx1 < potx) {
		return 0;
	}
	if (idx0 > potx + 50 * w || idx1 > potx + 50 * w) {
		return 0;
	}
	if (idy0 < poty || idy1 < poty) {
		return 0;
	}
	if (idy0 > poty + 50 * h || idy1 > poty + 50 * h) {
		return 0;
	}
	return 1;
}

bool oneInSquare() {
	int potx = (400 - 25 * w);
	int poty = (400 - 25 * h);

	if (which_mou) {
		if (idx0 < potx || idx0 > potx + 50 * w || idy0 < poty || idy0 > poty + 50 * h) {
			return 0;
		}
		return 1;
	}
	else {
		if (idx1 < potx || idx1 > potx + 50 * w || idy1 < poty || idy1 > poty + 50 * h) {
			return 0;
		}
		return 1;
	}
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (which_mou) {
		idx1 = point.x;
		idy1 = point.y;
		which_mou = 0;
	}
	else {
		idx0 = point.x;
		idy0 = point.y;
		which_mou = 1;
	}
	character.SetTopLeft(idx0, idy0);
	if (inSquare() && CanDelete()) {
		int row0 = (idy0 - (400 - 25 * h)) / 50;
		int column0 = (idx0 - (400 - 25 * w)) / 50;
		int row1 = (idy1 - (400 - 25 * h)) / 50;
		int column1 = (idx1 - (400 - 25 * w)) / 50;
		
		candy[(idy0 - (400 - 25 * h)) / 50][(idx0 - (400 - 25 * w)) / 50]
			.SetFrameIndexOfBitmap(which_candy[(idy0 - (400 - 25 * h)) / 50][(idx0 - (400 - 25 * w)) / 50]);
		candy[(idy1 - (400 - 25 * h)) / 50][(idx1 - (400 - 25 * w)) / 50]
			.SetFrameIndexOfBitmap(which_candy[(idy1 - (400 - 25 * h)) / 50][(idx1 - (400 - 25 * w)) / 50]);
		candy[(idy0 - (400 - 25 * h)) / 50][(idx0 - (400 - 25 * w)) / 50]
			.SetFrameIndexOfBitmap(which_candy[(idy0 - (400 - 25 * h)) / 50][(idx0 - (400 - 25 * w)) / 50]);
		candy[(idy1 - (400 - 25 * h)) / 50][(idx1 - (400 - 25 * w)) / 50]
			.SetFrameIndexOfBitmap(which_candy[(idy1 - (400 - 25 * h)) / 50][(idx1 - (400 - 25 * w)) / 50]);
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				candy[i][j].ShowBitmap();
			}
		}
	}

	/*if (oneInSquare()) {
		cursor.SetTopLeft((point.x - (400 - 25 * w)) / 50 * 50 + (400 - 25 * w),
			(point.y - (400 - 25 * h)) / 50 * 50 + (400 - 25 * h));
		cursor.ShowBitmap();
	}*/


}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)
{


}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)
{
}

void CGameStateRun::OnShow()
{
	show_image_by_phase();
	show_text_by_phase();
}

void CGameStateRun::show_image_by_phase() {
	if (phase <= 6) {
		background.SetFrameIndexOfBitmap((phase - 1) * 2 + (sub_phase - 1));
		background.ShowBitmap();
		character.ShowBitmap();
		
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				candy[i][j].ShowBitmap();
			}
		}
		cursor.ShowBitmap();

		if (phase == 3 && sub_phase == 1) {
			chest_and_key.ShowBitmap();
		}
		if (phase == 4 && sub_phase == 1) {
			bee.ShowBitmap();
		}
		if (phase == 5 && sub_phase == 1) {
			for (int i = 0; i < 3; i++) {
				door[i].ShowBitmap();
			}
		}
		if (phase == 6 && sub_phase == 1) {
			ball.ShowBitmap();
		}
	}
}

void CGameStateRun::show_text_by_phase() {
	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "å¾®ï¿½??ï¿???ï¿???ï¿???", RGB(0, 0, 0), 800);

	if (phase == 1 && sub_phase == 1) {
		CTextDraw::Print(pDC, 50, 70, "timer: " + to_string(clock()));
	}
	else if (phase == 2 && sub_phase == 1) {
		CTextDraw::Print(pDC, 26, 128, "ï¿???ï¿?????????æ®µï¿½??ï¿?????ï¿½ï¿½?ï¿½é¹¿??ï¿½ï¿½????????ï¿???ï¿???å·¦ï¿½?ï¿½ç§»?????ï¿½ï¿½?????ï¿???ç½®ï¿½??");
		CTextDraw::Print(pDC, 373, 537, "???ï¿??? Enter ??ï¿½ï¿½??ï¿???ï¿???");
	}
	else if (phase == 3 && sub_phase == 1) {
		CTextDraw::Print(pDC, 205, 128, "å¹«ï¿½??ï¿??????ï¿???ï¿??????å¯¶ç®±");
		CTextDraw::Print(pDC, 68, 162, "è¨­ï¿½??ï¿???ï¿???ï¿?????ï¿½ï¿½?ï¿½é¹¿??ï¿½ï¿½?ï¿½å¯¶ç®±ï¿½??ï¿???ï¿???å¯¶ç®±ï¿???å¤±ï¿½??");
		CTextDraw::Print(pDC, 68, 196, "ï¿???ï¿???å¯¶ç®±ï¿?????ï¿½ï¿½??ï¿???ä½¿ï¿½?? RGB(255, 255, 255)");
		CTextDraw::Print(pDC, 373, 537, "???ï¿??? Enter ??ï¿½ï¿½??ï¿???ï¿???");
	}
	else if (phase == 4 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "å¹«ï¿½??ï¿??????ï¿???ï¿????????????å¥½ï¿½?????");
		CTextDraw::Print(pDC, 89, 162, "å·²ï¿½??å¹«ï¿½?????ï¿?????ï¿½ï¿½??????????ï¿½ï¿½??ï¿???ï¿?????ï¿½ä»¥ï¿???ï¿???ç§»ï¿½??");
		CTextDraw::Print(pDC, 110, 196, "å¯«ï¿½??ï¿???ï¿???ï¿???ï¿???ï¿????????????å¥½ï¿½????????????????ï¿½ï¿½??");
		CTextDraw::Print(pDC, 373, 537, "???ï¿??? Enter ??ï¿½ï¿½??ï¿???ï¿???");
	}
	else if (phase == 5 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "å¹«ï¿½??ï¿??????ï¿???ï¿?????????");
		CTextDraw::Print(pDC, 89, 162, "è¨­ï¿½??ï¿???ï¿???ï¿?????ï¿½ï¿½?ï¿½é¹¿??ï¿½ï¿½?ï¿½ï¿½??ï¿???ï¿???ï¿???????????????");
		CTextDraw::Print(pDC, 373, 537, "???ï¿??? Enter ??ï¿½ï¿½??ï¿???ï¿???");
	}
	else if (phase == 6 && sub_phase == 1) {
		CTextDraw::Print(pDC, 173, 128, "å¹«ï¿½??ï¿??????ï¿???ï¿???ï¿???????????ï¿½ï¿½?????");
		CTextDraw::Print(pDC, 89, 162, "è¨­ï¿½??ï¿???ï¿???ï¿???????????ï¿½ï¿½????ï¿½ï¿½??é¡¯ç¤º OK ï¿??????æ­¢ï¿½?????");
		CTextDraw::Print(pDC, 373, 537, "???ï¿??? Enter ??ï¿½ï¿½??ï¿???ï¿???");
	}
	else if (sub_phase == 2) {
		CTextDraw::Print(pDC, 268, 128, "ï¿??????ï¿???");
	}

	CDDraw::ReleaseBackCDC();
}

bool CGameStateRun::validate_phase_1() {
	return character.GetImageFileName() == "resources/giraffe.bmp";
}

bool CGameStateRun::validate_phase_2() {
	return character.GetTop() > 204 && character.GetTop() < 325 && character.GetLeft() > 339 && character.GetLeft() < 459;
}

bool CGameStateRun::validate_phase_3() {
	return (
		character.GetTop() + character.GetHeight() >= chest_and_key.GetTop()
		&& character.GetLeft() + character.GetWidth() >= chest_and_key.GetLeft()
		&& chest_and_key.GetFrameIndexOfBitmap() == 1
		&& chest_and_key.GetFilterColor() == RGB(255, 255, 255)
		);
}

bool CGameStateRun::validate_phase_4() {
	return bee.IsAnimation() && bee.GetFrameSizeOfBitmap() == 2;
}

bool CGameStateRun::validate_phase_5() {
	bool check_all_door_is_open = true;
	for (int i = 0; i < 3; i++) {
		check_all_door_is_open &= (door[i].GetFrameIndexOfBitmap() == 1);
	}
	return check_all_door_is_open;
}

bool CGameStateRun::validate_phase_6() {
	return ball.IsAnimationDone() && !ball.IsAnimation() && ball.GetFrameIndexOfBitmap() == ball.GetFrameSizeOfBitmap() - 1;
}