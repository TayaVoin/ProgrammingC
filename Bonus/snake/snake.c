#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 30
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int direction; // 0 - up, 1 - right, 2 - down, 3 - left
} Snake;

typedef struct {
    Point position;
    bool isSpecial;
    time_t spawnTime;
} Food;

typedef struct {
    int score;
    int level;
    int speed;
    bool gameOver;
    int highScore;
    time_t levelStartTime;
    int levelDuration;
} GameState;

Snake snake;
Food food;
GameState game;

void setup();
void draw();
void input();
void logic();
void spawnFood(bool isSpecial);
void loadHighScore();
void saveHighScore();
int max(int a, int b);

int main() {
    // Инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    setup();
    while (!game.gameOver) {
        draw();
        input();
        logic();
        usleep(game.speed * 1000); // Преобразуем миллисекунды в микросекунды
    }
    
    endwin(); // Завершаем работу ncurses
    
    printf("Game Over! Final Score: %d\n", game.score);
    if (game.score > game.highScore) {
        printf("New High Score!\n");
    }
    printf("Press any key to exit...");
    getchar();
    return 0;
}

void setup() {
    // Initialize snake
    snake.length = 3;
    snake.direction = 1; // Start moving right
    
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = WIDTH / 2 - i;
        snake.body[i].y = HEIGHT / 2;
    }
    
    // Initialize game state
    game.score = 0;
    game.level = 1;
    game.speed = 150; // milliseconds
    game.gameOver = false;
    game.levelDuration = 60; // seconds
    
    loadHighScore();
    
    // Spawn first food
    spawnFood(false);
    
    // Initialize random seed
    srand(time(NULL));
    game.levelStartTime = time(NULL);
}

void draw() {
    clear();
    
    // Draw border
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                mvaddch(y, x, '#'); // Border
            }
        }
    }
    
    // Draw snake
    for (int i = 0; i < snake.length; i++) {
        if (i == 0) {
            mvaddch(snake.body[i].y, snake.body[i].x, 'O'); // Head
        } else {
            mvaddch(snake.body[i].y, snake.body[i].x, 'o'); // Body
        }
    }
    
    // Draw food
    mvaddch(food.position.y, food.position.x, food.isSpecial ? '@' : '*');
    
    // Print game info
    mvprintw(HEIGHT, 0, "Score: %d | Level: %d | High Score: %d", 
             game.score, game.level, game.highScore);
    mvprintw(HEIGHT + 1, 0, "Time remaining: %d seconds", 
             game.levelDuration - (time(NULL) - game.levelStartTime));
    
    refresh();
}

void input() {
    int ch = getch();
    if (ch != ERR) {
        switch (ch) {
            case 'w':
            case KEY_UP:
                if (snake.direction != 2) snake.direction = 0;
                break;
            case 'd':
            case KEY_RIGHT:
                if (snake.direction != 3) snake.direction = 1;
                break;
            case 's':
            case KEY_DOWN:
                if (snake.direction != 0) snake.direction = 2;
                break;
            case 'a':
            case KEY_LEFT:
                if (snake.direction != 1) snake.direction = 3;
                break;
            case 'x':
                game.gameOver = true;
                break;
        }
    }
}

void logic() {
    // Move snake
    Point prev = snake.body[0];
    Point prev2;
    
    switch (snake.direction) {
        case 0: // Up
            snake.body[0].y--;
            break;
        case 1: // Right
            snake.body[0].x++;
            break;
        case 2: // Down
            snake.body[0].y++;
            break;
        case 3: // Left
            snake.body[0].x--;
            break;
    }
    
    for (int i = 1; i < snake.length; i++) {
        prev2 = snake.body[i];
        snake.body[i] = prev;
        prev = prev2;
    }
    
    // Check collision with borders
    if (snake.body[0].x <= 0 || snake.body[0].x >= WIDTH - 1 || 
        snake.body[0].y <= 0 || snake.body[0].y >= HEIGHT - 1) {
        game.gameOver = true;
    }
    
    // Check collision with self
    for (int i = 1; i < snake.length; i++) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
            game.gameOver = true;
        }
    }
    
    // Check if food eaten
    if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
        // Increase snake length
        if (snake.length < MAX_SNAKE_LENGTH) {
            snake.length++;
            snake.body[snake.length - 1] = snake.body[snake.length - 2];
        }
        
        // Increase score
        if (food.isSpecial) {
            game.score += 5;
            // Add extra length
            if (snake.length < MAX_SNAKE_LENGTH - 2) {
                snake.length += 2;
                snake.body[snake.length - 1] = snake.body[snake.length - 2];
                snake.body[snake.length - 2] = snake.body[snake.length - 3];
                snake.body[snake.length - 3] = snake.body[snake.length - 4];
            }
        } else {
            game.score += 1;
        }
        
        // Spawn new food (10% chance for special food)
        spawnFood((rand() % 10) == 0);
        
        // Check for level up
        if (game.score >= game.level * 10) {
            game.level++;
            game.speed = max(50, game.speed - 10); // Increase speed, but not below 50ms
            game.levelStartTime = time(NULL);
        }
        
        // Update high score if needed
        if (game.score > game.highScore) {
            game.highScore = game.score;
            saveHighScore();
        }
    }
    
    // Check level timer
    if (time(NULL) - game.levelStartTime > game.levelDuration) {
        game.level++;
        game.speed = max(50, game.speed - 10);
        game.levelStartTime = time(NULL);
        
        // Spawn new food when level changes
        spawnFood(false);
    }
}

void spawnFood(bool isSpecial) {
    bool validPosition = false;
    
    while (!validPosition) {
        food.position.x = rand() % (WIDTH - 2) + 1;
        food.position.y = rand() % (HEIGHT - 2) + 1;
        food.isSpecial = isSpecial;
        food.spawnTime = time(NULL);
        
        validPosition = true;
        
        // Check if food spawns on snake
        for (int i = 0; i < snake.length; i++) {
            if (snake.body[i].x == food.position.x && snake.body[i].y == food.position.y) {
                validPosition = false;
                break;
            }
        }
        
        // Check if food spawns on border
        if (food.position.x == 0 || food.position.x == WIDTH - 1 || 
            food.position.y == 0 || food.position.y == HEIGHT - 1) {
            validPosition = false;
        }
    }
}

void loadHighScore() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &game.highScore);
        fclose(file);
    } else {
        game.highScore = 0;
    }
}

void saveHighScore() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", game.highScore);
        fclose(file);
    }
}

int max(int a, int b) {
    return (a > b) ? a : b;
}