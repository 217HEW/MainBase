// �w�i�`�� [bg.h]
#pragma once
#include "main.h"

// �֐��v���g�^�C�v
class CBG
{
public:
	/*CBG();
	~CBG();*/

	virtual HRESULT InitBG();	// ������
	virtual void UninitBG();	// �I������
	virtual void UpdateBG();	// �X�V
	virtual void DrawBG();		// �`��

private:

};

