// BGM/SE定義 (SoundData.h)

enum eBGM {
	BGM_000 = 0,	// BGM0
	BGM_001,
	BGM_002,

	//ここから追加音源

	BGM_003,	//タイトル
	BGM_004,	//メニュー
	BGM_005,	//バトル
	BGM_006,	//風
	BGM_007,	//夏の夕暮れ
	BGM_008,	//岸辺
	BGM_009,	//エンディング


	MAX_BGM	
};

enum eSE {
	SE_DECIDE = 0,	// 決定音
	SE_CANCEL,		// キャンセル音
	SE_SELECT,		// 選択音
	SE_SHOT,		// 弾発射音

	SE_004,			//1S&S
	SE_005,			//2S6S
	SE_006,			//1Slash
	SE_007,			//2Slash
	SE_008,			//voice000
	SE_009,			//ししおどし


	MAX_SE
};
