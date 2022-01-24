//=============================================================================
//
// �X�e�[�W�Z���N�g���� [StageSelect.h]
//
//=============================================================================
#pragma once

#include "main.h"

// �X�e�[�W���j���[
typedef enum {
	//SELECT_TITLE = 0,
	STAGE_1 = 0,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	STAGE_8,
	STAGE_9,
	STAGE_10,
	SELECT_TITLE,
	SELECT,

	STAGE_MENU_MAX
} SELECT_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

void SetSelectMenu(void);
SELECT_MENU GetSelectMenu(void);
void ResetSelectMenu(void);
