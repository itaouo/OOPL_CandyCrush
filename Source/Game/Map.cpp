#include <stdafx.h>
#include <mmsystem.h>
#include <ddraw.h>
#include<sstream>
#include<fstream>
#include "../Core/Resource.h"
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "Candy.h"
#include "Map.h"
#include "Ice.h"
#include "Surface.h"

Map::Map() {
	
}

void Map::Init() {
	for (int i = 0; i < 9; i++) {
		vector<Candy> temp_candy_row;
		vector<Ice> temp_ice_row;
		vector<Surface> temp_surface_row;
		for (int j = 0; j < 9; j++) {
			Candy candy;
			Ice ice;
			Surface surface;
			candy.Init();
			ice.Init();
			surface.Init();
			temp_candy_row.push_back(candy);
			temp_ice_row.push_back(ice);
			temp_surface_row.push_back(surface);
		}
		_candy_map.push_back(temp_candy_row);
		_ice_map.push_back(temp_ice_row);
		_surface_map.push_back(temp_surface_row);
	}
	idx0 = 0, idx1 = 0;
	idy0 = 0, idy1 = 0;
	_win_rule.Init();
}

void Map::BeginState(){
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			_candy_map[i][j].BeginState();
		}
	}
	loadLevel();
	loadMapWidthAndHeight();
	loadCandyMap();
	loadIceMap();
	loadSurfaceMap();
	_win_rule.LoadWinCondition();
}

void Map::Show() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			_ice_map[i][j].ice().ShowBitmap();
		}
	}
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			_candy_map[i][j].candy().ShowBitmap();
		}
	}
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			_surface_map[i][j].surface().ShowBitmap();
		}
	}
	_win_rule.Show();
}


int Map::level() {
	return _level;
}

int Map::width(){
	return _width;
}

int Map::height(){
	return _height;
}

void Map::loadLevel() {
	ifstream in;
	in.open("Resources/init_map/choose_level.txt"); //"Resources/init_map/choose_level.txt"
	in >> this->_level;
	in.close();
}

void Map::loadMapWidthAndHeight() {
	ifstream in;
	in.open("Resources/init_map/" + to_string(level()) + ".txt");
	in >> this->_height >> this->_width;
	in.close();
}

void Map::loadCandyMap() {
	ifstream in;
	in.open("Resources/candy_maps/" + to_string(level()) + ".txt");
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			int x = 0;
			in >> x;
			_candy_map[i][j]._type = x;
		}
	}
	in.close();
	updateCandyMap();
}

void Map::loadIceMap() {
	ifstream in;
	in.open("Resources/ice_maps/" + to_string(level()) + ".txt");
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			int x = 0;
			in >> x;
			if (x == -999) {
				break;
			}
			_ice_map[i][j]._layer = x;
		}
	}
	in.close();
	updateIceMap();
}

void Map::loadSurfaceMap() {
	ifstream in;
	in.open("Resources/surface_maps/" + to_string(level()) + ".txt");
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			int x = 0;
			in >> x;
			_surface_map[i][j]._type = x;
		}
	}
	in.close();
	updateSurfaceMap();
}

void Map::updateCandyMap() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			_candy_map[i][j]._position.first = (400 - 25 * width()) + j * 50; // (top(),left())
			_candy_map[i][j]._position.second = (400 - 25 * height()) + i * 50;
			_candy_map[i][j].updateCandy();
		}
	}
}

void Map::updateIceMap() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			_ice_map[i][j]._position.first = (400 - 25 * width()) + j * 50;
			_ice_map[i][j]._position.second = (400 - 25 * height()) + i * 50;
			_ice_map[i][j].updateIce();
		}
	}
}

void Map::updateSurfaceMap() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			_surface_map[i][j]._position.first = (400 - 25 * width()) + j * 50;
			_surface_map[i][j]._position.second = (400 - 25 * height()) + i * 50;
			_surface_map[i][j].updateSurface();
		}
	}
}

