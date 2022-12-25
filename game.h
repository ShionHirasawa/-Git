/*==========================================================================================================================================================

ゲーム内のメイン処理[main.h]
Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "time.h"

//マクロ定義
#define END_FADE	(60)		//終了余韻

//ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,		//何もしていない状態
	GAMESTATE_NORMAL,		//通常状態
	GAMESTATE_END,			//終了状態
	GAMESTATE_START,		//ゲーム開始
	GAMESTATE_MAX
}GAMESTATE;

void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
int ResultChoice(void);
GAMESTATE GetGameState(void);

#endif