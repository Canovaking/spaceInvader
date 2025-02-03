#include "enemy.h"
#include <stdlib.h>

void enemy_manager_init(uint8_t rows, uint8_t cols, uint8_t enemy_type) {
    num_rows = rows;
    invaders_per_row = cols;

    uint8_t row;
    uint8_t col;

    // Initialize enemies
    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < invaders_per_row; col++) {
            enemies[row][col].x = 20 + col * ENEMY_SPACING_X;
            enemies[row][col].y = 20 + row * ENEMY_SPACING_Y;
            enemies[row][col].prev_x = 20 + col * ENEMY_SPACING_X; // Initialize previous position
            enemies[row][col].prev_y = 20 + row * ENEMY_SPACING_Y; // Initialize previous position
            enemies[row][col].type = enemy_type; // Cycle through sprite types
            enemies[row][col].active = 1;
        }
    }
}

void enemy_manager_update(void) {
    uint8_t row, col, projectile_spawn_probability;
    uint8_t reached_edge = 0;

    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < invaders_per_row; col++) {
            if (enemies[row][col].active) {
                // Store the current position as the previous position
                enemies[row][col].prev_x = enemies[row][col].x;
                enemies[row][col].prev_y = enemies[row][col].y;

                // Update the position
                enemies[row][col].x += direction * enemy_speed;

                // Check for edge collision
                if (enemies[row][col].x + ENEMY_WIDTH >= SCREEN_WIDTH || enemies[row][col].x <= 0) {
                    reached_edge = 1;
                }
                switch(enemies[row][col].type){
                    case 0: projectile_spawn_probability = 2; break;
                    case 1: projectile_spawn_probability = 3; break;
                    case 2: projectile_spawn_probability = 4; break;
                }
                // Enemy Shooting Mechanism
                if (rand() % 100 < projectile_spawn_probability) { // projectile_spawn_probability% chance per frame for an enemy to shoot
                    projectile_manager_spawn(enemies[row][col].x + ENEMY_WIDTH / 2,
                                            enemies[row][col].y + ENEMY_HEIGHT,
                                            PROJECTILE_TYPE_ENEMY);
                }
            }
        }
    }

    // Handle direction reversal and downward movement
    if (reached_edge) {
        direction = -direction;
        for (row = 0; row < num_rows; row++) {
            for (col = 0; col < invaders_per_row; col++) {
                if (enemies[row][col].active) {
                    enemies[row][col].y += ENEMY_SPACING_Y;
                }
            }
        }
    }
}

void enemy_manager_draw(void) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < invaders_per_row; col++) {
            if (enemies[row][col].active) {
                // Erase the old position
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                graphics_draw_single_invader(enemies[row][col].prev_x, enemies[row][col].prev_y, level);

                // Draw the enemy at the new position
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
                graphics_draw_single_invader(enemies[row][col].x, enemies[row][col].y, level);
            } else {
                // Clear the enemy from its old position if inactive
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                graphics_draw_single_invader(enemies[row][col].prev_x, enemies[row][col].prev_y, level);
            }
        }
    }
}

uint8_t enemy_manager_has_reached_player(void) {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < invaders_per_row; col++) {
            if (enemies[row][col].active && enemies[row][col].y >= 110) {
                return 1; // An enemy has reached the player area
            }
        }
    }
    return 0;
}

//Returns 1 if all enemies are not active, else 0
int is_every_enemy_dead(void){
    uint8_t row,col;
    for (row = 0; row < num_rows; row++) {
        for (col = 0; col < invaders_per_row; col++) {
            if(enemies[row][col].active)
                return 0;
        }
    }
    return 1;
}
