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
//	2021/12/22	プレイヤーの跳躍操作実装(仮)
//				プレイヤーのZ軸座標固定
//				移動範囲再設定、不要な処理の削除
//				コメントの編集
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/22	GetPlayerSizeを作成
//				他の所で当たり判定が必要になった為
//				GetPlayerJumpを作成
//				ブロックの耐久値を減らす際に飛んでるか知りたい為
//				SetPlayerJumpを作成
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/24	コントローラーの処理を追加
//				スティックの値を取得するにしています。
//				コントローラーが無い場合、反応しません
//				スティックを傾けてボタンを押すと
//				プレイヤーがその方向にジャンプするようにしました。
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/25	スティックを傾けると飛ぶ方向が表示されるようにしました
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/27	サウンドを追加しました。ジャンプ音
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/28	エリアクリア時、次のエリアに遷移するよう変更
//				CreateField.hをインクルード
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2021/12/30	コントローラーが無かったら発生してしまうバグを
//				取り除きました。無い場合はスティックの値がゼロに
//				なります。
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/30	モデルのスケールをデファインにしました。
//	編集者：上月大地
//--------------------------------------------------------------
//	2021/12/30	当たり判定用、プレイヤーの境界球半径を取得する関数の名前を変更
//	編集者：柴山凜太郎
//--------------------------------------------------------------
//	2022/01/07	プレイヤーのサイズを取得するための関数作成
//				そのため、サイズ用の変数「g_sizeModel」をグローバル変数に宣言
//	編集者：柴山凜太郎
//**************************************************************

//**************************************************************
// インクルード部
//**************************************************************
#include "player.h"
#include <xinput.h>		// コントローラー情報取得に必要
#include "AssimpModel.h"
#include "debugproc.h"
#include "shadow.h"
#include "bullet.h"
#include "effect.h"
#include "collision.h"
#include "explosion.h"
#include "life.h"
#include "Fade.h"
#include "CreateField.h"
#include "Sound.h"

#pragma comment (lib, "xinput.lib")	// コントローラー情報取得に必要
//**************************************************************
// マクロ定義
//**************************************************************
#define MODEL_PLAYER	 "data/model/Character02.fbx"

#define	VALUE_MOVE_PLAYER	(0.155f)	// 移動速度
#define	SPEED_MOVE_PLAYER	(50)		// 跳躍速度
#define	RATE_MOVE_PLAYER	(0.025f)	// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(4.5f)		// 回転速度
#define	RATE_ROTATE_PLAYER	(0.1f)		// 回転慣性係数
#define SCALE_PLAYER		XMFLOAT3(2.0f, 1.5f, 1.0f) //	プレイヤーのモデルスケール

#define	PLAYER_RADIUS		(10.0f)		// 境界球半径
#define DAMAGE_TIMER		(120)		// ダメージ後の無敵時間

//**************************************************************
// グローバル変数
//**************************************************************
static CAssimpModel	g_model;		// モデル

static XMFLOAT3		g_posModel;		// 現在の位置
static XMFLOAT3		g_rotModel;		// 現在の向き
static XMFLOAT3		g_rotDestModel;	// 目的の向き
static XMFLOAT3		g_moveModel;	// 移動量
static XMFLOAT3		g_sizeModel;	// 移動量

static XMFLOAT4X4	g_mtxWorld;		// ワールドマトリックス

//static int		g_nShadow;		// 丸影番号
static int			g_nDamage;		// 点滅中
static bool			g_bInv;			// ダメージ時の無敵判定	true:無敵
static bool			g_bLand;		// 地面判定	true:飛んでない

// コントローラー
static DWORD	Joycon;		// コントローラー情報
static DWORD	JoyState;	// 接続確認用
static XMFLOAT2 Stick;		// スティックの傾き用	

//**************************************************************
// 初期化処理
//**************************************************************
HRESULT InitPlayer(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();
	ID3D11DeviceContext* pDeviceContext = GetDeviceContext();

	g_nDamage = 0;

	// 位置・回転・スケールの初期設定
	g_posModel = XMFLOAT3(50.0f, -800.0f, 0.0f);
	g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sizeModel = SCALE_PLAYER;
	g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	 Stick = XMFLOAT2(0.0f, 0.0f);

	// モデルデータの読み込み
	if (!g_model.Load(pDevice, pDeviceContext, MODEL_PLAYER)) {
		MessageBoxA(GetMainWnd(), "自機モデルデータ読み込みエラー", "InitModel", MB_OK);
		return E_FAIL;
	}

	// 丸影の生成
	//g_nShadow = CreateShadow(g_posModel, 12.0f);

	// 壁接触,無敵判定初期化
	g_bLand = true;
	g_bInv = false;

	return hr;
}

