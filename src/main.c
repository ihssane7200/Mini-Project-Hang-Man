#include "raylib.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6

typedef enum { SCREEN_MENU, SCREEN_LEVEL, SCREEN_PLAYING } GameScreen;
typedef enum { LEVEL_EASY, LEVEL_MEDIUM, LEVEL_HARD } DifficultyLevel;

typedef struct {
    char word[MAX_WORD_LENGTH];
    char hint[MAX_WORD_LENGTH];
} WordWithHint;

typedef struct {
    GameScreen screen;
    DifficultyLevel difficulty;
    const char* secretWord;
    const char* hint;
    int wordLength;
    char* guessedWord;
    bool* guessedLetters;
    int tries;
    bool gameWon;
    bool gameOver;
} GameState;

// Word lists
WordWithHint easyWords[] = {
    { "game", "Something you play for fun" }, { "love", "A strong feeling of affection" },
    { "play", "Engage in activity for enjoyment" }, { "book", "Pages bound together to read" },
    { "tree", "A tall plant with a trunk and leaves" }, { "star", "A glowing point in the night sky" },
    { "moon", "Earth's natural satellite" }, { "fish", "An aquatic creature with gills" }
};

WordWithHint medWords[] = {
    { "jump", "Push oneself off a surface into the air" }, { "wind", "The perceptible natural movement of the air" },
    { "fire", "Combustion producing heat and light" }, { "cold", "Of or at a low or relatively low temperature" },
    { "fast", "Moving or capable of moving at high speed" }, { "slow", "Moving or operating at a low speed" },
    { "king", "The male ruler of an independent state" }, { "road", "A wide way leading from one place to another" }
};

WordWithHint hardWords[] = {
    { "quiz", "A test of knowledge, especially a brief one" }, { "flux", "Continuous change" },
    { "lynx", "A wild cat with yellowish-brown fur" }, { "zany", "Amusingly unconventional and idiosyncratic" },
    { "jinx", "A person or thing that brings bad luck" }, { "vowd", "An extremely rare word (Hard!)" },
    { "glyph", "A hieroglyphic character or symbol" }, { "wisp", "A small thin or twisted bunch, piece, or amount" }
};

// Modern Colors (Dark Mode style)
Color bgDark = { 15, 23, 42, 255 };          // Slate 900
Color primaryColor = { 56, 189, 248, 255 };  // Sky 400
Color primaryHover = { 125, 211, 252, 255 }; // Sky 300
Color textLight = { 248, 250, 252, 255 };    // Slate 50
Color drawColor = { 148, 163, 184, 255 };    // Slate 400
Color errorColor = { 244, 63, 94, 255 };     // Rose 500
Color successColor = { 34, 197, 94, 255 };   // Green 500

GameState* InitGame() {
    GameState* game = (GameState*)malloc(sizeof(GameState));
    game->guessedWord = (char*)calloc(MAX_WORD_LENGTH, sizeof(char));
    game->guessedLetters = (bool*)calloc(26, sizeof(bool));
    game->screen = SCREEN_MENU;
    game->difficulty = LEVEL_EASY;
    srand(time(NULL));
    return game;
}

void StartSession(GameState* game) {
    WordWithHint* list = easyWords;
    if(game->difficulty == LEVEL_MEDIUM) list = medWords;
    if(game->difficulty == LEVEL_HARD) list = hardWords;

    int idx = rand() % 8; // 8 words per category
    game->secretWord = list[idx].word;
    game->hint = list[idx].hint;
    game->wordLength = strlen(game->secretWord);
    
    memset(game->guessedWord, 0, MAX_WORD_LENGTH);
    memset(game->guessedLetters, 0, 26 * sizeof(bool));
    game->tries = 0;
    game->gameWon = false;
    game->gameOver = false;
}

