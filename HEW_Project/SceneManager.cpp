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
//--------------------------------------------------------------
//	2021/01/22	10�G���A���̃X�C�b�`��p�ӂ��܂����B
//				����������������ƒZ���Ȃ邩���ł�
//	�ҏW�ҁF�㌎��n
//--------------------------------------------------------------
//	2022/01/30	�V�[���p�̈�����int�ɕύX���܂���
//	�ҏW�ҁF�㌎��n
//**************************************************************

//**************************************************************
// �C���N���[�h��
//**************************************************************
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameover.h"
#include "SceneGameclear.h"
#include "StageSelect.h"
#include "CreateStage.h"
#include "Fade.h"
#include "bg.h"


CSceneManager::CSceneManager()
{
	m_fade = nullptr;
	m_nScene = SCENE_TITLE;		// �ŏ��̉�ʂ��^�C�g���ɐݒ�
}
CSceneManager::~CSceneManager()
{
	delete m_fade;
}
//**************************************************************
// ����������
//**************************************************************
HRESULT CSceneManager::Init()
{
	HRESULT hr = S_OK;
	// �t�F�[�h�C���X�^���X
	m_fade = new CFade();
	// �t�F�[�h������
	m_fade->Init();
	
	hr = Set(SCENE_TITLE);	//�ŏ��̓^�C�g����ʂ�

	return hr;
}

//**************************************************************
// �I������
//**************************************************************
void CSceneManager::Uninit()
{
	Set(SCENE_NONE);	// ���݂̉�ʂ��I��
	m_fade->Uninit();			// �t�F�[�h�I������
}

//**************************************************************
// �X�V����
//**************************************************************
void CSceneManager::Update()
{
	switch (m_nScene)
	{
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		m_SGame->Update();
		break;
	case SCENE_STAGE2:	// �Q�[�����
		m_SGame->Update();
		break;
	case SCENE_STAGE3:	// �Q�[�����
		m_SGame->Update();
		break;
	case SCENE_STAGE4: // �Q�[����ʃG���A4
		m_SGame->Update();
		break;
	case SCENE_STAGE5: // �Q�[����ʃG���A5
		m_SGame->Update();
		break;
	case SCENE_STAGE6: // �Q�[����ʃG���A6
		m_SGame->Update();
		break;
	case SCENE_STAGE7: // �Q�[����ʃG���A7
		m_SGame->Update();
		break;
	case SCENE_STAGE8: // �Q�[����ʃG���A8
		m_SGame->Update();
		break;
	case SCENE_STAGE9: // �Q�[����ʃG���A9
		m_SGame->Update();
		break;
	case SCENE_STAGE10: // �Q�[����ʃG���A10
		m_SGame->Update();
		break;
	// case SCENE_STAGE_DEBUG:		// �Q�[�����
	// 	m_SGame->Update();
	// 	break;
	
	// �����V�[����ǉ�����ꍇ�͂������
	case SCENE_GAMEOVER:
		UpdateGameover();
		break;
	case SCENE_GAMECLEAR:	// �Q�[���N���A���
		UpdateGameclear();
		break;
	case SCENE_SELECT:		// �Z���N�g���
		UpdateSelect();
		break;
	}

	//�t�F�[�h�X�V
	m_fade->Update();
}

