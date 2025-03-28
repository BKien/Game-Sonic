#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath> // For particle angle calculations and glow pulsing

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Define obstacle sizes as constants for easier adjustment
const int SPIKE_SIZE = 50;
const int TALL_OBSTACLE_WIDTH = 50;
const int TALL_OBSTACLE_HEIGHT = 80;
const int MOVING_OBSTACLE_SIZE = 32; // Adjust to match moving_obstacle.png dimensions
const int GAP_WIDTH = 50;
const int GAP_HEIGHT = 20;
const int BIRD_SIZE = 50;
const int COLLECTABLE_SIZE = 15; // For coins, power-ups, score boosters, rockets, speed boosts

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* planeTexture = nullptr;
SDL_Texture* spikeTexture = nullptr;
SDL_Texture* tallObstacleTexture = nullptr;
SDL_Texture* movingObstacleTexture = nullptr;
SDL_Texture* gapTexture = nullptr;
SDL_Texture* groundTexture = nullptr;
std::vector<SDL_Texture*> backgroundTextures; // Vector for multiple background textures
int currentBackgroundTextureIndex = 0; // Track current background texture
float backgroundChangeTimer = 0; // Timer for background change
const float BACKGROUND_CHANGE_DURATION = 30.0f; // Change background every 30 seconds
SDL_Texture* birdTexture = nullptr;
SDL_Texture* powerUpTexture = nullptr;
SDL_Texture* scoreBoosterTexture = nullptr;
SDL_Texture* coinTexture = nullptr;
SDL_Texture* rocketTexture = nullptr;
SDL_Texture* speedBoostTexture = nullptr;
TTF_Font* font = nullptr;
SDL_Texture* scoreTexture = nullptr;
SDL_Rect scoreRect;
SDL_Texture* immortalTimerTexture = nullptr;
SDL_Rect immortalTimerRect;
SDL_Texture* coinCountTexture = nullptr;
SDL_Rect coinCountRect;
SDL_Texture* flyingTimerTexture = nullptr;
SDL_Rect flyingTimerRect;
SDL_Texture* speedBoostTimerTexture = nullptr;
SDL_Rect speedBoostTimerRect;

// Menu variables
enum class GameState {
    MENU,
    STORE, // New state for the store
    PLAYING,
    HIGH_SCORE,
    EXIT
};
GameState gameState = GameState::MENU;
int selectedMenuItem = 0; // 0: Start Game, 1: High Score, 2: Store, 3: Exit
SDL_Texture* menuTitleTexture = nullptr;
SDL_Texture* menuStartTexture = nullptr;
SDL_Texture* menuHighScoreTexture = nullptr;
SDL_Texture* menuStoreTexture = nullptr; // New texture for store option
SDL_Texture* menuExitTexture = nullptr;
SDL_Rect menuTitleRect, menuStartRect, menuHighScoreRect, menuStoreRect, menuExitRect;
int highScore = 0; // Store the high score
SDL_Texture* highScoreScreenTexture = nullptr;
SDL_Rect highScoreScreenRect;

// Store variables
int selectedStoreItem = 0; // 0: Rocket, 1: Immortal, 2: Speed Boost, 3: Back
SDL_Texture* storeTitleTexture = nullptr;
SDL_Texture* storeRocketTexture = nullptr;
SDL_Texture* storeImmortalTexture = nullptr;
SDL_Texture* storeSpeedBoostTexture = nullptr;
SDL_Texture* storeBackTexture = nullptr;
SDL_Rect storeTitleRect, storeRocketRect, storeImmortalRect, storeSpeedBoostRect, storeBackRect;
SDL_Texture* storeCoinCountTexture = nullptr;
SDL_Rect storeCoinCountRect;

// Store item costs
const int ROCKET_COST = 10;
const int IMMORTAL_COST = 15;
const int SPEED_BOOST_COST = 12;

// Player inventory for store items
int rocketCount = 0;
int immortalCount = 0;
int speedBoostCount = 0;

// Flashing effect variables
float flashTimer = 0;
const float FLASH_INTERVAL = 0.25f; // Flash every 0.25 seconds
bool playerVisible = true; // Toggle player visibility for flashing

// Particle struct for burst effect
struct Particle {
    float x, y; // Position
    float velocityX, velocityY; // Velocity
    float lifetime; // Lifetime in seconds
    float maxLifetime; // Initial lifetime for fading
    SDL_Color color; // Particle color
};

std::vector<Particle> particles; // Store active particles

enum class ObstacleType {
    SPIKE,
    TALL,
    MOVING,
    GAP,
    BIRD,
    POWERUP,
    SCORE_BOOSTER,
    COIN,
    ROCKET,
    SPEED_BOOST
};

struct Entity {
    float x, y;
    int w, h;
    bool active;
    ObstacleType type;
    float velocityY;
    float velocityX;
};

Entity player;
std::vector<Entity> obstacles;
float groundY = SCREEN_HEIGHT - 50;
float groundX = 0;
float playerVelocityY = 0;
bool isJumping = false;
int jumpCount = 0;
bool spacePressedLastFrame = false;
bool isSliding = false;
bool hasDownJumped = false;
bool isImmortal = false;
float immortalityTimer = 0;
const float IMMORTALITY_DURATION = 10.0f;
float baseScrollSpeed = 6.0f;
float scrollSpeed = baseScrollSpeed;
float speedMultiplier = 0;
const float MAX_SPEED_MULTIPLIER = 2.5f;
float obstacleTimer = 0;
float powerUpTimer = 0;
float scoreBoosterTimer = 0;
float coinTimer = 0;
float rocketTimer = 0;
float speedBoostTimer = 0;
int score = 0;
int coinCount = 0;
float scoreTimer = 0;
bool isFlying = false;
float flyingTimer = 0;
const float FLYING_DURATION = 8.0f;
const float FLYING_HEIGHT = 150.0f;
float initialPlayerX = 0;
bool isSpeedBoosted = false;
float speedBoostTimerValue = 0;
const float SPEED_BOOST_DURATION = 5.0f;
const float SPEED_BOOST_FACTOR = 1.5f;

// Animation variables
const int FRAME_WIDTH = 24;
const int FRAME_HEIGHT = 32;
const int FRAME_COUNT = 4;
const int PLANE_WIDTH = 64;
const int PLANE_HEIGHT = 50;
int currentFrame = 0;
float animationTimer = 0;
const float FRAME_TIME = 0.1f; // Base frame time
const float SPEED_BOOST_ANIMATION_FACTOR = 1.5f; // Animation speed multiplier during speed boost

// Sliding variables
const int SLIDE_HEIGHT = 16;
float slideTimer = 0;
const float SLIDE_DURATION = 0.5f;

// Flags to prevent multiple particle spawns for the same power-up
bool justGotImmortal = false;
bool justGotSpeedBoost = false;

// Glow effect variables
float glowTimer = 0; // Timer for pulsing effect
const float GLOW_PULSE_SPEED = 5.0f; // Speed of the pulsing effect

