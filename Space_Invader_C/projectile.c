#include "projectile.h"
#include <stdlib.h>

void add_projectile(int x, int y) {
    Projectile *new_proj = (Projectile *)malloc(sizeof(Projectile));
    if (!new_proj) return;
    new_proj->x = x;
    new_proj->y = y;
    new_proj->next = projectiles;
    projectiles = new_proj;
}
