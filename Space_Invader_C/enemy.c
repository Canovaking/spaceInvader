#include "game_logic.h"

void add_enemy(int x, int y) {
    Enemy *new_enemy = (Enemy *)malloc(sizeof(Enemy));
    if (!new_enemy) return;
    new_enemy->x = x;
    new_enemy->y = y;
    new_enemy->next = enemies;
    enemies = new_enemy;
}

void move_enemies() {
    static int direction = 1; // 1 for right, -1 for left

    Enemy *current = enemies;
    while (current) {
        current->x += direction * GRID_SIZE / 2;
        if (current->y >= player.y) {
            game_over();
            return;
        }
        current = current->next;
    }

    // Check if enemies hit the wall
    current = enemies;
    while (current) {
        if (current->x < 0 || current->x + GRID_SIZE > SCREEN_WIDTH) {
            direction *= -1;
            Enemy *temp = enemies;
            while (temp) {
                temp->y += GRID_SIZE; // Move enemies down
                temp = temp->next;
            }
            break;
        }
        current = current->next;
    }
}
