#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "projectile.h"
#include "enemy.h"
#include "player.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define GRID_SIZE 20
#define PROJECTILE_COOLDOWN 300

// Global variables
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern int running;
extern int score;
extern int round_number;
extern int show_message;
extern char message_text[50];
extern Uint32 message_start_time;
extern Uint32 last_projectile_time;



void init_game();
void update_game();
void close_game();
void check_collisions();
void restart_round();
void game_over();
void render_game();
void render_text(const char *text, int x, int y);
void display_message(const char* message);

#endif // GAME_LOGIC_H

