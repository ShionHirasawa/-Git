/*==========================================================================================================================================================

リザルト処理[result.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "result.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "fade.h"
#include "color.h"

//ゲーム終了の種類構造体
typedef enum
{
	RESULT_TYPE_OVER = ZERO,	//ゲームオーバー
	RESULT_TYPE_CLEAR,			//ゲームクリア
	RESULT_TYPE_MAX
}RESULT_TYPE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureResult[RESULT_TYPE_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//頂点バッファへのポインタ
DWORD g_ResultTime;											//リザルト表示時間

//------------------------------------------------
//				リザルト初期化処理
//------------------------------------------------
void InitResult(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[RESULT_TYPE_OVER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[RESULT_TYPE_CLEAR]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * RESULT_TYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCnt = START_CNT; nCnt < RESULT_TYPE_MAX; nCnt++, pVtx += VTX_MAX)
	{
		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);
		}

		//rhwの設定
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//頂点カラーの設定
		{
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, F_ZERO);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(F_ZERO, F_TEN);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffResult->Unlock();
}

//------------------------------------------------
//				リザルト終了処理
//------------------------------------------------
void UninitResult(void)
{
	for (int nCntResult = START_CNT; nCntResult < RESULT_TYPE_MAX; nCntResult++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//------------------------------------------------
//				リザルト更新処理
//------------------------------------------------
void UpdateResult(void)
{
	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//モード設定（ランキング画面に遷移)
		SetFade(MODE_RANKING);
	}
}

//------------------------------------------------
//				リザルト描画処理
//------------------------------------------------
void DrawResult(void)
{
	int result = ResultChoice();

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffResult, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureResult[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}