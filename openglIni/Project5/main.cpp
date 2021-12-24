#pragma comment(linker, "/subsystem:windows")
#pragma comment(lib, "opengl32.lib")

#include <Windows.h>

#include "application.h"

class OpenGLApp : public Application
{
public:
    OpenGLApp() {}
    OpenGLApp(UINT width, UINT height) : Application(width, height) {}

    void Initialize()
    {
        Application::Initialize();
    }
    virtual void Run() override
    {
        MSG msg = { 0 };

        while (msg.message != WM_QUIT)
        {
            // If there are Window messages then process them.
            if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            // Otherwise, do animation/game stuff.
            else
            {
                Render();
            }
        }
    }
    virtual LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        switch (msg)
        {
        case WM_DESTROY:
        {
            if (hwnd == m_Hwnd)
                PostQuitMessage(0);
            break;
        }
        default:
            break;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    virtual void Render() override
    {
        glClearColor(1.0f, 1.0f, 0.0f, 0.0f);

        // Clear the screen and depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SwapBuffers(m_DC);
    }
};

int _stdcall wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow
)
{
    OpenGLApp theApp;
    theApp.Initialize();
    theApp.Run();

    return 0;
}