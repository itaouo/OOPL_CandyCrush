#include <stdafx.h>
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <math.h>
#include <utility>
#include <fstream>
#include <queue>
#include <algorithm>
#include "Map.h"
#include "Candy.h"
using namespace std;
using namespace game_framework;



CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{	
	map.BeginState();

	cursor.LoadBitmapByString({ "Resources/texture_pack_original/cursor.bmp" }, RGB(255, 255, 255));
	cursor.SetTopLeft(-100,-100);

}



int rnd_number(int start, int end) { //v
	int min = start;
	int max = end;
	int x = rand() % (max - min + 1) + min;

	return x;
}


void delay(int ms) {
	int set_clock = clock() + ms;
	int now = clock();
	while (now < set_clock) {
		now = clock();
	}
}

void CGameStateRun::OnMove()
{	
	map.animatedCandy();

	if (!map.is_animating()) {

		if (!map.still_fall()) {
			map.checkMapStatus();
			map.removeObstacleLayerAll();
		}
		if (!map.still_fall()) {
			map.Shuffle();
		}

		map.updateMap();

		if (map.still_fall()) {

			map.fallCandyAll();
			map.idx0 = 0, map.idx1 = 0, map.idy0 = 0, map.idy1 = 0;
			for (int i = 0; i < map.height(); i++) {
				for (int j = 0; j < map.width(); j++) {
					map._candy_map[i][j]._will_be_special_candy = 0;
				}
			}
			
		}
	}

	map.ScoreAndMovesCalculate();

	if (map._win_rule.isWin()) {
		ofstream ofs("Resources/win_or_loose.txt");
		ofs << 'W';
		ofs.close();
		GotoGameState(GAME_STATE_OVER);
		
	}
	else if (map._win_rule.isGameOver()) {
		ofstream ofs("Resources/win_or_loose.txt");
		ofs << 'L';
		ofs.close();
		GotoGameState(GAME_STATE_OVER);
	}

}

void CGameStateRun::OnInit()
{
	map.Init();
	
	background.LoadBitmapByString({ "resources/texture_pack_original/bg_screens/game.bmp" });
	background.SetTopLeft(0, 0);

	cursor.LoadBitmapByString({ "Resources/texture_pack_original/cursor.bmp" }, RGB(255, 255, 255));
	cursor.SetTopLeft(-100, -100);

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) {
		previous_map();
	}
	if (nChar == VK_RIGHT) {
		next_map();
	}
	if (nChar == 0x51) { // Q
		map._animation_speed = 0;
	}
	if (nChar == 0x57) { // W
		map._animation_speed = 5;
	}
	if (nChar == 0x45) { // E
		map._animation_speed = 10;
	}
	if (nChar == 0x52) { // R
		map._animation_speed = 25;
	}
}

void CGameStateRun::previous_map() {
	ifstream in;
	int map_name;
	in.open("Resources/init_map/choose_level.txt");
	in >> map_name;
	in.close();
	if (map_name - 1 > 0) {
		ofstream ofs("Resources/init_map/choose_level.txt");
		ofs << map_name - 1;
		ofs.close();
		ofstream ofs2("Resources/init_map/retry.txt");
		ofs2 << 1;
		ofs2.close();
		GotoGameState(GAME_STATE_INIT);
	}
}

void CGameStateRun::next_map() {
	ifstream in;
	int map_name;
	in.open("Resources/init_map/choose_level.txt");
	in >> map_name;
	in.close();
	if (map_name + 1 <= 40) {
		ofstream ofs("Resources/init_map/choose_level.txt");
		ofs << map_name + 1;
		ofs.close();
		ofstream ofs2("Resources/init_map/retry.txt");
		ofs2 << 1;
		ofs2.close();
		GotoGameState(GAME_STATE_INIT);
	}
	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}



void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	map.idx1 = point.x;
	map.idy1 = point.y;

	if (!map.is_inSquare() || !map.can_switch_then_switch()) {
		map.idx0 = map.idx1;
		map.idy0 = map.idy1;
	}
	else {
		map._win_rule.moves -= 1;
		map._win_rule.score += 40;
	}
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
	background.SetFrameIndexOfBitmap(0);
	background.ShowBitmap();
	map.Show();

	CDC *pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 21, "", RGB(0, 0, 0), 800);
	CTextDraw::Print(pDC, 80, 30, "level: " + to_string(map.level()));
	CTextDraw::Print(pDC, 80, 60, "steps: " + to_string(map._win_rule.moves));

	int k = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < int(map._win_rule.condition_number[i].size()); j++) {
			CTextDraw::Print(pDC, 400, 80, to_string(map._win_rule.condition_number[i][j].second > 0 ? map._win_rule.condition_number[i][j].second : 0));
		}
	}

	CDDraw::ReleaseBackCDC();
}
