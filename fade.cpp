/*==========================================================================================================================================================

�t�F�[�h��ʏ���[fade.cpp]
Author:���V����

============================================================================================================================================================*/
#include "fade.h"
#include "color.h"

//�}�N����`

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^		
FADE g_fade;											//�t�F�[�h�̏��
MODE g_modeNext;										//���̉��(���[�h)
D3DXCOLOR g_colorfade;									//�|���S��(�t�F�[�h)�̐F

//------------------------------------------------
//			�t�F�[�h�̏���������
//------------------------------------------------
void InitFade(MODE modeNext)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_fade = FADE_IN;			//�t�F�[�h�C����Ԃ�
	g_modeNext = modeNext;		//���̉��(���[�h)��ݒ�
	g_colorfade = XCOL_BLACK;	//�������(�s����)�ɂ��Ă���

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffFade, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_���W�̐ݒ�
	{
		pVtx[VTX_LE_UP].pos = D3DXVECTOR3(ORIGIN_POS, ORIGIN_POS, NIL_F);
		pVtx[VTX_RI_UP].pos = D3DXVECTOR3(SCREEN_WIDTH, ORIGIN_POS, NIL_F);
		pVtx[VTX_LE_DO].pos = D3DXVECTOR3(ORIGIN_POS, SCREEN_HEIGHT, NIL_F);
		pVtx[VTX_RI_DO].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, NIL_F);
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
		pVtx[VTX_LE_UP].col = g_colorfade;
		pVtx[VTX_RI_UP].col = g_colorfade;
		pVtx[VTX_LE_DO].col = g_colorfade;
		pVtx[VTX_RI_DO].col = g_colorfade;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(g_modeNext);
}

//------------------------------------------------
//			�t�F�[�h�̏I������
//------------------------------------------------
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//------------------------------------------------
//			�t�F�[�h�̍X�V����
//------------------------------------------------
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		//�t�F�[�h�C������
		if (g_fade == FADE_IN)
		{
			g_colorfade.a -= 0.015f;					//�|���S���𓧖��ɂ��Ă���

			if (g_colorfade.a <= 0.0f)
			{
				g_colorfade.a = 0.0f;
				g_fade = FADE_NONE;						//�������Ă��Ȃ���Ԃ�
			}
		}

		//�t�F�[�h�A�E�g����
		else if (g_fade == FADE_OUT)
		{
			g_colorfade.a += 0.015f;					//�|���S����s�����ɂ��Ă���

			if (g_colorfade.a >= 1.0f)
			{
				g_colorfade.a = 1.0f;
				g_fade = FADE_IN;						//�t�F�[�h�C����Ԃ�

														//���[�h�ݒ�(���̉�ʂɈڍs)
				SetMode(g_modeNext);
			}
		}

		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

		pVtx[VTX_LE_UP].col = g_colorfade;
		pVtx[VTX_RI_UP].col = g_colorfade;
		pVtx[VTX_LE_DO].col = g_colorfade;
		pVtx[VTX_RI_DO].col = g_colorfade;

		//���_�o�b�t�@�����b�N����
		g_pVtxBuffFade->Unlock();
	}
}

//------------------------------------------------
//			�t�F�[�h�̕`�揈��
//------------------------------------------------
void DrawFade(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffFade, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(ZERO, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}

//�t�F�[�h�̐ݒ菈��
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;			//�t�F�[�h�A�E�g��Ԃ�
	g_modeNext = modeNext;		//���̉��(���[�h)��ݒ�
	g_colorfade = XCOL_NONE;	//�����|���S��(����)�ɂ��Ă���
}

//�t�F�[�h�̎擾����
FADE GetFade(void)
{
	return g_fade;
}