//**************************************************************
// 終了処理
//**************************************************************
void UninitPlayer(void)
{
	// 丸影の解放
	//ReleaseShadow(g_nShadow);

	// モデルの解放
	g_model.Release();
}

//**************************************************************
// 更新処理
//**************************************************************
void UpdatePlayer(void)
{
	//ゲームパッドの状態を取得
	XINPUT_STATE state;

	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	// -------コントローラー操作------------------------------------------
	GetJoyState(Joycon);
	// コントローラーの接続状況確認
	JoyState = XInputGetState(0, &state);

	if (JoyState == ERROR_SUCCESS)
	{	// 接続有り↓
		if (g_bLand)
		{
			// スティック入力時処理
			if (GetJoyX(Joycon) != 0 || GetJoyY(Joycon) != 0)
			{
				// スティックのデッドゾーン処理
				if ((GetJoyX(Joycon) < JOYSTICK_DEADZONE && GetJoyX(Joycon) > -JOYSTICK_DEADZONE) &&
					(GetJoyY(Joycon) < JOYSTICK_DEADZONE && GetJoyY(Joycon) > -JOYSTICK_DEADZONE))
				{	// 傾きが少ない場合↓
					// スティックの値をゼロにする
					Stick.x = 0.0f;
					Stick.y = 0.0f;
				}
				else
				{	// 大きく傾けた場合↓
					// スティックの値－1～1で代入
					Stick.x = static_cast<FLOAT>(GetJoyX(Joycon) / 32767.0);
					Stick.y = -static_cast<FLOAT>(GetJoyY(Joycon) / 32767.0);
					SetEffect(XMFLOAT3((g_posModel.x + (60.0f * (Stick.x /2))), (g_posModel.y + (60.0f * Stick.y)),0.0f),
						XMFLOAT3(0.0f, 0.0f, 0.0f),
						XMFLOAT4(1.0f, 0.05f, 0.05f, 0.80f),
						XMFLOAT2(9.0f, 18.0f), 5);
					for (int i = 0; i < 5; i++)
					{
						SetEffect(XMFLOAT3((g_posModel.x + ((60.0f / 6 * i) * (Stick.x / 2))), (g_posModel.y + ((60.0f / 6 * i) * Stick.y)), 0.0f),
							XMFLOAT3(0.0f, 0.0f, 0.0f),
							XMFLOAT4(1.0f, 0.05f, 0.05f, 0.80f),
							XMFLOAT2(4.5f, 9.0f), 5);
					}
					// Bボタンが押されたら
					if (GetJoyTrigger(Joycon, JOYSTICKID2))
					{
						g_moveModel.x = Stick.x *7.5f;
						g_moveModel.y = Stick.y *15;
						g_bLand = false;
						CSound::Play(SE_JUMP);
						CSound::SetVolume(SE_JUMP, 0.01f);
					}
				}
			}
		}
	}
	else
	{	// 接続無し↓
		Stick.x = 0.0f;
		Stick.y = 0.0f;
	}
	
	/*do
	{*/
	if (g_nDamage > 0) {
		--g_nDamage;
		if (g_nDamage <= 0) {
			g_bInv = false;
			g_nDamage = 0;
		}
		//break;
	}

	// -------プレイヤー操作------------------------------------------
		// 壁等に接触しているとき
	if (g_bLand)
	{
		// 初期化
		g_moveModel.x = 0;
		g_moveModel.y = 0;
		g_moveModel.z = 0;

		// 上下移動
		if (GetKeyTrigger(VK_UP)) {
			StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			g_moveModel.y += SPEED_MOVE_PLAYER;
			CSound::Play(SE_JUMP);
			CSound::SetVolume(SE_JUMP, 0.01f);
			g_bLand = false;
		}
		else if (GetKeyTrigger(VK_DOWN)) {
			StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			g_moveModel.y -= SPEED_MOVE_PLAYER;
			CSound::Play(SE_JUMP);
			CSound::SetVolume(SE_JUMP, 0.01f);
			g_bLand = false;
		}

		//現在横方向の壁に振れていると逆側には飛べない
		// 壁接触時左右移動しない
		if (!g_bLand)
		{
			// 左右移動
			if (GetKeyPress(VK_LEFT)) {
				g_moveModel.x -= SPEED_MOVE_PLAYER;
			}
			else if (GetKeyPress(VK_RIGHT)) {
				g_moveModel.x += SPEED_MOVE_PLAYER;
			}
			// 奥手前移動
			//if (GetKeyPress(VK_UP)) {
			//	g_moveModel.z += SPEED_MOVE_PLAYER;
			//}
			//else if (GetKeyPress(VK_DOWN)) {
			//	g_moveModel.z -= SPEED_MOVE_PLAYER;
			//}
		}
	}
	//if (GetKeyPress(VK_LEFT)) {
	//	if (GetKeyPress(VK_UP)) {
	//		// 左前移動
	//		g_moveModel.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y + 135.0f;
	//	} else if (GetKeyPress(VK_DOWN)) {
	//		// 左後移動
	//		g_moveModel.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y + 45.0f;
	//	} else {
	//		// 左移動
	//		g_moveModel.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y + 90.0f;
	//	}
	//} else if (GetKeyPress(VK_RIGHT)) {
	//	if (GetKeyPress(VK_UP)) {
	//		// 右前移動
	//		g_moveModel.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y - 135.0f;
	//	} else if (GetKeyPress(VK_DOWN)) {
	//		// 右後移動
	//		g_moveModel.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y - 45.0f;
	//	} else {
	//		// 右移動
	//		g_moveModel.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
	//		//g_moveModel.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
	//
	//		g_rotDestModel.y = rotCamera.y - 90.0f;
	//	}
	//} else if (GetKeyPress(VK_UP)) {
	//	// 前移動
	//	g_moveModel.x -= SinDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
	//	//g_moveModel.z -= CosDeg(180.0f + rotCamera.y) * VALUE_MOVE_PLAYER;
	//
	//	g_rotDestModel.y = 180.0f + rotCamera.y;
	//} else if (GetKeyPress(VK_DOWN)) {
	//	// 後移動
	//	g_moveModel.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
	//	//g_moveModel.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
	//
	//	g_rotDestModel.y = rotCamera.y;
	//}

	if (GetKeyPress(VK_I)) {
		// 上に移動
		g_moveModel.y += VALUE_MOVE_PLAYER;
	}
	if (GetKeyPress(VK_K)) {
		// 下に移動
		g_moveModel.y -= VALUE_MOVE_PLAYER;
	}

	if (GetKeyPress(VK_J)) {
		// 左回転
		g_rotDestModel.y -= VALUE_ROTATE_PLAYER;
		if (g_rotDestModel.y < -180.0f) {
			g_rotDestModel.y += 360.0f;
		}
	}
	if (GetKeyPress(VK_L)) {
		// 右回転
		g_rotDestModel.y += VALUE_ROTATE_PLAYER;
		if (g_rotDestModel.y >= 180.0f) {
			g_rotDestModel.y -= 360.0f;
		}
	}

	// -------移動の制限&制御------------------------------------------

		// 目的の角度までの差分
	float fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if (fDiffRotY >= 180.0f) {
		fDiffRotY -= 360.0f;
	}
	if (fDiffRotY < -180.0f) {
		fDiffRotY += 360.0f;
	}

	// 目的の角度まで慣性をかける
	g_rotModel.y += fDiffRotY * RATE_ROTATE_PLAYER;
	if (g_rotModel.y >= 180.0f) {
		g_rotModel.y -= 360.0f;
	}
	if (g_rotModel.y < -180.0f) {
		g_rotModel.y += 360.0f;
	}

	// 位置移動
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	//g_posModel.z += g_moveModel.z;

	// 移動量に慣性をかける
	// g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_PLAYER;
	// g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_PLAYER;
	// //g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_PLAYER;
	// 
	if (g_posModel.x < -630.0f) {
		g_posModel.x = -630.0f;

		StartFadeOut(SCENE_GAMEOVER);
	}
	if (g_posModel.x > 1630.0f) {
		g_posModel.x = 1630.0f;

		StartFadeOut(SCENE_GAMEOVER);
	}
	//  if (g_posModel.z < 0.0f) {
	//  	g_posModel.z = 0.0f;
	//  }
	//  if (g_posModel.z > 0.0f) {
	//  	g_posModel.z = 0.0f;
	//  }
	//  if (g_posModel.y < -199.0f) {
	//  	g_posModel.y = -200.0f;
	//	StartFadeOut(SCENE_GAMEOVER);
	//  }
	if (g_posModel.y > 800.0f) {
		g_posModel.y = 800.0f;
		// 今のエリアから次のエリアへ
		switch (GetAreaState())
		{
		case AREA_1:
			StartFadeOut(SCENE_AREA2);
			break;
		case AREA_2:
			StartFadeOut(SCENE_AREA3);
			break;
		case AREA_3:
			StartFadeOut(SCENE_AREA_BOSS);
			break;
		case AREA_BOSS:
			StartFadeOut(SCENE_GAMECLEAR);
			break;
		default:
			break;
		}
	}
	// 
	if (GetKeyPress(VK_RETURN)) {
		// リセット
		g_posModel = XMFLOAT3(0.0f, -800.0f, 0.0f);
		g_moveModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_rotDestModel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// -------ワールドマトリクス制御------------------------------------------

		// 1.ワールドマトリクス、2.回転移動、3.平行移動
	XMMATRIX mtxWorld, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotModel.x),
										  XMConvertToRadians(g_rotModel.y),
										  XMConvertToRadians(g_rotModel.z));
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// モデルのサイズ
	mtxWorld = XMMatrixScaling(g_sizeModel.x, g_sizeModel.y, g_sizeModel.z);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_posModel.x, g_posModel.y, g_posModel.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックス設定
	XMStoreFloat4x4(&g_mtxWorld, mtxWorld);

	// 丸影の移動
	//MoveShadow(g_nShadow, g_posModel);

