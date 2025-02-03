#ifndef PLAYER_H
#define PLAYER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Game state variables
extern uint16_t highscore;
extern uint8_t game;

// Player structure
typedef struct {
    uint8_t x;
    uint8_t y;
} Player;

// Player function declarations
void player_init(Player *p);
void player_move(Player *p, uint8_t x, uint8_t y);

#endif // PLAYER_H
