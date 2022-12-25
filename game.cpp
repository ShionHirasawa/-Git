/*==========================================================================================================================================================

ゲーム内のメイン処理[game.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "color.h"
//#include "player.h"
//#include "bg.h"
//#include "pause.h"

//マクロ定義
#define GAME_TEXTURE			"data/TEXTURE/game.png"	//カウントダウンのテクスチャ

//ゲーム終了のクリア判定の構造体
typedef enum
{
	RESULT_OVER = ZERO,
	RESULT_CLEAR,
	RESULT_MAX
}RESULT;

typedef enum
{
	COUNT_SAN = ZERO,
	COUNT_SEC,
	COUNT_ONE,
	COUNT_MAX,
}COUNTDOWN;

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureGame = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffGame = NULL;			//頂点バッファへのポインタ
GAMESTATE					g_gameState = GAMESTATE_NONE;	//ゲームの状態
RESULT						g_numResult;					//リザルト分岐
bool						g_bPause;						//ポーズ状態のON/OFF

//------------------------------------------------
//				ゲーム初期化処理
//------------------------------------------------
void InitGame(void)
{
	//ゲーム画面に使われるオブジェクトの初期化処理
	//InitBg();			//背景初期化
	//InitPlayer();		//プレイヤーの初期化
	//InitPause();		//ポーズ画面の初期化処理

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, GAME_TEXTURE, &g_pTextureGame);

	g_gameState = GAMESTATE_START;	//ゲーム状態の初期化
	g_bPause = false;				//ポーズの初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGame, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGame->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点情報の設定
	{
		//頂点座標の設定
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);

		//rhwの設定
		pVtx[VTX_LE_UP].rhw = RHW_NUM;
		pVtx[VTX_RI_UP].rhw = RHW_NUM;
		pVtx[VTX_LE_DO].rhw = RHW_NUM;
		pVtx[VTX_RI_DO].rhw = RHW_NUM;

		//頂点カラーの設定
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//テクスチャの座標
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, F_ZERO);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(F_ZERO, F_TEN);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
	}

	//頂点バッファをロックする
	g_pVtxBuffGame->Unlock();
}

//------------------------------------------------
//				ゲーム終了処理
//------------------------------------------------
void UninitGame(void)
{
	/*======================================================================
	各種オブジェクトの終了処理
	========================================================================*/
	//ゲーム画面に使ったオブジェクトの終了処理
	//UninitBg();			//背景の終了処理
	//UninitPlayer();		//プレイヤーの終了処理
	//UninitPause();		//ポーズ画面の終了処理

	//テクスチャの破棄
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
}

//------------------------------------------------
//				ゲーム更新処理
//------------------------------------------------
void UpdateGame(void)
{
	FADE fadeState = GetFade();

	//ポーズ中ではない
	if (g_bPause == false)
	{
		/*======================================================================
		各種オブジェクトの更新処理
		========================================================================*/
		//UpdateBg();			//背景の更新処理
		//UpdatePlayer();		//プレイヤーの更新処理
	}

	//ポーズ中であり、敵を倒した余韻中ではない
	else
	{

	}

	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//モード設定（リザルト画面に遷移)
		SetFade(MODE_RESULT);
	}

	//ポーズキーのON/OFF
	if (GetKeyboardTrigger(DIK_P) == true)
	{//Pキーが押された & 敵を倒した余韻中ではない
		g_bPause = g_bPause ? false : true;
	}
}

//------------------------------------------------
//				ゲーム描画処理
//------------------------------------------------
void DrawGame(void)
{
	/*======================================================================
	各種オブジェクトの描画処理
	========================================================================*/
	//ゲーム内オブジェクトの描画処理		
	//DrawBg();		//背景の描画処理
	//DrawPlayer();	//プレイヤーの描画処理

	//ゲーム開始のカウントダウン
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffGame, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureGame);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);

	if (g_bPause == true)
	{
		//DrawPause();
	}
}

int ResultChoice(void)
{
	return g_numResult;
}

GAMESTATE GetGameState(void)
{
	return g_gameState;
}