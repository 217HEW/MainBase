//�w�i�`��[Number.h]
#pragma once
#include "main.h"

#define NUMBER_SIZE_X 32
#define NUMBER_SIZE_Y 32

//�֐��̃v���g�^�C�v
class CNumber
{
public:
	/*CNumber();
	~CNumber();*/
	virtual HRESULT InitNumber();	//������
	virtual void UninitNumber();	//�I������
	virtual void DrawNumber(XMFLOAT2 vPos, unsigned, int nWidth, float fSizeX, float fSizeY);		//�`��

	static CNumber* Get() { return m_pNumber; };
private:
	static CNumber* m_pNumber;
};
