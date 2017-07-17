#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

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
		CW_USEDEFAULT,		//윈도우 가로크기
		CW_USEDEFAULT,		//윈도우 세로크기	CW_USEDEFAULT는 커널에 의해 기본값을 지정
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
	case WM_CREATE:	//윈도우가 만들어졌을때
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
			Ellipse(hdc, coordinates[i][0] - 20, coordinates[i][1] - 20, coordinates[i][0] + 20, coordinates[i][1] + 20);		//나머지

		hPen = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldPen = SelectObject(hdc, hPen);
		Ellipse(hdc, coordinates[0][0] - 20, coordinates[0][1] - 20, coordinates[0][0] + 20, coordinates[0][1] + 20);	//머리

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
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
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
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
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
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
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
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
				}
				coordinates[0][0] += 40;
			}
			chk = TRUE;
			break;
		}
		if(chk)
			InvalidateRgn(hwnd, NULL, TRUE);
		break;
	case WM_DESTROY:	//윈도우가 꺼졌을때
		PostQuitMessage(0);	//GetMessage함수가 0을 반환하게함
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널에서 처리
}