// -------エフェクト制御------------------------------------------
	if ((g_moveModel.x * g_moveModel.x
	   + g_moveModel.y * g_moveModel.y
	   + g_moveModel.z * g_moveModel.z) > 1.0f) {
		XMFLOAT3 pos;

		pos.x = g_posModel.x + SinDeg(g_rotModel.y) * 10.0f;
		pos.y = g_posModel.y + 2.0f;
		pos.z = g_posModel.z + CosDeg(g_rotModel.y) * 10.0f;

		// エフェクトの設定
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.85f, 0.05f, 0.65f, 0.50f),
					   XMFLOAT2(14.0f, 14.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.65f, 0.85f, 0.05f, 0.30f),
					   XMFLOAT2(10.0f, 10.0f), 20);
		SetEffect(pos, XMFLOAT3(0.0f, 0.0f, 0.0f),
					   XMFLOAT4(0.45f, 0.45f, 0.05f, 0.15f),
					   XMFLOAT2(5.0f, 5.0f), 20);
	}

	// 弾発射
	//if (GetKeyRepeat(VK_SPACE)) {
	//	FireBullet(g_posModel, XMFLOAT3(-g_mtxWorld._31, -g_mtxWorld._32, -g_mtxWorld._33),
	//		BULLETTYPE_PLAYER);
	//}
