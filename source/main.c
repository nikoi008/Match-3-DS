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
typedef enum
{
    START,
    SELECT_SAVE,
    INIT_MATCH,
    GAME_MATCH,
    INIT_FARM,
    GAME_FARM,
    PAUSE_MATCH,
    PAUSE_FARM,
}States;
typedef enum
{
    NEUTRAL,
    UP,
    LEFT,
    DOWN,
    RIGHT
}directions;
int grid[GRID_ROWS][GRID_COLS] = {0};
int spriteIdGrid[GRID_ROWS][GRID_COLS];



void initGrid(){
    for(int i = 0; i < GRID_ROWS;i++)
    {
        for(int j = 0; j < GRID_COLS;j++)
        {
            grid[i][j] = rand() % 7;
        }
    }
}
int totalMatches = 0;
bool matchGrid[8][8];
bool findMatches()
{
    totalMatches = 0;
    bool found = false;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            matchGrid[i][j] = false;
        }
    }
    for(int x = 0; x < 8; x++){
        for(int y = 8; y < 16 - 2; y++){
        int currentTile = grid[y][x];
        if(currentTile == grid[y + 1][x] && currentTile == grid[y + 2][x] && currentTile != -1)
            {
                int matchGridY = y - 8;
                matchGrid[matchGridY][x] = true;
                matchGrid[matchGridY + 1][x] = true;
                matchGrid[matchGridY + 2][x] = true;
                found = true;
                totalMatches += 1;
            }
        }
    }
    for(int y = 8; y < 16; y++)
    {
        for(int x = 0; x < 8 - 2; x++)
        {
            int currentTile = grid[y][x];
            if(currentTile == grid[y][x + 1] && currentTile == grid[y][x + 2] && currentTile != -1)
            {
                int matchGridY = y - 8;
                matchGrid[matchGridY][x] = true;
                matchGrid[matchGridY][x + 1] = true;
                matchGrid[matchGridY][x + 2] = true;
                found = true;
                totalMatches += 1;
            }   
        }
    }
    return found;
}

void animateSpriteSwipe(Swipe SwipeDir, int gridX, int gridY) {
    int targetX = gridX;
    int targetY = gridY;

    if (SwipeDir == SwipeLeft)  targetX--;
    if (SwipeDir == SwipeRight) targetX++;
    if (SwipeDir == SwipeUp)    targetY--;
    if (SwipeDir == SwipeDown)  targetY++;

    if (targetX < 0 || targetX > 7 || targetY < 8 || targetY > 15) return;

    int sprite1 = spriteIdGrid[gridY][gridX];
    int sprite2 = spriteIdGrid[targetY][targetX];

    int x1Start = 34 + (gridX * 24);
    int y1Start = ((gridY - 8) * 24) + 2;

    int x2Start = 34 + (targetX * 24);
    int y2Start = ((targetY - 8) * 24) + 2;

    int frames = 15;
    for (int i = 1; i <= frames; i++)
    {
        int currentX1 = x1Start + ((x2Start - x1Start) * i / frames);
        int currentY1 = y1Start + ((y2Start - y1Start) * i / frames);
        int currentX2 = x2Start + ((x1Start - x2Start) * i / frames);
        int currentY2 = y2Start + ((y1Start - y2Start) * i / frames);

        NF_Move3dSprite(sprite1, currentX1, currentY1);
        NF_Move3dSprite(sprite2, currentX2, currentY2);
        NF_Draw3dSprites();       
        glFlush(0);               
        swiWaitForVBlank();       
        NF_Update3dSpritesGfx();  
    }
}
    
