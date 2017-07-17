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
	static BOOL chk;
	static int score;
	static int cat_x, cat_y;
	static int mouse_x, mouse_y;
	static double d_x, d_y;
	static TCHAR str[100];

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
		cat_x = 100, cat_y = 100;
		mouse_x = 0, mouse_y = 0;
		score = 0;
		chk = FALSE;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, (int)cat_x, (int)cat_y, _T("�����"), _tcslen(_T("�����")));
		if (chk)
		{
			SetTimer(hwnd, 1, 100, NULL);
			TextOut(hdc, mouse_x, mouse_y, _T("��"), _tcslen(_T("��")));
		}
		wsprintf(str, _T("���� : %d"), score);
		TextOut(hdc, 940, 0, str, _tcslen(str));
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		chk = TRUE;
		SetTimer(hwnd, 2, 100, NULL);
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_MOUSEMOVE:
		if (chk)
		{
			mouse_x = LOWORD(lParam);
			mouse_y = HIWORD(lParam);
			InvalidateRgn(hwnd, NULL, TRUE);
		}
		break;

	case WM_LBUTTONUP:
		chk = FALSE;
		KillTimer(hwnd, 1);
		KillTimer(hwnd, 2);
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			d_x = mouse_x - cat_x;
			d_y = mouse_y - cat_y;

			cat_x += (int)(d_x / 7);
			cat_y += (int)(d_y / 7);

			InvalidateRgn(hwnd, NULL, TRUE);

			if ((-100 < (d_x * 7) && (d_x * 7) < 100) && (-100 < (d_y * 7) && (d_y * 7) < 100))
			{
				chk = FALSE;
				KillTimer(hwnd, 1);
				MessageBox(hwnd, _T("����.."), _T("���"), MB_OK);
				MessageBox(hwnd, str, _T("��������"),  MB_OK);
				PostQuitMessage(0);
			}
			break;

		case 2:
			score++;
			break;
		}
		break;

	case WM_DESTROY:	//�����찡 ��������
		KillTimer(hwnd, 1);
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}