#include<windows.h>
#include<windowsx.h>
#include<stdlib.h>
#include<time.h>
#include<d3d9.h>
#include<d3dx9.h>
#include "dxgraphics.h";
#include "game.h";


ATOM MyRegisterClass(HINSTANCE);
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// Helper function to set up window properties
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WinProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = APPTITLE;
    wc.hIconSm = NULL;

    return RegisterClassEx(&wc);
}

// The window callback function
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
            if (d3ddev != NULL)
            {
                d3ddev->Release();
            }
            if (d3d != NULL)
            {
                d3d->Release();
            }
            Game_End(hWnd);
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;

    MyRegisterClass(hInstance);

    HWND hWnd;

    hWnd = CreateWindow(
        APPTITLE,
        APPTITLE,
        FULLSCREEN ? WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP : WS_OVERLAPPED,
        640, // initial x
        360, // initial y
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
        );

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if (!Init_Direct3D(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN))
    {
        return 0;
    }

    if (!Game_Init(hWnd))
    {
        return 0;
    }

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Game_Frame(hWnd);
        }
    }
    
    return msg.wParam;
}