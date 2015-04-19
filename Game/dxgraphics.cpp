#include<d3d9.h>
#include<d3dx9.h>
#include "dxgraphics.h"

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;

int Init_Direct3D(HWND hWnd, int width, int height, bool fullscreen)
{
    HRESULT result;

    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d == NULL)
    {
        MessageBox(hWnd, "Error initializing Direct3D", "Error", MB_OK);
        return 0;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = !fullscreen;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.hDeviceWindow = hWnd;

    d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp,
        &d3ddev
        );

    if (d3ddev == NULL)
    {
        MessageBox(hWnd, "Error creating Direct3D Device", "Error", MB_OK);
        return 0;
    }

    // draw black
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    // set back buffer pointer
    d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

    d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // don't remove backward facing triangles 

    return 1;
}


LPDIRECT3DSURFACE9 LoadSurface(char *filename, D3DCOLOR transcolor)
{
    LPDIRECT3DSURFACE9 image = NULL;
    D3DXIMAGE_INFO info;
    HRESULT result;

    result = D3DXGetImageInfoFromFile(filename, &info);
    if (result != D3D_OK)
    {
        return NULL;
    }

    result = d3ddev->CreateOffscreenPlainSurface(
        info.Width,
        info.Height,
        D3DFMT_X8R8G8B8,
        D3DPOOL_DEFAULT,
        &image,
        NULL
        );

    if (FAILED(result))
        return NULL;

    result = D3DXLoadSurfaceFromFile(
        image,
        NULL,
        NULL,
        filename,
        NULL,
        D3DX_DEFAULT,
        0,
        NULL
        );

    if (FAILED(result))
        return 0;

    return image;
}


LPDIRECT3DTEXTURE9 LoadTexture(char *filename, D3DCOLOR transcolor)
{
    LPDIRECT3DTEXTURE9 texture = NULL;
    D3DXIMAGE_INFO info;
    HRESULT result;

    result = D3DXGetImageInfoFromFile(filename, &info);
    if (FAILED(result))
    {
        return NULL;
    }

    D3DXCreateTextureFromFileEx(
        d3ddev,
        filename,
        info.Width,
        info.Height,
        1,
        D3DPOOL_DEFAULT,
        D3DFMT_UNKNOWN,
        D3DPOOL_DEFAULT,
        D3DX_DEFAULT,
        D3DX_DEFAULT,
        transcolor,
        &info,
        NULL,
        &texture
        );

    if (FAILED(result))
    {
        return NULL;
    }

    return texture;
}