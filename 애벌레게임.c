#include <Windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void End_Game(HWND hwnd, int score, int* status);
void Start_Setting(HWND, int(*)[2], int(*)[2], int*, int*, int*, int*, int*, int*, int*);

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

#define SEC 1000				//1��

#define LOGO 0				//�ΰ�
#define SETTING 1			//����
#define COUNTDOWN 2	//ī��Ʈ�ٿ�
#define GAME 3				//����
#define GAMEOVER 4		//���ӿ���

#define REGAME 0			//�ٽý���(���ӿ����޴�)
#define EXIT 1					//������(���ӿ����޴�)

#define LOGO_TIMER 1					//�ΰ��� �ֹ��� �����̴� Ÿ�̸�
#define COUNTDOWN_TIMER 2	//ī��Ʈ�ٿ� Ÿ�̸�
#define GAME_TIMER 3				//���ӿ��� �ֹ��� Ÿ�̸�

#define EASY 80				//����(�ӵ�)
#define MEDIUM 60			//����(�ӵ�)
#define DIFFICULT 40		//�����(�ӵ�)

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;					//����̽����ؽ�Ʈ, �޸𸮵���̽����ؽ�Ʈ
	PAINTSTRUCT ps;					//�ؽ�Ʈ
	HFONT hFont, oldFont;			//��Ʈ
	HBRUSH hBrush, oldBrush;	//�귯��
	HPEN hPen, oldPen;				//��
	HBITMAP hBit, oldBit;			//����

	static RECT rectView;		//Ŭ���̾�Ʈ ũ��
	static TCHAR buffer[100];

	static int coordinates[12][2] = { 0, };			//��ǥ
	static int items[10][2] = { 0, };					//��������ǥ

	static int status;		//���� ����
	static int difficulty;		//���̵�(�ӵ�)
	static int minus;			//������
	static int cnt_time = 3;		//ī��Ʈ�ٿ�� 3�� ������
	static int cnt_item;		//������ ����
	static int cnt_circle;	//���� ����
	static int score;			//����
	static int direction;	//ȭ��ǥ ����
	static int menu;		//���� ������ �޴�

	int i, x;	//�ݺ����� x��ǥ ������ ���

	srand((unsigned)time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
	{
		GetClientRect(hwnd, &rectView);	//Ŭ���̾�Ʈ ũ��
		Start_Setting(hwnd, coordinates, items, &status, &difficulty, &menu, &direction, &score, &cnt_circle, &cnt_item);
		break;	//iMsg�� break
	}

	/*
	******************
	�׸��� ����
	******************
	*/
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		
		/*���¿� ����*/
		switch (status)
		{
		/*�ΰ�׸���*/
		case LOGO:
			hPen = CreatePen(BS_SOLID, 1, RGB(255, 255, 255));
			oldPen = SelectObject(hdc, hPen);

			for (i = 0; i < cnt_circle; i++)
			{
				hBrush = CreateSolidBrush(RGB(0, 90 + (i * 15), 0));		//���λ�
				oldBrush = SelectObject(hdc, hBrush);
				Ellipse(hdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//������
			}

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			oldFont = SelectObject(hdc, hFont);

			TextOut(hdc, 300, 250, _T("�ֹ��� ����!"), _tcslen(_T("�ֹ��� ����!")));

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			SelectObject(hdc, hFont);

			if (coordinates[0][1] == 100)
				TextOut(hdc, 330, 290, _T("Press Any Key"), _tcslen(_T("Press Any Key")));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status�� break

		/*���ÿ��� �׸���*/
		case SETTING:
		{
			hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			oldFont = SelectObject(hdc, hFont);

			TextOut(hdc, 300, 100, _T("���̵� ����"), _tcslen(_T("���̵� ����")));

			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			SelectObject(hdc, hFont);

			TextOut(hdc, 360, 200, _T("����"), _tcslen(_T("����")));
			TextOut(hdc, 360, 230, _T("����"), _tcslen(_T("����")));
			TextOut(hdc, 350, 260, _T("�����"), _tcslen(_T("�����")));

			/*ȭ��ǥ �׸���*/
			switch (difficulty)
			{
			case EASY:
				TextOut(hdc, 300, 200, _T("��"), _tcslen(_T("��")));
				break;
			case MEDIUM:
				TextOut(hdc, 300, 230, _T("��"), _tcslen(_T("��")));
				break;
			case DIFFICULT:
				TextOut(hdc, 300, 260, _T("��"), _tcslen(_T("��")));
				break;
			}

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status�� break
		}

		/*ī��Ʈ �ٿ� �׸���*/
		case COUNTDOWN:
			hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));		//ī��Ʈ�ٿ�
			oldFont = (HFONT)SelectObject(hdc, hFont);

			wsprintf(buffer, _T("%d���� �����մϴ�"), cnt_time);
			TextOut(hdc, 280, 230, buffer, _tcslen(buffer));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);	//ī��Ʈ�ٿ�
			break;
	
		/*���ӿ��� �׸��� (�ֹ��� �����Ÿ�)*/
		case GAME:
		{
			memdc = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, 800, 500);

			oldBit = (HBITMAP)SelectObject(memdc, hBit);

			Rectangle(memdc, 0, 0, 800, 500);	//���� �Ͼ������

			for (i = 1; i <= 40; i++)
				Rectangle(memdc, (i - 1) * 20, 0, i * 20, 20);
			for (i = 2; i <= 39; i++)
				Rectangle(memdc, (i - 1) * 20, 441, i * 20, 461);
			for (i = 2; i < 25; i++)
				Rectangle(memdc, 0, (i - 1) * 20, 20, i * 20);
			for (i = 2; i < 25; i++)
				Rectangle(memdc, 764, (i - 1) * 20, 784, i * 20);		//���׸���

			for (i = 0; i < 10; i++)		//������ ���
				Ellipse(memdc, items[i][0] - 10, items[i][1] - 10, items[i][0] + 10, items[i][1] + 10);

			for (i = 0; i < cnt_circle; i++)
			{
				hBrush = CreateSolidBrush(RGB(0, 90 + (i * 15), 0));		//���λ�
				oldBrush = SelectObject(memdc, hBrush);
				Ellipse(memdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//������
			}

			SelectObject(memdc, oldBrush);
			DeleteObject(hBrush);			//�귯�� ����

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));		//���� ���
			oldFont = (HFONT)SelectObject(memdc, hFont);

			wsprintf(buffer, _T("���� : %d"), score);
			TextOut(memdc, 0, 0, buffer, _tcslen(buffer));

			SelectObject(memdc, oldFont);
			DeleteObject(hFont);	//��Ʈ ����

			BitBlt(hdc, 0, 0, 800, 500, memdc, 0, 0, SRCCOPY);

			SelectObject(memdc, oldBit);
			DeleteDC(memdc);
			break;	//status�� break
		}

		/*���ӿ��� �޴� ����â*/
		case GAMEOVER:
			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("�����ٸ����"));
			oldFont = SelectObject(hdc, hFont);

			if (score == 0)
			{
				TextOut(hdc, 310, 100, _T("GAME OVER"), _tcslen(_T("GAME OVER")));
				wsprintf(buffer, _T("�������� : %d"), score);
				TextOut(hdc, 320, 200, buffer, _tcslen(buffer));
			}
			else
			{
				wsprintf(buffer, _T("�������� : %d"), score);
				TextOut(hdc, 310, 200, buffer, _tcslen(buffer));
			}

			TextOut(hdc, 340, 300, _T("�ٽ��ϱ�"), _tcslen(_T("�ٽ��ϱ�")));
			TextOut(hdc, 360, 340, _T("����"), _tcslen(_T("����")));

			/*ȭ��ǥ �׸���*/
			switch (menu)
			{
			case REGAME:
				TextOut(hdc, 300, 300, _T("��"), _tcslen(_T("��")));
				break;
			case EXIT:
				TextOut(hdc, 300, 340, _T("��"), _tcslen(_T("��")));
				break;
			}

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status�� break
		}
		EndPaint(hwnd, &ps);
		break;	//iMsg�� break
	}
	/*
	******************
	�׸��� ��
	******************
	*/

	/*
	******************
	�Է¹ޱ� ����
	******************
	*/

	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)	//ESCŰ�ϰ�� Ż��
			DestroyWindow(hwnd);

		/*���¿� ����*/
		switch (status)
		{
		/*�ΰ��϶� �Է��� ������*/
		case LOGO:
			KillTimer(hwnd, LOGO_TIMER);		//�ΰ��� �ֹ��� Ÿ�̸� ����
			status = SETTING;							//���� ����
			InvalidateRgn(hwnd, NULL, TRUE);	//�ٽñ׸���
			break;	//status�� break

		/*�����϶� �Է��� ������*/
		case SETTING:
			if (wParam == VK_RETURN)	//���͸� ���� ����
			{
				cnt_circle = 2;	//���� ó�� ����
				x = 30;		//��ó�� x��ǥ
				for (i = cnt_circle - 1; i >= 0; i--)
				{
					coordinates[i][0] = x, coordinates[i][1] = 30;
					x += 20;
				}	//������ ��ǥ����

				status = COUNTDOWN;		//���� ����

				SetTimer(hwnd, COUNTDOWN_TIMER, SEC, NULL);		//ī��ƮŸ�̸� ����
			}
			else if (wParam == VK_UP && difficulty != EASY)		//��Ű�̸鼭 EASY�� �ƴϸ�
				difficulty += 20;
			else if (wParam == VK_DOWN && difficulty != DIFFICULT)		//�Ʒ�Ű�̸鼭 DIFFICULT�� �ƴϸ�
				difficulty -= 20;

			InvalidateRgn(hwnd, NULL, TRUE);
			break;	//status�� break

		case GAME:
			if (status == GAME && wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN)
				direction = wParam;
			break;	//status�� break

		case GAMEOVER:
			if (wParam == VK_RETURN)
			{
				switch (menu)
				{
				case REGAME:
					Start_Setting(hwnd, coordinates, items, &status, &difficulty, &menu, &direction, &score, &cnt_circle, &cnt_item);
					InvalidateRgn(hwnd, NULL, TRUE);
					break;
				case EXIT:
					DestroyWindow(hwnd);
					break;
				}
			}
			else if (wParam == VK_UP && menu == EXIT)
				menu = REGAME;
			else if (wParam == VK_DOWN && menu == REGAME)
				menu = EXIT;
			InvalidateRgn(hwnd, NULL, TRUE);
			break;	//status�� break
		}
		break;	//iMsg�� break
	}

	/*
	******************
	Ÿ�̸�
	******************
	*/

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

		case COUNTDOWN_TIMER:
			InvalidateRgn(hwnd, NULL, TRUE);
			cnt_time--;
			if (cnt_time == 0)
			{
				KillTimer(hwnd, COUNTDOWN_TIMER);
				status = GAME;

				switch (difficulty)
				{
				case EASY:
					minus = 30;
					SetTimer(hwnd, GAME_TIMER, EASY, NULL);
					break;
				case MEDIUM:
					minus = 40;
					SetTimer(hwnd, GAME_TIMER, MEDIUM, NULL);
					break;
				case DIFFICULT:
					minus = 50;
					SetTimer(hwnd, GAME_TIMER, DIFFICULT, NULL);
					break;
				}
				cnt_time = 3;
			}
			break;		//wParam�� break;

		case GAME_TIMER:
			for (i = 0; i < 10; i++)
			{
				if (coordinates[0][0] == items[i][0] && coordinates[0][1] == items[i][1])		//�Ӹ��� �������� ������
				{
					cnt_circle++;		//�� ���� �ø���
					cnt_item--;	//������ ���� ���̱�
					items[i][0] = 0, items[i][1] = 0;
					score += 100;	//���ھ� �߰�
				}
			}
			score -= minus;		//�� ���̵��� �ӵ����� minus��ŭ ����
			InvalidateRgn(hwnd, NULL, FALSE);		//�ٽñ׸���

			if (score == 0 || cnt_item == 0)	//���ھ 0�̰ų� �������� �� ������
				End_Game(hwnd, score, &status);

			switch (direction)		//���⿡ ���� ������
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
	KillTimer(hwnd, GAME_TIMER);
	if (score == 0)
		MessageBox(hwnd, _T("�׾���� �Ф�"), _T("����"), MB_OK);
	else
	{
		wsprintf(buffer, _T("�������� : %d"), score);
		MessageBox(hwnd, buffer, _T("CLEAR!"), MB_OK);
	}

	*status = GAMEOVER;
	InvalidateRgn(hwnd, NULL, TRUE);
}

/*
*************************
�����Ҷ� ����
*************************
*/

void Start_Setting(HWND hwnd, int (*coordinates)[2], int (*items)[2], int *status, int *difficulty, int *menu, int *direction, int *score, int *cnt_circle, int *cnt_item)
{
	int i, x;

	*status = LOGO;			//���´� �ΰ�
	*difficulty = EASY;		//���̵��� ����
	*menu = REGAME;		//�޴��� �ٽý���

	*score = 10000;
	*direction = VK_RIGHT;

	*cnt_item = 10;
	*cnt_circle = 12;

	for (i = 0; i < 10; i++)	//������ ������ġ
	{
		items[i][0] = (rand() % 36) * 20 + 30;
		items[i][1] = (rand() % 20) * 20 + 30;
	}
	x = -240;
	for (i = *cnt_circle - 1; i >= 0; i--)
	{
		coordinates[i][0] = x, coordinates[i][1] = 100;
		x += 20;
	}

	SetTimer(hwnd, LOGO_TIMER, 200, NULL);
}