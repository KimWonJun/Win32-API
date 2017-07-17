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
		_T("�ֹ���"),	//������ Ÿ��Ʋ�̸�
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
	HPEN hPen, oldPen;
	static RECT rectView;
	static int coordinates[10][2] = { 0, };
	static int circle_cnt;
	int i, x;
	BOOL chk;

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
		GetClientRect(hwnd, &rectView);
		circle_cnt = 2;
		x = 20;
		for (i = circle_cnt - 1; i >= 0; i--)
		{
			coordinates[i][0] = x, coordinates[i][1] = 20;
			x += 40;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		SelectObject(hdc, hPen);
		for (i = 1; i < circle_cnt; i++)
			Ellipse(hdc, coordinates[i][0] - 20, coordinates[i][1] - 20, coordinates[i][0] + 20, coordinates[i][1] + 20);		//������

		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = SelectObject(hdc, hPen);
		Ellipse(hdc, coordinates[0][0] - 20, coordinates[0][1] - 20, coordinates[0][0] + 20, coordinates[0][1] + 20);	//�Ӹ�

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		chk = FALSE;
		switch (wParam)
		{
		case VK_SPACE:
			if (circle_cnt < 9)
				circle_cnt++;
			break;
		case VK_BACK:
			if (circle_cnt > 2)
				circle_cnt--;
			chk = TRUE;
			break;
		case VK_UP:
			if (coordinates[0][1] - 40 > rectView.top)
			{
				for (i = circle_cnt - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][1] -= 40;
			}
			chk = TRUE;
			break;
		case VK_DOWN:
			if (coordinates[0][1] + 40 < rectView.bottom)
			{
				for (i = circle_cnt - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][1] += 40;
			}
			chk = TRUE;
			break;
		case VK_LEFT:
			if (coordinates[0][0] - 40 > rectView.left)
			{
				for (i = circle_cnt - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][0] -= 40;
			}
			chk = TRUE;
			break;
		case VK_RIGHT:
			if (coordinates[0][0] + 40 < rectView.right)
			{
				for (i = circle_cnt - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][0] += 40;
			}
			chk = TRUE;
			break;
		}
		if(chk)
			InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:	//�����찡 ��������
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}