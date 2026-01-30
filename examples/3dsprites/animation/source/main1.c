#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>


int main(){
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    srand(time(NULL));
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");
     NF_Set3D(1, 0);

    // Initialize 2D engine in the bottom
    NF_Set2D(0, 3);
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(0);       // Top screen
    NF_InitTiledBgSys(1);       // Bottom screen

    // Initialize 3D sprite system
    NF_InitSpriteBuffers();     // Initialize storage buffers
    NF_Init3dSpriteSys();

    // Load background files from NitroFS
    NF_LoadTiledBg("bg/nfl", "nfl", 256, 256);
    NF_LoadTiledBg("bg/bg3", "bg3", 256, 256);

    for(int i = 0; i < 64; i++){
        Nf_LoadSpriteGfx("sprite/numbers",i,16,16);
    }
    NF_LoadSpritePal("sprite/numbers", 0);
    for(int i = 0; i < 64; i++){
        NF_Vram3dSpriteGfx(i, i, true);
    }
    NF_CreateTiledBg(0, 3, "bg3");
    NF_CreateTiledBg(1, 3, "nfl");

    for(int i = 0; i < 64; i++){
        int x = 32 + (i * 16);
        int y = 16 * i;
        NF_Create3dSprite(i, i , 0, x, y);
        NF_Set3dSpriteFrame(i, rand() % 6);
    }
    NF_Sort3dSprites();

    while(1){
        NF_Draw3dSprites();

        // Tell the GPU to draw the scene and wait until it's done
        glFlush(0);

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Update textures of any 3D sprite with "keepframes == true"
        NF_Update3dSpritesGfx(); 
    }
}