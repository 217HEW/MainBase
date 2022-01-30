//**************************************************************
//
//	Player.cpp
//	プレイヤー処理
//
//--------------------------------------------------------------
//	製作者：柴山凜太郎
//--------------------------------------------------------------
//**************************************************************

//**************************************************************
//	開発履歴
//	2021/12/22	コメントの編集&追加
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/03	プレイヤーのサイズを取得する関数の作成
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/29	プレイヤーの方向指定をする列挙体を作成
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/29	無敵かを判断する関数を追加しました
//	編集者：上月大地
//**************************************************************
#pragma once

//*****************************************************************************
// インクルード部
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// 列挙体宣言
//*****************************************************************************

// プレイヤーがブロックに当たっている方向
enum PlayerDir
{
	DIR_NONE,	// 壁に触れていない
	DIR_RIGHT,	// 右
	DIR_LEFT,	// 左
	DIR_UP,		// 上
	DIR_DOWN,	// 下

	MAX_DIR		// 最大方向数
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);	// 初期化
void UninitPlayer(void);	// 終了
void UpdatePlayer(void);	// 更新
void DrawPlayer(void);		// 描画

// プレイヤー位置情報取得
XMFLOAT3& GetPlayerPos();
// プレイヤーサイズ情報取得
XMFLOAT3& GetPlayerSize();
// プレイヤー当たり判定用サイズ情報取得
float GetPlayerRadSize();
// 飛んでいるかのboolを取得する	true:飛んでいない
bool GetPlayerJump();
// 飛んでいるかのboolをセットする	
void SetPlayerJump(bool jump);
// プレイヤー操作してるか
bool GetPlayerInv();
// プレイヤーの向いてる方向をセット
void SetPlayerDir(PlayerDir dir);
// プレイヤーを設置するための関数
void SetPlayer(XMFLOAT3 pos);
// プレイヤーとの当たり判定取得
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage);
