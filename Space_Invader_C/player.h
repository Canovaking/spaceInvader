#ifndef PLAYER_H
#define PLAYER_H

// Structure for the player
typedef struct Player {
    int x, y;
    int width;
} Player;

// Player movement directions
typedef enum { LEFT, RIGHT } PlayerDirection;

extern Player player;

void handle_input();

#endif // PLAYER_H

