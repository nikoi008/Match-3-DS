#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>
#include "simplenfl.h"
#include <nf_lib.h>
int grid[8][8] = {0};
typedef enum{
    UP,
    LEFT,
    DOWN,
    RIGHT
}swipeDirection;
void drawGrid(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            NF_Set3dSpriteFrame((i * 8) + j, grid[i][j]);
        }
    }

}
void initGrid(){
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j++){
            grid[i][j] = rand() % 6;
        }
    }
}
void clearLines(){
    int counter = 1;
    int lastTile = -1;
    bool matching = false;
    int toclearRows[32][2];
    memset(toclearRows,-1,sizeof(toclearRows));
    int toClearRowsLength[32];
    memset(toClearRowsLength,-1,sizeof(toClearRowsLength));
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j++){
            if(j == 0){
                lastTile = grid[i][0];
                continue;
            }
            if(grid[i][j] == lastTile){
                counter++;
                matching = true;
            }else{
                matching = false
            }
            if(counter >= 3 && matching == false){
                
            }else{
                coutnter = 0;
            }
        
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
            if(gridY + 1 < 7) return;
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
     NF_LoadTextFont("fnt/default", "normal", 256, 256, 0); 
     NF_CreateTextLayer(0, 1, 0, "normal");
    NF_InitSpriteBuffers();    
    NF_Init3dSpriteSys();
    for(int i  = 0; i < 64; i ++){
        NF_LoadSpriteGfx("sprite/numbers", i, 16, 16);
    }
        
    NF_LoadSpritePal("sprite/numbers", 0);
    for(int i = 0; i < 64; i++){
        NF_Vram3dSpriteGfx(i, i, true);
    }
    NF_Vram3dSpritePal(0, 0);
    NF_Sort3dSprites();
    lcdSwap();
    int frame = 0;
    char mytext[32] = "";
    char bufferx[2] = "";
    char buffery[2] = "";
    initGrid();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            int x = 16 * i;
            int y = 16 * j;
            NF_Create3dSprite((i * 8) + j,(i * 8) + j,0,x,y);
            NF_Set3dSpriteFrame((i * 8) + j, grid[i][j]);
        }
    }


     NF_Sort3dSprites();
    int gridx = 0;
    int gridy = 0;
while(1){
    scanKeys();
    keys_held = keysHeld();

    if (keys_held & KEY_TOUCH)
            touchRead(&touch_pos);
            gridx =  (int)(touch_pos.px/ 16);
            gridy = touch_pos.py / 16;
    NF_WriteText(0, 1, 20, 5, mytext);
   
    
   

    
    snprintf(bufferx,sizeof(bufferx),"%d",gridx);
    snprintf(buffery,sizeof(buffery),"%d",gridy);
    NF_WriteText(0, 1, 20, 7, bufferx);
    NF_WriteText(0, 1, 20, 8, buffery);
    
    if(getSwipeGesture(SwipeLeft))  { strcpy(mytext, "LEFT  "); swipeBlocks(gridx + 1, gridy, SwipeLeft);}
    if(getSwipeGesture(SwipeUp))    { strcpy(mytext, "UP"); swipeBlocks( gridx, gridy + 1, SwipeUp); }
    if(getSwipeGesture(SwipeDown))  { strcpy(mytext, "DOWN  "); swipeBlocks( gridx, gridy - 1, SwipeDown); }
    if(getSwipeGesture(SwipeRight)) { strcpy(mytext, "RIGHT "); swipeBlocks(gridx - 1, gridy, SwipeRight);}

     NF_Draw3dSprites();

   

    glFlush(0);

    swiWaitForVBlank();
     NF_UpdateTextLayers();

    NF_Update3dSpritesGfx();
}
}
