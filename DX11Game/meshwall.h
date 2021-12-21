//=============================================================================
//
// メッシュ壁の処理 [meshwall.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once

#include "main.h"

//**************************************
// 描画部分
//**************************************
enum EDrawPart {
	DRAWPART_ALL = 0,		// 区別なく表示
	DRAWPART_OPAQUE,		// 不透明のみ描画
	DRAWPART_TRANSLUCENT,	// 半透明のみ描画

	DRAWPART_MAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct TBLOCK {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3    m_size;		// 現在のサイズ
	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス

	int			m_nLife;	// 壁の耐久値
	bool		use;		// 使用しているか

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMeshWall(void);
HRESULT SetMeshWall(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 col,
	int nNumBlockX, int nNumBlockY, XMFLOAT2 sizeBlock);
TBLOCK *GetMesh();
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(EDrawPart dp = DRAWPART_ALL);
HRESULT SetMeshBlock(XMFLOAT3 pos);		// 箱生成
