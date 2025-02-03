#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

extern uint16_t highscore;

// Sprite data for game objects (simple pixel art representations)
static const uint8_t PLAYER_SPRITE[] = {
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,1,1,1,1,1,1,1,1,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1
};

// Base Invader
static const uint8_t INVADER_SPRITE_TYPE0[] = {
    0,1,0,0,1,1,1,0,0,1,
    0,0,1,1,1,1,1,1,0,0,
    0,1,1,1,1,1,1,1,1,0,
    1,1,0,1,1,1,1,0,1,1
};

// Crab-like Invader
static const uint8_t INVADER_SPRITE_TYPE1[] = {
    1,0,1,1,0,0,1,1,0,1,
    0,1,1,1,1,1,1,1,1,0,
    0,0,1,0,1,1,0,1,0,0,
    0,1,0,0,1,1,0,0,1,0
};

// Octopus-like Invader
static const uint8_t INVADER_SPRITE_TYPE2[] = {
    0,1,1,0,1,1,0,1,1,0,
    1,1,1,1,1,1,1,1,1,1,
    0,1,0,1,1,1,1,0,1,0,
    1,0,0,1,0,0,1,0,0,1
};

// Array of sprite pointers for different invader types
static const uint8_t* INVADER_SPRITES[] = {
    INVADER_SPRITE_TYPE0,
    INVADER_SPRITE_TYPE1,
    INVADER_SPRITE_TYPE2
};

void graphics_init(){
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

}

void graphics_init_menu() {
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"SPACE INVADERS",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    30,
                                    OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)"Press left to start",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    55,
                                    OPAQUE_TEXT);
    char string[19];
    sprintf(string, "highscore: %5d", highscore);
    Graphics_drawStringCentered(&g_sContext,
                                    (int8_t *)string,
                                    19,
                                    64,
                                    105,
                                    OPAQUE_TEXT);
}

/*
 * Draw the player
 */
void graphics_draw_player(uint8_t x, uint8_t y) {
    int row;
    int col;
    for(row = 0; row < PLAYER_HEIGHT/2; row++) {
            for(col = 0; col < PLAYER_WIDTH; col++) {
                if(PLAYER_SPRITE[row * PLAYER_WIDTH + col]) {
                    Graphics_drawPixel(&g_sContext, x + col, y + row);
                    Graphics_drawPixel(&g_sContext, x + col, y + PLAYER_HEIGHT - 1 - row); // Mirror bottom half
                }
            }
        }
}

void graphics_draw_score(uint8_t x, uint8_t y, uint16_t score, uint8_t level) {
    char score_string[11]; // Buffer to hold the score as a string
    sprintf(score_string, "Score: %d", score); // Convert the score to a string

    char lv_string[6]; // Buffer to hold the score as a string
    sprintf(lv_string, "LV: %d", level); // Convert the score to a string

    // Clear the area where the score will be updated (erase old score)
    Graphics_Rectangle score_area = {x, y, x + 15, y + 15};
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &score_area);

    // Draw the score string on the screen at the specified coordinates
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext,
                        (int8_t *)score_string,
                        AUTO_STRING_LENGTH,
                        x,
                        y,
                        OPAQUE_TEXT);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
       Graphics_drawString(&g_sContext,
                           (int8_t *)lv_string,
                           AUTO_STRING_LENGTH,
                           x,
                           y+10,
                           OPAQUE_TEXT);
}

//updates both lives and score
void graphics_update_screen(uint8_t score, uint8_t lives, uint8_t level){
    graphics_draw_score(5, 5, score,level);
    graphics_draw_lives(80, 5, lives);

}

void graphics_draw_lives(uint8_t x, uint8_t y, uint16_t lives) {
    char score_string[11]; // Buffer to hold the score as a string
    sprintf(score_string, "Lives: %d", lives); // Convert the score to a string

    // Clear the area where the score will be updated (erase old score)
    Graphics_Rectangle score_area = {x, y, x + 15, y + 5};
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &score_area);

    // Draw the score string on the screen at the specified coordinates
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawString(&g_sContext,
                        (int8_t *)score_string,
                        AUTO_STRING_LENGTH,
                        x,
                        y,
                        OPAQUE_TEXT);
}

void graphics_draw_single_invader(uint8_t x, uint8_t y, uint8_t level) {
    const uint8_t* invaderSprite;

    // Select sprite based on level
    switch (level) {
        case 1:
            invaderSprite = INVADER_SPRITE_TYPE0;
            break;
        case 2:
            invaderSprite = INVADER_SPRITE_TYPE1;
            break;
        case 3:
            invaderSprite = INVADER_SPRITE_TYPE2;
            break;
        default:
            return; // Invalid level, do nothing
    }

    uint8_t row, col;
    // Draw invader sprite pixel by pixel
    for (row = 0; row < INVADER_HEIGHT / 2; row++) {
        for (col = 0; col < INVADER_WIDTH; col++) {
            if (invaderSprite[row * INVADER_WIDTH + col]) {
                // Draw the pixel at the correct position
                Graphics_drawPixel(&g_sContext, x + col, y + row);
                // Mirror bottom half for symmetry
                Graphics_drawPixel(&g_sContext, x + col, y + INVADER_HEIGHT - 1 - row);
            }
        }
    }
}

