#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>
/**/
// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: NightFox & Co., 2009-2011
//
// Example of animating tiled backgrounds.
// http://www.nightfoxandco.com

#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>

#define SPEED 2         // Scroll speed
#define MAP_X 32      // Amount of tiles that can be held right now
#define MAP_Y 24    // same here
#define WATER_FRAMES 5 // Number of frames of the animation
#define WATER_DELAY 10  // Animation speed (number of frames to wait to change)

u8 CUBE_NET[MAP_X][MAP_Y] = {{1,2,2},{1,3,1},{4,3,3}}; // 512 pixels, 64 tiles, every frame is 4x4 tiles

void fillvoard(){   
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 24; j++){
            CUBE_NET[i][j] = (rand() %6) + 5 ;
        }
    }
}
void DrawSticker(void)
{
    // Change the required tiles for the animation
    for (int y = 0; y < MAP_Y; y++)
    {
        for (int x = 0; x < MAP_X; x++) 
        {
            // Calculate the map 4x4 region to fill
            int start_x = x * 2; //ALSO AFFECTS IT
            int end_x = start_x + 2;
            int start_y = y * 2;
            int end_y = start_y + 2;

        


 

                // Fill the 4x4 region
                for (int tile_y = start_y; tile_y < end_y; tile_y++)
                {
                    for (int tile_x = start_x; tile_x < end_x; tile_x++)
                    {
                        NF_SetTileOfMap(1, 3, ((int)tile_x / 2), ((int)tile_y / 2), (CUBE_NET[x][y] * 16) );
                        
                    }
                }
            }
        
    }

    // Update map in VRAM
    NF_UpdateVramMap(1, 3);
}
// screen: 0 (top) or 1 (bottom)
// layer: 0 to 3
// grid_x: Horizontal position (0 to 28)
// grid_y: Vertical position (0 to 20)
// frame_id: Which of your 6 frames to draw (0 to 5)



int main(int argc, char **argv)
{

    // Set random seed based on the current time
    srand(time(NULL));

    // Prepare a NitroFS initialization screen
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();

    // Initialize NitroFS and set it as the root folder of the filesystem
    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");

    // Initialize 2D engine in both screens and use mode 0
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    // Initialize tiled backgrounds system
    NF_InitTiledBgBuffers();    // Initialize storage buffers
    NF_InitTiledBgSys(0);       // Top screen
    NF_InitTiledBgSys(1);       // Bottom screen

    // Load background files from NitroFS
    NF_LoadTiledBg("bg/nfl", "nfl", 256, 256);
    NF_LoadTilesForBg("bg/frames", "water", 512, 512, 0, 175);

    // Create top screen background
    NF_CreateTiledBg(0, 3, "nfl");
    // Create bottom screen backgrounds
    NF_CreateTiledBg(1, 3, "water");

    // Initialize background animation


    // Variables
    s16 bg_x = 0;
    s16 bg_y = 0;
    fillvoard();
    while (1)
    {


        // Animate background
        DrawSticker();

        // Wait for the screen refresh
        swiWaitForVBlank();


    }

    return 0;
}