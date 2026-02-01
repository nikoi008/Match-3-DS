#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>
#include <maxmod9.h>
#include "simplenfl.h"
#include <stdbool.h>
#include <nf_lib.h>
#include <maxmod9.h>
#include "soundbank.h"
int grid[8][8] = {0};
int gridTop[8][8] = {0};
typedef enum{
    UP,
    LEFT,
    DOWN,
    RIGHT
}swipeDirection;
void drawGrid(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(grid[i][j] == -1){
                NF_Set3dSpriteFrame((i* 8)+ j,0);
            }
            NF_Set3dSpriteFrame((i * 8) + j, grid[i][j] + 1);
        }
    }

}

void initGridTop(){
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j++){
            gridTop[i][j] = rand() % 6;
        }
    }
}
void initGridBottom(){
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j++){
            grid[i][j] = rand() % 6;
        }
    }
}
typedef enum{
    ROW,COLUMN
}lineClearType;
typedef struct{
    lineClearType type;
    int x;
    int y;
    int length;
}lineClear;
int matchGrid[8][8];
bool findMatches(){
    bool found = false;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            matchGrid[i][j] = false;
        }
    }
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8 - 2; y++){
        int currentTile = grid[y][x];
        if(currentTile == grid[y + 1][x] && currentTile == grid[y + 2][x] && currentTile != -1){
            matchGrid[y][x] = true;
            matchGrid[y + 1][x] = true;
            matchGrid[y + 2][x] = true;
            found = true;
        }
        }
    }
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8 - 2; x++){
        int currentTile = grid[y][x];
        if(currentTile == grid[y][x + 1] && currentTile == grid[y][x + 2] && currentTile != -1){
            matchGrid[y][x] = true;
            matchGrid[y][x + 1] = true;
            matchGrid[y][x + 2] = true;
            found = true;
            }
        }
    }
    return found;
}
void applyMatches(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(matchGrid[i][j] == true){
                grid[i][j] = -1;
            }
        }
    }
}
void dropPieces(){
    for(int i = 0; i < 8; i++){
        for(int j =0; j < 8; j++){

        }
    }
}
void swipeBlocks(int gridX, int gridY, Swipe swipeDir){
    if(gridX > 7 || gridX < 0 || gridY > 7 || gridX < 0) return;
    
    switch(swipeDir){
        case(SwipeLeft):{
            if(gridX - 1 < 0) return;
            
            int temp = grid[gridX][gridY];
            grid[gridX][gridY] = grid[gridX -1 ][gridY];
            grid[gridX - 1][gridY] = temp;

            break;
        }
        case(SwipeRight):{
            if(gridX + 1 > 7) return;
            int temp = grid[gridX][gridY];
            grid[gridX][gridY] = grid[gridX + 1 ][gridY];
            grid[gridX + 1][gridY] = temp;
            break;
        }
        case(SwipeUp):{
            if(gridY - 1 < 0 ) return;
            int temp = grid[gridX][gridY];
            grid[gridX][gridY] = grid[gridX][gridY - 1];
            grid[gridX][gridY - 1] = temp;
            break;
        }
        case(SwipeDown):{
            if(gridY + 1 > 7) return;
            int temp = grid[gridX][gridY];
            grid[gridX][gridY] = grid[gridX][gridY + 1];
            grid[gridX][gridY + 1] = temp;
            break;
        }
    }
    drawGrid();
}
 
