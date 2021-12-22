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
//	2021/12/19	Draw�֐���SetBlendState�֐��̒ǉ�
//				�t�F�[�h���������A�Ó]�����]���\/�㌎��n
//--------------------------------------------------------------
//	2021/12/21	SetScene�֐��̌^���uint��HRESULT�v�ɕύX
//				�V�[���̏��������������ɂ������^�C�g��������������SetScene�֐��֕ύX
//				SetBlendState�֐����uSceneManager.cpp����Fade.cpp�ֈړ��v
//	�ύX�ҁF�ĎR꣑��Y
//--------------------------------------------------------------
//
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
	InitFade();		// �t�F�[�h������
	g_eScene = SCENE_TITLE;	// �ŏ��̉�ʂ��^�C�g���ɐݒ�
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
	case SCENE_TITLE:		// �^�C�g�����
		UpdateTitle();
		break;
	case SCENE_GAME:		// �Q�[�����
		UpdateGame();
		break;
	case SCENE_AREA2:		// �Q�[�����
		UpdateArea2();
		break;
	case SCENE_AREA3:		// �Q�[�����
		UpdateArea3();
		break;
	// �����V�[����ǉ�����ꍇ�͂������
	case SCENE_GAMEOVER:	// �Q�[���I�[�o�[���
		UpdateGameover();
		break;
	case SCENE_GAMECLEAR:	// �Q�[���N���A���
		UpdateGameclear();
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
	case SCENE_AREA2:		// �Q�[�����
		DrawArea2();
		break;
	case SCENE_AREA3:		// �Q�[�����
		DrawArea3();
		break;
	// �ǉ��V�[���̕`��
	case SCENE_GAMEOVER:		// �Q�[���I�[�o�[���
		DrawGameover();
		break;
	case SCENE_GAMECLEAR:		// �Q�[���N���A���
		DrawGameclear();
		break;
	}

	DrawFade();				// �t�F�[�h�`��

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
	case SCENE_TITLE: // �^�C�g�����
		UninitTitle();
		break;
	case SCENE_GAME: // �Q�[�����
		UninitGame();
		break;
	case SCENE_AREA2: // �Q�[�����
		UninitArea2();
		break;
	case SCENE_AREA3: // �Q�[�����
		UninitArea3();
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
	case SCENE_TITLE: // �^�C�g�����
		InitTitle();
		break;
	case SCENE_GAME: // �Q�[�����
		InitGame();
		break;
	case SCENE_AREA2: // �Q�[����ʃG���A2
		InitArea2();
		break;
	case SCENE_AREA3: // �Q�[����ʃG���A3
		InitArea3();
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
