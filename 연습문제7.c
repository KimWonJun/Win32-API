#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS WndClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//��� ����
	WndClass.lpfnWndProc = WndProc;	//�޽��� ó���� ���� �Լ� �̸� ����
	WndClass.cbClsExtra = 0;	//������ Ŭ�����޸� (��� X)
	WndClass.cbWndExtra = 0;	//������ ������޸� (��� X)
	WndClass.hInstance = hInstance;	//�޸𸮻󿡼� ���α׷��� ��ġ ����
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//�����ܸ��
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//Ŀ�����
	WndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));	//����
	WndClass.lpszMenuName = NULL;	//�޴�
	WndClass.lpszClassName = _T("Windows Class Name");	//������ Ŭ�����̸�
	RegisterClass(&WndClass);	//Ŀ�ο� ���
	hwnd = CreateWindow(
		_T("Windows Class Name"),	//������ Ŭ�����̸�
		_T("API Studying"),	//������ Ÿ��Ʋ�̸�
		WS_OVERLAPPEDWINDOW,	//�����콺Ÿ�� (Ÿ��Ʋ���� �ּ�ȭ, �ִ�ȭ, �ݱ�) OR��ȣ�� ������ ��������
		CW_USEDEFAULT,		//x��ǥ
		CW_USEDEFAULT,		//y��ǥ
		CW_USEDEFAULT,		//������ ����ũ��
		CW_USEDEFAULT,		//������ ����ũ��	CW_USEDEFAULT�� Ŀ�ο� ���� �⺻���� ����
		NULL,	// �θ� ������ �ڵ�
		NULL,	// �޴� �ڵ�
		hInstance,		//���� ���α׷� �ν��Ͻ�
		NULL		//���� ������ ����
	);
	ShowWindow(hwnd, nCmdShow);	//(��Ÿ�� ������ �ڵ鰪, �����츦 ȭ�鿡 ��Ÿ���� ������� ����� ���� ex)SW_MAXIMIZE)
	UpdateWindow(hwnd);	//������ ȭ�鿡 �⺻ ����ϱ�
	while (GetMessage(&msg, NULL, 0, 0))	//�޽���ť���� MSG����ü������ ����
	{
		TranslateMessage(&msg);		//��ȯ?
		DispatchMessage(&msg);		//�޽����� ó���ϴ� �Լ��� �޽����� ����
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen;
	HBRUSH hBrush;
	static int chk;

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
		chk = 0;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hPen), SelectObject(hdc, hBrush);
		Rectangle(hdc, 450, 50, 550, 150);	//��
		Rectangle(hdc, 450, 150, 550, 250);		//���
		Rectangle(hdc, 450, 250, 550, 350);		//�Ʒ�
		Rectangle(hdc, 350, 150, 450, 250);		//����
		Rectangle(hdc, 550, 150, 650, 250);		//������
		TextOut(hdc, 460, 100, _T("����"), _tcslen(_T("����")));
		TextOut(hdc, 460, 300, _T("�Ʒ���"), _tcslen(_T("�Ʒ���")));
		TextOut(hdc, 460, 200, _T("���"), _tcslen(_T("���")));
		TextOut(hdc, 360, 200, _T("����"), _tcslen(_T("����")));
		TextOut(hdc, 560, 200, _T("������"), _tcslen(_T("������")));
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdc, hPen), SelectObject(hdc, hBrush);
		switch (chk)
		{
		case 1:
			Rectangle(hdc, 350, 150, 450, 250);		//����
			break;
		case 2:
			Rectangle(hdc, 550, 150, 650, 250);		//������
			break;
		case 3:
			Rectangle(hdc, 450, 50, 550, 150);	//��
			break;
		case 4:
			Rectangle(hdc, 450, 250, 550, 350);		//�Ʒ�
			break;
		case 5:
			Rectangle(hdc, 450, 150, 550, 250);		//���
			break;
		}
		SelectObject(hdc, hPen), SelectObject(hdc, hBrush);
		DeleteObject(hPen), DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;

		case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			chk = 1;
			break;
		case VK_RIGHT:
			chk = 2;
			break;
		case VK_UP:
			chk = 3;
			break;
		case VK_DOWN:
			chk = 4;
			break;
		case VK_SPACE:
			chk = 5;
			break;
		default:
			chk = 0;
		}
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_KEYUP:
		InvalidateRgn(hwnd, NULL, TRUE);
		chk = 0;
		break;

	case WM_DESTROY:	//�����찡 ��������
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}