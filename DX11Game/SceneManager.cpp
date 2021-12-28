//**************************************************************
//
//	SceneManager.cpp
//	�V�[���̐����S���v���O����
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	�J������
//	2021/11/30	�V�[�����䏈���̎����J�n
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/03	�V�[�����䏈���x�[�X��������
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/06	�N���X�ň������Ǝ��݂����f�O�A�ĎR꣑��Y�N���g��ł��ꂽ�v���O������u������
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/19	Draw�֐���SetBlendState�֐��̒ǉ�
//				�t�F�[�h���������A�Ó]�����]���\
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2021/12/21	SetScene�֐��̌^���uint��HRESULT�v�ɕύX
//				�V�[���̏��������������ɂ������^�C�g��������������SetScene�֐��֕ύX
//				SetBlendState�֐����uSceneManager.cpp����Fade.cpp�ֈړ��v
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//	2021/12/28	�J�ڂ���V�[���ɐV�����{�X�G���A�̒ǉ�
//				�G���A�ncpp��4�又������Game.cpp��4�又���ɕύX
//	�ҏW�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneArea2.h"
#include "SceneArea3.h"
#include "SceneGameover.h"
#include "SceneGameclear.h"
#include "Fade.h"
#include "bg.h"
//**************************************************************
// �O���[�o���錾
//**************************************************************
static EScene g_eScene;	// �V�[���ݒ�p�ϐ�

//**************************************************************
// ����������
//**************************************************************
HRESULT InitScene()
{
	HRESULT hr = S_OK;
	// �t�F�[�h������
	InitFade();
	g_eScene = SCENE_TITLE;		// �ŏ��̉�ʂ��^�C�g���ɐݒ�
	hr = SetScene(SCENE_TITLE);	//�ŏ��̓^�C�g����ʂ�

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
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_AREA2:		// �Q�[�����
		UpdateGame();
		break;
	case SCENE_AREA3:		// �Q�[�����
		UpdateGame();
		break;
	case SCENE_AREA_BOSS:		// �Q�[�����
		UpdateGame();
		break;
	// �����V�[����ǉ�����ꍇ�͂������
	case SCENE_GAMEOVER:
		UpdateGameover();
		break;
	case SCENE_GAMECLEAR:	// �Q�[���N���A���
		UpdateGameclear();
		break;
	}

	//�t�F�[�h�X�V
	UpdateFade();
}

//**************************************************************
// �`�揈��
//**************************************************************
void DrawScene()
{
	switch (g_eScene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_AREA2:		// �Q�[�����
		DrawGame();
		break;
	case SCENE_AREA3:		// �Q�[�����
		DrawGame();
		break;
	case SCENE_AREA_BOSS:		// �Q�[�����
		DrawGame();
		break;
	// �ǉ��V�[���̕`��
	case SCENE_GAMEOVER:
		DrawGameover();
		break;
	case SCENE_GAMECLEAR:		// �Q�[���N���A���
		DrawGameclear();
		break;
	}

	// �t�F�[�h�`��
	DrawFade();

}

//*******************************
//
//	�V�[���̐؂�ւ�����
//	
//	����:����ւ������V�[���ԍ�
//
//	�߂�l
//			:�G���[���ǂ���
//
//*******************************
HRESULT SetScene(EScene eScene)
{
	HRESULT hr = S_OK;
	// ���݂̉�ʂ��I��
	switch (g_eScene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_AREA2: // �Q�[�����
		UninitGame();
		break;
	case SCENE_AREA3: // �Q�[�����
		UninitGame();
		break;
	case SCENE_AREA_BOSS: // �Q�[�����
		UninitGame();
		break;
	case SCENE_GAMEOVER: // �Q�[���I�[�o�[���
		UninitGameover();
		break;
	case SCENE_GAMECLEAR: // �Q�[���N���A���
		UninitGameclear();
		break;
	}

	// ��ʂ����ւ�
	g_eScene = eScene;

	// ���̉�ʂ�������
	switch (g_eScene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitGame(AREA_1);
		break;
	case SCENE_AREA2: // �Q�[����ʃG���A2
		InitGame(AREA_2);
		break;
	case SCENE_AREA3: // �Q�[����ʃG���A3
		InitGame(AREA_3);
		break;
	case SCENE_AREA_BOSS: // �Q�[����ʃG���A3
		InitGame(AREA_BOSS);
		break;
	case SCENE_GAMEOVER: // �Q�[���I�[�o�[���
		InitGameover();
		break;
	case SCENE_GAMECLEAR: // �Q�[���N���A���
		InitGameclear();
		break;
	}
	return hr;
}
