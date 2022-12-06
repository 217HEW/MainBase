//=============================================================================
//
// 一時停止処理 [clearpause.h]
// Author : 澤村瑠人
//
//=============================================================================
#pragma once

#include "main.h"

// ポーズメニュー
typedef enum {
	C_PAUSE_MENU_NEXTSTAGE = 0,	// 次のステージ選択
	C_PAUSE_MENU_SELECT,			// セレクト画面に戻る
	C_PAUSE_MENU_QUIT,			// クイット
	C_PAUSE_MENU_MAX
} C_PAUSE_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
class CClearPause
{
public:
	/*CClearPause();
	~CClearPause();*/

	virtual HRESULT InitC_Pause(void);
	virtual void UninitC_Pause(void);
	virtual void UpdateC_Pause(void);
	virtual void DrawC_Pause(void);

private:

};



void SetC_PauseMenu(void);
C_PAUSE_MENU GetC_PauseMenu(void);
void ResetC_PauseMenu(void);

bool GetClearPause();
void SetClearPause(bool Setpause);