int main(){
    uint16_t keys_held;
    touchPosition touch_pos;
    srand(time(NULL));
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    swiWaitForVBlank();
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");
    NF_Set3D(0, 0);
    NF_Set2D(1, 0);
    NF_InitTiledBgBuffers();    
    NF_InitTiledBgSys(0);       
    NF_InitTiledBgSys(1);       
    NF_InitTextSys(0);
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);
    NF_InitSpriteSys(1);
    NF_LoadTextFont("fnt/default", "normal", 256, 256, 0); 
    NF_CreateTextLayer(0, 1, 0, "normal");
    NF_InitSpriteBuffers();    
    NF_Init3dSpriteSys();
    mmInitDefault("nitro:/soundbank.bin");
    mmLoadEffect(SFX_FIRE_EXPLOSION);
    soundEnable();
    initGridTop();
    int semitones = 0;
    //3d sprites
    const uint16_t combo_pitches[] = {1024, 1085, 1149, 1218, 1290, 1366, 1448, 1534, 1625, 1722, 1825, 1933, 2048};
    for(int i  = 0; i < 64; i ++){
        NF_LoadSpriteGfx("sprite/numbers", i, 16, 16);
        
    }
        
    NF_LoadSpritePal("sprite/numbers", 0);
    for(int i = 0; i < 64; i++){
        NF_Vram3dSpriteGfx(i, i, true);
        

    }
    NF_Vram3dSpritePal(0, 0);
    NF_Sort3dSprites();

    //2d sprites
    for(int i = 64; i < 128; i++ ){
        NF_LoadSpriteGfx("sprite/character", i, 32, 32);//todo change back to 16x16 and use NF_SpriteRotScale  examples/graphics/spriteaffine/
    }
    NF_LoadSpritePal("sprite/character", 1);
    NF_VramSpritePal(1, 1, 1);
    for(int i = 64; i < 128; i++){
        NF_VramSpriteGfx(1, i, i, true); 
    }




    lcdSwap();

    char mytext[32] = "";
    char bufferx[2] = "";
    char buffery[2] = "";
    //drawing
    initGridBottom();
    initGridTop();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int x = 38 + ( 20 * i);
            int y = 16 + ( 20 * j);
            NF_Create3dSprite((i * 8) + j,(i * 8) + j,0,x,y);
            NF_Set3dSpriteFrame((i * 8) + j, grid[i][j]);
            NF_Scale3dSprite((i * 8) + j, 80, 80);
        }
    }

    
     NF_Sort3dSprites();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int x = 38 + ( 20 * i);
            int y = 16 + ( 20 * j);
            NF_CreateSprite(1,((i * 8) + j) + 64,((i * 8) + j) + 64,1,x,y);
            NF_SpriteFrame(1, ((i * 8) + j) + 64, gridTop[i][j]);
        }
    }
    int gridx = 0;
    int gridy = 0;
while(1){
    
    scanKeys();
    keys_held = keysHeld();

    if (keys_held & KEY_TOUCH)
            touchRead(&touch_pos);
            gridx =  (int)((touch_pos.px - 38)/ 20);
            gridy = (int)((touch_pos.py - 16)/ 20);
    NF_WriteText(0, 1, 20, 5, mytext);
   
    
   

    
    snprintf(bufferx,sizeof(bufferx),"%d",gridx);
    snprintf(buffery,sizeof(buffery),"%d",gridy);
    NF_WriteText(0, 1, 20, 7, bufferx);
    NF_WriteText(0, 1, 20, 8, buffery);
    
    if(getSwipeGesture(SwipeLeft))  { strcpy(mytext, "LEFT  "); swipeBlocks(gridx + 1, gridy, SwipeLeft); }
    if(getSwipeGesture(SwipeUp))    { strcpy(mytext, "UP"); swipeBlocks( gridx, gridy + 1, SwipeUp); }
    if(getSwipeGesture(SwipeDown))  { strcpy(mytext, "DOWN  "); swipeBlocks( gridx, gridy - 1, SwipeDown); }
    if(getSwipeGesture(SwipeRight)) { strcpy(mytext, "RIGHT "); swipeBlocks(gridx - 1, gridy, SwipeRight);}
    if(findMatches()){
        mm_sfxhand h = mmEffect(SFX_FIRE_EXPLOSION);
        mmEffectRate(h, combo_pitches[semitones]);

        semitones = (semitones + 1) % 13;
    }
    applyMatches();
    drawGrid();
     NF_Draw3dSprites();

   

    glFlush(0);
    NF_SpriteOamSet(1);
    swiWaitForVBlank();
    oamUpdate(&oamMain);
    oamUpdate(&oamSub);
     NF_UpdateTextLayers();

    NF_Update3dSpritesGfx();
}
}
