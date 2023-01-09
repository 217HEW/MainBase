//=============================================================================
//
// ステージセレクト処理 [StageSelect.h]
//
//=============================================================================
#pragma once

#include "main.h"

// ステージメニュー
typedef enum {
	//SELECT_TITLE = 0,
	S_STAGE_1 = 0,
	S_STAGE_2,
	S_STAGE_3,
	S_STAGE_4,
	S_STAGE_5,
	S_STAGE_6,
	S_STAGE_7,
	S_STAGE_8,
	S_STAGE_9,
	S_STAGE_10,
	S_SELECT_TITLE,
	S_SELECT,

	S_STAGE_MENU_MAX
} SELECT_MENU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************z
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

void SetSelectMenu(void);
SELECT_MENU GetSelectMenu(void);
void ResetSelectMenu(void);