// Flag to prevent spamming the "Landed" message
bool wasOnGroundLastFrame = false; // Add this variable to track if the player was on the ground last frame

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - Error: " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* renderText(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to render text: " << TTF_GetError() << "\n";
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void spawnParticles(float centerX, float centerY, SDL_Color color) {
    const int NUM_PARTICLES = 20; // Number of particles to spawn
    const float PARTICLE_LIFETIME = 1.0f; // Lifetime of each particle in seconds
    const float PARTICLE_SPEED = 2.0f; // Speed of particles

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Particle particle;
        particle.x = centerX;
        particle.y = centerY;
        particle.maxLifetime = PARTICLE_LIFETIME;
        particle.lifetime = PARTICLE_LIFETIME;
        particle.color = color;

        // Calculate angle for circular distribution (0 to 2π)
        float angle = (float)(rand() % 360) * (M_PI / 180.0f);
        particle.velocityX = cos(angle) * PARTICLE_SPEED;
        particle.velocityY = sin(angle) * PARTICLE_SPEED;

        particles.push_back(particle);
    }
}

void initGame() {
    player = {100, groundY - FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT, true, ObstacleType::SPIKE, 0, 0};
    obstacles.clear();
    particles.clear(); // Clear particles on game reset
    groundX = 0;
    currentFrame = 0;
    animationTimer = 0;
    // Update high score if current score is higher
    if (score > highScore) {
        highScore = score;
    }
    score = 0;
    scoreTimer = 0;
    speedMultiplier = 0;
    scrollSpeed = baseScrollSpeed;
    jumpCount = 0;
    spacePressedLastFrame = false;
    isSliding = false;
    slideTimer = 0;
    hasDownJumped = false;
    isImmortal = false;
    immortalityTimer = 0;
    powerUpTimer = 0;
    scoreBoosterTimer = 0;
    coinTimer = 0;
    rocketTimer = 0;
    speedBoostTimer = 0;
    isFlying = false;
    flyingTimer = 0;
    initialPlayerX = player.x;
    isSpeedBoosted = false;
    speedBoostTimerValue = 0;
    justGotImmortal = false; // Reset flags
    justGotSpeedBoost = false;
    glowTimer = 0; // Reset glow timer
    flashTimer = 0; // Reset flash timer
    playerVisible = true; // Ensure player is visible
    wasOnGroundLastFrame = false; // Reset landing flag

    // Reset background change variables
    backgroundChangeTimer = 0;
    currentBackgroundTextureIndex = 0;

    scoreRect = {10, 10, 0, 0};
    immortalTimerRect = {10, 40, 0, 0};
    coinCountRect = {10, 70, 0, 0};
    flyingTimerRect = {10, 100, 0, 0};
    speedBoostTimerRect = {10, 130, 0, 0};
}

void spawnObstacle() {
    ObstacleType type = static_cast<ObstacleType>(rand() % 9);
    Entity obstacle;

    switch (type) {
        case ObstacleType::SPIKE:
            obstacle = {SCREEN_WIDTH, groundY - 15, SPIKE_SIZE, SPIKE_SIZE, true, ObstacleType::SPIKE, 0, 0};
            break;
        case ObstacleType::TALL:
            obstacle = {SCREEN_WIDTH, groundY - 80, TALL_OBSTACLE_WIDTH, TALL_OBSTACLE_HEIGHT, true, ObstacleType::TALL, 0, 0};
            break;
        case ObstacleType::MOVING:
            obstacle = {SCREEN_WIDTH, groundY - 150, MOVING_OBSTACLE_SIZE, MOVING_OBSTACLE_SIZE, true, ObstacleType::MOVING, 3.0f, 0};
            break;
        case ObstacleType::GAP:
            obstacle = {SCREEN_WIDTH, groundY, GAP_WIDTH, GAP_HEIGHT, true, ObstacleType::GAP, 0, 0};
            break;
        case ObstacleType::BIRD:
            obstacle = {SCREEN_WIDTH, groundY - (rand() % 100 + 50), BIRD_SIZE, BIRD_SIZE, true, ObstacleType::BIRD, 0, -(scrollSpeed + 2.0f)};
            break;
        case ObstacleType::POWERUP:
            obstacle = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                       COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::POWERUP, 0, -scrollSpeed};
            break;
        case ObstacleType::SCORE_BOOSTER:
            obstacle = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                       COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SCORE_BOOSTER, 0, -scrollSpeed};
            break;
        case ObstacleType::COIN:
            obstacle = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                       COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::COIN, 0, 0};
            break;
        case ObstacleType::ROCKET:
            obstacle = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                       COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::ROCKET, 0, -scrollSpeed};
            break;
        case ObstacleType::SPEED_BOOST:
            obstacle = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                       COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SPEED_BOOST, 0, -scrollSpeed};
            break;
    }

    obstacles.push_back(obstacle);

    if (rand() % 100 < 30) {
        ObstacleType secondType = static_cast<ObstacleType>(rand() % 9);
        Entity secondObstacle;

        switch (secondType) {
            case ObstacleType::SPIKE:
                secondObstacle = {SCREEN_WIDTH + 20, groundY - 50, SPIKE_SIZE, SPIKE_SIZE, true, ObstacleType::SPIKE, 0, 0};
                break;
            case ObstacleType::TALL:
                secondObstacle = {SCREEN_WIDTH + 20, groundY - 80, TALL_OBSTACLE_WIDTH, TALL_OBSTACLE_HEIGHT, true, ObstacleType::TALL, 0, 0};
                break;
            case ObstacleType::MOVING:
                secondObstacle = {SCREEN_WIDTH + 20, groundY - 150, MOVING_OBSTACLE_SIZE, MOVING_OBSTACLE_SIZE, true, ObstacleType::MOVING, 3.0f, 0};
                break;
            case ObstacleType::GAP:
                secondObstacle = {SCREEN_WIDTH + 20, groundY, GAP_WIDTH, GAP_HEIGHT, true, ObstacleType::GAP, 0, 0};
                break;
            case ObstacleType::BIRD:
                secondObstacle = {SCREEN_WIDTH + 20, groundY - (rand() % 100 + 50), BIRD_SIZE, BIRD_SIZE, true, ObstacleType::BIRD, 0, -(scrollSpeed + 2.0f)};
                break;
            case ObstacleType::POWERUP:
                secondObstacle = {SCREEN_WIDTH + 20, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                                 COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::POWERUP, 0, -scrollSpeed};
                break;
            case ObstacleType::SCORE_BOOSTER:
                secondObstacle = {SCREEN_WIDTH + 20, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                                 COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SCORE_BOOSTER, 0, -scrollSpeed};
                break;
            case ObstacleType::COIN:
                secondObstacle = {SCREEN_WIDTH + 20, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                                 COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::COIN, 0, -scrollSpeed};
                break;
            case ObstacleType::ROCKET:
                secondObstacle = {SCREEN_WIDTH + 20, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                                 COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::ROCKET, 0, -scrollSpeed};
                break;
            case ObstacleType::SPEED_BOOST:
                secondObstacle = {SCREEN_WIDTH + 20, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                                 COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SPEED_BOOST, 0, -scrollSpeed};
                break;
        }

        obstacles.push_back(secondObstacle);
    }
}

void handleMenuInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        gameState = GameState::EXIT;
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
            selectedMenuItem--;
            if (selectedMenuItem < 0) selectedMenuItem = 3; // Wrap around to "Exit"
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            selectedMenuItem++;
            if (selectedMenuItem > 3) selectedMenuItem = 0; // Wrap around to "Start Game"
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            if (selectedMenuItem == 0) { // Start Game
                gameState = GameState::PLAYING;
                initGame();
            } else if (selectedMenuItem == 1) { // High Score
                gameState = GameState::HIGH_SCORE;
            } else if (selectedMenuItem == 2) { // Store
                gameState = GameState::STORE;
                selectedStoreItem = 0; // Reset store selection
            } else if (selectedMenuItem == 3) { // Exit
                gameState = GameState::EXIT;
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Check if Start Game is clicked
        if (mouseX >= menuStartRect.x && mouseX <= menuStartRect.x + menuStartRect.w &&
            mouseY >= menuStartRect.y && mouseY <= menuStartRect.y + menuStartRect.h) {
            gameState = GameState::PLAYING;
            initGame();
        }
        // Check if High Score is clicked
        else if (mouseX >= menuHighScoreRect.x && mouseX <= menuHighScoreRect.x + menuHighScoreRect.w &&
                 mouseY >= menuHighScoreRect.y && mouseY <= menuHighScoreRect.y + menuHighScoreRect.h) {
            gameState = GameState::HIGH_SCORE;
        }
        // Check if Store is clicked
        else if (mouseX >= menuStoreRect.x && mouseX <= menuStoreRect.x + menuStoreRect.w &&
                 mouseY >= menuStoreRect.y && mouseY <= menuStoreRect.y + menuStoreRect.h) {
            gameState = GameState::STORE;
            selectedStoreItem = 0; // Reset store selection
        }
        // Check if Exit is clicked
        else if (mouseX >= menuExitRect.x && mouseX <= menuExitRect.x + menuExitRect.w &&
                 mouseY >= menuExitRect.y && mouseY <= menuExitRect.y + menuExitRect.h) {
            gameState = GameState::EXIT;
        }
    }
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        // Highlight menu item on hover
        if (mouseX >= menuStartRect.x && mouseX <= menuStartRect.x + menuStartRect.w &&
            mouseY >= menuStartRect.y && mouseY <= menuStartRect.y + menuStartRect.h) {
            selectedMenuItem = 0;
        } else if (mouseX >= menuHighScoreRect.x && mouseX <= menuHighScoreRect.x + menuHighScoreRect.w &&
                   mouseY >= menuHighScoreRect.y && mouseY <= menuHighScoreRect.y + menuHighScoreRect.h) {
            selectedMenuItem = 1;
        } else if (mouseX >= menuStoreRect.x && mouseX <= menuStoreRect.x + menuStoreRect.w &&
                   mouseY >= menuStoreRect.y && mouseY <= menuStoreRect.y + menuStoreRect.h) {
            selectedMenuItem = 2;
        } else if (mouseX >= menuExitRect.x && mouseX <= menuExitRect.x + menuExitRect.w &&
                   mouseY >= menuExitRect.y && mouseY <= menuExitRect.y + menuExitRect.h) {
            selectedMenuItem = 3;
        }
    }
}

void handleStoreInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        gameState = GameState::EXIT;
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
            selectedStoreItem--;
            if (selectedStoreItem < 0) selectedStoreItem = 3; // Wrap around to "Back"
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            selectedStoreItem++;
            if (selectedStoreItem > 3) selectedStoreItem = 0; // Wrap around to "Rocket"
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            if (selectedStoreItem == 0) { // Buy Rocket
                if (coinCount >= ROCKET_COST) {
                    coinCount -= ROCKET_COST;
                    rocketCount++;
                    std::cout << "Purchased Rocket! Rocket Count: " << rocketCount << "\n";
                } else {
                    std::cout << "Not enough coins to buy Rocket!\n";
                }
            } else if (selectedStoreItem == 1) { // Buy Immortal
                if (coinCount >= IMMORTAL_COST) {
                    coinCount -= IMMORTAL_COST;
                    immortalCount++;
                    std::cout << "Purchased Immortal! Immortal Count: " << immortalCount << "\n";
                } else {
                    std::cout << "Not enough coins to buy Immortal!\n";
                }
            } else if (selectedStoreItem == 2) { // Buy Speed Boost
                if (coinCount >= SPEED_BOOST_COST) {
                    coinCount -= SPEED_BOOST_COST;
                    speedBoostCount++;
                    std::cout << "Purchased Speed Boost! Speed Boost Count: " << speedBoostCount << "\n";
                } else {
                    std::cout << "Not enough coins to buy Speed Boost!\n";
                }
            } else if (selectedStoreItem == 3) { // Back
                gameState = GameState::MENU;
            }
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gameState = GameState::MENU;
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Check if Rocket is clicked
        if (mouseX >= storeRocketRect.x && mouseX <= storeRocketRect.x + storeRocketRect.w &&
            mouseY >= storeRocketRect.y && mouseY <= storeRocketRect.y + storeRocketRect.h) {
            if (coinCount >= ROCKET_COST) {
                coinCount -= ROCKET_COST;
                rocketCount++;
                std::cout << "Purchased Rocket! Rocket Count: " << rocketCount << "\n";
            } else {
                std::cout << "Not enough coins to buy Rocket!\n";
            }
        }
        // Check if Immortal is clicked
        else if (mouseX >= storeImmortalRect.x && mouseX <= storeImmortalRect.x + storeImmortalRect.w &&
                 mouseY >= storeImmortalRect.y && mouseY <= storeImmortalRect.y + storeImmortalRect.h) {
            if (coinCount >= IMMORTAL_COST) {
                coinCount -= IMMORTAL_COST;
                immortalCount++;
                std::cout << "Purchased Immortal! Immortal Count: " << immortalCount << "\n";
            } else {
                std::cout << "Not enough coins to buy Immortal!\n";
            }
        }
        // Check if Speed Boost is clicked
        else if (mouseX >= storeSpeedBoostRect.x && mouseX <= storeSpeedBoostRect.x + storeSpeedBoostRect.w &&
                 mouseY >= storeSpeedBoostRect.y && mouseY <= storeSpeedBoostRect.y + storeSpeedBoostRect.h) {
            if (coinCount >= SPEED_BOOST_COST) {
                coinCount -= SPEED_BOOST_COST;
                speedBoostCount++;
                std::cout << "Purchased Speed Boost! Speed Boost Count: " << speedBoostCount << "\n";
            } else {
                std::cout << "Not enough coins to buy Speed Boost!\n";
            }
        }
        // Check if Back is clicked
        else if (mouseX >= storeBackRect.x && mouseX <= storeBackRect.x + storeBackRect.w &&
                 mouseY >= storeBackRect.y && mouseY <= storeBackRect.y + storeBackRect.h) {
            gameState = GameState::MENU;
        }
    }
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        // Highlight store item on hover
        if (mouseX >= storeRocketRect.x && mouseX <= storeRocketRect.x + storeRocketRect.w &&
            mouseY >= storeRocketRect.y && mouseY <= storeRocketRect.y + storeRocketRect.h) {
            selectedStoreItem = 0;
        } else if (mouseX >= storeImmortalRect.x && mouseX <= storeImmortalRect.x + storeImmortalRect.w &&
                   mouseY >= storeImmortalRect.y && mouseY <= storeImmortalRect.y + storeImmortalRect.h) {
            selectedStoreItem = 1;
        } else if (mouseX >= storeSpeedBoostRect.x && mouseX <= storeSpeedBoostRect.x + storeSpeedBoostRect.w &&
                   mouseY >= storeSpeedBoostRect.y && mouseY <= storeSpeedBoostRect.y + storeSpeedBoostRect.h) {
            selectedStoreItem = 2;
        } else if (mouseX >= storeBackRect.x && mouseX <= storeBackRect.x + storeBackRect.w &&
                   mouseY >= storeBackRect.y && mouseY <= storeBackRect.y + storeBackRect.h) {
            selectedStoreItem = 3;
        }
    }
}

void handleHighScoreInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        gameState = GameState::EXIT;
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gameState = GameState::MENU;
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        gameState = GameState::MENU; // Click anywhere to return to menu
    }
}

void handleInput(SDL_Event& event) {
    bool spacePressedThisFrame = false;
    bool downPressedThisFrame = false;

    if (event.type == SDL_QUIT) {
        gameState = GameState::EXIT;
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            spacePressedThisFrame = true;
            std::cout << "Space pressed\n";
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && isJumping && !hasDownJumped) {
            downPressedThisFrame = true;
            std::cout << "Down pressed, initiating downward jump\n";
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_S && !isSliding && !isJumping && !isFlying) {
            isSliding = true;
            slideTimer = 0;
            player.h = SLIDE_HEIGHT;
            player.y = groundY - SLIDE_HEIGHT;
            std::cout << "Slide triggered\n";
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gameState = GameState::MENU; // Return to menu on ESC
            return;
        }
        // Use store items during gameplay
        if (event.key.keysym.scancode == SDL_SCANCODE_1 && rocketCount > 0 && !isFlying) {
            rocketCount--;
            isFlying = true;
            flyingTimer = 0;
            initialPlayerX = player.x;
            std::cout << "Used Rocket! Player is now flying. Rocket Count: " << rocketCount << "\n";
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_2 && immortalCount > 0 && !isImmortal) {
            immortalCount--;
            isImmortal = true;
            immortalityTimer = 0;
            if (!justGotImmortal) {
                spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 255, 0, 255});
                justGotImmortal = true;
            }
            std::cout << "Used Immortal! Player is now immortal. Immortal Count: " << immortalCount << "\n";
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_3 && speedBoostCount > 0 && !isSpeedBoosted) {
            speedBoostCount--;
            isSpeedBoosted = true;
            speedBoostTimerValue = 0;
            if (isImmortal) {
                isImmortal = false;
                immortalityTimer = 0;
                justGotImmortal = false;
                std::cout << "Immortality removed due to speed boost usage\n";
            }
            if (!justGotSpeedBoost) {
                spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 165, 0, 255});
                justGotSpeedBoost = true;
            }
            std::cout << "Used Speed Boost! Speed increased. Speed Boost Count: " << speedBoostCount << "\n";
        }
    }

    if (spacePressedThisFrame && !spacePressedLastFrame) {
        if (!isJumping && jumpCount == 0 && !isFlying) {
            std::cout << "First jump triggered, jumpCount = 1, velocity = -8.0\n";
            playerVelocityY = -8.0f;
            isJumping = true;
            jumpCount = 1;
            hasDownJumped = false;
        } else if (isJumping && jumpCount == 1 && !isFlying) {
            std::cout << "Double jump triggered, jumpCount = 2, velocity = -10.0\n";
            playerVelocityY = -10.0f;
            jumpCount = 2;
            hasDownJumped = false;
        }
    }

    if (downPressedThisFrame && !isSliding && isJumping && !hasDownJumped && !isFlying) {
        playerVelocityY = 8.0f;
        hasDownJumped = true;
    }

    spacePressedLastFrame = spacePressedThisFrame;
}

