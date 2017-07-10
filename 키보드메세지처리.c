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
	WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);	//Ŀ�����
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)	//�޽��� ó���Լ�
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect = { 0, 0, 1000, 1000 };
	static TCHAR str[1000];
	static SIZE size = { 0, };
	static int count;

	switch (iMsg)
	{
	case WM_CREATE:	//�����찡 �����������
		CreateCaret(hwnd, NULL, 2, 15);	//CreateCaret(�ڵ鰪, ��Ʈ��(NULL�� ������), �ʺ�, ����);
		ShowCaret(hwnd);	//ĳ�� ���̱�
		count = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);	//����̽� ���ؽ�Ʈ Ȯ��
		GetTextExtentPoint(hdc, str, _tcslen(str), &size);	//���ڿ��� ũ�� �˱�
		DrawText(hdc, str, _tcslen(str), &rect, DT_TOP | DT_LEFT);		//���ڿ� ���
		SetCaretPos(size.cx, 20);	//ĳ�� ����
		EndPaint(hwnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == VK_BACK && count > 0) count --;	//�齺���̽� �� ���� �����
		else str[count++] = wParam;	//�齺���̽��� �ƴ϶�� �迭�� �ְ� count 1 ����
		str[count] = NULL;	//�迭�� �����ڴ� �ι���
		InvalidateRgn(hwnd, NULL, TRUE);		//InvalidateRgn(hwnd, NULL, ��λ���(TRUE) Ȥ�� �����κ�(FALSE)��);
		break;
	case WM_DESTROY:	//�����찡 ��������
		HideCaret(hwnd);		//ĳ�������
		DestroyCaret();		//ĳ�����ֱ�
		PostQuitMessage(0);	//GetMessage�Լ��� 0�� ��ȯ�ϰ���
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);	//�������� Ŀ�ο��� ó��
}