/*==========================================================================================================================================================

�����L���O����[ranking.cpp]
Author:���V����

============================================================================================================================================================*/
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "color.h"

//�}�N����`
#define RANK_DATA		"data/RANKING.txt"			//�����L���O�t�@�C��
#define RANK_TEXTURE	(1)							//�����L���O�Ɏg�p����摜�̐�
#define RANK_NUM		(5)							//TOP5�܂ŕ\��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureRanking[RANK_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;				//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3				g_posRank[RANK_NUM];					//�e���ʂ��Ƃ̍��W
int						g_nRanking[RANK_NUM];					//�����N�̒l

//------------------------------------------------
//				�����L���O����������
//------------------------------------------------
void InitRanking(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &g_pTextureRanking[0]);	//�w�i�摜

	//�X�R�A�̏��̏���������у����L���O�t�@�C���̓ǂݍ���
	//ScanRanking();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffRanking, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//�w�i�摜�̐ݒ�
	{
		//���_���W�̐ݒ�
		{
			pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, F_ZERO);
			pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, F_ZERO);
			pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, F_ZERO);
			pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, F_ZERO);
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
			pVtx[VTX_LE_UP].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[VTX_RI_UP].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[VTX_LE_DO].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[VTX_RI_DO].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	////�����L���O�Ɏg�p����e���ʂ̊e���̏�񏉊���
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

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffRanking->Unlock();
}

//------------------------------------------------
//				�����L���O�I������
//------------------------------------------------
void UninitRanking(void)
{
	//�e�N�X�`���̔j��(2��)
	for (int nCntRankTex = START_CNT; nCntRankTex < RANK_TEXTURE; nCntRankTex++)
	{
		if (g_pTextureRanking[nCntRankTex] != NULL)
		{
			g_pTextureRanking[nCntRankTex]->Release();
			g_pTextureRanking[nCntRankTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//------------------------------------------------
//				�����L���O�X�V����
//------------------------------------------------
void UpdateRanking(void)
{
	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//���[�h�ݒ�i�����L���O��ʂɑJ��)
		SetFade(MODE_TITLE);
	}
}

//------------------------------------------------
//				�����L���O�`�揈��
//------------------------------------------------
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffRanking, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�w�i�摜�̐ݒ�
	pDevice->SetTexture(ZERO, g_pTextureRanking[0]);

	//�������̃|���S���̕`��
	for (nCntScore = START_CNT; nCntScore < 1; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * VTX_MAX, SEC);
	}
}

//�����L���O�̐ݒ菈��
void ScanRanking(void)
{
	FILE *pFile;

	pFile = fopen(RANK_DATA, "r");	//�����L���O�t�@�C���ǂݍ���

	if (pFile != NULL)
	{
		for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
		{
			fscanf(pFile, "%d", &g_nRanking[nCntRanking]);		//�����L���O�z��Ɋi�[
		}

		fclose(pFile);

		DownSort();		//���ёւ������Ɉړ�
	}
}

//���ёւ�����
void DownSort(void)
{
	int PlayerRank = ZERO;	//�v���C���[�̃����L���O

	if (PlayerRank > g_nRanking[FOUR])
	{//�v���C���[�̃X�R�A���A�ǂݍ��܂ꂽ���l�̍ŏ��l���傫�����������ւ�
		g_nRanking[FOUR] = PlayerRank;

		for (int nCntRanking = START_CNT; nCntRanking < FOUR; nCntRanking++)				//��r����鐔�l(��̒l)
		{
			for (int mCntRanking = nCntRanking + ONE; mCntRanking < RANK_NUM; mCntRanking++)	//��r���鐔�l
			{
				//��r�������l����̒l���傫����΃o�u���\�[�g���s��
				if (g_nRanking[nCntRanking] < g_nRanking[mCntRanking])
				{
					int nTmp = g_nRanking[nCntRanking];					//��̒l���ꎞ�ۊǏꏊ�Ɋi�[

					g_nRanking[nCntRanking] = g_nRanking[mCntRanking];	//��̒l�̏ꏊ���㏑��

					g_nRanking[mCntRanking] = nTmp;						//��r���鐔�l�̏ꏊ���㏑��
				}
			}
		}

		FILE *pFile;	//�t�@�C���|�C���^

		pFile = fopen(RANK_DATA, "w");

		//�����L���O������������
		if (pFile != NULL)
		{
			for (int nCntRanking = START_CNT; nCntRanking < RANK_NUM; nCntRanking++)
			{
				fprintf(pFile, "%.8d\n", g_nRanking[nCntRanking]);		//���ʂ��Ƃ̃X�R�A�������o��
			}

			fclose(pFile);
		}
	}
}