void updateGame() {
    // Update background change timer
    backgroundChangeTimer += 0.016f; // Assuming 60 FPS (16ms per frame)
    if (backgroundChangeTimer >= BACKGROUND_CHANGE_DURATION) {
        // Switch to the next background texture
        currentBackgroundTextureIndex = (currentBackgroundTextureIndex + 1) % backgroundTextures.size();
        // Reset the timer
        backgroundChangeTimer = 0;
    }

    // Update glow timer for pulsing effect
    glowTimer += 0.016f;

    // Update flash timer for flashing effect (for both speed boost and immortality)
    flashTimer += 0.016f;
    bool shouldFlash = false;

    // Check for flashing in speed boost state (last 2 seconds)
    if (isSpeedBoosted && speedBoostTimerValue >= SPEED_BOOST_DURATION - 2.0f) {
        shouldFlash = true;
        std::cout << "Flashing due to Speed Boost (last 2 seconds)\n";
    }

    // Check for flashing in immortal state (last 2 seconds)
    if (isImmortal && immortalityTimer >= IMMORTALITY_DURATION - 2.0f) {
        shouldFlash = true;
        std::cout << "Flashing due to Immortal (last 2 seconds)\n";
    }

    // Apply flashing if either condition is true
    if (shouldFlash) {
        if (flashTimer >= FLASH_INTERVAL) {
            playerVisible = !playerVisible; // Toggle visibility
            flashTimer = 0;
            std::cout << "Player visibility toggled: " << (playerVisible ? "Visible" : "Invisible") << "\n";
        }
    } else {
        playerVisible = true; // Ensure player is visible when not flashing
        flashTimer = 0;
    }

    // Update particles
    for (auto it = particles.begin(); it != particles.end();) {
        it->x += it->velocityX;
        it->y += it->velocityY;
        it->lifetime -= 0.016f; // Decrease lifetime
        if (it->lifetime <= 0) {
            it = particles.erase(it); // Remove expired particles
        } else {
            ++it;
        }
    }

    // Player physics (vertical movement)
    if (!isFlying) {
        player.y += playerVelocityY;
        playerVelocityY += 0.5f;
        if (player.y >= groundY - player.h) {
            player.y = groundY - player.h;
            playerVelocityY = 0;
            if (isJumping) { // Only reset and print message if the player was jumping
                isJumping = false;
                jumpCount = 0;
                hasDownJumped = false;
                std::cout << "Landed, jumpCount reset to 0\n";
            }
            scrollSpeed = baseScrollSpeed * speedMultiplier;
            if (isSpeedBoosted) {
                scrollSpeed *= SPEED_BOOST_FACTOR;
            }
            player.w = FRAME_WIDTH;
            player.h = FRAME_HEIGHT;
            wasOnGroundLastFrame = true; // Player is on the ground this frame
        } else {
            wasOnGroundLastFrame = false; // Player is not on the ground
        }
    } else {
        player.y = groundY - player.h - FLYING_HEIGHT;
        flyingTimer += 0.016f;
        if (flyingTimer >= FLYING_DURATION) {
            isFlying = false;
            flyingTimer = 0;
            playerVelocityY = 0;
            std::cout << "Flight ended, landing initiated\n";
            scrollSpeed = baseScrollSpeed * speedMultiplier;
            if (isSpeedBoosted) {
                scrollSpeed *= SPEED_BOOST_FACTOR;
            }
            player.w = FRAME_WIDTH;
            player.h = FRAME_HEIGHT;
        }
        if (player.y >= groundY - player.h) {
            player.y = groundY - player.h;
            player.x = initialPlayerX;
            isFlying = false;
            flyingTimer = 0;
            std::cout << "Landed at initial x position\n";
            scrollSpeed = baseScrollSpeed * speedMultiplier;
            if (isSpeedBoosted) {
                scrollSpeed *= SPEED_BOOST_FACTOR;
            }
            player.w = FRAME_WIDTH;
            player.h = FRAME_HEIGHT;
            wasOnGroundLastFrame = true; // Player is on the ground after landing
        } else {
            wasOnGroundLastFrame = false; // Player is flying
        }
        player.w = PLANE_WIDTH;
        player.h = PLANE_HEIGHT;
    }

    // Update sliding
    if (isSliding) {
        slideTimer += 0.016f;
        if (slideTimer >= SLIDE_DURATION) {
            isSliding = false;
            player.h = FRAME_HEIGHT;
            player.y = groundY - FRAME_HEIGHT;
        }
    }

    // Update immortality timer
    if (isImmortal) {
        immortalityTimer += 0.016f;
        if (immortalityTimer >= IMMORTALITY_DURATION) {
            isImmortal = false;
            immortalityTimer = 0;
            justGotImmortal = false; // Reset flag
            std::cout << "Immortality ended\n";
        }
    }

    // Update speed boost timer
    if (isSpeedBoosted) {
        speedBoostTimerValue += 0.016f;
        if (speedBoostTimerValue >= SPEED_BOOST_DURATION) {
            isSpeedBoosted = false;
            speedBoostTimerValue = 0;
            justGotSpeedBoost = false; // Reset flag
            scrollSpeed = baseScrollSpeed * speedMultiplier;
            std::cout << "Speed boost ended\n";
        }
    }

    // Update speed based on score and flight state
    speedMultiplier = 0.5f + (score / 10000.0f);
    if (speedMultiplier > MAX_SPEED_MULTIPLIER) {
        speedMultiplier = MAX_SPEED_MULTIPLIER;
    }
    scrollSpeed = baseScrollSpeed * speedMultiplier;
    if (isSpeedBoosted) {
        scrollSpeed *= SPEED_BOOST_FACTOR;
    }
    if (isFlying) {
        scrollSpeed *= 2.0f;
    }

    // Scroll ground
    groundX -= scrollSpeed;
    if (groundX <= -SCREEN_WIDTH) groundX = 0;

    // Spawn power-ups less frequently
    powerUpTimer += 0.00001f;
    if (powerUpTimer > 200000 && rand() % 100 < 0.05) {
        Entity powerUp = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                         COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::POWERUP, 0, -scrollSpeed};
        obstacles.push_back(powerUp);
        powerUpTimer = 0;
        std::cout << "Power-up spawned (less frequent)\n";
    }

    // Spawn score boosters
    scoreBoosterTimer += 0.0001f;
    if (scoreBoosterTimer > 30000 && rand() % 100 < 0.1) {
        Entity scoreBooster = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                             COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SCORE_BOOSTER, 0, -scrollSpeed};
        obstacles.push_back(scoreBooster);
        scoreBoosterTimer = 0;
        std::cout << "Score Booster spawned\n";
    }

    // Spawn coins more frequently
    coinTimer += 0.1f;
    if (coinTimer > 5 && rand() % 100 < 75) {
        Entity coin = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                      COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::COIN, 0, -scrollSpeed};
        obstacles.push_back(coin);
        coinTimer = 0;
        std::cout << "Coin spawned\n";
    }

    // Spawn rockets
    rocketTimer += 0.0000016f;
    if (rocketTimer > 30000000 && rand() % 10000000 < 5) {
        Entity rocket = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                        COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::ROCKET, 0, -scrollSpeed};
        obstacles.push_back(rocket);
        rocketTimer = 0;
        std::cout << "Rocket spawned\n";
    }

    // Spawn speed boosts
    speedBoostTimer += 0.0001f;
    if (speedBoostTimer > 250000 && rand() % 100 < 10) {
        Entity speedBoost = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                           COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SPEED_BOOST, 0, -scrollSpeed};
        obstacles.push_back(speedBoost);
        speedBoostTimer = 0;
        std::cout << "Speed Boost spawned\n";
    }

    // Spawn obstacles
    obstacleTimer += 0.1f;
    if (obstacleTimer > 10) {
        spawnObstacle();
        obstacleTimer = 0;
    }

    // Update obstacles and check collisions
    for (auto& obstacle : obstacles) {
        if (obstacle.active) {
            obstacle.x += obstacle.velocityX;
            if (obstacle.type == ObstacleType::MOVING) {
                obstacle.y += obstacle.velocityY;
                // Adjust bounds to account for obstacle height
                if (obstacle.y + obstacle.h < groundY - 180 || obstacle.y > groundY - 20) {
                    obstacle.velocityY = -obstacle.velocityY;
                }
                // Debug output to track movement
                std::cout << "Moving obstacle y: " << obstacle.y << ", velocityY: " << obstacle.velocityY << "\n";
            }

            // Allow MOVING obstacles to scroll left with the background
            if (obstacle.type != ObstacleType::COIN && obstacle.type != ObstacleType::ROCKET
                && obstacle.type != ObstacleType::POWERUP && obstacle.type != ObstacleType::SCORE_BOOSTER
                && obstacle.type != ObstacleType::SPEED_BOOST) {
                obstacle.x -= scrollSpeed;
            }

            // Collision detection
            bool collision = (player.x < obstacle.x + obstacle.w &&
                             player.x + player.w > obstacle.x &&
                             player.y < obstacle.y + obstacle.h &&
                             player.y + player.h > obstacle.y);

            if (obstacle.type == ObstacleType::POWERUP && collision) {
                obstacle.active = false;
                isImmortal = true;
                immortalityTimer = 0;
                if (!justGotImmortal) {
                    // Spawn yellow particles for immortality
                    spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 255, 0, 255});
                    justGotImmortal = true;
                }
                std::cout << "Power-up collected! Player is now immortal for " << IMMORTALITY_DURATION << " seconds\n";
            } else if (obstacle.type == ObstacleType::SCORE_BOOSTER && collision) {
                obstacle.active = false;
                score += 100;
                std::cout << "Score Booster collected! Score increased by 100. New score: " << score << "\n";
            } else if (obstacle.type == ObstacleType::COIN && collision) {
                obstacle.active = false;
                coinCount += 1;
                std::cout << "Coin collected! Total coins: " << coinCount << "\n";
            } else if (obstacle.type == ObstacleType::ROCKET && collision) {
                obstacle.active = false;
                isFlying = true;
                flyingTimer = 0;
                initialPlayerX = player.x;
                std::cout << "Rocket collected! Player is now flying at fixed height for " << FLYING_DURATION << " seconds\n";
            } else if (obstacle.type == ObstacleType::SPEED_BOOST && collision) {
                obstacle.active = false;
                isSpeedBoosted = true;
                speedBoostTimerValue = 0;
                // Remove immortality (shield) when speed boost is collected
                if (isImmortal) {
                    isImmortal = false;
                    immortalityTimer = 0;
                    justGotImmortal = false;
                    std::cout << "Immortality removed due to speed boost collection\n";
                }
                if (!justGotSpeedBoost) {
                    // Spawn orange particles for speed boost
                    spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 165, 0, 255});
                    justGotSpeedBoost = true;
                }
                std::cout << "Speed Boost collected! Speed increased for " << SPEED_BOOST_DURATION << " seconds\n";
            } else if (obstacle.type != ObstacleType::GAP && collision && !isImmortal) {
                gameState = GameState::MENU; // Return to menu on game over
                std::cout << "Collision with obstacle! Game reset\n";
            } else if (obstacle.type == ObstacleType::GAP && player.x < obstacle.x + obstacle.w &&
                       player.x + player.w > obstacle.x && player.y + player.h >= obstacle.y && !isJumping && !isFlying) {
                gameState = GameState::MENU; // Return to menu on game over
                std::cout << "Fell into gap! Game reset\n";
            }

            if (obstacle.x < -obstacle.w) obstacle.active = false;
        }
    }

    // Update score
    scoreTimer += 0.016f;
    if (scoreTimer >= 0.1f) {
        score += 10;
        scoreTimer = 0;
    }

    // Update animation
    if (!isJumping && !isFlying) {
        animationTimer += 0.016f;
        // Adjust frame time based on speed boost
        float effectiveFrameTime = FRAME_TIME;
        if (isSpeedBoosted) {
            effectiveFrameTime /= SPEED_BOOST_ANIMATION_FACTOR; // Faster animation during speed boost
        }
        if (animationTimer >= effectiveFrameTime) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
        }
    } else {
        currentFrame = 1;
    }
}

