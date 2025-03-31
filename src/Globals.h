#ifndef GLOBALS_H
#define GLOBALS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FEEDBACK_DURATION = 2000; // 2 seconds
const int SLIDE_HEIGHT = 20;
const int SPEED_BOOST_DURATION = 5000; // 5 seconds
const int IMMORTALITY_DURATION = 10000; // 10 seconds
const int SPEED_BOOST_COST = 50;
const int IMMORTALITY_COST = 100;
const int ROCKET_COST = 75;

// Global SDL variables
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern Mix_Music* backgroundMusic;
extern bool isMusicPlaying;

// Texture variables
extern SDL_Texture* playerTexture;
extern SDL_Texture* planeTexture;
extern SDL_Texture* spikeTexture;
extern SDL_Texture* tallObstacleTexture;
extern SDL_Texture* movingObstacleTexture;
extern SDL_Texture* gapTexture;
extern SDL_Texture* groundTexture;
extern std::vector<SDL_Texture*> backgroundTextures;
extern SDL_Texture* birdTexture;
extern SDL_Texture* powerUpTexture;
extern SDL_Texture* scoreBoosterTexture;
extern SDL_Texture* coinTexture;
extern SDL_Texture* rocketTexture;
extern SDL_Texture* speedBoostTexture;

// Menu Rects
extern SDL_Rect menuTitleRect;
extern SDL_Rect menuStartRect;
extern SDL_Rect menuHighScoreRect;
extern SDL_Rect menuStoreRect;
extern SDL_Rect menuExitRect;
extern SDL_Rect highScoreScreenRect;
extern SDL_Rect storeTitleRect;
extern SDL_Rect storeCoinCountRect;
extern SDL_Rect storeRocketRect;
extern SDL_Rect storeImmortalRect;
extern SDL_Rect storeSpeedBoostRect;
extern SDL_Rect storeBackRect;

// Menu Textures
extern SDL_Texture* menuTitleTexture;
extern SDL_Texture* menuStartTexture;
extern SDL_Texture* menuHighScoreTexture;
extern SDL_Texture* menuStoreTexture;
extern SDL_Texture* menuExitTexture;
extern SDL_Texture* highScoreScreenTexture;
extern SDL_Texture* storeTitleTexture;
extern SDL_Texture* storeRocketTexture;
extern SDL_Texture* storeImmortalTexture;
extern SDL_Texture* storeSpeedBoostTexture;
extern SDL_Texture* storeBackTexture;
extern SDL_Texture* storeCoinCountTexture;

// Game State Enum
enum class GameState { MENU, STORE, PLAYING, HIGH_SCORE, EXIT };
extern GameState gameState;

// Game Variables
extern int coinCount;
extern std::string feedbackMessage;
extern Uint32 feedbackTimer;
extern bool isJumping;
extern bool hasDownJumped;
extern bool isSliding;
extern bool isFlying;
extern Uint32 slideTimer;
extern int groundY;
extern Uint32 flyingTimer;
extern int initialPlayerX;
extern bool isImmortal;
extern Uint32 immortalityTimer;
extern bool justGotImmortal;
extern bool isSpeedBoosted;
extern Uint32 speedBoostTimerValue;
extern bool justGotSpeedBoost;
extern float playerVelocityY;
extern int jumpCount;
extern bool spacePressedLastFrame;

#endif // GLOBALS_H