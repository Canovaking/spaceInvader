#include "game_logic.h"

void handle_input() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = 0;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    player.x -= GRID_SIZE;
                    if (player.x < 0) player.x = 0;
                    break;
                case SDLK_RIGHT:
                    player.x += GRID_SIZE;
                    if (player.x + player.width > SCREEN_WIDTH)
                        player.x = SCREEN_WIDTH - player.width;
                    break;
                case SDLK_SPACE:
                    // Check the cooldown before adding a new projectile
                    if (SDL_GetTicks() - last_projectile_time >= PROJECTILE_COOLDOWN) {
                        add_projectile(player.x + player.width / 2, player.y);
                        last_projectile_time = SDL_GetTicks(); // Update last projectile time
                    }
                    break;
            }
        }
    }
}
