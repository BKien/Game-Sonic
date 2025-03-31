#include "Globals.h"

// SDL Variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;
Mix_Music* backgroundMusic = nullptr;
bool isMusicPlaying = false;

// Texture Variables
SDL_Texture* playerTexture = nullptr;
SDL_Texture* planeTexture = nullptr;
SDL_Texture* spikeTexture = nullptr;
SDL_Texture* tallObstacleTexture = nullptr;
SDL_Texture* movingObstacleTexture = nullptr;
SDL_Texture* gapTexture = nullptr;
SDL_Texture* groundTexture = nullptr;
std::vector<SDL_Texture*> backgroundTextures;
SDL_Texture* birdTexture = nullptr;
SDL_Texture* powerUpTexture = nullptr;
SDL_Texture* scoreBoosterTexture = nullptr;
SDL_Texture* coinTexture = nullptr;
SDL_Texture* rocketTexture = nullptr;
SDL_Texture* speedBoostTexture = nullptr;

// Menu Rects
SDL_Rect menuTitleRect;
SDL_Rect menuStartRect;
SDL_Rect menuHighScoreRect;
SDL_Rect menuStoreRect;
SDL_Rect menuExitRect;
SDL_Rect highScoreScreenRect;
SDL_Rect storeTitleRect;
SDL_Rect storeCoinCountRect;
SDL_Rect storeRocketRect;
SDL_Rect storeImmortalRect;
SDL_Rect storeSpeedBoostRect;
SDL_Rect storeBackRect;

// Menu Textures
SDL_Texture* menuTitleTexture = nullptr;
SDL_Texture* menuStartTexture = nullptr;
SDL_Texture* menuHighScoreTexture = nullptr;
SDL_Texture* menuStoreTexture = nullptr;
SDL_Texture* menuExitTexture = nullptr;
SDL_Texture* highScoreScreenTexture = nullptr;
SDL_Texture* storeTitleTexture = nullptr;
SDL_Texture* storeRocketTexture = nullptr;
SDL_Texture* storeImmortalTexture = nullptr;
SDL_Texture* storeSpeedBoostTexture = nullptr;
SDL_Texture* storeBackTexture = nullptr;
SDL_Texture* storeCoinCountTexture = nullptr;

// Game State
GameState gameState = GameState::MENU;

// Game Variables
int coinCount = 0;
std::string feedbackMessage;
Uint32 feedbackTimer = 0;
bool isJumping = false;
bool hasDownJumped = false;
bool isSliding = false;
bool isFlying = false;
Uint32 slideTimer = 0;
int groundY = SCREEN_HEIGHT - 100;
Uint32 flyingTimer = 0;
int initialPlayerX = 100;
bool isImmortal = false;
Uint32 immortalityTimer = 0;
bool justGotImmortal = false;
bool isSpeedBoosted = false;
Uint32 speedBoostTimerValue = 0;
bool justGotSpeedBoost = false;
float playerVelocityY = 0.0f;
int jumpCount = 0;
bool spacePressedLastFrame = false;