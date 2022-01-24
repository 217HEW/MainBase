// BGM/SE定義 (SoundData.hpp)


//**************************************************************
//	開発履歴
//	音ファイルを追加しています。
//	※印は上月大地が独断で採用している音ファイルです
//	編集者：上月大地
//**************************************************************


LPCWSTR g_pszFileBGM[MAX_BGM] = {
	L"BGM_Title000.mp3",	// タイトル		※仮
	L"BGM_Game000.mp3",		// ゲーム		※仮
	L"BGM_Boss000.mp3",		// ボスバトル	※仮
	L"BGM_Select000.mp3",		// ボスバトル	※仮
	// L"BGM_Nostalgic.mp3",	// ノスタルジック
	// L"BGM_minmin.mp3",		// セミの鳴き声

};

LPCWSTR g_pszFileSE[MAX_SE] = {
	L"SE_Decision000.mp3",	// 決定 ※上月追加
	L"SE_Cancel000.mp3",	// キャンセル	※仮
	L"SE_Select000.mp3",	// セレクト移動 ※仮
	L"SE_Jump000.mp3",		// ジャンプ		※仮
	L"SE_Landing000.mp3",	// 着地			※仮
	L"SE_WallBreak000.mp3",	// 壁破壊		※仮
	L"SE_PDamage000.mp3",	// ダメージ
	L"SE_Heal000.mp3",		// 回復音
	L"SE_Beep000.mp3",		// 警告音		※仮
	L"SE_EnemyBreak000.mp3",// 敵破壊		※仮
	L"SE_Laser000.mp3",		// レーザー
	L"SE_GameOver000.mp3",	// ジングル(ゲームオーバー)
	L"SE_GameClear000.mp3",	// ジングル(ゲームクリア)
};