//draws explosion
void graphics_draw_explosion(uint8_t x, uint8_t y){
    int i;
    for(i = -3; i <= 3; i++) {
        Graphics_drawPixel(&g_sContext, x+i, y);
        Graphics_drawPixel(&g_sContext, x, y+i);
        Graphics_drawPixel(&g_sContext, x+i/2, y+i/2);
        Graphics_drawPixel(&g_sContext, x-i/2, y+i/2);
    }
}

//draws bullet
void graphics_draw_bullet(uint8_t x, uint8_t y) {
    Graphics_Rectangle bullet = {
        x, y, x + BULLET_WIDTH - 1, y + BULLET_HEIGHT - 1
    };
    Graphics_fillRectangle(&g_sContext, &bullet);
}

//hides player
void graphics_hide_player(uint8_t x, uint8_t y) {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    graphics_draw_player(x,y);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
}

void graphics_lose() {
    // Clear the entire screen
    Graphics_clearDisplay(&g_sContext);

    // Set text color and display the "Game Over" message
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    // Draw "GAME OVER" text
    Graphics_drawStringCentered(&g_sContext,
                                (int8_t *)"GAME OVER",
                                AUTO_STRING_LENGTH,
                                64,  // X center of the screen
                                25,  // Y position for the text
                                OPAQUE_TEXT);

    // Draw the score
    char sstring[14];
    sprintf(sstring, "Score: %5d", score);
    Graphics_drawStringCentered(&g_sContext,
                                (int16_t *)sstring,
                                14,  // Length of the string
                                64,  // X center of the screen
                                50,  // Y position for the score
                                OPAQUE_TEXT);

    // Draw the high score
    char hsstring[19];
    if(score > highscore){
        highscore = score;
    }
    sprintf(hsstring, "Highscore: %5d", highscore);
    Graphics_drawStringCentered(&g_sContext,
                                (int16_t *)hsstring,
                                19,  // Length of the string
                                64,  // X center of the screen
                                70,  // Y position for the high score
                                OPAQUE_TEXT);

    // Draw instructions to return to the menu
    Graphics_drawStringCentered(&g_sContext,
                                (int8_t *)"Press left button",
                                AUTO_STRING_LENGTH,
                                64,  // X center of the screen
                                95,  // Y position
                                OPAQUE_TEXT);
    Graphics_drawStringCentered(&g_sContext,
                                (int8_t *)"to go back to menu",
                                AUTO_STRING_LENGTH,
                                64,  // X center of the screen
                                105, // Y position
                                OPAQUE_TEXT);
}

void graphics_win(){
    // Clear the entire screen
       Graphics_clearDisplay(&g_sContext);

       // Set text color and display the "Game Over" message
       Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

       // Draw "GAME OVER" text
       Graphics_drawStringCentered(&g_sContext,
                                   (int8_t *)"YOU WIN",
                                   AUTO_STRING_LENGTH,
                                   64,  // X center of the screen
                                   25,  // Y position for the text
                                   OPAQUE_TEXT);

       // Draw the score
       char sstring[14];
       sprintf(sstring, "Score: %5d", score);
       Graphics_drawStringCentered(&g_sContext,
                                   (int16_t *)sstring,
                                   14,  // Length of the string
                                   64,  // X center of the screen
                                   50,  // Y position for the score
                                   OPAQUE_TEXT);

       // Draw the high score
       char hsstring[19];
       if(score > highscore){
           highscore = score;
       }
       sprintf(hsstring, "Highscore: %5d", highscore);
       Graphics_drawStringCentered(&g_sContext,
                                   (int16_t *)hsstring,
                                   19,  // Length of the string
                                   64,  // X center of the screen
                                   70,  // Y position for the high score
                                   OPAQUE_TEXT);

       // Draw instructions to return to the menu
       Graphics_drawStringCentered(&g_sContext,
                                   (int8_t *)"Press left button",
                                   AUTO_STRING_LENGTH,
                                   64,  // X center of the screen
                                   95,  // Y position
                                   OPAQUE_TEXT);
       Graphics_drawStringCentered(&g_sContext,
                                   (int8_t *)"to go back to menu",
                                   AUTO_STRING_LENGTH,
                                   64,  // X center of the screen
                                   105, // Y position
                                   OPAQUE_TEXT);
}




