//=============================================================================
//
// �ꎞ��~���� [clearpause.h]
// Author : �V���ڐl
//
//=============================================================================
#pragma once

#include "main.h"

// �|�[�Y���j���[
typedef enum {
	C_PAUSE_MENU_NEXTSTAGE = 0,	// ���̃X�e�[�W�I��
	C_PAUSE_MENU_SELECT,			// �Z���N�g��ʂɖ߂�
	C_PAUSE_MENU_QUIT,			// �N�C�b�g
	C_PAUSE_MENU_MAX
} C_PAUSE_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitC_Pause(void);
void UninitC_Pause(void);
void UpdateC_Pause(void);
void DrawC_Pause(void);

void SetC_PauseMenu(void);
C_PAUSE_MENU GetC_PauseMenu(void);
void ResetC_PauseMenu(void);
