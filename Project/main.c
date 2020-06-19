/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_dac.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <ff.h>
#include <diskio.h>
#include <math.h>
#include <stdio.h>
#include "map.h"
#include <stdlib.h>
#include <string.h>

int main(void) { 
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_timer2_init();
  f3d_dac_init();
  f3d_delay_init();
  f3d_lcd_init();
  f3d_rtc_init();
  f3d_systick_init(100);
  f3d_i2c1_init();
  printf("i2c1 inited\n");
  printf("Reset\n");
  f3d_nunchuk_init();
  printf("nun inited\n");
  f3d_accel_init();
  nunchuk_t nunchuk[1];
  f3d_nunchuk_read(nunchuk);
  f3d_nunchuk_read(nunchuk);
  int  button_c = 0;
  int  button_z = 0;
  f3d_lcd_fillScreen2(BLACK);
  f3d_lcd_drawString(45, 59, "FROGGER", WHITE, BLACK);
  f3d_lcd_drawString(10, 69, "PRESS Z/C TO START", WHITE, BLACK);
  while(!(nunchuk->c || nunchuk->z)){
    f3d_nunchuk_read(nunchuk);
    f3d_nunchuk_read(nunchuk);
  }
  f3d_nunchuk_read(nunchuk);
  initMap();
  int end = 0;
  int grid[16][20];
  initEnvironment(grid);
  while(1){
    while(!end){
      updateGrid(grid, nunchuk);
      updateMap(grid);
      end = checkPlayer(grid);
      delay(250);
      //printGrid(grid);
    }
    
    if(end == 2){
      printf("win\n");
      f3d_lcd_fillScreen2(WHITE);
      f3d_lcd_drawString(10, 59, "THE FROG SURVIVED", BLACK, WHITE);
      f3d_lcd_drawString(7, 69, "TO HOP ANOTHER DAY", BLACK, WHITE);
      f3d_lcd_drawString(5, 69, "PRESS Z/C TO RESTART", BLACK, WHITE);
    }else{
      printf("loss\n");
      f3d_lcd_fillScreen2(RED);
      f3d_lcd_drawString(35, 59, "RIP FROGGER :(", BLACK, RED);
      f3d_lcd_drawString(5, 69, "PRESS Z/C TO RESTART", BLACK, RED);
    }
    while(!(nunchuk->c || nunchuk->z)){
      f3d_nunchuk_read(nunchuk);
      f3d_nunchuk_read(nunchuk);
    }
    end = 0;
    player.x = 8;
    player.y = 0;
    initMap();
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif


/* main.c ends here */
