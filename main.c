#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILE_SIZE 100
#define BOARD_SIZE 4

SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

TTF_Font* g_font = NULL;

typedef struct {
    int x;
    int y;
} t_position;

typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    t_position empty_pos;
} t_puzzle;

t_puzzle g_puzzle;

void ft_initialize_font() {
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }

    g_font = TTF_OpenFont("Mochalatte-JRorB.ttf", 28);
    if (g_font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }
}

void ft_initialize_puzzle() {
    int i, j, count = 1;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            g_puzzle.board[i][j] = count++;
        }
    }
    g_puzzle.empty_pos.x = BOARD_SIZE - 1;
    g_puzzle.empty_pos.y = BOARD_SIZE - 1;
    g_puzzle.board[g_puzzle.empty_pos.x][g_puzzle.empty_pos.y] = 0;
}

void ft_shuffle_puzzle() {
    int moves_count = 1000;
    srand(time(NULL));

    for (int i = 0; i < moves_count; i++) {
        int direction = rand() % 4; // 0: up, 1: down, 2: left, 3: right

        switch (direction) {
            case 0: // up
                if (g_puzzle.empty_pos.y > 0) {
                    g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y - 1][g_puzzle.empty_pos.x];
                    g_puzzle.board[g_puzzle.empty_pos.y - 1][g_puzzle.empty_pos.x] = 0;
                    g_puzzle.empty_pos.y--;
                }
                break;
            case 1: // down
                if (g_puzzle.empty_pos.y < BOARD_SIZE - 1) {
                    g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y + 1][g_puzzle.empty_pos.x];
                    g_puzzle.board[g_puzzle.empty_pos.y + 1][g_puzzle.empty_pos.x] = 0;
                    g_puzzle.empty_pos.y++;
                }
                break;
            case 2: // left
                if (g_puzzle.empty_pos.x > 0) {
                    g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x - 1];
                    g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x - 1] = 0;
                    g_puzzle.empty_pos.x--;
                }
                break;
            case 3: // right
                if (g_puzzle.empty_pos.x < BOARD_SIZE - 1) {
                    g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x + 1];
                    g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x + 1] = 0;
                    g_puzzle.empty_pos.x++;
                }
                break;
        }
    }
}

void ft_draw_puzzle() {
    int i, j;
    SDL_Rect rect;
    rect.w = SCREEN_WIDTH / BOARD_SIZE;
    rect.h = SCREEN_HEIGHT / BOARD_SIZE;

    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
    SDL_RenderClear(g_renderer);

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            rect.x = j * rect.w;
            rect.y = i * rect.h;
            if (g_puzzle.board[i][j] != 0) {
                SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(g_renderer, &rect);
                SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(g_renderer, &rect);

                SDL_Color text_color = {0, 0, 0, 255};
                char number[3];
                sprintf(number, "%d", g_puzzle.board[i][j]);
                SDL_Surface* surface = TTF_RenderText_Solid(g_font, number, text_color);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
                SDL_Rect text_rect;
                text_rect.x = rect.x + rect.w / 2 - surface->w / 2;
                text_rect.y = rect.y + rect.h / 2 - surface->h / 2;
                text_rect.w = surface->w;
                text_rect.h = surface->h;
                SDL_RenderCopy(g_renderer, texture, NULL, &text_rect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
        }
    }

    SDL_RenderPresent(g_renderer);
}

void ft_handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (g_puzzle.empty_pos.y < BOARD_SIZE - 1) {
                            g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y + 1][g_puzzle.empty_pos.x];
                            g_puzzle.board[g_puzzle.empty_pos.y + 1][g_puzzle.empty_pos.x] = 0;
                            g_puzzle.empty_pos.y++;
                        }
                        break;
                    case SDLK_DOWN:
                        if (g_puzzle.empty_pos.y > 0) {
                            g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y - 1][g_puzzle.empty_pos.x];
                            g_puzzle.board[g_puzzle.empty_pos.y - 1][g_puzzle.empty_pos.x] = 0;
                            g_puzzle.empty_pos.y--;
                        }
                        break;
                    case SDLK_LEFT:
                        if (g_puzzle.empty_pos.x < BOARD_SIZE - 1) {
                            g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x + 1];
                            g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x + 1] = 0;
                            g_puzzle.empty_pos.x++;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (g_puzzle.empty_pos.x > 0) {
                            g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x - 1];
                            g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x - 1] = 0;
                            g_puzzle.empty_pos.x--;
                        }
                        break;
                }
                break;
        }
    }
}

int ft_is_puzzle_solved() {
    int i, j, count = 1;

    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (g_puzzle.board[i][j] != count) {
                return 0;
            }
            count++;
            if (i == BOARD_SIZE - 1 && j == BOARD_SIZE - 1) {
                if (g_puzzle.board[i][j] != 0) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

void ft_move_tile(int direction) {
    int moved = 0;

    if (direction == 0 && g_puzzle.empty_pos.y > 0) {
        g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y - 1][g_puzzle.empty_pos.x];
        g_puzzle.board[g_puzzle.empty_pos.y - 1][g_puzzle.empty_pos.x] = 0;
        g_puzzle.empty_pos.y--;
        moved = 1;
    } else if (direction == 1 && g_puzzle.empty_pos.y < BOARD_SIZE - 1) {
        g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y + 1][g_puzzle.empty_pos.x];
        g_puzzle.board[g_puzzle.empty_pos.y + 1][g_puzzle.empty_pos.x] = 0;
        g_puzzle.empty_pos.y++;
        moved = 1;
    } else if (direction == 2 && g_puzzle.empty_pos.x > 0) {
        g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x - 1];
        g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x - 1] = 0;
        g_puzzle.empty_pos.x--;
        moved = 1;
    } else if (direction == 3 && g_puzzle.empty_pos.x < BOARD_SIZE - 1) {
        g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x] = g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x + 1];
        g_puzzle.board[g_puzzle.empty_pos.y][g_puzzle.empty_pos.x + 1] = 0;
        g_puzzle.empty_pos.x++;
        moved = 1;
    }

    if (moved) {
        if (ft_is_puzzle_solved()) {
            printf("Win\n");
            SDL_Delay(2000);
            exit(0);
        }
    }
}

void ft_load_board_from_file(t_puzzle* puzzle, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            fscanf(file, "%d", &puzzle->board[i][j]);
            if (puzzle->board[i][j] == 0) {
                puzzle->empty_pos.x = j;
                puzzle->empty_pos.y = i;
            }
        }
    }

    fclose(file);
}

int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    g_window = SDL_CreateWindow("Taquin Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    ft_initialize_font();
    ft_initialize_puzzle();

    printf("Choisissez l'option :\n");
    printf("1. Charger une grille\n");
    printf("2. Générer une grille aléatoire\n");

    int option;
    scanf("%d", &option);

    if (option == 1) {
        char filename[256];
        printf("Entrez le nom du fichier : ");
        scanf("%s", filename);
        ft_load_board_from_file(&g_puzzle, filename);

        if (ft_is_puzzle_solved()) {
            printf("Win\n");
            SDL_Delay(2000);
        }
    } else if (option == 2) {
        ft_shuffle_puzzle();
    } else {
        printf("Option non valide. Quitting...\n");
        return 1;
    }

    while (1) {
        ft_handle_events();
        ft_draw_puzzle();

        if (ft_is_puzzle_solved()) {
            printf("Win\n");
            SDL_Delay(2000);
            break;
        }
    }

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    TTF_CloseFont(g_font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}