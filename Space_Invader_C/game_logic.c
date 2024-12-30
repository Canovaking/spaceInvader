#include "game_logic.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
Player player;
Projectile *projectiles = NULL;
Enemy *enemies = NULL;
int running = 1;
int score = 0;
int round_number = 1;
int show_message = 0;
char message_text[50] = "";
Uint32 message_start_time = 0;
Uint32 last_projectile_time = 0; // Track last projectile fire time


void init_game() {
    // Initialize player
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - GRID_SIZE * 2;
    player.width = GRID_SIZE * 3;

    // Add initial enemies
    restart_round();
}

void update_game() {
    if (show_message) {
        // Continue moving projectiles even during the message
        Projectile *prev_proj = NULL, *current_proj = projectiles;
        while (current_proj) {
            current_proj->y -= GRID_SIZE;
            if (current_proj->y < 0) {
                // Remove projectile
                if (prev_proj) {
                    prev_proj->next = current_proj->next;
                } else {
                    projectiles = current_proj->next;
                }
                free(current_proj);
                current_proj = (prev_proj) ? prev_proj->next : projectiles;
            } else {
                prev_proj = current_proj;
                current_proj = current_proj->next;
            }
        }

        // Continue countdown for the message
        if (SDL_GetTicks() - message_start_time > 3000) { // Shorten message time to 3 seconds
            show_message = 0;
            restart_round();
        }
        return; // Skip enemy movement and collision checks
    }

    // Move projectiles
    Projectile *prev_proj = NULL, *current_proj = projectiles;
    while (current_proj) {
        current_proj->y -= GRID_SIZE;
        if (current_proj->y < 0) {
            // Remove projectile
            if (prev_proj) {
                prev_proj->next = current_proj->next;
            } else {
                projectiles = current_proj->next;
            }
            free(current_proj);
            current_proj = (prev_proj) ? prev_proj->next : projectiles;
        } else {
            prev_proj = current_proj;
            current_proj = current_proj->next;
        }
    }

    // Move enemies
    move_enemies();

    // Check for collisions
    check_collisions();

    // Check if all enemies are destroyed
    if (!enemies) {
        display_message("Next attack coming");
    }
}

void close_game() {
    // Free projectiles
    while (projectiles) {
        Projectile *temp = projectiles;
        projectiles = projectiles->next;
        free(temp);
    }

    // Free enemies
    while (enemies) {
        Enemy *temp = enemies;
        enemies = enemies->next;
        free(temp);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void check_collisions() {
    Projectile *prev_proj = NULL, *current_proj = projectiles;
    while (current_proj) {
        Enemy *prev_enemy = NULL, *current_enemy = enemies;
        while (current_enemy) {
            if (current_proj->x < current_enemy->x + GRID_SIZE &&
                current_proj->x + GRID_SIZE / 2 > current_enemy->x &&
                current_proj->y < current_enemy->y + GRID_SIZE &&
                current_proj->y + GRID_SIZE > current_enemy->y) {
                // Collision detected
                score += 10; // Increment score
                if (prev_enemy) {
                    prev_enemy->next = current_enemy->next;
                } else {
                    enemies = current_enemy->next;
                }
                free(current_enemy);

                if (prev_proj) {
                    prev_proj->next = current_proj->next;
                } else {
                    projectiles = current_proj->next;
                }
                free(current_proj);
                current_proj = (prev_proj) ? prev_proj->next : projectiles;
                return;
            }
            prev_enemy = current_enemy;
            current_enemy = current_enemy->next;
        }
        prev_proj = current_proj;
        current_proj = current_proj->next;
    }
}

void restart_round() {
    
    int max_rows = (round_number < 4) ? round_number : 4; // Cap rows at 4
    int row, col;

    for (row = 0; row < max_rows; row++) {
        for (col = 0; col < 10; col++) { // Keep the same number of columns
            add_enemy(col * GRID_SIZE * 2, row * GRID_SIZE * 2 + GRID_SIZE * 2);
        }
    }
    round_number++;
}
 

void game_over() {
    display_message("Game Over!");
    render_game(); // Render the game over screen immediately
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); // Show message for 2 seconds before exiting
    running = 0;
}

void render_game() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw player
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect player_rect = {player.x, player.y, player.width, GRID_SIZE};
    SDL_RenderFillRect(renderer, &player_rect);

    // Draw projectiles
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Projectile *current_proj = projectiles;
    while (current_proj) {
        SDL_Rect proj_rect = {current_proj->x, current_proj->y, GRID_SIZE / 2, GRID_SIZE};
        SDL_RenderFillRect(renderer, &proj_rect);
        current_proj = current_proj->next;
    }

    // Draw enemies
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    Enemy *current_enemy = enemies;
    while (current_enemy) {
        SDL_Rect enemy_rect = {current_enemy->x, current_enemy->y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &enemy_rect);
        current_enemy = current_enemy->next;
    }

    // Draw score
    char score_text[50];
    sprintf(score_text, "Score: %d", score);
    render_text(score_text, SCREEN_WIDTH - 150, 10);

    // Draw message if active
    if (show_message) {
        render_text(message_text, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 25);
    }

    SDL_RenderPresent(renderer);
}

void render_text(const char *text, int x, int y) {
    SDL_Color color = {255, 255, 255}; // White color
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    if (text_surface) {
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect render_quad = {x, y, text_surface->w, text_surface->h};
        SDL_RenderCopy(renderer, text_texture, NULL, &render_quad);
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    } else {
        printf("Failed to render text: %s\n", TTF_GetError());
    }
}


void display_message(const char* message) {
    strcpy(message_text, message);
    show_message = 1;
    message_start_time = SDL_GetTicks();
}
