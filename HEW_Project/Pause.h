//=============================================================================
//
// �ꎞ��~���� [pause.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#pragma once

#include "main.h"

// �|�[�Y���j���[
typedef enum {
	PAUSE_MENU_CONTINUE = 0,	// �R���e�B�j���[
	PAUSE_MENU_RETRY,			// ���g���C
	PAUSE_MENU_QUIT,			// �N�C�b�g
	PAUSE_MENU_MAX
} PAUSE_MENU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CPause
{
public:
	/*Pause();
	~Pause();*/

	virtual HRESULT InitPause(void);
	virtual void UninitPause(void);
	virtual void UpdatePause(void);
	virtual void DrawPause(void);

private:

};


void SetPauseMenu(void);
PAUSE_MENU GetPauseMenu(void);
void ResetPauseMenu(void);
