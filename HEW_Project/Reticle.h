//=============================================================================
//
// ���e�B�N������ [Reticle.h]
// Author : �㌎��n
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CReticle
{
public:
	/*CReticle();
	~CReticle();*/

	virtual HRESULT InitReticle(void);
	virtual void UninitReticle(void);
	virtual void UpdateReticle(void);
	virtual void DrawReticle(void);

private:

};




// �Ə��ݒu
int SetReticle(XMFLOAT3 pos, XMFLOAT3 move, XMFLOAT4 col, XMFLOAT2 size, int timer);