void Map::updateMap() {
	updateCandyMap();
	updateIceMap();
	updateSurfaceMap();
}

void Map::removeObstacleLayerAll() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (_candy_map[i][j].fall_status() == 4) { // :(
				_candy_map[i][j]._fall_status = 0;
				_surface_map[i][j].removeJelly();
				removeAroundObstacle(i, j);
			}
			if (_candy_map[i][j].fall_status() == 2) {
				if(_candy_map[i][j].is_obstacle())
					_candy_map[i][j].removeObstacleLayer();
				else if (_candy_map[i][j].can_remove()) {
					_candy_map[i][j].changeToBlank();
				}
				else{
					_candy_map[i][j]._fall_status = 0;
				}
				_surface_map[i][j].removeJelly();
				_surface_map[i][j].removeLock();
			}
			if (_candy_map[i][j].fall_status() == 1) {
				_candy_map[i][j].changeToBlank();
				_surface_map[i][j].removeJelly();
				_surface_map[i][j].removeLock();
				removeAroundObstacle(i, j);
			}
			if (i == height() - 1) {
				_candy_map[i][j].removeDragon();
			}
		}
	}
}

void Map::startCandyAnimation(int i, int j, int direction) {
	if (_candy_map[i][j].next_direction() == 0 && _candy_map[i][j].next_y() == 0) {
		_candy_map[i][j]._is_animating = true;
		_candy_map[i][j]._next_position.first = direction; // x
		_candy_map[i][j]._next_position.second = _candy_map[i][j].candy().GetTop() + 50; // y
	}
	if (i == 0) { //potential bug (fall candy probably not start from i=0)
		_candy_map[i][j]._fall_status = 0;
		produceCandy(i, j);
		return;
	}
	if (_candy_map[i - 1][j].can_dropped() && !_surface_map[i - 1][j].is_surface()) {
		_candy_map[i][j]._type = _candy_map[i - 1][j].type();
		_candy_map[i][j]._fall_status = _candy_map[i - 1][j].fall_status();
		startCandyAnimation(i - 1, j, 0);
		return;
	}
	if (j > 0) {
		if (_candy_map[i - 1][j - 1].can_dropped() && !_surface_map[i - 1][j - 1].is_surface()) {
			_candy_map[i][j]._type = _candy_map[i - 1][j - 1].type();
			_candy_map[i][j]._fall_status = _candy_map[i - 1][j - 1].fall_status();
			startCandyAnimation(i - 1, j - 1, -1);
			return;
		}
	}
	if (j < width() - 1) {
		if (_candy_map[i - 1][j + 1].can_dropped() && !_surface_map[i - 1][j + 1].is_surface()) {
			_candy_map[i][j]._type = _candy_map[i - 1][j + 1].type();
			_candy_map[i][j]._fall_status = _candy_map[i - 1][j + 1].fall_status();
			startCandyAnimation(i - 1, j + 1, 1);
			return;
		}
	}
	_candy_map[i][j]._fall_status = 0;
	_candy_map[i][j]._is_animating = false;
	_candy_map[i][j]._next_position.first = 0; // x
	_candy_map[i][j]._next_position.second = 0; // y
	_candy_map[i][j]._type = 99;
}

void Map::animatedCandy() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (_candy_map[i][j].next_direction() != 0 || _candy_map[i][j].next_y() != 0) {
				_candy_map[i][j]._candy.SetTopLeft(_candy_map[i][j].candy().GetLeft() - _candy_map[i][j].next_direction() * 5, _candy_map[i][j].candy().GetTop() + 5);
				if (_candy_map[i][j].candy().GetTop() == _candy_map[i][j].next_y()) {
					_candy_map[i][j]._next_position.first = 0;
					_candy_map[i][j]._next_position.second = 0;
					_candy_map[i][j]._is_animating = false;
				}
			}
		}
	}
}


