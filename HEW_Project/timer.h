// ��ʑJ�� [timer.h]
#pragma once
#include "main.h"

// �֐��v���g�^�C�v
class CTimer
{
public:
	/*Timer();
	~Timer();*/
	virtual HRESULT InitTimer(); // ������
	virtual void UninitTimer(); // �I������
	virtual void UpdateTimer(); // �X�V
	virtual void DrawTimer(); // �`��

private:

};


void AddTimer(int nSec = 30);// �ؑ�_�g���ĂȂ�
int GetTimer();