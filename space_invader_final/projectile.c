#include "projectile.h"
#include "enemy.h"
#include "graphics.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern uint16_t score;
extern uint8_t level;


// Array to store all projectiles
static Projectile projectiles[MAX_PROJECTILES];

// Initialize the projectile manager
void projectile_manager_init(void) {
    uint8_t i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = 0; // Set all projectiles as inactive
    }
}

// Spawn a new projectile
void projectile_manager_spawn(int16_t x, int16_t y, ProjectileType type) {
    uint8_t i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) { // Find an inactive projectile
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].prev_x = x;
            projectiles[i].prev_y = y;
            projectiles[i].type = type;
            projectiles[i].active = 1; // Activate the projectile
            return; // Spawn only one projectile
        }
    }
}

// Update all projectiles
void projectile_manager_update(void) {
    uint8_t i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            projectiles[i].prev_y = projectiles[i].y;
            // Move the projectile based on its type
            if (projectiles[i].type == PROJECTILE_TYPE_PLAYER) {
                projectiles[i].y -= 5; // Move up
            } else if (projectiles[i].type == PROJECTILE_TYPE_ENEMY) {
                projectiles[i].y += 5; // Move down
            }

            // Deactivate if it goes off-screen
            if (projectiles[i].y <= 5 || projectiles[i].y >= SCREEN_HEIGHT - 1) {
                projectiles[i].active = 0;
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                Graphics_Rectangle proj = {
                    projectiles[i].x, projectiles[i].y, projectiles[i].prev_x + PROJECTILE_WIDTH - 1, projectiles[i].prev_y + PROJECTILE_HEIGHT - 1
                };
                // Draw the projectile as a rectangle
                Graphics_fillRectangle(&g_sContext, &proj);
            }
        }
    }
}

// Draw all projectiles
void projectile_manager_draw(void) {
    uint8_t i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            // Erase the old position
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
            Graphics_Rectangle proj_prev = {
                projectiles[i].prev_x, projectiles[i].prev_y, projectiles[i].prev_x + PROJECTILE_WIDTH - 1, projectiles[i].prev_y + PROJECTILE_HEIGHT - 1
            };
            // Draw the projectile as a rectangle
            Graphics_fillRectangle(&g_sContext, &proj_prev);

            // Draw the projectile at the new position
            // Set color based on projectile type
            if (projectiles[i].type == PROJECTILE_TYPE_PLAYER) {
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
            } else if (projectiles[i].type == PROJECTILE_TYPE_ENEMY) {
                Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
            }
            Graphics_Rectangle proj = {
                projectiles[i].x, projectiles[i].y, projectiles[i].x + PROJECTILE_WIDTH - 1, projectiles[i].y + PROJECTILE_HEIGHT - 1
            };
            // Draw the projectile as a rectangle
            Graphics_fillRectangle(&g_sContext, &proj);
        }
    }
}

void projectile_manager_check_collisions(Player* player) {
    uint8_t i, row, col;
    for(i = 0; i < MAX_PROJECTILES; i++) {
        if(projectiles[i].active && projectiles[i].type == PROJECTILE_TYPE_PLAYER) {  // Only check active player projectiles
            for(row = 0; row < num_rows; row++) {
                for(col = 0; col < invaders_per_row; col++) {
                    if(enemies[row][col].active) {
                        // Check for bounding box collision with more precise hitbox
                        if(projectiles[i].x < (enemies[row][col].x + ENEMY_WIDTH) &&
                           (projectiles[i].x + PROJECTILE_WIDTH) > enemies[row][col].x &&
                           projectiles[i].y < (enemies[row][col].y + ENEMY_HEIGHT) &&
                           (projectiles[i].y + PROJECTILE_HEIGHT) > enemies[row][col].y) {

                            // Clear the projectile first
                            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                            Graphics_Rectangle proj = {
                                projectiles[i].x, projectiles[i].y,
                                projectiles[i].x + PROJECTILE_WIDTH - 1,
                                projectiles[i].y + PROJECTILE_HEIGHT - 1
                            };
                            Graphics_fillRectangle(&g_sContext, &proj);

                            // Clear the enemy
                            Graphics_Rectangle enemy = {
                                enemies[row][col].x, enemies[row][col].y,
                                enemies[row][col].x + ENEMY_WIDTH - 1,
                                enemies[row][col].y + ENEMY_HEIGHT - 1
                            };
                            Graphics_fillRectangle(&g_sContext, &enemy);

                            // Draw explosion effect
                            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                            graphics_draw_explosion(enemies[row][col].x + ENEMY_WIDTH/2,
                                                 enemies[row][col].y + ENEMY_HEIGHT/2);

                            // Deactivate both objects
                            projectiles[i].active = 0;
                            enemies[row][col].active = 0;

                            switch(enemies[row][col].type){
                                case 0:
                                    score += 1;
                                    break;
                                case 1:
                                    score += 3;
                                    break;
                                case 2:
                                    score += 5;
                                    break;
                            }
                            graphics_draw_score(5, 5, score, level);

                            // Add a small delay to make explosion visible
                            __delay_cycles(1000000);

                            // Clear explosion
                            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
                            Graphics_Rectangle explosion = {
                                enemies[row][col].x + ENEMY_WIDTH/2 - 3,
                                enemies[row][col].y + ENEMY_HEIGHT/2 - 3,
                                enemies[row][col].x + ENEMY_WIDTH/2 + 3,
                                enemies[row][col].y + ENEMY_HEIGHT/2 + 3
                            };
                            Graphics_fillRectangle(&g_sContext, &explosion);
                        }
                    }
                }
            }
        }
        else if (projectiles[i].active && projectiles[i].type == PROJECTILE_TYPE_ENEMY){
			if (projectiles[i].x < player->x + PLAYER_WIDTH &&
				projectiles[i].x + PROJECTILE_WIDTH > player->x &&
				projectiles[i].y < player->y + PLAYER_HEIGHT &&
				projectiles[i].y + PROJECTILE_HEIGHT > player->y) {

				// Projectile hit the player
				projectiles[i].active = 0;
				lives -= 1;

				// Clear the projectile
				Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
				Graphics_Rectangle proj = {
					projectiles[i].x, projectiles[i].y,
					projectiles[i].x + PROJECTILE_WIDTH - 1,
					projectiles[i].y + PROJECTILE_HEIGHT - 1
				};
				Graphics_fillRectangle(&g_sContext, &proj);
			}
		}
    }
}
