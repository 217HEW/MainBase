//**************************************************************
//
//	EnemyRange.h
//	遠隔敵のヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/12/14	EnemyBaseからコピーして作成
//	編集者：??
//--------------------------------------------------------------
//	2022/01/22	構造体定義の場所を.cppから.hに移動
//	編集者：柴山凜太郎
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
class CEnemyRange
{
public:
	CEnemyRange();
	~CEnemyRange();

	virtual HRESULT InitEnemyRange();	// 初期化
	virtual void UninitEnemyRange();	// 終了
	virtual void UpdateEnemyRange();	// 更新
	virtual void DrawEnemyRange();		// 描画

private:

};

CEnemyRange::CEnemyRange()
{
}

CEnemyRange::~CEnemyRange()
{
}


int SetEnemyRange(XMFLOAT3 pos,int id);	// 遠隔敵設置
TEnemyRange* GetEnemyRange();		// 遠隔敵情報取得