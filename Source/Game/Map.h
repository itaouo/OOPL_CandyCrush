#pragma once
#include <mmsystem.h>
#include <ddraw.h>
#include "../Core/Resource.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Candy.h"
#include"Ice.h"
#include "WinRule.h"

class Map {
private:
	


public:
	int _level;
	int _width;
	int _height;
	
	vector<vector<Candy>> _candy_map;
	vector<vector<Ice>> _ice_map;
	WinRule _win_rule;
	bool _is_animation_finished;
	bool _is_fall_candy;

	Map();
	int level();
	int width();
	int height();
	int idx0 = 0, idx1 = 0;
	int idy0 = 0, idy1 = 0;

	void Init();
	void BeginState();
	void loadLevel();
	void loadMapWidthAndHeight();
	void loadCandyMap();
	void loadIceMap();
	void updateCandyMap();
	void updateIceMap();
	void updateMap();
	void fallCandyAll();
	void startCandyAnimation(int i, int j, int direction);
	void fallCandy(int i, int j);
	void removeAroundObstacle(int i, int j);
	void removeObstacle(int i, int j);
	void produceCandy(int i, int j);
	void Show();
  
	bool is_LTypeCandy(int i, int j);
	bool is_ITypeCandy(int i, int j);
	bool is_TTypeCandy(int i, int j);
	bool is_ETypeCandy(int i, int j);
	bool is_ChocoCandy(int i, int j);
	bool can_change_candy();
	bool can_switch_then_switch();
	bool is_inSquare();
	void deleteRow(int i, int j);
	void deleteColumn(int i, int j);
	void Switch(int i1, int j1, int i2, int j2);
	void checkMapStatus();
	void ScoreAndMovesCalculate();
};