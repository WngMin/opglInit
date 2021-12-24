#pragma once
#include <glad/glad.h>
#include <glad/glad_wgl.h>

#include <Windows.h>

#include "appUtil.h"

class Application
{
public:
	Application(UINT width = 640, UINT height = 480, BOOL vsyn = false);

	virtual ~Application();

	virtual void Run() = 0;

	virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual void Render() = 0;

	void Initialize();

	static Application* GetApp();

protected:
	static Application* m_TheApp;

	HINSTANCE m_Hinstance;
	HWND m_Hwnd;
	UINT m_Width;
	UINT m_Height;

	BOOL m_VSYN;
	HDC m_DC;
	HGLRC m_RC;
};