// -------テスト操作------------------------------------------
	// ダメージテスト
	if (GetKeyRepeat(VK_D))
	{
		g_bInv = true;
		DelLife();
		g_nDamage = DAMAGE_TIMER;

	}
	// 回復テスト
	if (GetKeyRepeat(VK_H))
	{
		AddLife();
		//g_nDamage = DAMAGE_TIMER;

	}
	//} while (0);
	//当たり判定
	//テスト壁との当たり判定でlifeが減る
	//XMFLOAT2 vRect(COLLISION_WIDTH, COLLISION_HEIGHT);
	//if (CollisionEnemy(g_vPos, vRect, 0.0f) >= 0)
	//{
	//	//lifeを減算
	//	DelLife();
	//	g_nDamage = DAMAGE_TIMER;
	//	CSound::Play(SE_HIT);
	//}

	// PrintDebugProc("[ﾋｺｳｷ ｲﾁ : (%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	// PrintDebugProc("[ﾋｺｳｷ ﾑｷ : (%f) < ﾓｸﾃｷ ｲﾁ:(%f) >]\n", g_rotModel.y, g_rotDestModel.y);
	// PrintDebugProc("\n");

	// PrintDebugProc("*** ﾋｺｳｷ ｿｳｻ ***\n");
	// PrintDebugProc("ﾏｴ   ｲﾄﾞｳ : \x1e\n");//↑
	// PrintDebugProc("ｳｼﾛ  ｲﾄﾞｳ : \x1f\n");//↓
	// PrintDebugProc("ﾋﾀﾞﾘ ｲﾄﾞｳ : \x1d\n");//←
	// PrintDebugProc("ﾐｷﾞ  ｲﾄﾞｳ : \x1c\n");//→
	// PrintDebugProc("ｼﾞｮｳｼｮｳ   : I\n");
	// PrintDebugProc("ｶｺｳ       : K\n");
	// PrintDebugProc("ﾋﾀﾞﾘ ｾﾝｶｲ : J\n");
	// PrintDebugProc("ﾐｷﾞ  ｾﾝｶｲ : L\n");
	// PrintDebugProc("ﾀﾏ   ﾊｯｼｬ : Space\n");
	// PrintDebugProc("\n");
}

