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
		_T("API Studying"),	//윈도우 타이틀이름
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
	static BOOL chk;
	static int cat_x, cat_y;
	static int mouse_x, mouse_y;
	static double d_x, d_y;

	switch (iMsg)
	{
	case WM_CREATE:	//윈도우가 만들어졌을때
		cat_x = 100, cat_y = 100;
		mouse_x = 0, mouse_y = 0;
		chk = FALSE;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, (int)cat_x, (int)cat_y, _T("고양이"), _tcslen(_T("고양이")));
		if (chk)
		{
			SetTimer(hwnd, 1, 100, NULL);
			TextOut(hdc, mouse_x, mouse_y, _T("쥐"), _tcslen(_T("쥐")));
		}
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		chk = TRUE;
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
		InvalidateRgn(hwnd, NULL, TRUE);
		break;

	case WM_TIMER:
		d_x = mouse_x - cat_x;
		d_y = mouse_y - cat_y;

		cat_x += (int)(d_x / 7);
		cat_y += (int)(d_y / 7);

		InvalidateRgn(hwnd, NULL, TRUE);
		if ((-100 < (d_x * 7) && (d_x * 7) < 100) && (-100 < (d_y * 7) && (d_y * 7)<  100))
		{
			KillTimer(hwnd, 1);
			PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:	//윈도우가 꺼졌을때
		PostQuitMessage(0);	//GetMessage함수가 0을 반환하게함
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널에서 처리
}