#include <stdafx.h>
#include <mmsystem.h>
#include <ddraw.h>
#include<sstream>
#include<fstream>
#include "../Core/Resource.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "WinRule.h"

using namespace game_framework;

WinRule::WinRule() {

}
void WinRule::Init() {
	for (int i = 0; i < 4; i++) {
		condition_number[i].clear();
	}
	
	for (int i = 0; i < 255; i++) {
		score_image[i].LoadBitmapByString({ "resources/texture_pack_original/bg_screens/score.bmp" });
		score_image[i].SetTopLeft(270, 25);
		score_image[i].SetFrameIndexOfBitmap(0);
	}
	score_edge.LoadBitmapByString({ "resources/texture_pack_original/bg_screens/score_edge.bmp" });
	score_edge.SetTopLeft(270, 25);
	score_edge.SetFrameIndexOfBitmap(0);
	condition_icon.LoadBitmapByString({
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
		"Resources/texture_pack_original/candy/-1.bmp",
		"Resources/texture_pack_original/candy/-10.bmp",
		"Resources/texture_pack_original/candy/-11.bmp",
		"Resources/texture_pack_original/candy/-12.bmp",
		"Resources/texture_pack_original/candy/-13.bmp",
		"Resources/texture_pack_original/candy/-99.bmp",
		"Resources/texture_pack_original/candy/99.bmp",
		"Resources/texture_pack_original/candy/999.bmp",
		"Resources/texture_pack_original/candy/7.bmp",
		"Resources/texture_pack_original/ice/ice1.bmp" }, RGB(255,255,255));
	
	
}

void WinRule::BeginState() {
	for (int i = 0; i < 4; i++) {
		condition_number[i].clear();
	}
	LoadWinCondition();
	score_edge.SetTopLeft(268, 23);
	score = 0;
	all_condition_number = 0;
	score_edge.SetFrameIndexOfBitmap(0);
	for (int i = 0; i < 255; i++) {
		score_image[i].SetTopLeft(272, 25);
		score_image[i].SetFrameIndexOfBitmap(0);
	}
}

void WinRule::LoadWinCondition() {
	ifstream in;
	int map_name;
	in.open("Resources/init_map/choose_level.txt");
	in >> map_name;
	in.close();
	in.open("Resources/win_rules/" + to_string(map_name) + ".txt");
	in >> moves;
	for (int i = 0; i < 3; i++) {
		in >> star_score[i];
	}
	for (int i = 0; i < 4; i++) {
		in >> has_conditon_type[i];
		if (has_conditon_type[i]) {
			int t, n;
			in >> t;
			while (t != -1) {
				all_condition_number += 1;
				in >> n;
				condition_number[i].push_back({ t,n });
				in >> t;
			}
		}
	}
	in.close();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < int(condition_number[i].size()); j++) {
			TRACE("number:%d\n", condition_number[i][j].first);
			condition_icon.SetFrameIndexOfBitmap(image_index(condition_number[i][j].first));
			condition_icon.SetTopLeft(300, 70);
		}
	}
}

void WinRule::Show() {
	score_edge.ShowBitmap();
	for (int i = 0; i < 250; i++) {
		score_image[i].ShowBitmap();
	}
	condition_icon.ShowBitmap();
}

int WinRule::image_index(int x) {
	if (x == -1) {
		return 26;
	}
	else if (x == -21)
	{
		return 25;
	}
	else if (x == -999) {
		return 35;
	}
	else if (x >= 60 && x <= 65) {
		return x % 10 + x / 6 / 10 * 6;
	}
	else if (x >= 60) {
		return 32;
	}
	else if (x <= -10) {
		return std::abs(x) + 17;
	}
	else if (x == 7) {
		return 34;
	}
	else if (x >= 0 && x <= 35) {
		return x / 10 * 6 + x % 10;
	}
	else {
		return 33;
	}
}

bool WinRule::isWin() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j<int(condition_number[i].size()); j++) {
			TRACE("con: %d\n", condition_number[i][j].second);
			if (condition_number[i][j].second > 0) {
				return false;
			}
		}
	}
	if (score < star_score[0]) {
		return false;
	}
	return true;
}

bool WinRule::isGameOver() {
	if (moves > 0) {
		return false;
	}
	return true;
}