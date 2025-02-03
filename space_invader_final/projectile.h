#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdint.h>
#include "player.h"

// Projectile constants
#define MAX_PROJECTILES 128
#define PROJECTILE_WIDTH 2
#define PROJECTILE_HEIGHT 3
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Projectile types
typedef enum {
    PROJECTILE_TYPE_PLAYER,
    PROJECTILE_TYPE_ENEMY
} ProjectileType;

// Projectile structure
typedef struct {
    uint8_t prev_x;
    uint8_t prev_y;
    uint8_t x;
    uint8_t y;
    ProjectileType type;
    uint8_t active;
} Projectile;

// Game state variable
extern volatile uint8_t lives;

// Projectile manager function declarations
void projectile_manager_init(void);
void projectile_manager_spawn(int16_t x, int16_t y, ProjectileType type);
void projectile_manager_update(void);
void projectile_manager_draw(void);
void projectile_manager_check_collisions(Player* player);

#endif // PROJECTILE_H