//**************************************************************
// �`�揈��
//**************************************************************
void CSceneManager::Draw()
{
	switch (m_nScene)
	{
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		m_SGame->Draw();
		break;
	case SCENE_STAGE2:		// �Q�[�����
		m_SGame->Update();
		break;
	case SCENE_STAGE3:		// �Q�[�����
		m_SGame->Update();
		break;
	case SCENE_STAGE4: // �Q�[����ʃG���A4
		m_SGame->Update();
		break;
	case SCENE_STAGE5: // �Q�[����ʃG���A5
		m_SGame->Update();
		break;
	case SCENE_STAGE6: // �Q�[����ʃG���A6
		m_SGame->Update();
		break;
	case SCENE_STAGE7: // �Q�[����ʃG���A7
		m_SGame->Update();
		break;
	case SCENE_STAGE8: // �Q�[����ʃG���A8
		m_SGame->Update();
		break;
	case SCENE_STAGE9: // �Q�[����ʃG���A9
		m_SGame->Update();
		break;
	case SCENE_STAGE10: // �Q�[����ʃG���A10
		m_SGame->Update();
		break;
	// case SCENE_STAGE_DEBUG:		// �Q�[�����
	// 	m_SGame->Update();
	// 	break;
	// �ǉ��V�[���̕`��
	case SCENE_GAMEOVER:
		DrawGameover();
		break;
	case SCENE_GAMECLEAR:		// �Q�[���N���A���
		DrawGameclear();
		break;
	case SCENE_SELECT:		// �Z���N�g���
		DrawSelect();
		break;
	}

	// �t�F�[�h�`��
	m_fade->Draw();

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
HRESULT CSceneManager::Set(int eScene)
{
	HRESULT hr = S_OK;
	// ���݂̉�ʂ��I��
	switch (m_nScene)
	{
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		m_SGame->Uninit();
		break;
	case SCENE_STAGE2: // �Q�[�����
		m_SGame->Uninit();
		break;
	case SCENE_STAGE3: // �Q�[�����
		m_SGame->Uninit();
		break;
	case SCENE_STAGE4: // �Q�[����ʃG���A4
		m_SGame->Uninit();
		break;
	case SCENE_STAGE5: // �Q�[����ʃG���A5
		m_SGame->Uninit();
		break;
	case SCENE_STAGE6: // �Q�[����ʃG���A6
		m_SGame->Uninit();
		break;
	case SCENE_STAGE7: // �Q�[����ʃG���A7
		m_SGame->Uninit();
		break;
	case SCENE_STAGE8: // �Q�[����ʃG���A8
		m_SGame->Uninit();
		break;
	case SCENE_STAGE9: // �Q�[����ʃG���A9
		m_SGame->Uninit();
		break;
	case SCENE_STAGE10: // �Q�[����ʃG���A10
		m_SGame->Uninit();
		break;
	//case SCENE_STAGE_DEBUG: // �Q�[�����
		//m_SGame->Uninit();
		//break;
	case SCENE_GAMEOVER: // �Q�[���I�[�o�[���
		UninitGameover();
		break;
	case SCENE_GAMECLEAR: // �Q�[���N���A���
		UninitGameclear();
		break;
	case SCENE_SELECT:	// �Z���N�g���
		UninitSelect();
		break;
	}

	// ��ʂ����ւ�
	m_nScene = eScene;

	// ���̉�ʂ�������
	switch (m_nScene)
	{
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		m_SGame->Init(STAGE_1);
		break;
	case SCENE_STAGE2: // �Q�[����ʃG���A2
		m_SGame->Init(STAGE_2);
		break;
	case SCENE_STAGE3: // �Q�[����ʃG���A3
		m_SGame->Init(STAGE_3);
		break;
	case SCENE_STAGE4: // �Q�[����ʃG���A4
		m_SGame->Init(STAGE_4);
		break;
	case SCENE_STAGE5: // �Q�[����ʃG���A5
		m_SGame->Init(STAGE_5);
		break;
	case SCENE_STAGE6: // �Q�[����ʃG���A6
		m_SGame->Init(STAGE_6);
		break;
	case SCENE_STAGE7: // �Q�[����ʃG���A7
		m_SGame->Init(STAGE_7);
		break;
	case SCENE_STAGE8: // �Q�[����ʃG���A8
		m_SGame->Init(STAGE_8);
		break;
	case SCENE_STAGE9: // �Q�[����ʃG���A9
		m_SGame->Init(STAGE_9);
		break;
	case SCENE_STAGE10: // �Q�[����ʃG���A10
		m_SGame->Init(STAGE_10);
		break;
	// case SCENE_STAGE_DEBUG: // �Q�[����ʃG���A3
		//m_SGame->Init(STAGE_DEBUG);
		break;
	case SCENE_GAMEOVER: // �Q�[���I�[�o�[���
		InitGameover();
		break;
	case SCENE_GAMECLEAR: // �Q�[���N���A���
		InitGameclear();
		break;
	case SCENE_SELECT:	// �Z���N�g���
		InitSelect();
		break;
	}
	return hr;
}
