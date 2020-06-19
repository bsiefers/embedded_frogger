#include <f3d_nunchuk.h>
#include <f3d_lcd_sd.h>
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#define GRAY 0x66444
#define ORANGE YELLOW
#define BROWN 0x00444
#define CAR 0
#define LOG 1
#define WATER 2
#define ROAD 3
#define GRASS 4

void initMap(){
    f3d_lcd_drawBlock(GREEN, 0, 0, 16*8, 1*8);
    f3d_lcd_drawBlock(GRAY, 0, 1*8, 16*8, 6*8);
    f3d_lcd_drawBlock(GREEN, 0, 6*8, 16*8, 7*8);
    f3d_lcd_drawBlock(GRAY, 0, 7*8, 16*8, 12*8);
    f3d_lcd_drawBlock(GREEN, 0, 12*8, 16*8, 13*8);
    f3d_lcd_drawBlock(BLUE, 0, 13*8, 16*8, 19*8);
    f3d_lcd_drawBlock(GREEN, 0, 19*8, 16*8, 20*8);
}

uint16_t getMapColor(int grid[][20]){
    if(lastPlayer.y == 0 || lastPlayer.y == 6 || lastPlayer.y == 12 || lastPlayer.y > 18){
        return GREEN;
    }else if((lastPlayer.y > 0 && lastPlayer.y < 6) || (lastPlayer.y > 6 && lastPlayer.y < 12)){
        if(grid[lastPlayer.x][lastPlayer.y] == CAR)
            return YELLOW;
        return GRAY;
    }else{
        if(grid[lastPlayer.x][lastPlayer.y] == LOG)
            return BROWN;
        return BLUE;
    }
}


void updateMap(int grid[][20]){
    int i, j, w;
    
    for( i = 0; i < 16; i++){
        for( j = 0; j < 20; j++){
            if(player.x == i && player.y == j){
                f3d_lcd_drawBlock(WHITE, player.x*8, player.y*8 + 1, (player.x*8)+8, (player.y*8)+6);
                f3d_lcd_drawBlock(getMapColor(grid), lastPlayer.x*8 + 1, lastPlayer.y*8, (lastPlayer.x*8)+8, (lastPlayer.y*8)+6);
            }else if(grid[i][j] == CAR){
                f3d_lcd_drawBlock(YELLOW, i*8, j*8 + 1, (i*8)+8, (j*8)+6);
            }else if(grid[i][j] == LOG){
                f3d_lcd_drawBlock(BROWN, i*8, j*8 + 1, (i*8)+8, (j*8)+6);
            }else if(grid[i][j] == WATER){
                f3d_lcd_drawBlock(BLUE, i*8, j*8, (i*8)+8, (j*8)+8);
            }else if(grid[i][j] == ROAD){
                f3d_lcd_drawBlock(GRAY, i*8, j*8, (i*8)+8, (j*8)+8);
                f3d_lcd_drawBlock(WHITE, i*8, j*8 + 7, (i*8)+2, (j*8)+8);
            }else if(grid[i][j] == GRASS){
                f3d_lcd_drawBlock(GREEN, i*8, j*8, (i*8)+8, (j*8)+8);
            }else{
                delay(100);
                printf("Bug in updateMap, grid %d, %d: %d \n\n", i, j, grid[i][j]);
            }         
        }
    }
}

void updateGrid(int grid[][20], nunchuk_t* nunchuk){
    //move player
    f3d_nunchuk_read(nunchuk);
    f3d_nunchuk_read(nunchuk);

    float accel[3];
    delay(25);
    f3d_accel_read(accel);
    float pitch = atan2(accel[0], sqrt(accel[1] * accel[1] + accel[2] * accel[2]))*20;
    float roll =  atan2(accel[1], sqrt(accel[0] * accel[0] + accel[2] * accel[2]))*20;
    delay(100);
    printf("pitch: %f, roll: %f\n", pitch, roll);
    if((nunchuk->jx < 90 || roll < -25.0f) && player.x > 0){
        player.x--;
        lastPlayer.x = player.x+1;
        lastPlayer.y = player.y;
    }else if((nunchuk->jx > 150 || roll > 25.0f) && player.x < 15){
        player.x++;
        lastPlayer.x = player.x-1;
        lastPlayer.y = player.y;
    }else if((nunchuk->jy < 90 || pitch < -25.0f) && player.y  > 0){
        player.y--;
        lastPlayer.x = player.x;
        lastPlayer.y = player.y +1;
    }else if((nunchuk->jy > 150 || pitch > 25.0f) && player.y  < 21){
        player.y++;
        lastPlayer.x = player.x;
        lastPlayer.y = player.y-1;
    }else{
        //Do nothing
    }
    //printf("jx: %d, jy: %d\n", nunchuk->jx, nunchuk->jy);
    //printf("px: %d, py: %d\n", player.x, player.y);
    int i, j;
    //move logs
    
    for(i=0; i<16; i++){
        for(j=13; j < 19; j++){
            grid[i][j] = WATER;               
        }
    }

    for( i=0; i<24; i++){
        if(i&1){
            if(player.x == logs[i].x && player.y == logs[i].y){
                player.x++;
            }
            logs[i].x = ++logs[i].x%16;
            grid[logs[i].x][logs[i].y] = LOG;
        }else{
            if(player.x == logs[i].x && player.y == logs[i].y){
                player.x--;
            }
            if(--logs[i].x == -1)
                logs[i].x = 15;
            grid[logs[i].x][logs[i].y] = LOG;
        }
    }

    for(i=0; i<16; i++){
        for(j=1; j < 6; j++){
            grid[i][j] = ROAD;                
        }
    }

    for(i=0; i<16; i++){
        for(j=7; j < 12; j++){
            grid[i][j] = ROAD;                
        }
    }

    for( i=0; i<20; i++){
        if(i&1){
            cars[i].x = ++cars[i].x%16;
            grid[cars[i].x][cars[i].y] = CAR;
        }else{
            if(--cars[i].x == -1)
                cars[i].x = 15;
            grid[cars[i].x][cars[i].y] = CAR;
        }
    }
}