void renderMenu() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    // Draw background
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

    // Render menu title
    SDL_RenderCopy(renderer, menuTitleTexture, nullptr, &menuTitleRect);

    // Render menu options with selection highlight
    SDL_Color normalColor = {255, 255, 255, 255};
    SDL_Color selectedColor = {255, 165, 0, 255}; // Orange for selected item

    if (menuStartTexture) SDL_DestroyTexture(menuStartTexture);
    menuStartTexture = renderText("Start Game", selectedMenuItem == 0 ? selectedColor : normalColor);
    if (menuStartTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(menuStartTexture, nullptr, nullptr, &textWidth, &textHeight);
        menuStartRect.w = textWidth;
        menuStartRect.h = textHeight;
        SDL_RenderCopy(renderer, menuStartTexture, nullptr, &menuStartRect);
    }

    if (menuHighScoreTexture) SDL_DestroyTexture(menuHighScoreTexture);
    menuHighScoreTexture = renderText("High Score", selectedMenuItem == 1 ? selectedColor : normalColor);
    if (menuHighScoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(menuHighScoreTexture, nullptr, nullptr, &textWidth, &textHeight);
        menuHighScoreRect.w = textWidth;
        menuHighScoreRect.h = textHeight;
        SDL_RenderCopy(renderer, menuHighScoreTexture, nullptr, &menuHighScoreRect);
    }

    if (menuStoreTexture) SDL_DestroyTexture(menuStoreTexture);
    menuStoreTexture = renderText("Store", selectedMenuItem == 2 ? selectedColor : normalColor);
    if (menuStoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(menuStoreTexture, nullptr, nullptr, &textWidth, &textHeight);
        menuStoreRect.w = textWidth;
        menuStoreRect.h = textHeight;
        SDL_RenderCopy(renderer, menuStoreTexture, nullptr, &menuStoreRect);
    }

    if (menuExitTexture) SDL_DestroyTexture(menuExitTexture);
    menuExitTexture = renderText("Exit", selectedMenuItem == 3 ? selectedColor : normalColor);
    if (menuExitTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(menuExitTexture, nullptr, nullptr, &textWidth, &textHeight);
        menuExitRect.w = textWidth;
        menuExitRect.h = textHeight;
        SDL_RenderCopy(renderer, menuExitTexture, nullptr, &menuExitRect);
    }

    SDL_RenderPresent(renderer);
}

void renderStore() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    // Draw background
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

    // Render store title
    SDL_RenderCopy(renderer, storeTitleTexture, nullptr, &storeTitleRect);

    // Render coin count
    if (storeCoinCountTexture) SDL_DestroyTexture(storeCoinCountTexture);
    std::stringstream coinSs;
    coinSs << "Coins: " << coinCount;
    storeCoinCountTexture = renderText(coinSs.str(), {255, 215, 0, 255});
    if (storeCoinCountTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeCoinCountTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeCoinCountRect.w = textWidth;
        storeCoinCountRect.h = textHeight;
        SDL_RenderCopy(renderer, storeCoinCountTexture, nullptr, &storeCoinCountRect);
    }

    // Render store items with selection highlight
    SDL_Color normalColor = {255, 255, 255, 255};
    SDL_Color selectedColor = {255, 165, 0, 255}; // Orange for selected item

    if (storeRocketTexture) SDL_DestroyTexture(storeRocketTexture);
    std::stringstream rocketSs;
    rocketSs << "Rocket (" << ROCKET_COST << " coins) - " << rocketCount;
    storeRocketTexture = renderText(rocketSs.str(), selectedStoreItem == 0 ? selectedColor : normalColor);
    if (storeRocketTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeRocketTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeRocketRect.w = textWidth;
        storeRocketRect.h = textHeight;
        SDL_RenderCopy(renderer, storeRocketTexture, nullptr, &storeRocketRect);
    }

    if (storeImmortalTexture) SDL_DestroyTexture(storeImmortalTexture);
    std::stringstream immortalSs;
    immortalSs << "Immortal (" << IMMORTAL_COST << " coins) - " << immortalCount;
    storeImmortalTexture = renderText(immortalSs.str(), selectedStoreItem == 1 ? selectedColor : normalColor);
    if (storeImmortalTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeImmortalTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeImmortalRect.w = textWidth;
        storeImmortalRect.h = textHeight;
        SDL_RenderCopy(renderer, storeImmortalTexture, nullptr, &storeImmortalRect);
    }

    if (storeSpeedBoostTexture) SDL_DestroyTexture(storeSpeedBoostTexture);
    std::stringstream speedBoostSs;
    speedBoostSs << "Speed Boost (" << SPEED_BOOST_COST << " coins) - " << speedBoostCount;
    storeSpeedBoostTexture = renderText(speedBoostSs.str(), selectedStoreItem == 2 ? selectedColor : normalColor);
    if (storeSpeedBoostTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeSpeedBoostTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeSpeedBoostRect.w = textWidth;
        storeSpeedBoostRect.h = textHeight;
        SDL_RenderCopy(renderer, storeSpeedBoostTexture, nullptr, &storeSpeedBoostRect);
    }

    if (storeBackTexture) SDL_DestroyTexture(storeBackTexture);
    storeBackTexture = renderText("Back", selectedStoreItem == 3 ? selectedColor : normalColor);
    if (storeBackTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeBackTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeBackRect.w = textWidth;
        storeBackRect.h = textHeight;
        SDL_RenderCopy(renderer, storeBackTexture, nullptr, &storeBackRect);
    }

    SDL_RenderPresent(renderer);
}

