#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPIKE_SIZE = 50;
const int TALL_OBSTACLE_WIDTH = 50;
const int TALL_OBSTACLE_HEIGHT = 80;
const int MOVING_OBSTACLE_SIZE = 32;
const int GAP_WIDTH = 50;
const int GAP_HEIGHT = 20;
const int BIRD_SIZE = 50;
const int COLLECTABLE_SIZE = 15;
const float BACKGROUND_CHANGE_DURATION = 30.0f;
const int FRAME_WIDTH = 24;
const int FRAME_HEIGHT = 32;
const int FRAME_COUNT = 4;
const int PLANE_WIDTH = 64;
const int PLANE_HEIGHT = 50;
const float FRAME_TIME = 0.1f;
const float SPEED_BOOST_ANIMATION_FACTOR = 1.5f;
const int SLIDE_HEIGHT = 16;
const float SLIDE_DURATION = 0.5f;
const float IMMORTALITY_DURATION = 10.0f;
const float FLYING_DURATION = 8.0f;
const float FLYING_HEIGHT = 250.0f;
const float SPEED_BOOST_DURATION = 5.0f;
const float SPEED_BOOST_FACTOR = 1.5f;
const float MAX_SPEED_MULTIPLIER = 2.5f;
const int ROCKET_COST = 10;
const int IMMORTAL_COST = 15;
const int SPEED_BOOST_COST = 12;
const float FLASH_INTERVAL = 0.25f;
const float FEEDBACK_DURATION = 2.0f;
const float GLOW_PULSE_SPEED = 5.0f;

// Global variables
extern Mix_Music* backgroundMusic;
extern bool isMusicPlaying;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* playerTexture;
extern SDL_Texture* planeTexture;
extern SDL_Texture* spikeTexture;
extern SDL_Texture* tallObstacleTexture;
extern SDL_Texture* movingObstacleTexture;
extern SDL_Texture* gapTexture;
extern SDL_Texture* groundTexture;
extern std::vector<SDL_Texture*> backgroundTextures;
extern int currentBackgroundTextureIndex;
extern float backgroundChangeTimer;
extern SDL_Texture* birdTexture;
extern SDL_Texture* powerUpTexture;
extern SDL_Texture* scoreBoosterTexture;
extern SDL_Texture* coinTexture;
extern SDL_Texture* rocketTexture;
extern SDL_Texture* speedBoostTexture;
extern TTF_Font* font;
extern SDL_Texture* scoreTexture;
extern SDL_Rect scoreRect;
extern SDL_Texture* immortalTimerTexture;
extern SDL_Rect immortalTimerRect;
extern SDL_Texture* coinCountTexture;
extern SDL_Rect coinCountRect;
extern SDL_Texture* flyingTimerTexture;
extern SDL_Rect flyingTimerRect;
extern SDL_Texture* speedBoostTimerTexture;
extern SDL_Rect speedBoostTimerRect;
extern SDL_Texture* rocketInventoryTexture;
extern SDL_Rect rocketInventoryRect;
extern SDL_Texture* immortalInventoryTexture;
extern SDL_Rect immortalInventoryRect;
extern SDL_Texture* speedBoostInventoryTexture;
extern SDL_Rect speedBoostInventoryRect;
extern std::string feedbackMessage;
extern float feedbackTimer;
extern SDL_Texture* feedbackTexture;
extern SDL_Rect feedbackRect;
// Define enum class without extern
enum class GameState { MENU, STORE, PLAYING, HIGH_SCORE, EXIT };
extern GameState gameState;
extern int selectedMenuItem;
extern SDL_Texture* menuTitleTexture;
extern SDL_Texture* menuStartTexture;
extern SDL_Texture* menuHighScoreTexture;
extern SDL_Texture* menuStoreTexture;
extern SDL_Texture* menuExitTexture;
extern SDL_Rect menuTitleRect, menuStartRect, menuHighScoreRect, menuStoreRect, menuExitRect;
extern int highScore;
extern SDL_Texture* highScoreScreenTexture;
extern SDL_Rect highScoreScreenRect;
extern int selectedStoreItem;
extern SDL_Texture* storeTitleTexture;
extern SDL_Texture* storeRocketTexture;
extern SDL_Texture* storeImmortalTexture;
extern SDL_Texture* storeSpeedBoostTexture;
extern SDL_Texture* storeBackTexture;
extern SDL_Rect storeTitleRect, storeRocketRect, storeImmortalRect, storeSpeedBoostRect, storeBackRect;
extern SDL_Texture* storeCoinCountTexture;
extern SDL_Rect storeCoinCountRect;
extern int rocketCount;
extern int immortalCount;
extern int speedBoostCount;
extern float flashTimer;
extern bool playerVisible;

// Particle struct
struct Particle {
    float x, y;
    float velocityX, velocityY;
    float lifetime;
    float maxLifetime;
    SDL_Color color;
};
extern std::vector<Particle> particles;

// Obstacle enum and struct
enum class ObstacleType {
    SPIKE, TALL, MOVING, GAP, BIRD, POWERUP, SCORE_BOOSTER, COIN, ROCKET, SPEED_BOOST
};

struct Entity {
    float x, y;
    int w, h;
    bool active;
    ObstacleType type;
    float velocityY;
    float velocityX;
};

extern Entity player;
extern std::vector<Entity> obstacles;
extern float groundY;
extern float groundX;
extern float playerVelocityY;
extern bool isJumping;
extern int jumpCount;
extern bool spacePressedLastFrame;
extern bool isSliding;
extern bool hasDownJumped;
extern bool isImmortal;
extern float immortalityTimer;
extern float baseScrollSpeed;
extern float scrollSpeed;
extern float speedMultiplier;
extern float obstacleTimer;
extern float powerUpTimer;
extern float scoreBoosterTimer;
extern float coinTimer;
extern float rocketTimer;
extern float speedBoostTimer;
extern int score;
extern int coinCount;
extern float scoreTimer;
extern bool isFlying;
extern float flyingTimer;
extern float initialPlayerX;
extern bool isSpeedBoosted;
extern float speedBoostTimerValue;
extern int currentFrame;
extern float animationTimer;
extern float slideTimer;
extern bool justGotImmortal;
extern bool justGotSpeedBoost;
extern float glowTimer;
extern bool wasOnGroundLastFrame;

// Function prototypes
void initGame();
void updateGame();
void render();
void handleMenuInput(SDL_Event& event);
void handleStoreInput(SDL_Event& event);
void handleHighScoreInput(SDL_Event& event);
void handleInput(SDL_Event& event);
void renderMenu();
void renderStore();
void renderHighScore();
SDL_Texture* loadTexture(const char* path);
SDL_Texture* renderText(const std::string& text, SDL_Color color);
void spawnParticles(float centerX, float centerY, SDL_Color color);
void spawnObstacle();

#endif