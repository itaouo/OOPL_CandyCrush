#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
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
	
	for (int i = 0; i < 100; i++) {
		ShowInitProgress(i, "");
		Sleep(2);
	}
	load_background();
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{
	
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);
}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();
	draw_text();
}

void CGameStateInit::load_background() {
	background.LoadBitmapByString({ "Resources/texture_pack_original/bg_screens/start.bmp" });
	background.SetTopLeft(0, 0);
}

void CGameStateInit::draw_text() {
	CDC *pDC = CDDraw::GetBackCDC();

	/* Print title */
	/*CTextDraw::ChangeFontLog(pDC, 36, "�L�n������", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 79, 228, "Candy Crush");*/

	/* Print info */
	/*CTextDraw::ChangeFontLog(pDC,  24, "�L�n������", RGB(0, 0, 0));
	CTextDraw::Print(pDC, 200, 600, "Press any key to start");*/

	CDDraw::ReleaseBackCDC();
}