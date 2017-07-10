#include <Windows.h>
#include <tchar.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd;	//������ �ڵ鰪
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
		_T("Drawing Shape"),	//������ Ÿ��Ʋ�̸�
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
	HDC hdc;	//����̽� ���ؽ�Ʈ�� �ڵ鰪 �����ϴ°�
	PAINTSTRUCT ps;
	POINT point[10] = { {10, 150}, {250, 30}, {500, 150}, {350, 300}, {150, 300} };
	HPEN hPen;	//���� �ڵ鰪 ����
	HBRUSH hBrush;	//�귯���� �ڵ鰪 ����

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hPen = CreatePen(PS_SOLID, 3, RGB(0, 255, 255));	//hepn = CreatePen(�� ����, �� �β�, �� ��);
		hPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(0, 255, 255));	//hBrush = CreateSolidBrush(RGB(����, �ʷ�, �Ķ�));
		hBrush = (HBRUSH)SelectObject(hdc, hBrush);
		MoveToEx(hdc, 700, 0, NULL);		//MoveToEx(hdc, x��ǥ, y��ǥ, ���� NULL);
		LineTo(hdc, 700, 515);		//LineTo(hdc, x��ǥ, y��ǥ);
		Ellipse(hdc, 850, 200, 950, 300);	//Ellipse(hdc, x1, y1, x2, y2);
		Rectangle(hdc, 800, 400, 900, 500);		//Rectangle(hdc, x1, y1, x2, y2);
		Polygon(hdc, point, 5);		//Polygon(hdc, point����ü, �ٰ����� ����������);
		SelectObject(hdc, hPen);	//������Ʈ ����
		DeleteObject(hPen);		//����
		SelectObject(hdc, hBrush);	//������Ʈ ����
		DeleteObject(hBrush);		//����
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:	//�����찡 ��������
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}