//**************************************************************
// 描画処理
//**************************************************************
void DrawPlayer(void)
{
	// モデル点滅処理
	if (g_nDamage > 0) {
		if (g_nDamage & 4)	// 4フレーム毎ごとに描画しない
			return;
	}
	ID3D11DeviceContext* pDC = GetDeviceContext();

	// 不透明部分を描画
	g_model.Draw(pDC, g_mtxWorld, eOpacityOnly);

	// // 半透明部分を描画
	// SetBlendState(BS_ALPHABLEND);	// アルファブレンド有効
	// SetZWrite(false);				// Zバッファ更新しない
	// g_model.Draw(pDC, g_mtxWorld, eTransparentOnly);
	// SetZWrite(true);				// Zバッファ更新する
	// SetBlendState(BS_NONE);			// アルファブレンド無効
}

//*******************************
//
//		位置情報取得
//	
//	戻り値
//		プレイヤーの位置
//
//*******************************
XMFLOAT3& GetPlayerPos()
{
	return g_posModel;
}

//*******************************
//
//		位置情報取得
//	
//	戻り値
//		プレイヤーの位置
//
//*******************************
XMFLOAT3& GetPlayerSize()
{
	return g_sizeModel;
}

//*******************************
//
//		サイズ情報取得
//	
//	戻り値
//		プレイヤーのサイズ(球体)
//
//*******************************
float GetPlayerRadSize()
{
	return PLAYER_RADIUS;
}

//*******************************
//
//		ジャンプ状態情報取得
//	 ジャンプのboolを取得する
//
//	戻り値
//		ジャンプ状態 true:飛んでいない
//
//*******************************
bool GetPlayerJump()
{
	return g_bLand;
}

//*******************************
//
//		ジャンプ状態セット
// ジャンプのboolを外から変更する
//
//	引数：
//		jump:ジャンプ状態のbool
//		true:飛んでいない
//
//	戻り値
//		無し
//
//*******************************
void SetPlayerJump(bool jump)
{
	g_bLand = jump;
}
//*******************************
//
//		プレイヤーとの衝突判定
//	
//	引数:
//		pos:プレイヤー3軸座標、radius:半径、damage:ダメージ数
//
//	戻り値
//		bool:当たったかどうか
//		true:飛んでいない
//
//***********************************************************
bool CollisionPlayer(XMFLOAT3 pos, float radius, float damage)
{
	bool hit = CollisionSphere(g_posModel, PLAYER_RADIUS, pos, radius);
	if (hit) {
		// 爆発開始
		int nExp = -1;
		if (damage > 0.0f) {
			nExp = StartExplosion(g_posModel, XMFLOAT2(40.0f, 40.0f));
			// 
		}
		else {
			nExp = StartExplosion(g_posModel, XMFLOAT2(20.0f, 20.0f));
		}
		// 爆発エフェクトの色を設定
		SetExplosionColor(nExp, XMFLOAT4(1.0f, 0.7f, 0.7f, 1.0f));
	}

	return hit;
}
