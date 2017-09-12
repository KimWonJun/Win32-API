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
		WS_OVERLAPPED,	//윈도우스타일 (타이틀바의 최소화, 최대화, 닫기) OR기호로 여러개 지정가능
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

#define SEC 1000				//1초

#define LOGO 0				//로고
#define SETTING 1			//설정
#define COUNTDOWN 2	//카운트다운
#define GAME 3				//게임
#define GAMEOVER 4		//게임오버

#define REGAME 0			//다시시작(게임오버메뉴)
#define EXIT 1					//나가기(게임오버메뉴)

#define LOGO_TIMER 1					//로고의 애벌레 움직이느 타이머
#define COUNTDOWN_TIMER 2	//카운트다운 타이머
#define GAME_TIMER 3				//게임에서 애벌레 타이머

#define EASY 80				//쉬움(속도)
#define MEDIUM 60			//보통(속도)
#define DIFFICULT 40		//어려움(속도)

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;					//디바이스콘텍스트, 메모리디바이스콘텍스트
	PAINTSTRUCT ps;					//텍스트
	HFONT hFont, oldFont;			//폰트
	HBRUSH hBrush, oldBrush;	//브러쉬
	HPEN hPen, oldPen;				//펜
	HBITMAP hBit, oldBit;			//사진

	static RECT rectView;		//클라이언트 크기
	static TCHAR buffer[100];

	static int coordinates[12][2] = { 0, };			//좌표
	static int items[10][2] = { 0, };					//아이템좌표

	static int status;		//게임 상태
	static int difficulty;		//난이도(속도)
	static int minus;			//감소폭
	static int cnt_time = 3;		//카운트다운시 3초 보관용
	static int cnt_item;		//아이템 개수
	static int cnt_circle;	//원의 개수
	static int score;			//점수
	static int direction;	//화살표 방향
	static int menu;		//게임 오버시 메뉴

	int i, x;	//반복문과 x좌표 담을때 사용

	srand((unsigned)time(NULL));

	switch (iMsg)
	{
	case WM_CREATE:	//윈도우가 만들어졌을때
	{
		GetClientRect(hwnd, &rectView);	//클라이언트 크기
		Start_Setting(hwnd, coordinates, items, &status, &difficulty, &menu, &direction, &score, &cnt_circle, &cnt_item);
		break;	//iMsg의 break
	}

	/*
	******************
	그리기 시작
	******************
	*/
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		
		/*상태에 따라*/
		switch (status)
		{
		/*로고그리기*/
		case LOGO:
			hPen = CreatePen(BS_SOLID, 1, RGB(255, 255, 255));
			oldPen = SelectObject(hdc, hPen);

			for (i = 0; i < cnt_circle; i++)
			{
				hBrush = CreateSolidBrush(RGB(0, 90 + (i * 15), 0));		//연두색
				oldBrush = SelectObject(hdc, hBrush);
				Ellipse(hdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//나머지
			}

			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));
			oldFont = SelectObject(hdc, hFont);

			TextOut(hdc, 300, 250, _T("애벌레 게임!"), _tcslen(_T("애벌레 게임!")));

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));
			SelectObject(hdc, hFont);

			if (coordinates[0][1] == 100)
				TextOut(hdc, 330, 290, _T("Press Any Key"), _tcslen(_T("Press Any Key")));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status의 break

		/*세팅에서 그리기*/
		case SETTING:
		{
			hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));
			oldFont = SelectObject(hdc, hFont);

			TextOut(hdc, 300, 100, _T("난이도 선택"), _tcslen(_T("난이도 선택")));

			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));
			SelectObject(hdc, hFont);

			TextOut(hdc, 360, 200, _T("쉬움"), _tcslen(_T("쉬움")));
			TextOut(hdc, 360, 230, _T("보통"), _tcslen(_T("보통")));
			TextOut(hdc, 350, 260, _T("어려움"), _tcslen(_T("어려움")));

			/*화살표 그리기*/
			switch (difficulty)
			{
			case EASY:
				TextOut(hdc, 300, 200, _T("▶"), _tcslen(_T("▶")));
				break;
			case MEDIUM:
				TextOut(hdc, 300, 230, _T("▶"), _tcslen(_T("▶")));
				break;
			case DIFFICULT:
				TextOut(hdc, 300, 260, _T("▶"), _tcslen(_T("▶")));
				break;
			}

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status의 break
		}

		/*카운트 다운 그리기*/
		case COUNTDOWN:
			hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));		//카운트다운
			oldFont = (HFONT)SelectObject(hdc, hFont);

			wsprintf(buffer, _T("%d초후 시작합니다"), cnt_time);
			TextOut(hdc, 280, 230, buffer, _tcslen(buffer));

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);	//카운트다운
			break;
	
		/*게임에서 그리기 (애벌레 깜빡거림)*/
		case GAME:
		{
			memdc = CreateCompatibleDC(hdc);
			hBit = CreateCompatibleBitmap(hdc, 800, 500);

			oldBit = (HBITMAP)SelectObject(memdc, hBit);

			Rectangle(memdc, 0, 0, 800, 500);	//바탕 하얀색으로

			for (i = 1; i <= 40; i++)
				Rectangle(memdc, (i - 1) * 20, 0, i * 20, 20);
			for (i = 2; i <= 39; i++)
				Rectangle(memdc, (i - 1) * 20, 441, i * 20, 461);
			for (i = 2; i < 25; i++)
				Rectangle(memdc, 0, (i - 1) * 20, 20, i * 20);
			for (i = 2; i < 25; i++)
				Rectangle(memdc, 764, (i - 1) * 20, 784, i * 20);		//벽그리기

			for (i = 0; i < 10; i++)		//아이템 출력
				Ellipse(memdc, items[i][0] - 10, items[i][1] - 10, items[i][0] + 10, items[i][1] + 10);

			for (i = 0; i < cnt_circle; i++)
			{
				hBrush = CreateSolidBrush(RGB(0, 90 + (i * 15), 0));		//연두색
				oldBrush = SelectObject(memdc, hBrush);
				Ellipse(memdc, coordinates[i][0] - 10, coordinates[i][1] - 10, coordinates[i][0] + 10, coordinates[i][1] + 10);		//나머지
			}

			SelectObject(memdc, oldBrush);
			DeleteObject(hBrush);			//브러쉬 삭제

			hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));		//점수 출력
			oldFont = (HFONT)SelectObject(memdc, hFont);

			wsprintf(buffer, _T("점수 : %d"), score);
			TextOut(memdc, 0, 0, buffer, _tcslen(buffer));

			SelectObject(memdc, oldFont);
			DeleteObject(hFont);	//폰트 삭제

			BitBlt(hdc, 0, 0, 800, 500, memdc, 0, 0, SRCCOPY);

			SelectObject(memdc, oldBit);
			DeleteDC(memdc);
			break;	//status의 break
		}

		/*게임오버 메뉴 선택창*/
		case GAMEOVER:
			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, _T("나눔바른고딕"));
			oldFont = SelectObject(hdc, hFont);

			if (score == 0)
			{
				TextOut(hdc, 310, 100, _T("GAME OVER"), _tcslen(_T("GAME OVER")));
				wsprintf(buffer, _T("최종점수 : %d"), score);
				TextOut(hdc, 320, 200, buffer, _tcslen(buffer));
			}
			else
			{
				wsprintf(buffer, _T("최종점수 : %d"), score);
				TextOut(hdc, 310, 200, buffer, _tcslen(buffer));
			}

			TextOut(hdc, 340, 300, _T("다시하기"), _tcslen(_T("다시하기")));
			TextOut(hdc, 360, 340, _T("종료"), _tcslen(_T("종료")));

			/*화살표 그리기*/
			switch (menu)
			{
			case REGAME:
				TextOut(hdc, 300, 300, _T("▶"), _tcslen(_T("▶")));
				break;
			case EXIT:
				TextOut(hdc, 300, 340, _T("▶"), _tcslen(_T("▶")));
				break;
			}

			SelectObject(hdc, oldFont);
			DeleteObject(hFont);
			break;	//status의 break
		}
		EndPaint(hwnd, &ps);
		break;	//iMsg의 break
	}
	/*
	******************
	그리기 끝
	******************
	*/

	/*
	******************
	입력받기 시작
	******************
	*/

	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE)	//ESC키일경우 탈출
			DestroyWindow(hwnd);

		/*상태에 따라*/
		switch (status)
		{
		/*로고일때 입력이 있으면*/
		case LOGO:
			KillTimer(hwnd, LOGO_TIMER);		//로고의 애벌레 타이머 삭제
			status = SETTING;							//상태 변경
			InvalidateRgn(hwnd, NULL, TRUE);	//다시그리기
			break;	//status의 break

		/*세팅일때 입력이 있으면*/
		case SETTING:
			if (wParam == VK_RETURN)	//엔터면 설정 적용
			{
				cnt_circle = 2;	//원의 처음 개수
				x = 30;		//맨처음 x좌표
				for (i = cnt_circle - 1; i >= 0; i--)
				{
					coordinates[i][0] = x, coordinates[i][1] = 30;
					x += 20;
				}	//원들의 좌표설정

				status = COUNTDOWN;		//상태 변경

				SetTimer(hwnd, COUNTDOWN_TIMER, SEC, NULL);		//카운트타이머 설정
			}
			else if (wParam == VK_UP && difficulty != EASY)		//위키이면서 EASY가 아니면
				difficulty += 20;
			else if (wParam == VK_DOWN && difficulty != DIFFICULT)		//아래키이면서 DIFFICULT가 아니면
				difficulty -= 20;

			InvalidateRgn(hwnd, NULL, TRUE);
			break;	//status의 break

		case GAME:
			if (status == GAME && wParam == VK_LEFT || wParam == VK_RIGHT || wParam == VK_UP || wParam == VK_DOWN)
				direction = wParam;
			break;	//status의 break

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
			break;	//status의 break
		}
		break;	//iMsg의 break
	}

	/*
	******************
	타이머
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
				coordinates[0][1] -= 15;	//꾸물꾸물

			InvalidateRgn(hwnd, NULL, TRUE);		//다시그림

			if (coordinates[11][0] > rectView.right)
			{
				x = -240;		//맨처음 x좌표
				for (i = cnt_circle - 1; i >= 0; i--)
				{
					coordinates[i][0] = x, coordinates[i][1] = 100;
					x += 20;
				}	//원들의 좌표설정
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
			break;		//wParam의 break;

		case GAME_TIMER:
			for (i = 0; i < 10; i++)
			{
				if (coordinates[0][0] == items[i][0] && coordinates[0][1] == items[i][1])		//머리와 아이템이 만나면
				{
					cnt_circle++;		//원 개수 늘리기
					cnt_item--;	//아이템 개수 줄이기
					items[i][0] = 0, items[i][1] = 0;
					score += 100;	//스코어 추가
				}
			}
			score -= minus;		//각 난이도의 속도마다 minus만큼 깎임
			InvalidateRgn(hwnd, NULL, FALSE);		//다시그리기

			if (score == 0 || cnt_item == 0)	//스코어가 0이거나 아이템을 다 먹으면
				End_Game(hwnd, score, &status);

			switch (direction)		//방향에 따라 움직임
			{
			case VK_UP:
				if (coordinates[0][1] - 20 < rectView.top + 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
				}
				coordinates[0][1] -= 20;
				break;

			case VK_DOWN:
				if (coordinates[0][1] + 20 > rectView.bottom - 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
				}
				coordinates[0][1] += 20;
				break;

			case VK_LEFT:
				if (coordinates[0][0] - 20 < rectView.left + 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
				}
				coordinates[0][0] -= 20;
				break;

			case VK_RIGHT:
				if (coordinates[0][0] + 20 > rectView.right - 20)
					score = 0, End_Game(hwnd, score, &status);

				for (i = cnt_circle - 1; i > 0; i--)
				{
					coordinates[i][0] = coordinates[i - 1][0];	//x같음
					coordinates[i][1] = coordinates[i - 1][1];	//y같음
				}
				coordinates[0][0] += 20;
				break;	//direction의 break
			}
			break;	//wParam의 break
		}
		break;		//iMsg의 break
	}

	case WM_DESTROY:	//윈도우가 꺼졌을때
		PostQuitMessage(0);	//GetMessage함수가 0을 반환하게함
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//나머지는 커널에서 처리
}

void End_Game(HWND hwnd, int score, int* status)
{
	TCHAR buffer[100] = { 0, };
	KillTimer(hwnd, GAME_TIMER);
	if (score == 0)
		MessageBox(hwnd, _T("죽었어요 ㅠㅠ"), _T("종료"), MB_OK);
	else
	{
		wsprintf(buffer, _T("최총점수 : %d"), score);
		MessageBox(hwnd, buffer, _T("CLEAR!"), MB_OK);
	}

	*status = GAMEOVER;
	InvalidateRgn(hwnd, NULL, TRUE);
}

/*
*************************
시작할때 세팅
*************************
*/

void Start_Setting(HWND hwnd, int (*coordinates)[2], int (*items)[2], int *status, int *difficulty, int *menu, int *direction, int *score, int *cnt_circle, int *cnt_item)
{
	int i, x;

	*status = LOGO;			//상태는 로고
	*difficulty = EASY;		//난이도는 쉬움
	*menu = REGAME;		//메뉴는 다시시작

	*score = 10000;
	*direction = VK_RIGHT;

	*cnt_item = 10;
	*cnt_circle = 12;

	for (i = 0; i < 10; i++)	//아이템 랜덤배치
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