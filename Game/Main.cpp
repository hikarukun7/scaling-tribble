#include <Windows.h>
#include <memory>
#include "Game.h"

//�֐��̃v���g�^�C�v�錾
//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
//�Q�[�����[�v�֐��̃v���g�^�C�v�錾
//bool loop(HWND hwnd);
//�N���C�A���g�̈�̃N���A�֐��̃v���g�^�C�v�錾
//void clear(HDC hdc, COLORREF color);


//�O���[�o���ϐ�
namespace
{
	//�E�B���h�E�̃N���X���ƃ^�C�g��
	const wchar_t* WinClassName = L"DXProj";
	const wchar_t* WinGameName = L"DirectX Game";
	//�E�B���h�E�̕��ƍ���
	constexpr UINT WinWidth = 640;
	constexpr UINT WinHeight = 480;

	//�Q�[���f�[�^
	std::unique_ptr<Game> game;

}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nCmdshow)
{
	//�E�B���h�E�̓o�^�Ɛ���
	//�E�B���h�E�N���X�\���̂̐ݒ�
	WNDCLASSEXW wc = {};
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIconW(nullptr, L"IDI_ICON");
	wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = WinClassName;
	wc.hIconSm = LoadIconW(nullptr, L"IDI_ICON");

	//�E�B���h�E�N���X�̓o�^
	if (!RegisterClassExW(&wc)) return 1;

	//�E�B���h�E�̐����ƕ\��
	RECT rc = {};
	rc.right = (LONG)WinWidth;
	rc.bottom = (LONG)WinHeight;

	HWND hwnd;
	DWORD style = WS_SYSMENU | WS_DLGFRAME | WS_MINIMIZEBOX;
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | style, FALSE);

	hwnd = CreateWindowExW(0, WinClassName, WinGameName, WS_OVERLAPPED | style,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		nullptr, nullptr, hInstance, nullptr);
	if (!hwnd) return 1;

	ShowWindow(hwnd, SW_SHOWNORMAL);
	//���b�Z�[�W���[�v
	{
		//�o�b�N�o�b�t�@�̏�����
		game = std::make_unique<Game>();
		game->initialize(hwnd, WinWidth, WinHeight);
	}

	MSG msg = {};
	while (true)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		if (msg.message == WM_QUIT) break;

		//�Q�[�����[�v
		if (!game->loop())
		{
			DestroyWindow(hwnd);
		}

	}

	//�f�[�^�J��
	

	//�E�B���h�E�̓o�^������
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

	return 0;
}

//�֐��̒�`

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		switch (wparam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			return 0;
		}
		break;
	}
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

//�֐��̒�`