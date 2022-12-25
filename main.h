/*==========================================================================================================================================================

ハッカソン-αブレンド-のメイン処理[main.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MAIN_H_						//このマクロ定義を一度もされていなかったら
#define _MAIN_H_

#include <Windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
#include "dInput.h"						//入力処理に必要
#include "Xinput.h"
#include "xaudio2.h"
#include <stdio.h>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//マクロ定義
#define ORIGIN_POS			(0.0f)		//ウインドウのサイズの原点
#define MAX_POS				(1.0f)		//ウインドウのサイズの限界点
#define SCREEN_WIDTH		(1280)		//ウインドウの幅
#define SCREEN_HEIGHT		(720)		//ウインドウの高さ
#define HALF_WIDTH			(640.0f)	//ウインドウ幅半分
#define HALF_HEIGHT			(360.0f)	//ウインドウ高さ半分

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット
#define RHW_NUM				(1.0f)	//rhwに使用

#define COLOR_MAX	(255)	//RGBACOLORの最大数

#define START_CNT	(0)		//for文の初期設定に使用

//整数のマクロ
#define MINUS_ONE	(-1)	
#define ZERO		(0)
#define ONE			(1)
#define SEC			(2)
#define SAN			(3)
#define FOUR		(4)
#define FIVE		(5)

//少数のマクロ
#define F_ZERO		(0.0f)
#define F_ONE		(0.1f)
#define F_SEC		(0.2f)
#define F_SAN		(0.3f)
#define F_FOUR		(0.4f)
#define F_FIVE		(0.5f)
#define F_SIX		(0.6f)
#define F_SEV		(0.7f)
#define F_ATE		(0.8f)
#define F_NINE		(0.9f)
#define F_TEN		(1.0f)

#define NIL_F		(0.0f)		//0.0f座標

//POS.Zに使用
#define POS_Z		(0.0f)

//角度のマクロ
#define UP			(1.0f)
#define DOWN		(0.0f)
#define RIGHT		(0.5f)
#define LEFT		(-0.5f)
#define UP_RIGHT	(0.75f)
#define UP_LEFT		(-0.75f)
#define DOWN_RIGHT	(0.25f)
#define DOWN_LEFT	(-0.25f)

//画面モードの種類
typedef enum
{
	MODE_TITLE = 0,	//タイトル画面
	MODE_TUTORIAL,	//チュートリアル画面
	MODE_GAME,		//ゲーム画面
	MODE_RESULT,	//リザルト画面
	MODE_RANKING,	//ランキング画面
	MODE_MAX
}MODE;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//4つの頂点座標の列挙型
typedef enum
{
	VTX_LE_UP = 0,	//左上
	VTX_RI_UP,		//右上
	VTX_LE_DO,		//左下
	VTX_RI_DO,		//右下
	VTX_MAX
}VTX;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXCOLOR col;			//色
	int nLife;				//寿命
	bool bUse;				//使用しているかどうか
}Bullet;

//ビーム構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXCOLOR col;			//色
	bool bUse;				//使用しているかどうか
}Beam;

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//出現状態（点滅）
	PLAYERSTATE_NORMAL,			//通常状態
	PLAYERSTATE_SPIN,			//スピン状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_WAIT,			//出現待ち状態（表示なし）
	PLAYERSTATE_DEATH,			//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 move;			//移動量
	D3DXVECTOR3 rot;			//向き
	int nLife;					//体力
	PLAYERSTATE state;			//プレイヤーの状態
	int nCounterState;			//状態管理カウント
	float fLength;				//対角線の長さ
	float fAngle;				//対角線の角度
	bool bDisp;					//プレイヤーを表示ON/OFF
}Player;

//敵のダメージ状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		//通常
	ENEMYSTATE_DAMAGE,			//ダメージ状態
	ENEMYSTATE_MAX,
}ENEMYSTATE;

//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 move;						//移動量
	float fLength;							//対角線の長さ
	float fAngle;							//対角線の角度
	int nLife;								//体力
	int nCounterState;						//状態管理カウンター
	bool bUse;								//使用の有無
}Enemy;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif