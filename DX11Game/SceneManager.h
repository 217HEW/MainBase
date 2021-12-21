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
	
// ���V�[���ǉ���
	SCENE_GAMEOVER,		 // �Q�[���I�[�o�[���
	SCENE_GAMECLEAR,
	MAX_SCENE // �V�[������
};

// �֐��v���g�^�C�v
HRESULT InitScene();	// ������
void UninitScene();		// �I������
void UpdateScene();		// �X�V
void DrawScene();		// �`��

HRESULT SetScene(EScene eScene); // �ؑ֏���

