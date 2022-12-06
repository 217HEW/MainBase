//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CEffect
{
public:
	/*CEffect();
	~CEffect();*/

	virtual HRESULT InitEffect(void);
	virtual void UninitEffect(void);
	virtual void UpdateEffect(void);
	virtual void DrawEffect(void);

private:

};


int SetEffect(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int timer);