void renderHighScore() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    // Draw background
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

    // Render high score
    if (highScoreScreenTexture) SDL_DestroyTexture(highScoreScreenTexture);
    std::stringstream ss;
    ss << "High Score: " << highScore;
    highScoreScreenTexture = renderText(ss.str(), {255, 255, 255, 255});
    if (highScoreScreenTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(highScoreScreenTexture, nullptr, nullptr, &textWidth, &textHeight);
        highScoreScreenRect.w = textWidth;
        highScoreScreenRect.h = textHeight;
        SDL_RenderCopy(renderer, highScoreScreenTexture, nullptr, &highScoreScreenRect);
    }

    SDL_RenderPresent(renderer);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    // Draw background
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

    // Draw scrolling ground with gaps
    for (int i = 0; i < SCREEN_WIDTH * 2; i += SCREEN_WIDTH) {
        bool drawGround = true;
        for (const auto& obstacle : obstacles) {
            if (obstacle.active && obstacle.type == ObstacleType::GAP) {
                if (i >= obstacle.x && i < obstacle.x + obstacle.w) {
                    drawGround = false;
                    break;
                }
            }
        }
        if (drawGround) {
            SDL_Rect groundRect = {(int)groundX + i, (int)groundY, SCREEN_WIDTH, SCREEN_HEIGHT - (int)groundY};
            SDL_RenderCopy(renderer, groundTexture, nullptr, &groundRect);
        }
    }

    // Draw particles
    for (const auto& particle : particles) {
        // Calculate alpha based on remaining lifetime (fade out effect)
        Uint8 alpha = (Uint8)(255 * (particle.lifetime / particle.maxLifetime));
        SDL_SetRenderDrawColor(renderer, particle.color.r, particle.color.g, particle.color.b, alpha);
        SDL_Rect particleRect = {(int)particle.x, (int)particle.y, 4, 4}; // Small square particle
        SDL_RenderFillRect(renderer, &particleRect);
    }

    // Draw glow effect around player only if immortal
    if (isImmortal) {
        SDL_Color glowColor = {255, 255, 0, 255}; // Yellow for immortality
        float remainingTime = IMMORTALITY_DURATION - immortalityTimer;
        float duration = IMMORTALITY_DURATION;

        // Calculate base alpha based on remaining time (fades as time runs out)
        Uint8 baseAlpha = (Uint8)(255 * (remainingTime / duration));

        // Add pulsing effect using sine wave
        float pulse = (sin(glowTimer * GLOW_PULSE_SPEED) + 1.0f) / 2.0f; // Oscillates between 0 and 1
        Uint8 alpha = (Uint8)(baseAlpha * (0.5f + 0.5f * pulse)); // Pulse between 50% and 100% of base alpha

        // Draw multiple layers of rectangles to create a glow effect
        SDL_SetRenderDrawColor(renderer, glowColor.r, glowColor.g, glowColor.b, alpha / 4);
        SDL_Rect glowRect = {(int)player.x - 8, (int)player.y - 8, player.w + 16, player.h + 16};
        SDL_RenderDrawRect(renderer, &glowRect); // Outermost layer (faint)
        
        SDL_SetRenderDrawColor(renderer, glowColor.r, glowColor.g, glowColor.b, alpha / 2);
        glowRect = {(int)player.x - 4, (int)player.y - 4, player.w + 8, player.h + 8};
        SDL_RenderDrawRect(renderer, &glowRect); // Middle layer
        
        SDL_SetRenderDrawColor(renderer, glowColor.r, glowColor.g, glowColor.b, alpha);
        glowRect = {(int)player.x - 2, (int)player.y - 2, player.w + 4, player.h + 4};
        SDL_RenderDrawRect(renderer, &glowRect); // Innermost layer (brightest)
    }

    // Draw player (only if visible due to flashing effect)
    if (playerVisible) {
        SDL_Rect playerSrc = {currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, isSliding ? SLIDE_HEIGHT : (isFlying ? PLANE_HEIGHT : FRAME_HEIGHT)};
        SDL_Rect playerDst = {(int)player.x, (int)player.y, player.w, isSliding ? SLIDE_HEIGHT : (isFlying ? PLANE_HEIGHT : player.h)};
        SDL_Texture* currentTexture = isFlying ? planeTexture : playerTexture;
        SDL_RenderCopy(renderer, currentTexture, &playerSrc, &playerDst);
    }

    // Draw obstacles, power-ups, score boosters, coins, rockets, and speed boosts
    for (const auto& obstacle : obstacles) {
        if (obstacle.active) {
            SDL_Rect obsRect = {(int)obstacle.x, (int)obstacle.y, obstacle.w, obstacle.h};
            SDL_Texture* texture = nullptr;

            switch (obstacle.type) {
                case ObstacleType::SPIKE:
                    texture = spikeTexture;
                    break;
                case ObstacleType::TALL:
                    texture = tallObstacleTexture;
                    break;
                case ObstacleType::MOVING:
                    texture = movingObstacleTexture;
                    break;
                case ObstacleType::GAP:
                    texture = gapTexture;
                    break;
                case ObstacleType::BIRD:
                    texture = birdTexture;
                    break;
                case ObstacleType::POWERUP:
                    texture = powerUpTexture;
                    break;
                case ObstacleType::SCORE_BOOSTER:
                    texture = scoreBoosterTexture;
                    break;
                case ObstacleType::COIN:
                    texture = coinTexture;
                    break;
                case ObstacleType::ROCKET:
                    texture = rocketTexture;
                    break;
                case ObstacleType::SPEED_BOOST:
                    texture = speedBoostTexture;
                    break;
            }

            if (texture) {
                SDL_RenderCopy(renderer, texture, nullptr, &obsRect);
            }
        }
    }

    // Render score
    std::stringstream ss;
    ss << "Score: " << score;
    SDL_Color textColor = {255, 255, 255, 255};
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    scoreTexture = renderText(ss.str(), textColor);

    if (scoreTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(scoreTexture, nullptr, nullptr, &textWidth, &textHeight);
        scoreRect.w = textWidth;
        scoreRect.h = textHeight;
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
    }

    // Render immortality timer
    if (isImmortal && immortalTimerTexture) SDL_DestroyTexture(immortalTimerTexture);
    if (isImmortal) {
        float remainingTime = IMMORTALITY_DURATION - immortalityTimer;
        std::stringstream timerSs;
        timerSs << "Immortal: " << std::fixed << std::setprecision(1) << remainingTime << "s";
        immortalTimerTexture = renderText(timerSs.str(), {255, 255, 0, 255});

        if (immortalTimerTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(immortalTimerTexture, nullptr, nullptr, &textWidth, &textHeight);
            immortalTimerRect.w = textWidth;
            immortalTimerRect.h = textHeight;
            SDL_RenderCopy(renderer, immortalTimerTexture, nullptr, &immortalTimerRect);
        }
    }

    // Render coin count
    if (coinCountTexture) SDL_DestroyTexture(coinCountTexture);
    std::stringstream coinSs;
    coinSs << "Coins: " << coinCount;
    coinCountTexture = renderText(coinSs.str(), {255, 215, 0, 255});

    if (coinCountTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(coinCountTexture, nullptr, nullptr, &textWidth, &textHeight);
        coinCountRect.w = textWidth;
        coinCountRect.h = textHeight;
        SDL_RenderCopy(renderer, coinCountTexture, nullptr, &coinCountRect);
    }

    // Render flying timer
    if (isFlying && flyingTimerTexture) SDL_DestroyTexture(flyingTimerTexture);
    if (isFlying) {
        float remainingTime = FLYING_DURATION - flyingTimer;
        std::stringstream timerSs;
        timerSs << "Flying: " << std::fixed << std::setprecision(1) << remainingTime << "s";
        flyingTimerTexture = renderText(timerSs.str(), {0, 255, 0, 255});

        if (flyingTimerTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(flyingTimerTexture, nullptr, nullptr, &textWidth, &textHeight);
            flyingTimerRect.w = textWidth;
            flyingTimerRect.h = textHeight;
            SDL_RenderCopy(renderer, flyingTimerTexture, nullptr, &flyingTimerRect);
        }
    }

    // Render speed boost timer
    if (isSpeedBoosted && speedBoostTimerTexture) SDL_DestroyTexture(speedBoostTimerTexture);
    if (isSpeedBoosted) {
        float remainingTime = SPEED_BOOST_DURATION - speedBoostTimerValue;
        std::stringstream timerSs;
        timerSs << "Speed Boost: " << std::fixed << std::setprecision(1) << remainingTime << "s";
        speedBoostTimerTexture = renderText(timerSs.str(), {255, 165, 0, 255});

        if (speedBoostTimerTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(speedBoostTimerTexture, nullptr, nullptr, &textWidth, &textHeight);
            speedBoostTimerRect.w = textWidth;
            speedBoostTimerRect.h = textHeight;
            SDL_RenderCopy(renderer, speedBoostTimerTexture, nullptr, &speedBoostTimerRect);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL Initialization failed!\n";
        return -1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf Initialization failed: " << TTF_GetError() << "\n";
        return -1;
    }

    window = SDL_CreateWindow("Subway Surfer 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load font
    font = TTF_OpenFont("D:/project/src/Font/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Load textures
    playerTexture = loadTexture("D:/project/src/img/sonic.png");
    planeTexture = loadTexture("D:/project/src/img/plane.png");
    spikeTexture = loadTexture("D:/project/src/img/spikes_2.png");
    tallObstacleTexture = loadTexture("D:/project/src/img/tallObstacleTexture.png");
    movingObstacleTexture = loadTexture("D:/project/src/img/moving_obstacle.png");
    gapTexture = loadTexture("D:/project/src/img/gap.jpg");
    groundTexture = loadTexture("D:/project/src/img/ground.jpg");
    backgroundTextures.push_back(loadTexture("D:/project/src/img/background.png")); // First background
    backgroundTextures.push_back(loadTexture("D:/project/src/img/background_2.jpg")); // Second background
    birdTexture = loadTexture("D:/project/src/img/blade_3.png");
    powerUpTexture = loadTexture("D:/project/src/img/powerup.png");
    scoreBoosterTexture = loadTexture("D:/project/src/img/scorebooster.png");
    coinTexture = loadTexture("D:/project/src/img/coin.png");
    rocketTexture = loadTexture("D:/project/src/img/rocket.png");
    speedBoostTexture = loadTexture("D:/project/src/img/speedboost.png");

    // Check if all textures loaded successfully
    if (!playerTexture || !planeTexture || !spikeTexture || !tallObstacleTexture || 
        !movingObstacleTexture || !gapTexture || !groundTexture || 
        !birdTexture || !powerUpTexture || !scoreBoosterTexture || 
        !coinTexture || !rocketTexture || !speedBoostTexture) {
        std::cerr << "Failed to load textures!\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    for (const auto& texture : backgroundTextures) {
        if (!texture) {
            std::cerr << "Failed to load a background texture!\n";
            SDL_DestroyTexture(playerTexture);
            SDL_DestroyTexture(planeTexture);
            SDL_DestroyTexture(spikeTexture);
            SDL_DestroyTexture(tallObstacleTexture);
            SDL_DestroyTexture(movingObstacleTexture);
            SDL_DestroyTexture(gapTexture);
            SDL_DestroyTexture(groundTexture);
            for (auto& tex : backgroundTextures) SDL_DestroyTexture(tex);
            SDL_DestroyTexture(birdTexture);
            SDL_DestroyTexture(powerUpTexture);
            SDL_DestroyTexture(scoreBoosterTexture);
            SDL_DestroyTexture(coinTexture);
            SDL_DestroyTexture(rocketTexture);
            SDL_DestroyTexture(speedBoostTexture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return -1;
        }
    }

    // Initialize menu positions
    menuTitleRect = {SCREEN_WIDTH / 2 - 100, 100, 0, 0};
    menuStartRect = {SCREEN_WIDTH / 2 - 50, 200, 0, 0};
    menuHighScoreRect = {SCREEN_WIDTH / 2 - 50, 250, 0, 0};
    menuStoreRect = {SCREEN_WIDTH / 2 - 50, 300, 0, 0};
    menuExitRect = {SCREEN_WIDTH / 2 - 50, 350, 0, 0};
    highScoreScreenRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 0, 0};

    // Initialize store positions
    storeTitleRect = {SCREEN_WIDTH / 2 - 50, 100, 0, 0};
    storeCoinCountRect = {SCREEN_WIDTH / 2 - 50, 150, 0, 0};
    storeRocketRect = {SCREEN_WIDTH / 2 - 100, 200, 0, 0};
    storeImmortalRect = {SCREEN_WIDTH / 2 - 100, 250, 0, 0};
    storeSpeedBoostRect = {SCREEN_WIDTH / 2 - 100, 300, 0, 0};
    storeBackRect = {SCREEN_WIDTH / 2 - 50, 350, 0, 0};

    // Render initial menu title
    menuTitleTexture = renderText("Subway Surfer 2D", {255, 255, 255, 255});
    if (menuTitleTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(menuTitleTexture, nullptr, nullptr, &textWidth, &textHeight);
        menuTitleRect.w = textWidth;
        menuTitleRect.h = textHeight;
    }

    // Render initial store title
    storeTitleTexture = renderText("Store", {255, 255, 255, 255});
    if (storeTitleTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeTitleTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeTitleRect.w = textWidth;
        storeTitleRect.h = textHeight;
    }

    bool running = true;
    SDL_Event event;
    const float FRAME_TIME = 0.016f; // Target 60 FPS (16ms per frame)
    Uint32 lastTime = SDL_GetTicks();
    float accumulator = 0.0f;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;

        // Process all events for this frame
        while (SDL_PollEvent(&event)) {
            switch (gameState) {
                case GameState::MENU:
                    handleMenuInput(event);
                    break;
                case GameState::STORE:
                    handleStoreInput(event);
                    break;
                case GameState::PLAYING:
                    handleInput(event);
                    break;
                case GameState::HIGH_SCORE:
                    handleHighScoreInput(event);
                    break;
                case GameState::EXIT:
                    running = false;
                    break;
            }
        }

        // Update and render based on game state
        switch (gameState) {
            case GameState::MENU:
                renderMenu();
                break;
            case GameState::STORE:
                renderStore();
                break;
            case GameState::PLAYING:
                // Fixed time step for updates
                accumulator += deltaTime;
                while (accumulator >= FRAME_TIME) {
                    updateGame();
                    accumulator -= FRAME_TIME;
                }
                render();
                break;
            case GameState::HIGH_SCORE:
                renderHighScore();
                break;
            case GameState::EXIT:
                running = false;
                break;
        }

        // Cap the frame rate to ~60 FPS
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
    }

    // Cleanup
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(planeTexture);
    SDL_DestroyTexture(spikeTexture);
    SDL_DestroyTexture(tallObstacleTexture);
    SDL_DestroyTexture(movingObstacleTexture);
    SDL_DestroyTexture(gapTexture);
    SDL_DestroyTexture(groundTexture);
    for (auto& texture : backgroundTextures) {
        SDL_DestroyTexture(texture);
    }
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(powerUpTexture);
    SDL_DestroyTexture(scoreBoosterTexture);
    SDL_DestroyTexture(coinTexture);
    SDL_DestroyTexture(rocketTexture);
    SDL_DestroyTexture(speedBoostTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(immortalTimerTexture);
    SDL_DestroyTexture(coinCountTexture);
    SDL_DestroyTexture(flyingTimerTexture);
    SDL_DestroyTexture(speedBoostTimerTexture);
    SDL_DestroyTexture(menuTitleTexture);
    SDL_DestroyTexture(menuStartTexture);
    SDL_DestroyTexture(menuHighScoreTexture);
    SDL_DestroyTexture(menuStoreTexture);
    SDL_DestroyTexture(menuExitTexture);
    SDL_DestroyTexture(highScoreScreenTexture);
    SDL_DestroyTexture(storeTitleTexture);
    SDL_DestroyTexture(storeRocketTexture);
    SDL_DestroyTexture(storeImmortalTexture);
    SDL_DestroyTexture(storeSpeedBoostTexture);
    SDL_DestroyTexture(storeBackTexture);
    SDL_DestroyTexture(storeCoinCountTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}