//**************************************************************
//
//	SceneManager.h
//	�V�[���}�l�[�W���[�w�b�_
//
//--------------------------------------------------------------
//	����ҁF�㌎��n
//--------------------------------------------------------------
//	�J������
//	2021/12/06	�ĎR꣑��Y�N�̃v���O���������ɍ쐬	
//
//**************************************************************
#pragma once

//---�C���N���[�h��---
#include "main.h"

// �萔��`
enum EScene
{
	SCENE_NONE = 0,	 // ���ݒ�
	SCENE_TITLE,	 // �^�C�g�����
	SCENE_GAME,		 // �Q�[�����
	SCENE_AREA2,	// �Q�[����ʃG���A2
	SCENE_AREA3,	// �Q�[����ʃG���A3
	
// ���V�[���ǉ���
	SCENE_GAMEOVER,		 // �Q�[���I�[�o�[���
	SCENE_GAMECLEAR,	//�Q�[���N���A���
	MAX_SCENE // �V�[������
};

// �֐��v���g�^�C�v
HRESULT InitScene();	// ������
void UninitScene();		// �I������
void UpdateScene();		// �X�V
void DrawScene();		// �`��

HRESULT SetScene(EScene eScene); // �ؑ֏���
HRESULT GetScene(); // �V�[�����擾