// Function to draw modern rounded buttons
bool DrawModernButton(Rectangle bounds, const char* text, Vector2 mousePoint) {
    bool clicked = false;
    bool isHover = CheckCollisionPointRec(mousePoint, bounds);

    if (isHover) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawRectangleRounded(bounds, 0.3f, 10, primaryColor); // Pressed
        } else {
            DrawRectangleRounded(bounds, 0.3f, 10, primaryHover); // Hover
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) clicked = true;
    } else {
        DrawRectangleRounded(bounds, 0.3f, 10, primaryColor); // Normal
    }

    DrawRectangleRoundedLines(bounds, 0.3f, 10, 2, textLight);
    int textW = MeasureText(text, 20);
    DrawText(text, bounds.x + bounds.width/2 - textW/2, bounds.y + bounds.height/2 - 10, 20, bgDark);
    
    return clicked;
}

void UpdateAndDraw(GameState* game) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Logical resolution (the base design format)
    float gameWidth = 800.0f;
    float gameHeight = 450.0f;
    
    // Calculate scaling to perfectly fit window
    float scale = (float)screenWidth / gameWidth;
    if ((float)screenHeight / gameHeight < scale) scale = (float)screenHeight / gameHeight;
    
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ (screenWidth - gameWidth * scale) * 0.5f, (screenHeight - gameHeight * scale) * 0.5f };
    camera.target = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = scale;

    // Convert screen mouse to virtual scaled world coordinates
    Vector2 mousePoint = GetScreenToWorld2D(GetMousePosition(), camera);

    BeginDrawing();
    ClearBackground(bgDark);
    
    // Start drawing inside the scalable camera scope
    BeginMode2D(camera);

    if (game->screen == SCREEN_MENU) {
        int titleW = MeasureText("HANGMAN", 60);
        DrawText("HANGMAN", 400 - titleW/2, 100, 60, primaryColor);
        
        int subtitleW = MeasureText("Modern Edition", 20);
        DrawText("Modern Edition", 400 - subtitleW/2, 170, 20, textLight);

        Rectangle btnStart = { 400 - 100, 240, 200, 50 };
        if (DrawModernButton(btnStart, "START GAME", mousePoint)) {
            StartSession(game);
            game->screen = SCREEN_PLAYING;
        }

        Rectangle btnLevel = { 400 - 100, 310, 200, 50 };
        if (DrawModernButton(btnLevel, "LEVELS", mousePoint)) {
            game->screen = SCREEN_LEVEL;
        }

    } else if (game->screen == SCREEN_LEVEL) {
        int titleW = MeasureText("SELECT DIFFICULTY", 40);
        DrawText("SELECT DIFFICULTY", 400 - titleW/2, 80, 40, textLight);

        Rectangle btnEasy = { 400 - 100, 160, 200, 50 };
        Rectangle btnMed = { 400 - 100, 230, 200, 50 };
        Rectangle btnHard = { 400 - 100, 300, 200, 50 };
        Rectangle btnBack = { 400 - 100, 380, 200, 50 };

        if (DrawModernButton(btnEasy, game->difficulty == LEVEL_EASY ? "EASY (Selected)" : "EASY", mousePoint)) game->difficulty = LEVEL_EASY;
        if (DrawModernButton(btnMed, game->difficulty == LEVEL_MEDIUM ? "MEDIUM (Selected)" : "MEDIUM", mousePoint)) game->difficulty = LEVEL_MEDIUM;
        if (DrawModernButton(btnHard, game->difficulty == LEVEL_HARD ? "HARD (Selected)" : "HARD", mousePoint)) game->difficulty = LEVEL_HARD;
        
        if (DrawModernButton(btnBack, "BACK TO MENU", mousePoint)) {
            game->screen = SCREEN_MENU;
        }
    } else if (game->screen == SCREEN_PLAYING) {
        
        // --- GAME LOGIC ---
        if (!game->gameOver && !game->gameWon) {
            int key = GetKeyPressed();
            if (key >= KEY_A && key <= KEY_Z) {
                char guess = tolower((char)key);
                if (!game->guessedLetters[guess - 'a']) {
                    game->guessedLetters[guess - 'a'] = true;
                    bool found = false;
                    for (int i = 0; i < game->wordLength; i++) {
                        if (game->secretWord[i] == guess) {
                            found = true;
                            game->guessedWord[i] = guess;
                        }
                    }
                    if (!found) game->tries++;
                    if (strcmp(game->secretWord, game->guessedWord) == 0) game->gameWon = true;
                    if (game->tries >= MAX_TRIES) game->gameOver = true;
                }
            }
        }

        // --- DRAW HANGMAN (Modern thick graphics) ---
        DrawLineEx((Vector2){150, 100}, (Vector2){150, 350}, 6, drawColor); // Post
        DrawLineEx((Vector2){147, 100}, (Vector2){300, 100}, 6, drawColor); // Top bar
        DrawLineEx((Vector2){300, 97}, (Vector2){300, 140}, 6, drawColor);  // Rope
        
        if (game->tries > 0) DrawCircle(300, 170, 30, errorColor); // Head
        if (game->tries > 0) DrawCircle(300, 170, 24, bgDark); // Inner Head (makes it hollow)
        if (game->tries > 1) DrawLineEx((Vector2){300, 200}, (Vector2){300, 280}, 6, errorColor); // Body
        if (game->tries > 2) DrawLineEx((Vector2){300, 220}, (Vector2){250, 260}, 6, errorColor); // Left Arm
        if (game->tries > 3) DrawLineEx((Vector2){300, 220}, (Vector2){350, 260}, 6, errorColor); // Right Arm
        if (game->tries > 4) DrawLineEx((Vector2){300, 280}, (Vector2){250, 330}, 6, errorColor); // Left Leg
        if (game->tries > 5) DrawLineEx((Vector2){300, 280}, (Vector2){350, 330}, 6, errorColor); // Right Leg

        // Draw Word & Blanks
        int startX = 400;
        for (int i = 0; i < game->wordLength; i++) {
            if (game->guessedWord[i] != '\0') {
                DrawText(TextFormat("%c", toupper(game->guessedWord[i])), startX + i * 40, 260, 40, primaryColor);
            }
            DrawLineEx((Vector2){startX + i*40, 305}, (Vector2){startX + 30 + i*40, 305}, 4, textLight);
        }

        // Draw Hint
        DrawText("HINT:", 400, 100, 20, primaryColor);
        DrawText(game->hint, 400, 130, 20, textLight);

        // Draw Guessed Letters
        DrawText("GUESSED:", 400, 180, 20, drawColor);
        int gx = 400; int gy = 210;
        for(int i = 0; i < 26; i++){
            if(game->guessedLetters[i]){
                DrawText(TextFormat("%c", 'A' + i), gx, gy, 20, errorColor);
                gx += 25;
                if (gx > 700) { gx = 400; gy += 25; }
            }
        }

        // UI Ends States
        if (game->gameOver || game->gameWon) {
            if (game->gameOver) {
                DrawText("GAME OVER!", startX, 335, 30, errorColor);
                DrawText(TextFormat("Answer: %s", game->secretWord), startX, 375, 20, textLight);
            } else {
                DrawText("YOU WON!", startX, 335, 30, successColor);
            }

            Rectangle btnMenu = { startX, 405, 120, 35 };
            Rectangle btnPlay = { startX + 130, 405, 150, 35 };
            if (DrawModernButton(btnMenu, "MENU", mousePoint)) game->screen = SCREEN_MENU;
            if (DrawModernButton(btnPlay, "PLAY AGAIN", mousePoint)) StartSession(game);
        } else {
            Rectangle btnBack = { 20, 20, 120, 40 };
            if (DrawModernButton(btnBack, "< MENU", mousePoint)) {
                game->screen = SCREEN_MENU;
            }
        }
    }

    EndMode2D();
    EndDrawing();
}

void CleanupGame(GameState* game) {
    if (game) {
        if (game->guessedWord) free(game->guessedWord);
        if (game->guessedLetters) free(game->guessedLetters);
        free(game);
    }
}

int main(void)
{
    // Make default window much larger
    const int screenWidth = 1024;
    const int screenHeight = 768;
    
    // EXTREMELY IMPORTANT: Allow the window to be resizable/maximized!
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    
    InitWindow(screenWidth, screenHeight, "Hangman - Modern UI Edition");
    SetWindowMinSize(800, 450); // Prevent shrinking below original base res
    
    SetTargetFPS(60);

    GameState* game = InitGame();

    while (!WindowShouldClose())
    {
        UpdateAndDraw(game);
    }

    CleanupGame(game);
    CloseWindow();
    return 0;
}
