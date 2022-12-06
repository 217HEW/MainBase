//=============================================================================
//
// �������� [explosion.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
class CExplosion
{
public:
	/*CExplosion();
	~CExplosion();*/

	virtual HRESULT InitExplosion(void);
	virtual void UninitExplosion(void);
	virtual void UpdateExplosion(void);
	virtual void DrawExplosion(void);

private:

};



int StartExplosion(XMFLOAT3 pos, XMFLOAT2 size);
void SetExplosionColor(int nExplosion, XMFLOAT4 color);
