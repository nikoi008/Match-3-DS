// SPDX-License-Identifier: CC0-1.0
// SPDX-FileContributor: NightFox & Co., 2009-2011

#include <stdio.h>
#include <nds.h>
#include <filesystem.h>
#include <nf_lib.h>

#define MAXSPRITES 8

int main(int argc, char **argv) {

    // 1. Basic NitroFS Initialization
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait...");
    swiWaitForVBlank();

    if (!nitroFSInit(NULL)) {
        printf("\n NitroFS failed to initialize!");
        while(1) swiWaitForVBlank();
    }
    NF_SetRootFolder("NITROFS");
    consoleClear(); 

    // 2. Initialize Systems
    // Top Screen (0) will handle Text
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitTextSys(0);
    NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);
    NF_CreateTextLayer(0, 0, 0, "normal"); // Layer 0 on Screen 0

    // Bottom Screen (1) will handle 3D Sprites
    NF_Set3D(1, 0); 
    NF_InitSpriteBuffers();
    NF_Init3dSpriteSys();

    // 3. Load and Transfer Assets
    NF_LoadSpriteGfx("sprite/blueball", 0, 64, 64);
    NF_LoadSpritePal("sprite/blueball", 0);
    NF_Vram3dSpriteGfx(0, 0, true);
    NF_Vram3dSpritePal(0, 0);

    // 4. Initialize Sprite Variables
    s16 x[MAXSPRITES];
    s16 y[MAXSPRITES];
    s16 ix = 4;
    s16 iy = 4;

    for (int n = 0; n < MAXSPRITES; n++) {
        x[n] = 128 - 32;
        y[n] = 96 - 32;
        NF_Create3dSprite(n, 0, 0, x[n], y[n]);
    }
    NF_Sort3dSprites();

    // 5. Initial Text Draw (Static text goes here)
    NF_WriteText(0, 0, 1, 1, "3D Sprites: Bottom Screen");
    NF_WriteText(0, 0, 1, 2, "Text System: Top Screen");
    NF_WriteText(0, 0, 10, 10, "Hola Mundo!");

    // Main Loop
    while (1) {
        scanKeys();
        u16 keys = keysHeld();
        touchPosition touchscreen;
        touchRead(&touchscreen);

        // Move sprites that follow the main one
        for (int n = MAXSPRITES - 1; n > 0; n--) {
            x[n] = x[n - 1];
            y[n] = y[n - 1];
            NF_Blend3dSprite(n, n + 1, 31 - (n * 4));
            NF_Move3dSprite(n, x[n], y[n]);
        }

        // Logic for the leader sprite
        if (keys & KEY_TOUCH) {
            x[0] = touchscreen.px - 32;
            y[0] = touchscreen.py - 32;
        } else {
            x[0] += ix;
            if ((x[0] < 0) || (x[0] > 192)) ix = -ix;
            y[0] += iy;
            if ((y[0] < 0) || (y[0] > 128)) iy = -iy;
        }
        NF_Move3dSprite(0, x[0], y[0]);

        // --- UPDATE SCREEN ---
        
        // Update Text
        NF_UpdateTextLayers(); 
        
        // Update 3D Scene
        NF_Draw3dSprites();
        glFlush(0);

        swiWaitForVBlank();
    }

    return 0;
}