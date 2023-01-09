//**************************************************************
//
//	EnemyRange.h
//	遠隔敵のヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//**************************************************************
#pragma once

//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"

//**************************************************************
// 構造体定義
//**************************************************************
struct TEnemyRange {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	bool		m_use;		// 使用してるか否か	ON:使用中
	float ReticleSize;		// レティクルのサイズ
	int			m_Time;		// 敵のタイマー

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
};
struct TEnemyRange1 {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	bool		m_use;		// 使用してるか否か	ON:使用中
	int			m_Time;		// 敵のタイマー

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
};
struct TEnemyRange2 {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	bool		m_use;		// 使用してるか否か	ON:使用中
	int			m_Time;		// 敵のタイマー

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
};
//**************************************************************
// プロトタイプ宣言
//**************************************************************
HRESULT InitEnemyRange(void);	// 初期化
void UninitEnemyRange(void);	// 終了
void UpdateEnemyRange(void);	// 更新
void DrawEnemyRange(void);		// 描画

int SetEnemyRange(XMFLOAT3 pos,int id);	// 遠隔敵設置
TEnemyRange* GetEnemyRange();		// 遠隔敵情報取得