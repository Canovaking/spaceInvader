#ifndef PROJECTILE_H
#define PROJECTILE_H

// Structure for projectiles
typedef struct Projectile {
    int x, y;
    struct Projectile *next;
} Projectile;

Projectile *projectiles;

void add_projectile(int x, int y);

#endif // PROJECTILE_H

