/*==========================================================================================================================================================

ランキング処理[ranking.cpp]
Author:平澤詩苑

============================================================================================================================================================*/
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "color.h"

//マクロ定義
#define RANK_DATA		"data/RANKING.txt"			//ランキングファイル
#define RANK_TEXTURE	(1)							//ランキングに使用する画像の数
#define RANK_NUM		(5)							//TOP5まで表示

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureRanking[RANK_TEXTURE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				//頂点バッファへのポインタ
D3DXVECTOR3				g_posRank[RANK_NUM];					//各順位ごとの座標
int						g_nRanking[RANK_NUM];					//ランクの値

//------------------------------------------------
//				ランキング初期化処理
//------------------------------------------------
void InitRanking(void)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &g_pTextureRanking[0]);	//背景画像

	//スコアの情報の初期化およびランキングファイルの読み込み
	//ScanRanking();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//背景画像の設定
	{
		//頂点座標の設定
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, F_ZERO);
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
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	////ランキングに使用する各順位の各桁の情報初期化
	//for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
	//{
	//	aTexU[0] = g_nRanking[nCntRanking] / 10000000;
	//	aTexU[1] = g_nRanking[nCntRanking] % 10000000 / 1000000;
	//	aTexU[2] = g_nRanking[nCntRanking] % 1000000 / 100000;
	//	aTexU[3] = g_nRanking[nCntRanking] % 100000 / 10000;
	//	aTexU[4] = g_nRanking[nCntRanking] % 10000 / 1000;
	//	aTexU[5] = g_nRanking[nCntRanking] % 1000 / 100;
	//	aTexU[6] = g_nRanking[nCntRanking] % 100 / 10;
	//	aTexU[7] = g_nRanking[nCntRanking] % 10;

	//頂点バッファをロックする
	g_pVtxBuffRanking->Unlock();
}

//------------------------------------------------
//				ランキング終了処理
//------------------------------------------------
void UninitRanking(void)
{
	//テクスチャの破棄(2枚)
	for (int nCntRankTex = START_CNT; nCntRankTex < RANK_TEXTURE; nCntRankTex++)
	{
		if (g_pTextureRanking[nCntRankTex] != NULL)
		{
			g_pTextureRanking[nCntRankTex]->Release();
			g_pTextureRanking[nCntRankTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//------------------------------------------------
//				ランキング更新処理
//------------------------------------------------
void UpdateRanking(void)
{
	//キーボードのENTER　か　ゲームパッドの　Aボタン　か　STARTボタンが押された
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//モード設定（ランキング画面に遷移)
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//				ランキング描画処理
//------------------------------------------------
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffRanking, ZERO, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//背景画像の設定
	pDevice->SetTexture(ZERO, g_pTextureRanking[0]);

	//桁数分のポリゴンの描画
	for (nCntScore = START_CNT; nCntScore < 1; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * VTX_MAX, SEC);
	}
}

//ランキングの設定処理
void ScanRanking(void)
{
	FILE *pFile;

	pFile = fopen(RANK_DATA, "r");	//ランキングファイル読み込み

	if (pFile != NULL)
	{
		for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
		{
			fscanf(pFile, "%d", &g_nRanking[nCntRanking]);		//ランキング配列に格納
		}

		fclose(pFile);

		DownSort();		//並び替え処理に移動
	}
}

//並び替え処理
void DownSort(void)
{
	int PlayerRank = ZERO;	//プレイヤーのランキング

	if (PlayerRank > g_nRanking[FOUR])
	{//プレイヤーのスコアが、読み込まれた数値の最小値より大きかったら入れ替え
		g_nRanking[FOUR] = PlayerRank;

		for (int nCntRanking = START_CNT; nCntRanking < FOUR; nCntRanking++)				//比較される数値(基準の値)
		{
			for (int mCntRanking = nCntRanking + ONE; mCntRanking < RANK_NUM; mCntRanking++)	//比較する数値
			{
				//比較した数値が基準の値より大きければバブルソートを行う
				if (g_nRanking[nCntRanking] < g_nRanking[mCntRanking])
				{
					int nTmp = g_nRanking[nCntRanking];					//基準の値を一時保管場所に格納

					g_nRanking[nCntRanking] = g_nRanking[mCntRanking];	//基準の値の場所を上書き

					g_nRanking[mCntRanking] = nTmp;						//比較する数値の場所を上書き
				}
			}
		}

		FILE *pFile;	//ファイルポインタ

		pFile = fopen(RANK_DATA, "w");

		//ランキング書き換え処理
		if (pFile != NULL)
		{
			for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
			{
				fprintf(pFile, "%.8d\n", g_nRanking[nCntRanking]);		//順位ごとのスコアを書き出し
			}

			fclose(pFile);
		}
	}
}