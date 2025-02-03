#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "player.h"
#include "graphics.h"
#include <stdlib.h>

#define INITIAL_X  58
#define INITIAL_Y  110
#define POSITION_OFFSET 5

void player_init(Player *p) {
    p->x = 58;
    p->y = 110;
    game = 1;
}

void player_move(Player *p, uint8_t x, uint8_t y) {
    if (x == MAX_RANGE) x = MAX_RANGE-5;
    if (x == MIN_RANGE) x = MIN_RANGE+5;
    graphics_draw_player(x,y);
}
