// BGM/SE定義 (SoundData.h)

enum eBGM {
	BGM_TITLE,		// タイトル
	BGM_GAME000,	// ゲーム0番
	BGM_BOSS,		// ボスバトル
	BGM_SELECT,

	MAX_BGM	
};

enum eSE {
	
	SE_DECISION,	// 決定音
	SE_CANCEL,		// キャンセル音
	SE_SELECT,		// セレクト音(ポーズの決定もこれ)
	SE_JUMP,		// ジャンプ音
	SE_LANDING,		// 着地音
	SE_WALLBREAK,	// 壁破壊音
	SE_DAMAGE,		// ダメージ音
	SE_HEAL,		// 回復音
	SE_BEEP,		// 警告音
	SE_ENEMYBREAK,	// 敵破壊音
	SE_LASER,		// レーザー音
	SE_GAMEOVER,	// ジングル(ゲームオーバー)
	SE_GAMECLEAR,   // ジングル(ゲームクリア)
	MAX_SE
};