bool swipeBlocks(int gridX, int gridY, Swipe swipeDir){
    
    if(gridX > 7 || gridX < 0 || gridY > 15 || gridY < 7) return false;
    
    switch(swipeDir){
        case(SwipeLeft):
        { 
            if(gridX - 1 < 0) return false; 
            int temp = grid[gridY][gridX];
            grid[gridY][gridX] = grid[gridY][gridX - 1]; 
            grid[gridY][gridX - 1] = temp;
            animateSpriteSwipe(SwipeLeft,gridX,gridY);
            if(findMatches())
            { 
                break;
            }
            else
            {

                grid[gridY][gridX - 1] = grid[gridY][gridX];
                grid[gridY][gridX] = temp;
                animateSpriteSwipe(SwipeRight,gridX - 1,gridY);
            }
            break;
        }
        case(SwipeRight):
        {
            if(gridX + 1 > 7) return false; 
            int temp = grid[gridY][gridX];
            grid[gridY][gridX] = grid[gridY][gridX + 1];
            grid[gridY][gridX + 1] = temp;
            animateSpriteSwipe(SwipeRight,gridX,gridY);
            if(findMatches()){ 
                break ;
            }
            else
            { 
                grid[gridY][gridX + 1] = grid[gridY][gridX];
                grid[gridY][gridX] = temp;
                animateSpriteSwipe(SwipeLeft,gridX + 1,gridY);
            }
            break;
        }
        case(SwipeUp):
        {
            if(gridY - 1 < 0 ) return false; 
            int temp = grid[gridY][gridX];
            grid[gridY][gridX] = grid[gridY - 1][gridX];
            grid[gridY - 1][gridX] = temp;
            animateSpriteSwipe(SwipeUp,gridX,gridY);
            if(findMatches())
            { 
                break;
            }
            else
            {
                grid[gridY - 1][gridX] = grid[gridY][gridX];
                grid[gridY][gridX] = temp;
                animateSpriteSwipe(SwipeDown,gridX,gridY - 1);
            }
            break;
        }
        case(SwipeDown):
        {
            if(gridY + 1 > 15) return false; 
            int temp = grid[gridY][gridX]; 
            grid[gridY][gridX] = grid[gridY + 1][gridX];
            grid[gridY + 1][gridX] = temp;
            animateSpriteSwipe(SwipeDown,gridX,gridY);
            if(findMatches())
            { 
                break;
            }
            else
            {
                grid[gridY + 1][gridX] = grid[gridY][gridX];
                grid[gridY][gridX] = temp;
                animateSpriteSwipe(SwipeUp,gridX,gridY + 1);
            }
            break;
        }
    }
    return true;
}
void drawGridBottom()
{
    for(int i = 0; i < 8; i++)
    {     
        for(int j = 0; j < 8; j++)
        {
            int tileValue = grid[j + 8][i];
            int spriteID = spriteIdGrid[j + 8][i]; 

       
            NF_Move3dSprite(spriteID, 34 + (i * 24), (j * 24) +2 );

            if(tileValue == -1)
            {
                NF_Set3dSpriteFrame(spriteID, 0);   
            }
            else
            {
                NF_Set3dSpriteFrame(spriteID, tileValue); 
            }
        }
    }
}

  


void applyMatches()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(matchGrid[i][j] == true)
            {
                grid[i + 8][j] = -1;
                
            }
        }
    }
}
void applyGravity()
{
    for(int row = 15; row >= 0; row--)
    {
        for(int col = 7; col >= 0; col--)
        {
            if(grid[row][col] != -1 && grid[row + 1][col ] == -1)
            {
                
                    grid[row + 1][col ] =grid[row][col];
                    grid[row][col] = -1;
            }
        }
    }
}

void fillEmptySpaces()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(grid[i][j] == -1)
            {
                grid[i][j] = rand() % 7;
            }
        }
    }
}
void drawGridTop()
{
    for(int i = 0; i < 8; i++) {     
        for(int j = 0; j < 8; j++) { 
            int tileValue = grid[j][i]; 
            int spriteID = (i * 8) + j; 

            if(tileValue == -1)
            {
                NF_SpriteFrame(1, spriteID, 0); 
            } 
            else
            {
                int frame = tileValue;
                if (frame > 6) frame = 6; 
                NF_SpriteFrame(1, spriteID, frame);
            }
        }
    }
}
void shuffle(){
    for(int i = 0; i < 8; i++){
        for(int j = 8; j < 16; j++){
            grid[j][i] = -1;
        }
        for(int i = 0; i <3; i++){
            applyGravity();
             fillEmptySpaces();
             
        }

    }
}

//void swapTopTiles(directions dir)
//{
    
//}

