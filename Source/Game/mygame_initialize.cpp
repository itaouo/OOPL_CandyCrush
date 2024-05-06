#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
bool isAccountSettingOpen = 0;
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
	
}

void CGameStateInit::OnInit()
{	
	
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	
	load_background();
	for (int i = 0; i < 25; i++) {
		ShowInitProgress(i*4, "");
		Sleep(1);
	}
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{
	
}

void CGameStateInit::OnMove()
{	
	if (isAccountSettingOpen == 1 && show_account.GetTop() < 0){
		show_account.SetTopLeft(0, show_account.GetTop() + 40);
	}
	else if (isAccountSettingOpen == 0 && show_account.GetTop() > -800) {
		show_account.SetTopLeft(0, show_account.GetTop() - 40);
	}
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{	
	int mouse_x = point.x;
	int mouse_y = point.y;

	if (mouse_x > 260 && mouse_y > 400 && mouse_x < 510 && mouse_y < 460 && isAccountSettingOpen == 0) {
		GotoGameState(GAME_STATE_RUN);
	}
	else if (mouse_x>200 && mouse_y>480 && mouse_x < 540 && mouse_y < 560 && isAccountSettingOpen == 0) {
		isAccountSettingOpen = 1;
	}
	else if(mouse_x > 625 && mouse_y > 0 && mouse_x < 720 && mouse_y < 90 && isAccountSettingOpen == 1){
		isAccountSettingOpen = 0;
	}
}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();
	show_account.ShowBitmap();
	draw_text();
}

void CGameStateInit::load_background() {

	background.LoadBitmapByString({ "Resources/texture_pack_original/bg_screens/start.bmp" });
	background.SetTopLeft(0, 0);
	show_account.LoadBitmapByString({ "Resources/texture_pack_original/bg_screens/account_setting.bmp" }, RGB(255,255,255));
	show_account.SetTopLeft(0, -800);
}

void CGameStateInit::draw_text() {

}