#include "game.h"

LPDIRECT3DSURFACE9 background;
LPDIRECT3DTEXTURE9 caveman_image;
SPRITE caveman;
LPD3DXSPRITE sprite_handler;

HRESULT result;

long start = GetTickCount();


int Game_Init(HWND hWnd)
{
    char s[20];
    int n;

    srand(time(NULL));

    result = D3DXCreateSprite(d3ddev, &sprite_handler);
    if (FAILED(result))
    {
        return 0;
    }
    
    caveman_image = LoadTexture("caveman.bmp", D3DCOLOR_XRGB(255, 0, 255));

    caveman.x = 100;
    caveman.y = 180;
    caveman.width = 50;
    caveman.height = 64;
    caveman.curframe = 1;
    caveman.lastframe = 8;
    caveman.animdelay = 3;
    caveman.animcount = 0;
    caveman.movex = 5;
    caveman.movey = 0;

    background = LoadSurface("background.bmp", NULL);

    return 1;
}


void StepCaveman()
{
    if (++caveman.animcount > caveman.animdelay)
    {
        caveman.animcount = 0;
        int lastframe = caveman.jumpingFrame / 2 > 0 ? 12 : caveman.lastframe;
        if (++caveman.curframe > lastframe)
        {
            caveman.curframe = 1;
        }
    }
}


void Game_Frame(HWND hWnd)
{
    if (d3ddev == NULL)
        return;

    if (GetTickCount() - start >= 30)
    {
        start = GetTickCount();

        // move x
        if (KEY_DOWN(VK_LEFT))
        {
            caveman.faceLeft = TRUE;
            caveman.x -= caveman.movex;
            if (caveman.jumpingFrame == 0)
            {
                StepCaveman();
            }
        }
        else if (KEY_DOWN(VK_RIGHT))
        {
            caveman.faceLeft = FALSE;
            caveman.x += caveman.movex;
            if (caveman.jumpingFrame == 0)
            {
                StepCaveman();
            }
        }
           
        // jump!!
        if (caveman.jumpingFrame > 0)
        {
            caveman.jumpingFrame--;
            StepCaveman();
        }
        if (caveman.jumpingFrame == 0 && KEY_DOWN(VK_SPACE))
        {
            caveman.jumpingFrame = 8;
            caveman.curframe = 9;
            caveman.y -= 15;
        }
        if (caveman.jumpingFrame == 6)
        {
            caveman.y -= 15;
        }
        else if (caveman.jumpingFrame == 4)
        {
            caveman.y += 15;
        }
        else if (caveman.jumpingFrame == 2)
        {
            caveman.y += 15;
        }

        // warp screen boundary
        if (caveman.x > SCREEN_WIDTH - caveman.width)
        {
            caveman.x = 0;
        }
        if (caveman.x < 0)
        {
            caveman.x = SCREEN_WIDTH - caveman.width;
        }
    }
    
    if (d3ddev->BeginScene())
    {
        d3ddev->StretchRect(background, NULL, backbuffer, NULL, D3DTEXF_NONE);

        sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

        // source rect
        RECT srcRect;
        int columns = 8;
        srcRect.left = (caveman.curframe % columns) * caveman.width;
        srcRect.top = (caveman.curframe / columns) * caveman.height;
        srcRect.right = srcRect.left + caveman.width;
        srcRect.bottom = srcRect.top + caveman.height;

        //transform
        D3DXVECTOR2 position(caveman.x, caveman.y);
        D3DXVECTOR2 spriteCentre(caveman.width / 2, 0);
        float rotation = 0.0f;
        D3DXVECTOR2 scaling(caveman.faceLeft ? -1 : 1, 1);
        D3DXMATRIX mat;
        D3DXMatrixTransformation2D(&mat, &spriteCentre, 0, &scaling, &spriteCentre, rotation, &position);
        sprite_handler->SetTransform(&mat);
        sprite_handler->Draw(
            caveman_image,
            &srcRect,
            NULL,
            NULL,
            D3DCOLOR_XRGB(255, 255, 255)
            );
        sprite_handler->End();

        d3ddev->EndScene();
    }

    d3ddev->Present(NULL, NULL, NULL, NULL);

    if (KEY_DOWN(VK_ESCAPE))
    {
        PostMessage(hWnd, WM_DESTROY, 0, 0);
    }
}


void Game_End(HWND hWnd)
{
    if (caveman_image != NULL)
        caveman_image->Release();

    if (background != NULL)
        background->Release();

    if (sprite_handler != NULL)
        sprite_handler->Release();
}