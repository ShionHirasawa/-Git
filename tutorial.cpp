/*==========================================================================================================================================================

チュートリアル処理[tutorial.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "color.h"

//マクロ定義

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureTutorial = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;		//頂点バッファへのポインタ

//------------------------------------------------
//				チュートリアル初期化処理
//------------------------------------------------
void InitTutorial(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &g_pTextureTutorial);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void* *)&pVtx, 0);

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

	//頂点バッファをロックする
	g_pVtxBuffTutorial->Unlock();
}

//------------------------------------------------
//				チュートリアル終了処理
//------------------------------------------------
void UninitTutorial(void)
{
	//テクスチャの破棄
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//------------------------------------------------
//				チュートリアル更新処理
//------------------------------------------------
void UpdateTutorial(void)
{
	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{//ENTERキーが押された
	 //モード設定（ゲーム画面に遷移)
		SetFade(MODE_GAME);
	}
}

//------------------------------------------------
//				チュートリアル描画処理
//------------------------------------------------
void DrawTutorial(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}