void Map::produceCandy(int i, int j) {
	int min = 0;
	int max = 3;
	int x = rand() % (max - min + 1) + min;
	_candy_map[i][j]._type = x;
}

void Map::removeAroundObstacle(int i, int j) {
	if (i > 0) {
		_candy_map[i - 1][j].removeObstacleLayer();
		_surface_map[i - 1][j].removeJelly();
	}
	if (j > 0) {
		_candy_map[i][j - 1].removeObstacleLayer();
		_surface_map[i][j - 1].removeJelly();
	}
	if (i < height() - 1) {
		_candy_map[i + 1][j].removeObstacleLayer();
		_surface_map[i + 1][j].removeJelly();
	}
	if (j < width() - 1) {
		_candy_map[i][j + 1].removeObstacleLayer();
		_surface_map[i][j + 1].removeJelly();
	}
}

bool Map::is_LTypeCandy(int i, int j) {
	if (i < 0 || j < 0) {
		return false;
	}
	if (j >= 2 && i < height() - 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 2])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j - 1]._will_be_special_candy = 1;
			_candy_map[i][j - 2]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			_candy_map[i+2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	if (j < width() - 2 && i < height() - 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 2])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i][j+2]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			_candy_map[i+2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	if (j >= 2 && i >= 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 2])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j-2]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i-2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	if (j < width() - 2 && i >= 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 2])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i][j+2]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i-2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	return false;
}

bool Map::is_ITypeCandy(int i, int j) {
	if (i < 0 || j < 0) {
		return false;
	}
	if (i >= 2 && i < height() - 1) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 2][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i-2][j]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			return true;
		}
	}
	if (i >= 1 && i < height() - 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 2][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i+2][j]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			return true;
		}
	}
	return false;
}

bool Map::is_TTypeCandy(int i, int j) {
	if (i < 0 || j < 0) {
		return false;
	}
	if (j < width() - 2 && i >= 1 && i < height() - 1) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 2])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i][j+2]._will_be_special_candy = 1;
			return true;
		}
	}
	if (j >= 2 && i >= 1 && i < height() - 1) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 2])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j-2]._will_be_special_candy = 1;
			return true;
		}
	}
	if (i >= 2 && j >= 1 && j < width() - 1) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i-2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	if (i < height() - 2 && j >= 1 && j < width() - 1) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			_candy_map[i+2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	return false;
}

bool Map::is_ETypeCandy(int i, int j) {
	if (i < 0 || j < 0) {
		return false;
	}
	if (j > 0 && j < width() - 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 2])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i][j+2]._will_be_special_candy = 1;
			return true;
		}

	}
	if (j > 1 && j < width() - 1) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 2])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i][j-2]._will_be_special_candy = 1;
			return true;
		}
	}
	return false;
}

bool Map::is_ChocoCandy(int i, int j) {
	if (i < 0 || j < 0) {
		return false;
	}
	if (i >= 2 && i < height() - 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i - 2][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 1][j])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i + 2][j])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i-1][j]._will_be_special_candy = 1;
			_candy_map[i-2][j]._will_be_special_candy = 1;
			_candy_map[i+1][j]._will_be_special_candy = 1;
			_candy_map[i+2][j]._will_be_special_candy = 1;
			return true;
		}
	}
	if (j >= 2 && j < width() - 2) {
		if (_candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j - 2])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 1])
			&& _candy_map[i][j].is_sameColor_candy_plus(_candy_map[i][j + 2])) {
			_candy_map[i][j]._will_be_special_candy = 1;
			_candy_map[i][j-1]._will_be_special_candy = 1;
			_candy_map[i][j-2]._will_be_special_candy = 1;
			_candy_map[i][j+1]._will_be_special_candy = 1;
			_candy_map[i][j+2]._will_be_special_candy = 1;
			return true;
		}
	}
	return false;
}

