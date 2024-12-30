#ifndef ENEMY_H
#define ENEMY_H

// Structure for enemies
typedef struct Enemy {
    int x, y;
    struct Enemy *next;
} Enemy;

Enemy *enemies;

void add_enemy(int x, int y);
void move_enemies();

#endif // ENEMY_H

