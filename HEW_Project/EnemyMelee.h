//**************************************************************
//
//	EnemyMelee.h
//	近接敵のヘッダ
//
//--------------------------------------------------------------
//	製作者：上月大地
//--------------------------------------------------------------
//	開発履歴
//	2021/12/14	EnemyBaseからコピーして作成
//	編集者：??
//--------------------------------------------------------------
//	2022/01/22	構造体の宣言場所を.cppから.hに移動
//	編集者：柴山凜太郎
//**************************************************************
#pragma once

//**************************************************************
// インクルード部
//**************************************************************
#include "main.h"

//**************************************************************
// 構造体定義
//**************************************************************
struct TEnemyMelee {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量
	bool		m_use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	int			m_speed;	// スピード
};
struct TEnemyMelee1 {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量
	bool		m_use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	int			m_speed;	// スピード
};
struct TEnemyMelee2 {
	XMFLOAT3	m_pos;		// 現在の位置
	XMFLOAT3	m_rot;		// 現在の向き
	XMFLOAT3	m_size;		// 現在のサイズ
	XMFLOAT3	m_rotDest;	// 目的の向き
	XMFLOAT3	m_move;		// 移動量
	bool		m_use;		// 使用してるか否か	ON:使用中

	XMFLOAT4X4	m_mtxWorld;	// ワールドマトリックス
	int			m_speed;	// スピード
};
//**************************************************************
// プロトタイプ宣言
//**************************************************************
class CEnemyMelee
{
public:
	virtual HRESULT InitEnemyMelee();	// 初期化
	virtual void UninitEnemyMelee();	// 終了
	virtual void UpdateEnemyMelee();	// 更新
	virtual void DrawEnemyMelee();		// 描画

private:


};



int SetEnemyMelee(XMFLOAT3 pos, int id);	 // 近接敵設置
TEnemyMelee* GetEnemyMelee();		// 近接敵情報取得
// 近接敵との当たり判定処理
//bool CollisionEnemyMelee(XMFLOAT3 pos, float radius);