void Map::deleteRow(int i, int j) {
	if (i >= height() || i < 0) {
		return;
	}
	for (int jj = j; jj >=0; jj--) {
		if (_candy_map[i][jj].type() == -1) {
			_candy_map[i][jj]._fall_status = 2;
			break;
		}
		_candy_map[i][jj]._fall_status = 2;
	}
	for (int jj = j; jj < width(); jj++) {
		if (_candy_map[i][jj].type() == -1) {
			_candy_map[i][jj]._fall_status = 2;
			break;
		}
		_candy_map[i][jj]._fall_status = 2;
	}
}

void Map::deleteColumn(int i, int j) {
	if (j >= width() || j < 0) {
		return;
	}
	for (int ii = i; ii >= 0; ii--) {
		if (_candy_map[ii][j].type() == -1) {
			_candy_map[ii][j]._fall_status = 2;
			break;
		}
		_candy_map[ii][j]._fall_status = 2;
	}
	for (int ii = i; ii < height(); ii++) {
		if (_candy_map[ii][j].type() == -1) {
			_candy_map[ii][j]._fall_status = 2;
			break;
		}
		_candy_map[ii][j]._fall_status = 2;
	}
}

void Map::Switch(int i1, int j1, int i2, int j2) {
	Candy xx = _candy_map[i1][j1];
	_candy_map[i1][j1] = _candy_map[i2][j2];
	_candy_map[i2][j2] = xx;
}

bool Map::can_switch_then_switch() {
	int row0 = (idy0 - (400 - 25 * height())) / 50;
	int column0 = (idx0 - (400 - 25 * width())) / 50;
	int row1 = (idy1 - (400 - 25 * height())) / 50;
	int column1 = (idx1 - (400 - 25 * width())) / 50;
	if (_surface_map[row0][column0].is_surface() || _surface_map[row1][column1].is_surface()) {
		return false;
	}
	if (!(std::abs(row0 - row1) == 1 && std::abs(column0 - column1) == 0) && !(std::abs(row0 - row1) == 0 && std::abs(column0 - column1) == 1)) {
		return false;
	}
	if (_candy_map[row0][column0].is_frosting() || _candy_map[row1][column1].is_frosting()) {
		return false;
	}
	Switch(row0, column0, row1, column1);
	if (_candy_map[row0][column0].type() >= 10 && _candy_map[row1][column1].type() >= 10) {
		return true;
	}
	if (_candy_map[row0][column0].type() == 7 || _candy_map[row1][column1].type() == 7) {
		updateCandyMap();
		return true;
	}
	if (can_change_candy()) {
	 	updateCandyMap();
	 	return true;
	 }
	Switch(row0, column0, row1, column1);
	return false;
}

bool Map::is_inSquare() {
	int potx = (400 - 25 * width());
	int poty = (400 - 25 * height());
	if (idx0 < potx || idx1 < potx) {
		return 0;
	}
	if (idx0 > potx + 50 * width() || idx1 > potx + 50 * width()) {
		return 0;
	}
	if (idy0 < poty || idy1 < poty) {
		return 0;
	}
	if (idy0 > poty + 50 * height() || idy1 > poty + 50 * height()) {
		return 0;
	}
	return 1;
}

bool Map::still_fall() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (_candy_map[i][j].fall_status()) {
				return true;
			}
		}
	}
	return false;
}

