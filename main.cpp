/*==========================================================================================================================================================

�n�b�J�\��-���u�����h-�̃��C������[main.cpp]
Author:���V����

============================================================================================================================================================*/
#include "main.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "tutorial.h"
//#include "player.h"
//#include "pause.h"
#include "ranking.h"

//�}�N����`
#define CLASS_NAME		"WindowClass"				//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME		"�n�b�J�\��-���u�����h-"	//�E�C���h�E�̖��O�i�L���v�V�����ɕ\���j

#define HALF_TIME		(500)					//0.5�b
#define SEN_TIMES		(1000)					//���l��1000�{�ɂ���Ƃ��Ɏg�p
#define ONE_FLAME		(1000 / 60)				//�P�t���[��
#define MAX_MOJI		(256)					//�f�o�b�O�p�Ɏg�p�����s�̕����̌��E��

//�O���[�o���ϐ�
LPDIRECT3D9			g_pD3D = NULL;				//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;		//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT			g_pFont = NULL;				//�t�H���g�ւ̃|�C���^
int					g_nCountFPS = ZERO;			//FPS�J�E���^

#if _DEBUG
MODE g_mode = MODE_TITLE;
#else
MODE g_mode = MODE_TITLE;						//���݂̃��[�h
#endif

//==========================================================================================================================
//                                                     ���C���֐�
//==========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;						//���ݎ���
	DWORD dwExecLastTime;						//�Ō�ɏ�����������
	DWORD dwFrameCount;							//�t���[���J�E���g
	DWORD dwFPSLastTime;						//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,								//�E�C���h�E�̃X�^�C��
		WindowProc,								//�E�C���h�E�v���V�[�W��
		NULL,									//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		NULL,									//�O�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,								//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),			//�^�X�N�o�[�̃A�C�R��
		LoadIcon(NULL,IDI_APPLICATION),			//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + ONE),			//�N���C�A���g�̈�̔w�i�F
		NULL,									//���j���[�o�[
		CLASS_NAME,								//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),			//�t�@�C���̃A�C�R��
	};

	HWND hWnd;									//�E�C���h�E�n���h���i���ʎq�j
	MSG  mSg;									//���b�Z�[�W���i�[����ϐ�

	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };	//��ʃT�C�Y�̍\����

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(ZERO,						//�g���E�C���h�E�X�^�C��
		CLASS_NAME,									//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,								//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,						//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,								//�E�C���h�E�̍���w���W
		CW_USEDEFAULT,								//�E�C���h�E�̍���x���W
		(rect.right - rect.left),					//�E�C���h�E�̕�
		(rect.bottom - rect.top),					//�E�C���h�E�̍���
		NULL,										//�e�E�C���h�E�̃n���h��
		NULL,										//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,									//�C���X�^���g�n���h��
		NULL);										//�E�C���h�E�쐬�f�[�^

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);						//�E�C���h�E�̕\����Ԃ�ݒ�

	UpdateWindow(hWnd);								//�N���C�A���g�̈���X�V

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s������
		return MINUS_ONE;
	}

	//����\��ݒ�
	timeBeginPeriod(ONE);

	//�`�揈���֘A�̊֐��̏�����
	dwCurrentTime = ZERO;
	dwExecLastTime = timeGetTime();

	//FPS�֘A�̊֐��̏�����
	dwFrameCount = ZERO;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (ONE)
	{
		if (PeekMessage(&mSg, NULL, ZERO, ZERO, PM_REMOVE) != ZERO)
		{//windows�̏���
			if (mSg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&mSg);					//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&mSg);					//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= HALF_TIME)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * SEN_TIMES) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = ZERO;					//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= ONE_FLAME)
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�

													//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;						//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)mSg.wParam;

	return ZERO;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:										//�E�C���h�E�j���̃��b�Z�[�W
															//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(ZERO);
		break;

	case WM_KEYDOWN:										//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:										//[ESC]�L�[�������ꂽ
															//�I�����邩�̊m�F
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				MessageBox(NULL, "�܂����Ă��������ˁ`", "������", MB_OK);
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;				//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;		//�v���[���e�[�V�����p�����[�^

										//DirectX3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�����̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʂ̃T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʂ̃T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

																	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	/*======================================================================
	�e��I�u�W�F�N�g�̏���������
	========================================================================*/
	//�����_�[�X�e�[�g
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//�e�N�X�`���̌J��Ԃ��̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�T�E���h�̏���������
	//InitSound(hWnd);

	//�t�F�[�h�̐ݒ�
	InitFade(g_mode);

	return S_OK;
}

//�I������
void Uninit(void)
{
	//����\��߂�
	timeEndPeriod(ONE);

	/*======================================================================
	�e��I�u�W�F�N�g�̏I������
	========================================================================*/
	UninitTitle();		//�^�C�g���I��
	UninitTutorial();	//�`���[�g���A���I��
	UninitGame();		//�Q�[���I��
	UninitResult();		//���U���g�I��
	UninitRanking();	//�����L���O�̏I������
	UninitKeyboard();	//�L�[�{�[�h�̏I������
	//StopSound();		//�T�E���h�̒�~
	//UninitSound();	//�T�E���h�̏I������
	UninitFade();		//�t�F�[�h�̏I������

	//�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//�X�V����
void Update(void)
{
	UpdateKeyboard();				//�L�[�{�[�h�̍X�V����

	FADE fadeState = GetFade();

	if (fadeState == FADE_NONE)
	{
		switch (g_mode)
		{
		case MODE_TITLE:		//�^�C�g����ʂ̍X�V
			UpdateTitle();
			break;
		case MODE_TUTORIAL:		//�`���[�g���A����ʂ̍X�V
			UpdateTutorial();
			break;
		case MODE_GAME:			//�Q�[����ʂ̍X�V
			UpdateGame();
			break;
		case MODE_RESULT:		//���U���g��ʂ̍X�V
			UpdateResult();
			break;
		case MODE_RANKING:		//�����L���O��ʂ̍X�V
			UpdateRanking();
			break;
		}
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();
}

//�`�揈��
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(ZERO, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(ZERO, ZERO, ZERO, ZERO), F_TEN, ZERO);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		/*======================================================================
							�e��I�u�W�F�N�g�̕`�揈��
		========================================================================*/
		switch (g_mode)
		{
		case MODE_TITLE:		//�^�C�g����ʕ`��
			DrawTitle();
			break;
		case MODE_TUTORIAL:		//�`���[�g���A����ʕ`��
			DrawTutorial();
			break;
		case MODE_GAME:			//�Q�[����ʕ`��
			DrawGame();
			DrawFPS();
			break;
		case MODE_RESULT:		//���U���g��ʕ`��
			DrawResult();
			break;
		case MODE_RANKING:		//�����L���O��ʕ`��
			DrawRanking();
			break;
		}

		//�t�F�[�h�̕`�揈��
		DrawFade();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//FPS�̕\��
void DrawFPS(void)
{
#if _DEBUG
	RECT rect = { ZERO, ZERO, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[MAX_MOJI];

	//�`�悷�镶���ݒ�
	wsprintf(&aStr[ZERO], "");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[ZERO], MINUS_ONE, &rect, DT_LEFT, D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX));
#endif
}

//�f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//���[�h�̐ݒ�
void SetMode(MODE mode)
{
	//���݂̉�ʁi���[�h�j�̏I������
	switch (g_mode)
	{
	case MODE_TITLE:		//�^�C�g����ʏI��
		UninitTitle();
		break;
	case MODE_TUTORIAL:		//�`���[�g���A����ʏI��
		UninitTutorial();
		break;
	case MODE_GAME:			//�Q�[����ʏI��
		UninitGame();
		break;
	case MODE_RESULT:		//���U���g��ʏI��
		UninitResult();
		break;
	case MODE_RANKING:		//�����L���O��ʏI��
		UninitRanking();
		break;
	}

	//�V������ʁi���[�h�j�̏���������
	switch (mode)
	{
	case MODE_TITLE:		//�^�C�g����ʏ�����
		InitTitle();
		break;
	case MODE_TUTORIAL:		//�`���[�g���A����ʏ�����
		InitTutorial();
		break;
	case MODE_GAME:			//�Q�[����ʏ�����
		InitGame();
		break;
	case MODE_RESULT:		//���U���g��ʏ�����
		InitResult();
		break;
	case MODE_RANKING:		//�����L���O��ʏ�����
		InitRanking();
		break;
	}

	g_mode = mode;		//���݂̃��[�h��؂�ւ���
}

//���[�h�̎擾
MODE GetMode(void)
{
	return g_mode;
}