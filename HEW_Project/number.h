//”wŒi•`‰æ[Number.h]
#pragma once
#include "main.h"

#define NUMBER_SIZE_X 32
#define NUMBER_SIZE_Y 32

//ŠÖ”‚Ìƒvƒƒgƒ^ƒCƒv
class CNumber
{
public:
	/*CNumber();
	~CNumber();*/
	virtual HRESULT InitNumber();	//‰Šú‰»
	virtual void UninitNumber();	//I—¹ˆ—
	virtual void DrawNumber(XMFLOAT2 vPos, unsigned, int nWidth, float fSizeX, float fSizeY);		//•`‰æ

	static CNumber* Get() { return m_pNumber; };
private:
	static CNumber* m_pNumber;
};
