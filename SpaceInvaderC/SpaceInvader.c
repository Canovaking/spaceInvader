#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Screen dimensions
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define GRID_SIZE 20

// Player movement directions
typedef enum { LEFT, RIGHT } PlayerDirection;

// Structure for the player
typedef struct {
    int x, y;
    int width;
} Player;

// Structure for projectiles
typedef struct Projectile {
    int x, y;
    struct Projectile *next;
} Projectile;

// Structure for enemies
typedef struct Enemy {
    int x, y;
    struct Enemy *next;
} Enemy;

// Global variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player;
Projectile *projectiles = NULL;
Enemy *enemies = NULL;
int running = 1;

// Function prototypes
void init_game();
void close_game();
void handle_input();
void update_game();
void render_game();
void add_projectile(int x, int y);
void add_enemy(int x, int y);
void move_enemies();
void check_collisions();

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Horizontal Space Invader",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    srand((unsigned int)time(NULL));
    init_game();

    // Game loop
    while (running) {
        handle_input();
        update_game();
        render_game();
        SDL_Delay(50); // Control the game speed
    }

    close_game();
    return 0;
}

void init_game() {
    // Initialize player
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT - GRID_SIZE * 2;
    player.width = GRID_SIZE * 3;
	int i = 0;
    // Add initial enemies
    for (i=0; i < 10; i++) {
        add_enemy(i * GRID_SIZE * 2, GRID_SIZE);
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

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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
                    add_projectile(player.x + player.width / 2, player.y);
                    break;
            }
        }
    }
}

void update_game() {
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

    SDL_RenderPresent(renderer);
}

void add_projectile(int x, int y) {
    Projectile *new_proj = (Projectile *)malloc(sizeof(Projectile));
    if (!new_proj) return;
    new_proj->x = x;
    new_proj->y = y;
    new_proj->next = projectiles;
    projectiles = new_proj;
}

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