void Map::checkMapStatus() { // 1:normal, 0.2:special
	TRACE("checkMapStatus\n\n");
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (_candy_map[i][j].is_obstacle()) {

			}
			else {
				/*if (_candy_map[i][j].type() / 10 == 6) {
					disapear = 1;
					boom_que.push_back({ 1, {i,j} });
				}*/

				if (i >= 2) {
					if (_candy_map[i][j].is_sameColor_candy(_candy_map[i - 1][j]) && _candy_map[i][j].is_sameColor_candy(_candy_map[i - 2][j])) {
						_candy_map[i][j]._fall_status = 1;
						_candy_map[i - 1][j]._fall_status = 1;
						_candy_map[i - 2][j]._fall_status = 1;
						for (int k = i - 2; k <= i; k++) {
							if (_candy_map[k][j].type() / 10 == 2) {
								deleteRow(k, j);
								_win_rule.score += 500;
							}
							if (_candy_map[k][j].type() / 10 == 3) {
								deleteColumn(k, j);
								_win_rule.score += 500;
							}
							/*if (_candy_map[k][j] / 10 == 1) {
								disapear = 1;
								boom_que.push_back({ 2, {k,j} });
							}*/

						}
					}
				}
				if (j >= 2) {
					if (_candy_map[i][j].is_sameColor_candy(_candy_map[i][j - 1]) && _candy_map[i][j].is_sameColor_candy(_candy_map[i][j - 2])) {
						_candy_map[i][j]._fall_status = 1;
						_candy_map[i][j - 1]._fall_status = 1;
						_candy_map[i][j - 2]._fall_status = 1;
						for (int k = j - 2; k <= j; k++) {
							if (_candy_map[i][k].type() / 10 == 2) {
								deleteRow(i, k);
							}
							if (_candy_map[i][k].type() / 10 == 3) {
								deleteColumn(i, k);
							}
							/*if (_candy_map[i][k] / 10 == 1) {
								disapear = 1;
								boom_que.push_back({ 2, {i,k} });
							}*/

						}

					}
				}
			}
		}
	}
	/*TRACE("wryyyyy %d\n", boom_que.size());*/
	//for (auto i : boom_que) {
	//	status = boom(status, i.second.first, i.second.second, i.first);
	//	score += 1000;
	//	if (i.first == 2) {
	//		status[i.second.first][i.second.second] = 1;
	//		_candy_map[i.second.first][i.second.second] %= 10;
	//		_candy_map[i.second.first][i.second.second] += 60;
	//		update_candy();
	//		disapear = 1;
	//	}
	//}
	//boom_que.clear();
	int i0 = (idy0 - (400 - 25 * height())) / 50;
	int j0 = (idx0 - (400 - 25 * width())) / 50;
	int i1 = (idy1 - (400 - 25 * height())) / 50;
	int j1 = (idx1 - (400 - 25 * width())) / 50;
	///*TRACE("\n0: %d %d\n1:%d %d\n", i0, j0, i1, j1);*/
	if (i0 >= 0 && i0 < height()
		&&i1 >= 0 && i1 < height()
		&&j0 >= 0 && j0 < width()
		&&j1 >= 0 && j1 < width()
		&& (i0 != i1 || j0 != j1)) {
		if ((_candy_map[i0][j0].type() / 10 == 2 || _candy_map[i0][j0].type() / 10 == 3) &&
			(_candy_map[i1][j1].type() / 10 == 2 || _candy_map[i1][j1].type() / 10 == 3)) {
			//disapear = 1;
			deleteRow(i1, j1);
			deleteColumn(i1, j1);
		}
		else if (((_candy_map[i0][j0].type() / 10 == 2 || _candy_map[i0][j0].type() / 10 == 3) && _candy_map[i1][j1].type() / 10 == 1) ||
			((_candy_map[i1][j1].type() / 10 == 2 || _candy_map[i1][j1].type() / 10 == 3) && _candy_map[i0][j0].type() / 10 == 1)) {
			//disapear = 1;
			deleteRow(i1, j1);
			deleteColumn(i1, j1);
			deleteRow(i1 - 1, j1);
			deleteColumn(i1, j1 - 1);
			deleteRow(i1 + 1, j1);
			deleteColumn(i1, j1 + 1);
		}
		else if (_candy_map[i0][j0].type() == 7) {
			//disapear = 1;
			_win_rule.score += 5000;
			if (_candy_map[i1][j1].type() < 7 && _candy_map[i1][j1].type() >= 0) {
				_candy_map[i0][j0].changeToBlank();
				int x = _candy_map[i1][j1].type();
				for (int i = 0; i < height(); i++) {
					for (int j = 0; j < width(); j++) {
						if (_candy_map[i][j].type() % 10 == x) {
							_candy_map[i][j].changeToBlank();
						}
					}
				}
			}
			else if (_candy_map[i1][j1].is_special_candy()) {
				_candy_map[i0][j0].changeToBlank();
				int x = _candy_map[i1][j1].type();
				for (int i = 0; i < height(); i++) {
					for (int j = 0; j < width(); j++) {
						if (_candy_map[i][j].type() % 10 == x % 10) {
							_candy_map[i][j].changeToBlank();
							_candy_map[i][j]._type = x;
						}
					}
				}
			}
		}
		else if (_candy_map[i1][j1].type() == 7) {
			//disapear = 1;
			_win_rule.score += 5000;
			if (_candy_map[i0][j0].type() < 7 && _candy_map[i0][j0].type() >= 0) {
				_candy_map[i1][j1].changeToBlank();
				int x = _candy_map[i0][j0].type();
				for (int i = 0; i < height(); i++) {
					for (int j = 0; j < width(); j++) {
						if (_candy_map[i][j].type() % 10 == x) {
							_candy_map[i][j].changeToBlank();
						}
					}
				}
			}
			else if (_candy_map[i0][j0].is_special_candy()) {
				_candy_map[i1][j1].changeToBlank();
				int x = _candy_map[i0][j0].type();
				for (int i = 0; i < height(); i++) {
					for (int j = 0; j < width(); j++) {
						if (_candy_map[i][j].type() % 10 == x % 10) {
							_candy_map[i][j].changeToBlank();
							_candy_map[i][j]._type = x;
						}
					}
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			int ii = (idy0 - (400 - 25 * height())) / 50;
			int jj = (idx0 - (400 - 25 * width())) / 50;
			if (i == 1) {
				ii = (idy1 - (400 - 25 * height())) / 50;
				jj = (idx1 - (400 - 25 * width())) / 50;
			}

			if (is_ChocoCandy(ii, jj)) {
				_candy_map[ii][jj]._fall_status = 4;
				_candy_map[ii][jj]._type = 7;
				//disapear = 1;
				updateCandyMap();
			}
			if (is_LTypeCandy(ii, jj) || is_TTypeCandy(ii, jj)) {
				_candy_map[ii][jj]._fall_status = 4;
				_candy_map[ii][jj]._type %= 10;
				_candy_map[ii][jj]._type += 10;
				//disapear = 1;
				updateCandyMap();
			}
			if (is_ETypeCandy(ii, jj)) {
				_candy_map[ii][jj]._fall_status = 4;
				_candy_map[ii][jj]._type %= 10;
				_candy_map[ii][jj]._type += 30;
				updateCandyMap();
				//disapear = 1;
			}
			if (is_ITypeCandy(ii, jj)) {
				//TRACE("AAAAAAAAA %d %d: %d", ii, jj, ITypeCandy(mp, ii, jj));
				_candy_map[ii][jj]._fall_status = 4;
				_candy_map[ii][jj]._type %= 10;
				_candy_map[ii][jj]._type += 20;
				//disapear = 1;
				updateCandyMap();
			}

		}
	}

	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (_candy_map[i][j].fall_status() == 1 || _candy_map[i][j].fall_status() == 2) {
				if (_candy_map[i][j].type() / 10 == 2) {
					deleteRow(i, j);
				}
				if (_candy_map[i][j].type() / 10 == 3) {
					deleteColumn(i, j);
				}
			}
		}
	}

	
	//for (int i = 0; i < height(); i++) {
	//	for (int j = 0; j < width(); j++) {
	//		if (is_ChocoCandy(i, j)) {
	//			_candy_map[i][j]._fall_status = 0;
	//			_candy_map[i][j]._type = 7;
	//			//disapear = 1;
	//			updateCandyMap();
	//		}
	//		if (is_LTypeCandy(i, j) || is_TTypeCandy(i, j)) {
	//			_candy_map[i][j]._fall_status = 0;
	//			_candy_map[i][j]._type %= 10;
	//			_candy_map[i][j]._type += 10;
	//			//disapear = 1;
	//			updateCandyMap();
	//		}
	//		if (is_ITypeCandy(i, j)) {
	//			_candy_map[i][j]._fall_status = 0;
	//			_candy_map[i][j]._type %= 10;
	//			_candy_map[i][j]._type += 20;
	//			//disapear = 1;
	//			updateCandyMap();
	//		}
	//		if (is_ETypeCandy(i, j)) {
	//			_candy_map[i][j]._fall_status = 0;
	//			_candy_map[i][j]._type %= 10;
	//			_candy_map[i][j]._type += 30;
	//			//disapear = 1;
	//			updateCandyMap();
	//		}
	//	}
	//}

	if (still_fall()) {
		for (int i = 0; i < height(); i++) {
			for (int j = 0; j < width(); j++) {
				if (_candy_map[i][j].type() == 99) {
					_candy_map[i][j].changeToBlank();
				}
			}
		}
	}
}

