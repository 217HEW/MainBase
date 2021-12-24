//**************************************************************
//
//	Block.h
//	ブロック処理
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/21	初期Wall.hから改造して制作
//	2021/12/21	ブロックのサイズを取得する関数の追加	||変更者：柴山凜太郎
//	2021/12/22	構造体とマクロ定数を外で使用するためヘッダに記載　||変更者：上月大地
//--------------------------------------------------------------
//	2021/12/21	ブロックのサイズを取得する関数の追加	
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************
#pragma once

//**************************************************************
// インクルード部
//**************************************************************
 #include "main.h"
#include < string >

//**************************************************************
// 構造体定義
//**************************************************************
struct TBLOCK {
	XMFLOAT3	m_pos;		// 現在の位置
	//XMFLOAT3    m_size;	// 現在のサイズ
	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	std::string	m_3Dmodel;	// モデル情報
	int			m_nLife;	// 壁の耐久値
	bool		m_use;		// 使用しているか
	bool		m_invincible;// 無敵かどうか　ON：無敵
};

//**************************************************************
// マクロ定義
//**************************************************************
#define BLOCK_X			(25)	// ブロック最大数X
#define BLOCK_Y			(25)	// ブロック最大数Y
#define MAX_BLOCK		(BLOCK_X * BLOCK_Y)	// ブロック最大数

//**************************************************************
// プロトタイプ宣言
//**************************************************************
HRESULT InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

int SetBlock(XMFLOAT3 pos);	// ブロック配置
XMFLOAT3 GetBlockSize();	// ブロックサイズ取得
TBLOCK* GetBlockArray();	// ブロック配列取得
