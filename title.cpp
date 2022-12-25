/*==========================================================================================================================================================

タイトル処理[title.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "color.h"

//マクロ定義
#define TITLE_LOGO		(1)			//タイトル画面で使うテクスチャの数

#define TITLE_BG_TEX	"data/TEXTURE/title.png"

//タイトル名構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	float fAngle;			//中心から頂点までの角度
	float flength;			//中心から頂点までの長さ
	float fLengthMinus;		//長さを徐々に元に戻すための変数
}TitleName;

//タイトル係数
typedef enum
{
	TITLE_DISPLAY = ZERO,
	TITLE_NAME,
	TITLE_ENTER,
	TITLE_MAX
}TITLE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureTitle[TITLE_LOGO] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				//頂点バッファへのポインタ
TitleName				g_Title;							//タイトルの情報

//------------------------------------------------
//				タイトル初期化処理
//------------------------------------------------
void InitTitle(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_BG_TEX, &g_pTextureTitle[TITLE_DISPLAY]);	//タイトル画面背景

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TITLE_LOGO, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	g_Title.pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);			//位置初期化
	g_Title.move = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);			//移動量を初期化する
	g_Title.rot = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);			//向きを初期化する(Z値を使用)

	g_Title.flength = sqrtf(0.0f * 0.0f + 0.0f * 0.0f) * 0.0f;	//長さを算出

	g_Title.fAngle = atan2f(0.0f, 0.0f);						//角度を算出

	//それぞれのUIを初期化
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++, pVtx += VTX_MAX)
	{
		switch (nCnt)
		{
		case TITLE_DISPLAY:		//タイトル画面
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
		break;
		}
	}
									//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();
}

//------------------------------------------------
//				タイトル終了処理
//------------------------------------------------
void UninitTitle(void)
{
	//テクスチャ(三枚)の破棄
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//------------------------------------------------
//				タイトル更新処理
//------------------------------------------------
void UpdateTitle(void)
{
	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点バッファをロックする
	g_pVtxBuffTitle->Unlock();

	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//モード設定（チュートリアル画面に遷移)
		SetFade(MODE_TUTORIAL);
	}
}

//------------------------------------------------
//				タイトル描画処理
//------------------------------------------------
void DrawTitle(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		pDevice->SetTexture(ZERO, g_pTextureTitle[nCnt]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * VTX_MAX, SEC);
	}
}