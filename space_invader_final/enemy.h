#ifndef ENEMY_H
#define ENEMY_H

#include <stdint.h>
#include "graphics.h"
#include "projectile.h"

// Enemy constants
#define ENEMY_WIDTH 10
#define ENEMY_HEIGHT 8
#define ENEMY_SPACING_X (ENEMY_WIDTH + 5)
#define ENEMY_SPACING_Y (ENEMY_HEIGHT + 5)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Enemy structure
typedef struct {
    uint8_t prev_x;
    uint8_t prev_y;
    uint8_t x;
    uint8_t y;
    uint8_t type;
    uint8_t active;
} Enemy;

// Enemy manager variables
Enemy enemies[10][10]; // Max 10 columns, 10 rows
uint8_t invaders_per_row;
uint8_t num_rows;
extern uint8_t level;

// Internal enemy movement variables
static int8_t direction = 1;  // 1 = right, -1 = left
static uint8_t enemy_speed = 5;  // Pixels per update

// Enemy manager function declarations
void enemy_manager_init(uint8_t rows, uint8_t cols, uint8_t enemy_type);
void enemy_manager_update(void);
void enemy_manager_draw(void);
uint8_t enemy_manager_has_reached_player(void);
int is_every_enemy_dead(void);

#endif // ENEMY_H