bool Map::is_animating() {
	for (int j = 0; j < width(); j++) {
		for (int i = 0; i < height(); i++) {
			if (_candy_map[i][j].is_animating()) {
				return true;
			}
		}
	}
	return false;
}

void Map::fallCandyAll() {
	TRACE("\nfallCandyAll\n");
	for (int j = 0; j < width(); j++) {
		for (int i = 0; i < height(); i++) {
			TRACE("%d", _candy_map[i][j].fall_status());
		}
		TRACE("\n");
	}
	TRACE("\nfallCandyAll\n");
	bool isFallCandy = false;
	for (int j = 0; j < width(); j++) {
		for (int i = 0; i < height(); i++) {
			if (_candy_map[i][j].fall_status() == 3) {
				_candy_map[i][j]._fall_status = 0;
				isFallCandy = true;
				startCandyAnimation(i, j, 0);
			}
			if (isFallCandy) {
				return;
			}
		}
	}
	
}

bool Map::can_change_candy() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (!_candy_map[i][j].is_obstacle()) {
				if (i >= 2) {
					if (_candy_map[i][j].is_sameColor_candy(_candy_map[i - 1][j]) && _candy_map[i][j].is_sameColor_candy(_candy_map[i - 2][j])) {
						return true;
					}
				}
				if (j >= 2) {
					if (_candy_map[i][j].is_sameColor_candy(_candy_map[i][j - 1]) && _candy_map[i][j].is_sameColor_candy(_candy_map[i][j - 2])) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Map::ScoreAndMovesCalculate() {
	for (int i = 0; i < height(); i++) {
		for (int j = 0; j < width(); j++) {
			if (this->_candy_map[i][j].fall_status() == 1) {
				this->_win_rule.score += 40;
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l<int(this->_win_rule.condition_number[k].size()); l++) {
						if (this->_win_rule.condition_number[k][l].first >= 10 && this->_win_rule.condition_number[k][l].first <= 35) {
							if (this->_candy_map[i][j].type() / 10 == this->_win_rule.condition_number[k][l].first / 10) {
								this->_win_rule.condition_number[k][l].second -= 1;
							}
						}
						else {
							if (this->_candy_map[i][j].type() == this->_win_rule.condition_number[k][l].first) {
								this->_win_rule.condition_number[k][l].second -= 1;
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < 245; i++) {
		if (i < 245 * this->_win_rule.score / this->_win_rule.star_score[2]) {
			this->_win_rule.score_image[i].SetTopLeft(20 + i, 100);
		}
	}
}
