/*==========================================================================================================================================================

�^�C�g������[title.cpp]
Author:���V����

============================================================================================================================================================*/
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "color.h"

//�}�N����`
#define TITLE_LOGO		(1)			//�^�C�g����ʂŎg���e�N�X�`���̐�

#define TITLE_BG_TEX	"data/TEXTURE/title.png"

//�^�C�g�����\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	float fAngle;			//���S���璸�_�܂ł̊p�x
	float flength;			//���S���璸�_�܂ł̒���
	float fLengthMinus;		//���������X�Ɍ��ɖ߂����߂̕ϐ�
}TitleName;

//�^�C�g���W��
typedef enum
{
	TITLE_DISPLAY = ZERO,
	TITLE_NAME,
	TITLE_ENTER,
	TITLE_MAX
}TITLE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9		g_pTextureTitle[TITLE_LOGO] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				//���_�o�b�t�@�ւ̃|�C���^
TitleName				g_Title;							//�^�C�g���̏��

//------------------------------------------------
//				�^�C�g������������
//------------------------------------------------
void InitTitle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TITLE_BG_TEX, &g_pTextureTitle[TITLE_DISPLAY]);	//�^�C�g����ʔw�i

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * VTX_MAX * TITLE_LOGO, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTitle, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	g_Title.pos = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);			//�ʒu������
	g_Title.move = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);			//�ړ��ʂ�����������
	g_Title.rot = D3DXVECTOR3(F_ZERO, F_ZERO, F_ZERO);			//����������������(Z�l���g�p)

	g_Title.flength = sqrtf(0.0f * 0.0f + 0.0f * 0.0f) * 0.0f;	//�������Z�o

	g_Title.fAngle = atan2f(0.0f, 0.0f);						//�p�x���Z�o

	//���ꂼ���UI��������
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++, pVtx += VTX_MAX)
	{
		switch (nCnt)
		{
		case TITLE_DISPLAY:		//�^�C�g�����
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
		break;
		}
	}
									//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//------------------------------------------------
//				�^�C�g���I������
//------------------------------------------------
void UninitTitle(void)
{
	//�e�N�X�`��(�O��)�̔j��
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//------------------------------------------------
//				�^�C�g���X�V����
//------------------------------------------------
void UpdateTitle(void)
{
	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffTitle->Unlock();

	//�L�[�{�[�h��ENTER�@���@�Q�[���p�b�h�́@A�{�^���@���@START�{�^���������ꂽ
	if ((GetKeyboardTrigger(DIK_RETURN) || GetGamePadTrigger(XINPUT_GAMEPAD_A) || GetGamePadTrigger(XINPUT_GAMEPAD_START)) == true)
	{
		//���[�h�ݒ�i�`���[�g���A����ʂɑJ��)
		SetFade(MODE_TUTORIAL);
	}
}

//------------------------------------------------
//				�^�C�g���`�揈��
//------------------------------------------------
void DrawTitle(void)
{
	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(ZERO, g_pVtxBuffTitle, ZERO, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (int nCnt = START_CNT; nCnt < TITLE_LOGO; nCnt++)
	{
		pDevice->SetTexture(ZERO, g_pTextureTitle[nCnt]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * VTX_MAX, SEC);
	}
}