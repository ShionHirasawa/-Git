/*==========================================================================================================================================================

�Q�[�����̃��C������[game.cpp]
Author:���V����

============================================================================================================================================================*/
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "color.h"
//#include "player.h"
//#include "bg.h"
//#include "pause.h"

//�}�N����`
#define GAME_TEXTURE			"data/TEXTURE/game.png"	//�J�E���g�_�E���̃e�N�X�`��

//�Q�[���I���̃N���A����̍\����
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

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureGame = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffGame = NULL;			//���_�o�b�t�@�ւ̃|�C���^
GAMESTATE					g_gameState = GAMESTATE_NONE;	//�Q�[���̏��
RESULT						g_numResult;					//���U���g����
bool						g_bPause;						//�|�[�Y��Ԃ�ON/OFF

//------------------------------------------------
//				�Q�[������������
//------------------------------------------------
void InitGame(void)
{
	//�Q�[����ʂɎg����I�u�W�F�N�g�̏���������
	//InitBg();			//�w�i������
	//InitPlayer();		//�v���C���[�̏�����
	//InitPause();		//�|�[�Y��ʂ̏���������

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, GAME_TEXTURE, &g_pTextureGame);

	g_gameState = GAMESTATE_START;	//�Q�[����Ԃ̏�����
	g_bPause = false;				//�|�[�Y�̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGame, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGame->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���̐ݒ�
	{
		//���_���W�̐ݒ�
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);

		//rhw�̐ݒ�
		pVtx[VTX_LE_UP].rhw = RHW_NUM;
		pVtx[VTX_RI_UP].rhw = RHW_NUM;
		pVtx[VTX_LE_DO].rhw = RHW_NUM;
		pVtx[VTX_RI_DO].rhw = RHW_NUM;

		//���_�J���[�̐ݒ�
		pVtx[VTX_LE_UP].col = RGBA_WHITE;
		pVtx[VTX_RI_UP].col = RGBA_WHITE;
		pVtx[VTX_LE_DO].col = RGBA_WHITE;
		pVtx[VTX_RI_DO].col = RGBA_WHITE;

		//�e�N�X�`���̍��W
		pVtx[VTX_LE_UP].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
		pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, F_ZERO);
		pVtx[VTX_LE_DO].tex = D3DXVECTOR2(F_ZERO, F_TEN);
		pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGame->Unlock();
}

//------------------------------------------------
//				�Q�[���I������
//------------------------------------------------
void UninitGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̏I������
	========================================================================*/
	//�Q�[����ʂɎg�����I�u�W�F�N�g�̏I������
	//UninitBg();			//�w�i�̏I������
	//UninitPlayer();		//�v���C���[�̏I������
	//UninitPause();		//�|�[�Y��ʂ̏I������

	//�e�N�X�`���̔j��
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
}

//------------------------------------------------
//				�Q�[���X�V����
//------------------------------------------------
void UpdateGame(void)
{
	FADE fadeState = GetFade();

	//�|�[�Y���ł͂Ȃ�
	if (g_bPause == false)
	{
		/*======================================================================
		�e��I�u�W�F�N�g�̍X�V����
		========================================================================*/
		//UpdateBg();			//�w�i�̍X�V����
		//UpdatePlayer();		//�v���C���[�̍X�V����
	}

	//�|�[�Y���ł���A�G��|�����]�C���ł͂Ȃ�
	else
	{

	}

	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//���[�h�ݒ�i���U���g��ʂɑJ��)
		SetFade(MODE_RESULT);
	}

	//�|�[�Y�L�[��ON/OFF
	if (GetKeyboardTrigger(DIK_P) == true)
	{//P�L�[�������ꂽ & �G��|�����]�C���ł͂Ȃ�
		g_bPause = g_bPause ? false : true;
	}
}

//------------------------------------------------
//				�Q�[���`�揈��
//------------------------------------------------
void DrawGame(void)
{
	/*======================================================================
	�e��I�u�W�F�N�g�̕`�揈��
	========================================================================*/
	//�Q�[�����I�u�W�F�N�g�̕`�揈��		
	//DrawBg();		//�w�i�̕`�揈��
	//DrawPlayer();	//�v���C���[�̕`�揈��

	//�Q�[���J�n�̃J�E���g�_�E��
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffGame, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureGame);

	//�|���S���̕`��
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