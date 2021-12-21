//**************************************************************
//
//	SceneManager.cpp
//	�V�[���̐����S���v���O����
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/11/30	�V�[�����䏈���̎����J�n	/�ĎR꣑��Y
//	2021/12/03	�V�[�����䏈���x�[�X��������/�ĎR꣑��Y
//	2021/12/06	�N���X�ň������Ǝ��݂����f�O�A�ĎR꣑��Y�N���g��ł��ꂽ�v���O������u������/�㌎��n
//	2021/12/19	�t�F�[�h�����������A�Ó]�����]���\/�㌎��n
//
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameover.h"
#include "Fade.h"

//**************************************************************
// �O���[�o���錾
//**************************************************************
static EScene g_eScene = SCENE_TITLE;	// �ŏ��̉�ʂ��^�C�g���ɐݒ�

//**************************************************************
// ����������
//**************************************************************
HRESULT InitScene()
{
	HRESULT hr = S_OK;
	InitFade();		// �t�F�[�h������
	InitTitle();

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void UninitScene()
{
	SetScene(SCENE_NONE);	// ���݂̉�ʂ��I��
	UninitFade();			// �t�F�[�h�I������
}

//**************************************************************
// �X�V����
//**************************************************************
void UpdateScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		// �^�C�g�����
		UpdateTitle();
		break;
	case SCENE_GAME:		// �Q�[�����
		UpdateGame();
		break;
	// �����V�[����ǉ�����ꍇ�͂������
	case SCENE_GAMEOVER:	// �Q�[���I�[�o�[���
		UpdateGameover();
		break;
	}
	UpdateFade();			//�t�F�[�h�X�V
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:		// �^�C�g�����
		DrawTitle();
		break;
	case SCENE_GAME:		// �Q�[�����
		DrawGame();
		break;
	// �ǉ��V�[���̕`��
	case SCENE_GAMEOVER:		// �Q�[���I�[�o�[���
		DrawGameover();
		break;
	}

	// ����(�A���t�@�l)��������Ƃ��̓Z�b�g�u�����h�ł͂���
	SetBlendState(BS_ALPHABLEND);
	DrawFade();				// �t�F�[�h�`��
	SetBlendState(BS_NONE);

}


//*******************************
//
//	�V�[���̐؂�ւ�����
//	
//	����:����ւ������V�[���ԍ�
//
//	�߂�l
//			:����
//
//*******************************
void SetScene(EScene eScene)
{
	// ���݂̉�ʂ��I��
	switch (g_eScene)
	{
	case SCENE_TITLE: // �^�C�g�����
		UninitTitle();
		break;
	case SCENE_GAME: // �Q�[�����
		UninitGame();
		break;
	case SCENE_GAMEOVER: // �Q�[���I�[�o�[���
		UninitGameover();
		break;
	}

	// ��ʂ����ւ�
	g_eScene = eScene;

	// ���̉�ʂ�������
	switch (g_eScene)
	{
	case SCENE_TITLE: // �^�C�g�����
		InitTitle();
		break;
	case SCENE_GAME: // �Q�[�����
		InitGame();
		break;
	case SCENE_GAMEOVER: // �Q�[���I�[�o�[���
		InitGameover();
		break;
	}
}