void drawHighlightGrid(int gridX, int gridY, int spriteId,bool aPressed,directions pointingDir, uint16_t *pkeys_held)
{
    if(*pkeys_held & KEY_START)
    {
        NF_CreateSprite(1,2990000,1,1,0,0);
    }
    if(*pkeys_held & KEY_A && aPressed && pointingDir != NEUTRAL);
    {
     //   swapTopTiles();
    }
    if(aPressed)
    {
        NF_ShowSprite( 1, 68, true);
    }
    else
    {
        NF_ShowSprite(1,68,false);
    }
    NF_MoveSprite(1, spriteId, (gridX * 24) + 34 , (gridY * 24) + 2);
    if(aPressed){
        switch(pointingDir)
        {
            case(UP):
                NF_MoveSprite(1,68,(gridX * 24) + 34,(gridY * 24) + 2- 24);
                break;
            case(LEFT):
                NF_MoveSprite(1,68,(gridX * 24) + 34 - 24,(gridY * 24) + 2);
                break;
            case(DOWN):
                NF_MoveSprite(1,68,(gridX * 24) + 34,(gridY * 24) + 2 + 24);
                break;
            case(RIGHT):
                NF_MoveSprite(1,68,(gridX * 24) + 34 + 24,(gridY * 24) + 2);
                break;
            case(NEUTRAL):
                NF_MoveSprite(1, 68, (gridX * 24) + 34 , (gridY * 24) + 2);
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
    NF_LoadTiledBg("bg/title_top","title_top",256,256);
    soundEnable();
    initGrid(); 
    int semitones = 0;
    const uint16_t combo_pitches[] = {1024, 1085, 1149, 1218, 1290, 1366, 1448, 1534, 1625, 1722, 1825, 1933, 2048};
    int gridx = 0;
    int gridy = 0;
    bool touched = false;
    uint16_t keys_up;
    uint16_t keys_down;
    bool swiped = false;
    States state = INIT_MATCH;
    mmLoad(MOD_TITLE);                                          
    mmStart(MOD_TITLE, MM_PLAY_LOOP);
    int topGridX = 0;
    int topGridY = 0;
    bool isAPressed = false;
    directions currentDir = NEUTRAL;
    
    while(1){
        scanKeys();
        keys_down = keysDown();
        keys_held = keysHeld();
        keys_up = keysUp();
        switch(state){
            case(START):
            {   

                //NF_LoadTiledBg("bg/title_bottom","title_bottom",256,256); tobemade
                

                NF_CreateTiledBg(0, 3, "title_top");
    
                glFlush(0);
                break;
            }
            case(INIT_MATCH):
            {
                    mmLoadEffect(SFX_POP);
                    mmLoad(MOD_GAME);
                    NF_LoadSpriteGfx("sprite/crops3d", 0, 32, 32);
                    NF_LoadSpritePal("sprite/crops3d", 0);
                    NF_Vram3dSpriteGfx(0, 0, false);
                    NF_Vram3dSpritePal(0, 0);
                    NF_Sort3dSprites();
                    NF_LoadSpriteGfx("sprite/character", 1, 32, 32);  
                    NF_LoadSpritePal("sprite/character", 1);                    
                    NF_VramSpritePal(1, 1, 1);
                    NF_VramSpriteGfx(1, 1, 1, false); 
                    NF_LoadTiledBg("bg/match_grid", "match_grid", 256, 256);
                    NF_CreateTiledBg(0, 3, "match_grid");
                    NF_CreateTiledBg(1, 3, "match_grid");
                    lcdSwap();
                    for(int i = 0; i < 8; i++)
                    {
                        for(int j = 0; j < 8; j++)
                        {
                        int x = 36 + (i * 24); 
                        int y = (j * 24) + 5;
                            spriteIdGrid[j + 8][i] = (i * 8) + j + 64;
                            NF_Create3dSprite((i * 8) + j + 64,0,0,x,y); 
                            NF_Set3dSpriteFrame((i * 8) + j + 64, grid[i + 8][j]);

                        }
                    } 
                    
                    NF_Sort3dSprites();


                    for(int i = 0; i < 8; i++)
                    {
                        for(int j = 0; j < 8; j++)
                        {
                            int x = 34+ (i * 24)  ; 

                            int y = (j * 24) + 2;
                            spriteIdGrid[j][i] = (i * 8) + j;
                            NF_CreateSprite(1,((i * 8) + j),1,1,x,y); 
                            NF_SpriteFrame(1, ((i * 8) + j), grid[j][i]);
                            
                        }  
                    }
                    NF_CreateSprite(1,67,1,1,0,0);//the grid at top screen
                    NF_SpriteFrame(1, 67, 7);
                    NF_CreateSprite(1,68,1,1,0,0);//the second grid at top screen soon to be balls
                    NF_SpriteFrame(1, 68, 7);

                    mmStart(MOD_GAME, MM_PLAY_LOOP);
                    mmSetModuleVolume(256);
                    state = GAME_MATCH;
            }
            case(GAME_MATCH):
            {
                
                if (keys_held & KEY_TOUCH && !touched)
                {
                    touchRead(&touch_pos);
                    gridx = (int)((touch_pos.px - 36)/ 24);
                    gridy = 8 + (int)((touch_pos.py - 5)/ 24);
                    touched = true;
                }
                if(keys_up & KEY_TOUCH && touched)
                {
                    touched = false;
                }

                NF_Draw3dSprites();
                if(getSwipeGesture(SwipeLeft))  {swiped = swipeBlocks(gridx, gridy, SwipeLeft); }
                if(getSwipeGesture(SwipeUp))    {swiped =  swipeBlocks( gridx, gridy , SwipeUp); }
                if(getSwipeGesture(SwipeDown))  {swiped = swipeBlocks( gridx, gridy, SwipeDown); }
                if(getSwipeGesture(SwipeRight)) {swiped =  swipeBlocks(gridx, gridy, SwipeRight);}
                if(keys_down & KEY_SELECT){
                    shuffle();
                }
                bool hasMatches = findMatches();
                
                if(hasMatches) 
                {
                    mm_sfxhand h = mmEffect(SFX_POP);
                    mmEffectRate(h, combo_pitches[totalMatches % 13]);
                    applyMatches();
                }
                if(keys_down & KEY_A)
                {      if(isAPressed == false)
                        {
                            isAPressed = true;
                        }
                        else
                        {
                            isAPressed = false;
                        }
                       
                }
                if(keys_down & KEY_UP)
                {
                    if(!isAPressed)
                    {
                        topGridY--;
                    }
                    else
                    {
                        if(currentDir == DOWN)
                        {
                            currentDir = NEUTRAL;
                        }
                        else
                        {
                            currentDir = UP;
                        }
                    }
                    
                }
                if(keys_down & KEY_DOWN)
                {
                    if(!isAPressed)
                    {
                        topGridY++;
                    }
                    else
                    {
                        if(currentDir == UP)
                        {
                            currentDir = NEUTRAL;
                        }
                        else
                        {
                            currentDir = DOWN;
                        }
                    }
                    
                }
                if(keys_down & KEY_LEFT)
                {
                    if(!isAPressed)
                    {
                        topGridX--;
                    }
                    else
                    {
                        if(currentDir == RIGHT)
                        {
                            currentDir = NEUTRAL;
                        }
                        else
                        {
                            currentDir = LEFT;
                        }
                    }
                    
                }
                if(keys_down & KEY_RIGHT)
                {
                    if(!isAPressed)
                    {
                        topGridX++;
                    }
                    else
                    {
                        if(currentDir == LEFT)
                        {
                            currentDir = NEUTRAL;
                        }
                        else
                        {
                            currentDir = RIGHT;
                        }
                    }
                    
                    
                }


                drawHighlightGrid(topGridX,topGridY,67,isAPressed,currentDir,&keys_held);    

                applyGravity();
                for(int i = 0; i < 2;i++) 
                {
                    fillEmptySpaces();
                    applyGravity();
                }
                drawGridTop();
                drawGridBottom();
                if(swiped)
                {
                    swiped = false;
                }
                else{
                    glFlush(0);
                }
                break;
                
        }
     }
       
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
        NF_UpdateTextLayers();

        NF_Update3dSpritesGfx();
    }
} 