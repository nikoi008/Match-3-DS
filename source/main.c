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
#define GRID_ROWS 16
#define GRID_COLS 8
int grid[GRID_ROWS][GRID_COLS] = {0};



void initGrid(){
    for(int i = 0; i < GRID_ROWS;i++){
        for(int j = 0; j < GRID_COLS;j++){
            grid[i][j] = rand() % 6;
        }
    }
}

void swipeBlocks(int gridX, int gridY, Swipe swipeDir){
    
    if(gridX > 7 || gridX < 0 || gridY > 15 || gridY < 7) return;
    
    switch(swipeDir){
        case(SwipeLeft):{ 
            if(gridX - 1 < 0) return; 
            
            int temp = grid[gridY][gridX];
            
            grid[gridY][gridX] = grid[gridY][gridX - 1]; 
            grid[gridY][gridX - 1] = temp;
            break;
        }
        case(SwipeRight):{
            if(gridX + 1 > 7) return; 
            int temp = grid[gridY][gridX];
            grid[gridY][gridX] = grid[gridY][gridX + 1];
            grid[gridY][gridX + 1] = temp;
            break;
        }
        case(SwipeUp):{
            if(gridY - 1 < 0 ) return; 
            
            int temp = grid[gridY][gridX];
            grid[gridY][gridX] = grid[gridY - 1][gridX];
            grid[gridY - 1][gridX] = temp;
            break;
        }
        case(SwipeDown):{
            if(gridY + 1 > 15) return; 
            
            int temp = grid[gridY][gridX]; 
            grid[gridY][gridX] = grid[gridY + 1][gridX];
            grid[gridY + 1][gridX] = temp;
            break;
        }
    }
}



void drawGridTop(){
    for(int i = 0; i < 8; i++){     
        for(int j = 0; j < 8; j++){ 
            
            int tileValue = grid[j][i]; 

            if(tileValue == -1){
                NF_SpriteFrame(1,((i * 8) + j) + 64, 0);
            }else{
                NF_SpriteFrame(1, ((i * 8) + j) + 64, tileValue + 1);
            }
        }
    }
}
bool matchGrid[8][8];

bool findMatches(){
    bool found = false;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            matchGrid[i][j] = false;
        }
    }
    for(int x = 0; x < 8; x++){
        for(int y = 8; y < 16 - 2; y++){
        int currentTile = grid[y][x];
        if(currentTile == grid[y + 1][x] && currentTile == grid[y + 2][x] && currentTile != -1){
            int matchGridY = y - 8;
            matchGrid[matchGridY][x] = true;
            matchGrid[matchGridY + 1][x] = true;
            matchGrid[matchGridY + 2][x] = true;
            found = true;
        }
        }
    }
    for(int y = 8; y < 16; y++){
        for(int x = 0; x < 8 - 2; x++){
        int currentTile = grid[y][x];
        if(currentTile == grid[y][x + 1] && currentTile == grid[y][x + 2] && currentTile != -1){
            int matchGridY = y - 8;
            matchGrid[matchGridY][x] = true;
            matchGrid[matchGridY][x + 1] = true;
            matchGrid[matchGridY][x + 2] = true;
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
                grid[i + 8][j] = -1;
            }
        }
    }
}
void applyGravity(){
    for(int row = 15; row >= 0; row--){
        for(int col = 7; col >= 0; col--){
            if(grid[row][col] != -1 && grid[row + 1][col ] == -1){
                
                    grid[row + 1][col ] =grid[row][col];
                    grid[row][col] = -1;
            }
        }
    }
}

void fillEmptySpaces(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(grid[i][j] == -1){
                grid[i][j] = rand() % 6;
            }
        }
    }
}
void drawGridBottom(){
    for(int i = 0; i < 8; i++){     
        for(int j = 0; j < 8; j++){
            

            int tileValue = grid[j + 8][i];

            if(tileValue == -1){
                NF_Set3dSpriteFrame((i* 8)+ j, 0);   
            }else{
                NF_Set3dSpriteFrame((i * 8) + j, tileValue + 1); 
            }
        }
    }
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
    initGrid(); 
    int semitones = 0;
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

    for(int i = 64; i < 128; i++ ){
        NF_LoadSpriteGfx("sprite/character", i, 32, 32);
    }
    NF_LoadSpritePal("sprite/character", 1); 
    NF_VramSpritePal(1, 1, 1);
    for(int i = 64; i < 128; i++){ 
        NF_VramSpriteGfx(1, i, i, true); 
    }
    
    lcdSwap();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int x = 38 + ( 20 * i); 
            int y = 16 + ( 20 * j);
            NF_Create3dSprite((i * 8) + j,(i * 8) + j,0,x,y);
            NF_Set3dSpriteFrame((i * 8) + j, grid[i + 8][j]);
            NF_Scale3dSprite((i * 8) + j, 80, 80);
        }
    }
    
    NF_Sort3dSprites();


    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int x = 38 + ( 20 * i);
            int y = 16 + ( 20 * j);
            NF_CreateSprite(1,((i * 8) + j) + 64,((i * 8) + j) + 64,1,x,y);
            NF_SpriteFrame(1, ((i * 8) + j) + 64, grid[j][i]);
        }
    }
    int gridx = 0;
    int gridy = 0;
        char mytext[32] = "";
    char bufferx[4] = "";
    char buffery[4] = "";
    bool touched = false;
    uint16_t keys_up;
    while(1){
        scanKeys();
        keys_held = keysHeld();
        keys_up = keysUp();

        if (keys_held & KEY_TOUCH && !touched){
                touchRead(&touch_pos);
                gridx = (int)((touch_pos.px - 38)/ 20);
                gridy = 8 + (int)((touch_pos.py - 16)/ 20);
                touched = true;
        }
        if(keys_up & KEY_TOUCH && touched){
            touched = false;
        }
        
        snprintf(bufferx,sizeof(bufferx),"%d",gridx);
        snprintf(buffery,sizeof(buffery),"%d",gridy);
        NF_WriteText(0, 1, 24, 7, bufferx);
        NF_WriteText(0, 1, 24, 8, buffery);
        NF_WriteText(0, 1, 20, 5, mytext);

             NF_Draw3dSprites();
 if(getSwipeGesture(SwipeLeft))  { strcpy(mytext, "LEFT  "); swipeBlocks(gridx, gridy, SwipeLeft); }
    if(getSwipeGesture(SwipeUp))    { strcpy(mytext, "UP"); swipeBlocks( gridx, gridy , SwipeUp); }
    if(getSwipeGesture(SwipeDown))  { strcpy(mytext, "DOWN  "); swipeBlocks( gridx, gridy, SwipeDown); }
    if(getSwipeGesture(SwipeRight)) { strcpy(mytext, "RIGHT "); swipeBlocks(gridx, gridy, SwipeRight);}
        findMatches();
        applyMatches();
        applyGravity();
        for(int i = 0; i < 2;i++) {
            fillEmptySpaces();
            applyGravity();
        }
        drawGridTop();
        drawGridBottom();
        glFlush(0);
        NF_SpriteOamSet(1);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
        NF_UpdateTextLayers();

        NF_Update3dSpritesGfx();
    }
} 