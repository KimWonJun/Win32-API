#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void End_Game(HWND hwnd, int score, int* status);

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
		WS_OVERLAPPED,	//�����콺Ÿ�� (Ÿ��Ʋ���� �ּ�ȭ, �ִ�ȭ, �ݱ�) OR��ȣ�� ������ ��������
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

#define LOGO 0
#define SETTING 1
#define GAME 2
#define GAMEOVER 3

#define SEC 1000

#define LOGO_TIMER 1
#define COUNTDOWN_TIMER 2
#define GAME_TIMER 3

#define EASY 1
#define MEDIUM 2
#define DIFFICULT 3

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	HFONT hFont, oldFont;
	HBRUSH hBrush, oldBrush;
	HBITMAP hBit, oldBit;

	static RECT rectView;
	static TCHAR buffer[100];

	static int coordinates[12][2] = { 0, };
	static int items[10][2] = { 0, };

	static int status;		//���� ����
	static int cnt_item;		//������ ����
	static int score;			//����
	static int cnt_circle;	//���� ����
	static int direction;	//ȭ��ǥ ����
	int Speed;		//�ӵ�
	int i, x;	//�ݺ����� x��ǥ ������ ���

	srand((unsigned)time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
	{
		GetClientRect(hwnd, &rectView);	//Ŭ���̾�Ʈ ũ��
		status = LOGO;		//ó�� ����
		cnt_item = 10;		//ó�� ������ ����
		score = 10000;		//ó�� ����
		direction = VK_RIGHT;	//ó�� ���� ����������

		for (i = 0; i < 10; i++)	//������ ������ġ
		{
			items[i][0] = (rand() % 36) * 20 + 30;
			items[i][1] = (rand() % 20) * 20 + 30;
		}

		cnt_circle = 12;	//���� ó�� ����
		x = -240;		//��ó�� x��ǥ
		for (i = cnt_circle - 1; i >= 0; i--)
		{
			coordinates[i][0] = x, coordinates[i][1] = 100;
			x += 20;
		}	//������ ��ǥ����

		SetTimer(hwnd, LOGO_TIMER, 500, NULL);
		break;	//iMsg�� break
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		switch (status)
		{
		case LOGO:
			hBrush = CreateSolidBrush(RGB(0, 180, 0));
			oldBrush = SelectObject(hdc, hBrush);

			for (i = 1; i < cnt_circle; i++)
				Ellipse(hdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//������

			hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));	//������
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, coordinates[0][0] - 10, coordinates[0][1] - 10, coordinates[0][0] + 10, coordinates[0][1] + 10);	//�Ӹ�

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			hFont = CreateFont(40,  0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			oldFont = SelectObject(hdc, hFont);

			DrawText(hdc, _T("�ֹ��� ����!"), _tcslen(_T("�ֹ��� ����!")), &rectView, DT_VCENTER | DT_CENTER);

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			SelectObject(hdc, hFont);

			if(coordinates[0][1] == 100)
				DrawText(hdc, _T("Press Any Key"), _tcslen(_T("Press Any Key")), &rectView, DT_VCENTER);
			
			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status�� break

		case SETTING:
			break;	//status�� break

		case GAME:
		{
			memdc = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, 800, 500);

			oldBit = (HBITMAP)SelectObject(memdc, hBit);

			Rectangle(memdc, 0, 0, 800, 500);

			for (i = 1; i <= 40; i++)
				Rectangle(memdc, (i - 1) * 20, 0, i * 20, 20);
			for (i = 2; i <= 39; i++)
				Rectangle(memdc, (i - 1) * 20, 441, i * 20, 461);
			for (i = 2; i < 25; i++)
				Rectangle(memdc, 0, (i - 1) * 20, 20, i * 20);
			for (i = 2; i < 25; i++)
				Rectangle(memdc, 764, (i - 1) * 20, 784, i * 20);	//���׸���

			for (i = 0; i < 10; i++)		//������ ���
				Ellipse(memdc, items[i][0] - 10, items[i][1] - 10, items[i][0] + 10, items[i][1] + 10);

			hBrush = CreateSolidBrush(RGB(0, 180, 0));
			oldBrush = SelectObject(hdc, hBrush);

			for (i = 1; i < cnt_circle; i++)
				Ellipse(hdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//������

			hBrush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));	//������
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, coordinates[0][0] - 10, coordinates[0][1] - 10, coordinates[0][0] + 10, coordinates[0][1] + 10);	//�Ӹ�

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));		//���� ���
			oldFont = (HFONT)SelectObject(memdc, hFont);

			wsprintf(buffer, _T("���� : %d"), score);
			TextOut(memdc, 0, 0, buffer, _tcslen(buffer));

			SelectObject(memdc, oldFont);
			DeleteObject(hFont);	//���� ���

			BitBlt(hdc, 0, 0, 800, 500, memdc, 0, 0, SRCCOPY);

			SelectObject(memdc, oldBit);
			DeleteDC(memdc);
			break;	//status�� break
		}
		case GAMEOVER:
			break;	//status�� break
		}
		EndPaint(hwnd, &ps);
		break;	//iMsg�� break
	}

	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		switch (status)
		{
		case LOGO:
			if (wParam == VK_SPACE)
			{
				KillTimer(hwnd, LOGO_TIMER);
				status = SETTING;
			}
			break;	//status�� break
		case SETTING:
			SetTimer(hwnd, GAME_TIMER, Speed, NULL);	//Speed / 1000 ���� ������
			break;	//status�� break
		case GAME:
			if (status == GAME && wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN)
				direction = wParam;
			break;	//status�� break
		case GAMEOVER:
			break;	//status�� break
		}
		break;	//iMsg�� break
	}

	case WM_TIMER:
	{
		switch (wParam)
		{
		case LOGO_TIMER:
			for (i = cnt_circle - 1; i > 0; i--)
				coordinates[i][0] = coordinates[i - 1][0], coordinates[i][1] = coordinates[i - 1][1];
			coordinates[0][0] += 20;

			if (coordinates[0][1] == 100)
				coordinates[0][1] += 15;
			else
				coordinates[0][1] -= 15;	//�ٹ��ٹ�

			InvalidateRgn(hwnd, NULL, TRUE);		//�ٽñ׸�

			if (coordinates[11][0] > rectView.right)
			{
				x = -240;		//��ó�� x��ǥ
				for (i = cnt_circle - 1; i >= 0; i--)
				{
					coordinates[i][0] = x, coordinates[i][1] = 100;
					x += 20;
				}	//������ ��ǥ����
			}
			break;
		case GAME_TIMER:
			for (i = 0; i < 10; i++)
			{
				if (coordinates[0][0] == items[i][0] && coordinates[0][1] == items[i][1])
				{
					cnt_circle++;		//�� ���� �ø���
					cnt_item--;	//������ ���� ���̱�
					items[i][0] = 0, items[i][1] = 0;
					score += 100;	//���ھ� �߰�
				}
			}
			score -= 50;		//0.1�ʸ��� 50���� ����
			InvalidateRgn(hwnd, NULL, FALSE);

			if (score == 0)
				End_Game(hwnd, score, &status);
			else if (cnt_item == 0)
				End_Game(hwnd, score, &status);

			switch (direction)
			{
			case VK_UP:
				if (coordinates[0][1] - 20 < rectView.top + 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][1] -= 20;
				break;

			case VK_DOWN:
				if (coordinates[0][1] + 20 > rectView.bottom - 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][1] += 20;
				break;

			case VK_LEFT:
				if (coordinates[0][0] - 20 < rectView.left + 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][0] -= 20;
				break;

			case VK_RIGHT:
				if (coordinates[0][0] + 20 > rectView.right - 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x����
					coordinates[i][1] = coordinates[i - 1][1];	//y����
				}
				coordinates[0][0] += 20;
				break;	//direction�� break
			}
			break;	//wParam�� break
		}
		break;		//iMsg�� break
	}

	case WM_DESTROY:	//�����찡 ��������
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}

void End_Game(HWND hwnd, int score, int* status)
{
	TCHAR buffer[100] = { 0, };
	KillTimer(hwnd, 1);
	if (score == 0)
		MessageBox(hwnd, _T("���.."), _T("����"), MB_OK);
	else
	{
		wsprintf(buffer, _T("�������� : %d"), score);
		MessageBox(hwnd, buffer, _T("CLEAR!"), MB_OK);
	}

	*status = GAMEOVER;
}