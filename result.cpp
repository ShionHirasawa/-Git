/*==========================================================================================================================================================

���U���g����[result.cpp]
Author:���V����

============================================================================================================================================================*/
#include "result.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "fade.h"
#include "color.h"

//�Q�[���I���̎�ލ\����
typedef enum
{
	RESULT_TYPE_OVER = ZERO,	//�Q�[���I�[�o�[
	RESULT_TYPE_CLEAR,			//�Q�[���N���A
	RESULT_TYPE_MAX
}RESULT_TYPE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureResult[RESULT_TYPE_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//���_�o�b�t�@�ւ̃|�C���^
DWORD g_ResultTime;											//���U���g�\������

//------------------------------------------------
//				���U���g����������
//------------------------------------------------
void InitResult(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[RESULT_TYPE_OVER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &g_pTextureResult[RESULT_TYPE_CLEAR]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * RESULT_TYPE_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCnt = START_CNT; nCnt < RESULT_TYPE_MAX; nCnt++, pVtx += VTX_MAX)
	{
		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, ORIGIN_POS);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, ORIGIN_POS);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, ORIGIN_POS);
		}

		//rhw�̐ݒ�
		{
			pVtx[VTX_LE_UP].rhw = RHW_NUM;
			pVtx[VTX_RI_UP].rhw = RHW_NUM;
			pVtx[VTX_LE_DO].rhw = RHW_NUM;
			pVtx[VTX_RI_DO].rhw = RHW_NUM;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[VTX_LE_UP].col = RGBA_WHITE;
			pVtx[VTX_RI_UP].col = RGBA_WHITE;
			pVtx[VTX_LE_DO].col = RGBA_WHITE;
			pVtx[VTX_RI_DO].col = RGBA_WHITE;
		}

		//�e�N�X�`���̍��W
		{
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(F_ZERO, F_ZERO);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(F_TEN, F_ZERO);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(F_ZERO, F_TEN);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffResult->Unlock();
}

//------------------------------------------------
//				���U���g�I������
//------------------------------------------------
void UninitResult(void)
{
	for (int nCntResult = START_CNT; nCntResult < RESULT_TYPE_MAX; nCntResult++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//------------------------------------------------
//				���U���g�X�V����
//------------------------------------------------
void UpdateResult(void)
{
	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//���[�h�ݒ�i�����L���O��ʂɑJ��)
		SetFade(MODE_RANKING);
	}
}

//------------------------------------------------
//				���U���g�`�揈��
//------------------------------------------------
void DrawResult(void)
{
	int result = ResultChoice();

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffResult, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureResult[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}