#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "player.h"

// Constants for graphics elements
#define PLAYER_WIDTH    12
#define PLAYER_HEIGHT   8
#define INVADER_WIDTH   10
#define INVADER_HEIGHT  8
#define BULLET_WIDTH    2
#define BULLET_HEIGHT   4

// Game state variables
extern uint16_t highscore;
extern uint16_t score;
extern volatile uint8_t lives;
extern uint8_t level;

// Graphics context
extern Graphics_Context g_sContext;

// LCD specifications
#define LCD_SIZE    128
#define MIN_RANGE   0
#define MAX_RANGE   31  // max margin is 128/RATIO-1
#define RATIO       4   // Scale factor: 1 node = 4 pixels

// Graphics function declarations
void graphics_init(void);
void graphics_init_menu(void);
void graphics_draw_player(uint8_t x, uint8_t y);
void graphics_draw_score(uint8_t x, uint8_t y, uint16_t score, uint8_t level);
void graphics_hide_player(uint8_t x, uint8_t y);
void graphics_lose(void);
void graphics_win(void);
void graphics_draw_single_invader(uint8_t x, uint8_t y, uint8_t level);
void graphics_draw_explosion(uint8_t x, uint8_t y);
void graphics_draw_bullet(uint8_t x, uint8_t y);
void graphics_draw_lives(uint8_t x, uint8_t y, uint16_t lives);
void graphics_update_screen(uint8_t score, uint8_t lives, uint8_t level);

#endif // GRAPHICS_H