void initEnvironment(int grid[][20]){
    player.x = 8; player.y = 0;
    //moving left
    cars[0].x = 2; cars[0].y = 1;
    cars[2].x = 3; cars[2].y = 1;
    cars[4].x = 4; cars[4].y = 2;
    cars[6].x = 5; cars[6].y = 2;
    cars[8].x = 10; cars[8].y = 3;
    cars[10].x = 11; cars[10].y = 3;
    cars[12].x = 13; cars[12].y = 4;
    cars[14].x = 14; cars[14].y = 4;
    cars[16].x = 14; cars[16].y = 5;
    cars[18].x = 15; cars[18].y = 5;

    cars[1].x = 10; cars[1].y = 7;
    cars[3].x = 11; cars[3].y = 7;
    cars[5].x = 7; cars[5].y = 8;
    cars[7].x = 8; cars[7].y = 8;
    cars[9].x = 12; cars[9].y = 9;
    cars[11].x = 13; cars[11].y = 9;
    cars[13].x = 4; cars[13].y = 10;
    cars[15].x = 5; cars[15].y = 10;
    cars[17].x = 14; cars[17].y = 11;
    cars[19].x = 15; cars[19].y = 11;
    
    //moving left
    logs[0].x = 7; logs[0].y = 13;
    logs[2].x = 6; logs[2].y = 13;

    //moving right
    logs[1].x = 10; logs[1].y = 14;
    logs[3].x = 11; logs[3].y = 14;

    //moving left
    logs[4].x = 0; logs[4].y = 15;
    logs[6].x = 1; logs[6].y = 15;

    //moving right
    logs[5].x = 4; logs[5].y = 16;
    logs[7].x = 5; logs[7].y = 16;

    //moving left
    logs[8].x = 10; logs[8].y = 17;
    logs[10].x = 11; logs[10].y = 17;

    //moving right
    logs[9].x = 4; logs[9].y = 18;
    logs[11].x = 5; logs[11].y = 18;

    //moving left
    logs[12].x = 13; logs[12].y = 13;
    logs[14].x = 14; logs[14].y = 13;

    //moving right
    logs[13].x = 3; logs[13].y = 14;
    logs[15].x = 4; logs[15].y = 14;

    //moving left
    logs[16].x = 5; logs[16].y = 15;
    logs[18].x = 6; logs[18].y = 15;

    //moving right
    logs[17].x = 10; logs[17].y = 16;
    logs[19].x = 11; logs[19].y = 16;

    //moving left
    logs[20].x = 6; logs[20].y = 17;
    logs[22].x = 7; logs[22].y = 17;

    //moving right
    logs[21].x = 14; logs[21].y = 18;
    logs[23].x = 15; logs[23].y = 18;

    int i, j, w;
    for(i=0; i<16; i++){
        grid[i][0] = GRASS;
    }

    for(i=0; i<16; i++){
        for(j=1; j < 6; j++){
            grid[i][j] = ROAD;                
        }
    }

    for(i=0; i<16; i++){
        grid[i][6] = GRASS;
    }

    for(i=0; i<16; i++){
        for(j=7; j < 12; j++){
            grid[i][j] = ROAD;                
        }
    }

    for(i=0; i<16; i++){
        grid[i][12] = GRASS;
    }

    for(i=0; i<16; i++){
        for(j=13; j < 19; j++){
            grid[i][j] = WATER;               
        }
    }

    for(i =0; i < 24; i++){
        grid[logs[i].x][logs[i].y] = LOG;
    } 

    for(i =0; i < 18; i++){
        grid[cars[i].x][cars[i].y] = CAR;
    } 


    for(i=0; i<16; i++){
        grid[i][19] = GRASS;
    }
}

void printGrid(int grid[][20]){
 int j, i;
 delay(25);
 printf("\ngrid start:\n\n");
 for(i = 0; i < 16; i++){
   delay(25);
   for(j = 0; j < 20; j++){
     delay(25);
     printf("%d ", grid[i][j]);
   }
   delay(25);
   printf("\n");
 }
 delay(25);
 printf("grid end:\n\n");
}

int checkPlayer(int grid[][20]){
    if(player.y == 19){
        return 2;
    }else if(grid[player.x][player.y] == WATER || grid[player.x][player.y] == CAR){
        return 1;
    }else if(player.x < 0 || player.x > 15){
        return 1;
    }else{
        return 0;
    }
}