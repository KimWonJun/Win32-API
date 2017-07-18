#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void End_Game(HWND hwnd, int score);

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
		800,		//������ ����ũ��
		500,		//������ ����ũ��	CW_USEDEFAULT�� Ŀ�ο� ���� �⺻���� ����
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

void End_Game(HWND hwnd, int score)
{
	TCHAR str[100] = { 0, };
	KillTimer(hwnd, 1);
	if(score == 0)
		MessageBox(hwnd, _T("���.."), _T("����"), MB_OK);
	else
	{
		wsprintf(str, _T("�������� : %d"), score);
		MessageBox(hwnd, str, _T("CLEAR!"), MB_OK);
	}
	PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hFont, oldFont;
	HPEN hPen, oldPen;

	static RECT rectView;
	static TCHAR str[100];

	static int coordinates[10][2] = { 0, };
	static int items[10][2] = { 0, }; 

	static int cnt_item;		//������ ����
	static int score;			//����
	static int circle_cnt;	//���� ����
	static int direction;	//ȭ��ǥ ����
	int i, x;	//�ݺ����� x��ǥ ������ ���

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
		GetClientRect(hwnd, &rectView);	//Ŭ���̾�Ʈ ũ��
		cnt_item = 10;		//ó�� ������ ����
		score = 10000;		//ó�� ����
		direction = VK_RIGHT;	//ó�� ���� ����������

		for (i = 0; i < 10; i++)	//������ ������ġ
		{
			items[i][0] = (rand() % 36) * 20 + 30;
			items[i][1] = (rand() % 20) * 20 + 30;
		}

		circle_cnt = 2;	//���� ó�� ����
		x = 30;		//��ó�� x��ǥ
		for (i = circle_cnt - 1; i >= 0; i--)
		{
			coordinates[i][0] = x, coordinates[i][1] = 30;
			x += 20;
		}	//������ ��ǥ����
		SetTimer(hwnd, 1, 50, NULL);	//0.05�ʸ��� ������
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (i = 1; i <= 40; i++)
			Rectangle(hdc, (i - 1) * 20, 0, i * 20, 20);
		for (i = 2; i <= 39; i++)
			Rectangle(hdc, (i - 1) * 20, 441, i * 20, 461);
		for (i = 2; i < 25; i++)
			Rectangle(hdc, 0, (i - 1) * 20, 20, i * 20);
		for (i = 2; i < 25; i++)
			Rectangle(hdc, 764, (i - 1) * 20, 784, i * 20);	//���׸���

		for (i = 0; i < 10; i++)
			Ellipse(hdc, items[i][0] - 10, items[i][1] - 10, items[i][0] + 10, items[i][1] + 10);

		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));	//���׸���, �Ķ���
		oldPen =	SelectObject(hdc, hPen);

		for (i = 1; i < circle_cnt; i++)
			Ellipse(hdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//������

		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));	//������
		oldPen = SelectObject(hdc, hPen);
		Ellipse(hdc, coordinates[0][0] - 10, coordinates[0][1] - 10, coordinates[0][0] + 10, coordinates[0][1] + 10);	//�Ӹ�

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);		//���׸���

		hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));		//���� ���
		oldFont = (HFONT)SelectObject(hdc, hFont);

		wsprintf(str, _T("���� : %d"), score);
		TextOut(hdc, 0, 0, str, _tcslen(str));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);	//���� ���

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN)
			direction = wParam;
		break;

	case WM_TIMER:
		if (score == 0)
			End_Game(hwnd, score);
		else if (cnt_item == 0)
			End_Game(hwnd, score);
		switch (direction)
		{
		case VK_UP:
			if (coordinates[0][1] - 20 < rectView.top + 20)
				score = 0, End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x����
				coordinates[i][1] = coordinates[i - 1][1];	//y����
			}
			coordinates[0][1] -= 20;
			break;

		case VK_DOWN:
			if (coordinates[0][1] + 20 > rectView.bottom - 20)
				score = 0, End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x����
				coordinates[i][1] = coordinates[i - 1][1];	//y����
			}
			coordinates[0][1] += 20;
			break;

		case VK_LEFT:
			if (coordinates[0][0] - 20 < rectView.left + 20)
				score = 0, End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x����
				coordinates[i][1] = coordinates[i - 1][1];	//y����
			}
			coordinates[0][0] -= 20;
			break;

		case VK_RIGHT:
			if (coordinates[0][0] + 20 > rectView.right - 20)
				score = 0, End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x����
				coordinates[i][1] = coordinates[i - 1][1];	//y����
			}
			coordinates[0][0] += 20;
			break;
		}
		for (i = 0; i < 10; i++)
		{
			if (coordinates[0][0] == items[i][0] && coordinates[0][1] == items[i][1])
			{
				circle_cnt++;		//�� ���� �ø���
				cnt_item--;	//������ ���� ���̱�
				items[i][0] = 0, items[i][1] = 0;		//������ �ʱ�ȭ
				score += 100;	//���ھ� �߰�
			}
		}
		score -= 50;		//0.1�ʸ��� 50���� ����
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:	//�����찡 ��������
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}