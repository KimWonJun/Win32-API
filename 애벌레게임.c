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
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//출력 형태
	WndClass.lpfnWndProc = WndProc;	//메시지 처리를 위한 함수 이름 전달
	WndClass.cbClsExtra = 0;	//여분의 클래스메모리 (사용 X)
	WndClass.cbWndExtra = 0;	//여분의 윈도우메모리 (사용 X)
	WndClass.hInstance = hInstance;	//메모리상에서 프로그램의 위치 전달
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//아이콘모양
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//커서모양
	WndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));	//배경색
	WndClass.lpszMenuName = NULL;	//메뉴
	WndClass.lpszClassName = _T("Windows Class Name");	//윈도우 클래스이름
	RegisterClass(&WndClass);	//커널에 등록
	hwnd = CreateWindow(
		_T("Windows Class Name"),	//윈도우 클래스이름
		_T("애벌레"),	//윈도우 타이틀이름
		WS_OVERLAPPEDWINDOW,	//윈도우스타일 (타이틀바의 최소화, 최대화, 닫기) OR기호로 여러개 지정가능
		CW_USEDEFAULT,		//x좌표
		CW_USEDEFAULT,		//y좌표
		800,		//윈도우 가로크기
		500,		//윈도우 세로크기	CW_USEDEFAULT는 커널에 의해 기본값을 지정
		NULL,	// 부모 윈도우 핸들
		NULL,	// 메뉴 핸들
		hInstance,		//응용 프로그램 인스턴스
		NULL		//생성 윈도우 정보
	);
	ShowWindow(hwnd, nCmdShow);	//(나타낼 윈도우 핸들값, 윈도우를 화면에 나타내는 방법으로 상수값 제공 ex)SW_MAXIMIZE)
	UpdateWindow(hwnd);	//윈도우 화면에 기본 출력하기
	while (GetMessage(&msg, NULL, 0, 0))	//메시지큐에서 MSG구조체변수에 저장
	{
		TranslateMessage(&msg);		//변환?
		DispatchMessage(&msg);		//메시지를 처리하는 함수에 메시지를 보냄
	}

	return (int)msg.wParam;
}

void End_Game(HWND hwnd, int score)
{
	TCHAR str[100] = { 0, };
	KillTimer(hwnd, 1);
	MessageBox(hwnd, _T("사망.."), _T("종료"), MB_OK);
	wsprintf(str, _T("최총점수 : %d"), score);
	MessageBox(hwnd, str, _T("최종점수"), MB_OK);
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
	static int score;
	static int coordinates[10][2] = { 0, };
	static int items[10][2] = { 0, }; 
	static int circle_cnt;
	static int direction;
	int i, x;

	switch (iMsg)
	{
	case WM_CREATE:	//윈도우가 만들어졌을때
		GetClientRect(hwnd, &rectView);
		score = 0;
		direction = VK_RIGHT;

		for (i = 0; i < 10; i++)
		{
			items[i][1] = (rand() % 760) + 20;
			items[i][2] = (rand() % 420) + 20;
		}

		circle_cnt = 2;	//머리랑 몸
		x = 30;		//맨처음 x좌표
		for (i = circle_cnt - 1; i >= 0; i--)
		{
			coordinates[i][0] = x, coordinates[i][1] = 30;
			x += 20;
		}	//원들의 좌표설정
		SetTimer(hwnd, 1, 50, NULL);	//0.05초마다 움직임
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
			Rectangle(hdc, 764, (i - 1) * 20, 784, i * 20);	//벽그리기

		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));	//원그리기, 파란색
		oldPen = SelectObject(hdc, hPen);

		for (i = 1; i < circle_cnt; i++)
			Ellipse(hdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//나머지

		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));	//빨강색
		oldPen = SelectObject(hdc, hPen);
		Ellipse(hdc, coordinates[0][0] - 10, coordinates[0][1] - 10, coordinates[0][0] + 10, coordinates[0][1] + 10);	//머리

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);		//원그리기

		hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));		//점수 출력
		oldFont = (HFONT)SelectObject(hdc, hFont);

		wsprintf(str, _T("점수 : %d"), score);
		TextOut(hdc, 0, 0, str, _tcslen(str));

		SelectObject(hdc, oldFont);
		DeleteObject(hFont);	//점수 출력

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN)
			direction = wParam;
		break;

	case WM_TIMER:
		switch (direction)
		{
		case VK_UP:
			if (coordinates[0][1] - 20 < rectView.top + 20)
				End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x같음
				coordinates[i][1] = coordinates[i - 1][1];	//y같음
			}
			coordinates[0][1] -= 20;
			break;

		case VK_DOWN:
			if (coordinates[0][1] + 20 > rectView.bottom - 20)
				End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x같음
				coordinates[i][1] = coordinates[i - 1][1];	//y같음
			}
			coordinates[0][1] += 20;
			break;

		case VK_LEFT:
			if (coordinates[0][0] - 20 < rectView.left + 20)
				End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x같음
				coordinates[i][1] = coordinates[i - 1][1];	//y같음
			}
			coordinates[0][0] -= 20;
			break;

		case VK_RIGHT:
			if (coordinates[0][0] + 20 > rectView.right - 20)
				End_Game(hwnd, score);

			for (i = circle_cnt - 1; i > 0; i--)
			{
				coordinates[i][0] = coordinates[i - 1][0];	//x같음
				coordinates[i][1] = coordinates[i - 1][1];	//y같음
			}
			coordinates[0][0] += 20;
			break;
		}
		score++;
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_DESTROY:	//윈도우가 꺼졌을때
		PostQuitMessage(0);	//GetMessage함수가 0을 반환하게함
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널에서 처리
}