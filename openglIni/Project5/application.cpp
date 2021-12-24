#include "application.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Application::GetApp()->WndProc(hwnd, message, wParam, lParam);
}

Application* Application::m_TheApp = nullptr;

Application::Application(UINT width, UINT height, BOOL vsyn) :
	m_Width(width),
	m_Height(height),
	m_VSYN(vsyn)
{
	if (m_TheApp == nullptr)
		m_TheApp = this;
}

Application::~Application()
{
	if (m_RC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_RC);
	}
	if (m_DC)
	{
		ReleaseDC(m_Hwnd, m_DC);
	}
}

void Application::Initialize()
{
	WNDCLASSEX wc;
	HWND hwnd;
	HDC dc;
	PIXELFORMATDESCRIPTOR pf;
	HGLRC rc;
	WCHAR versionString[128];

	int attributeListInt[19];
	int pixelFormat[1];
	unsigned int formatCount;
	int attributeList[5];
	m_Hinstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = ::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_Hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Temporary";
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, L"Temporary", L"Temporary", WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480, NULL, NULL, m_Hinstance, NULL);

	assert(hwnd);
	ShowWindow(hwnd, SW_HIDE);

	dc = GetDC(hwnd);
	assert(dc);
	assert(SetPixelFormat(dc, 1, &pf));
	rc = wglCreateContext(dc);
	assert(rc);
	assert(wglMakeCurrent(dc, rc));

	assert(gladLoadWGL(dc));
	MessageBoxW(nullptr, L"WGL Version 1.0 Loaded", L"", MB_OK);
	assert(gladLoadGL());
	wsprintf(versionString, L"OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);
	MessageBoxW(nullptr, versionString, L"", MB_OK);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(rc);
	ReleaseDC(hwnd, dc);
	DestroyWindow(hwnd);

	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, L"Temporary", L"Temporary", WS_OVERLAPPEDWINDOW,
		0, 0, 640, 480, NULL, NULL, m_Hinstance, NULL);
	assert(m_Hwnd);
	ShowWindow(m_Hwnd, SW_SHOW);
	UpdateWindow(m_Hwnd);

	m_DC = GetDC(m_Hwnd);
	assert(m_DC);
	attributeListInt[0] = WGL_SUPPORT_OPENGL_ARB;
	attributeListInt[1] = TRUE;

	// Support for rendering to a window.
	attributeListInt[2] = WGL_DRAW_TO_WINDOW_ARB;
	attributeListInt[3] = TRUE;

	// Support for hardware acceleration.
	attributeListInt[4] = WGL_ACCELERATION_ARB;
	attributeListInt[5] = WGL_FULL_ACCELERATION_ARB;

	// Support for 24bit color.
	attributeListInt[6] = WGL_COLOR_BITS_ARB;
	attributeListInt[7] = 24;

	// Support for 24 bit depth buffer.
	attributeListInt[8] = WGL_DEPTH_BITS_ARB;
	attributeListInt[9] = 24;

	// Support for double buffer.
	attributeListInt[10] = WGL_DOUBLE_BUFFER_ARB;
	attributeListInt[11] = TRUE;

	// Support for swapping front and back buffer.
	attributeListInt[12] = WGL_SWAP_METHOD_ARB;
	attributeListInt[13] = WGL_SWAP_EXCHANGE_ARB;

	// Support for the RGBA pixel type.
	attributeListInt[14] = WGL_PIXEL_TYPE_ARB;
	attributeListInt[15] = WGL_TYPE_RGBA_ARB;

	// Support for a 8 bit stencil buffer.
	attributeListInt[16] = WGL_STENCIL_BITS_ARB;
	attributeListInt[17] = 8;

	// Null terminate the attribute list.
	attributeListInt[18] = 0;
	assert(wglChoosePixelFormatARB(m_DC, attributeListInt, NULL, 1, pixelFormat, &formatCount));
	assert(SetPixelFormat(m_DC, pixelFormat[0], &pf));

	attributeList[0] = WGL_CONTEXT_MAJOR_VERSION_ARB;
	attributeList[1] = 4;
	attributeList[2] = WGL_CONTEXT_MINOR_VERSION_ARB;
	attributeList[3] = 0;

	// Null terminate the attribute list.
	attributeList[4] = 0;
	m_RC = wglCreateContextAttribsARB(m_DC, 0, attributeList);
	assert(m_RC);
	assert(wglMakeCurrent(m_DC, m_RC));

	glClearDepth(1.0f);
	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	// Set the polygon winding to front facing for the left handed system.
	glFrontFace(GL_CW);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (m_VSYN)
	{
		assert(wglSwapIntervalEXT(1));
	}
	else
	{
		assert(wglSwapIntervalEXT(0));
	}
}

Application* Application::GetApp()
{
	return m_TheApp;
}
