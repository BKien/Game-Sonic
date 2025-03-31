#include "game.h"

// Define global variables
Mix_Music* backgroundMusic = nullptr;
bool isMusicPlaying = false;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* planeTexture = nullptr;
SDL_Texture* spikeTexture = nullptr;
SDL_Texture* tallObstacleTexture = nullptr;
SDL_Texture* movingObstacleTexture = nullptr;
SDL_Texture* gapTexture = nullptr;
SDL_Texture* groundTexture = nullptr;
std::vector<SDL_Texture*> backgroundTextures;
int currentBackgroundTextureIndex = 0;
float backgroundChangeTimer = 0;
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
SDL_Texture* rocketInventoryTexture = nullptr;
SDL_Rect rocketInventoryRect = {SCREEN_WIDTH - 150, 10, 0, 0};
SDL_Texture* immortalInventoryTexture = nullptr;
SDL_Rect immortalInventoryRect = {SCREEN_WIDTH - 150, 40, 0, 0};
SDL_Texture* speedBoostInventoryTexture = nullptr;
SDL_Rect speedBoostInventoryRect = {SCREEN_WIDTH - 150, 70, 0, 0};
std::string feedbackMessage = "";
float feedbackTimer = 0;
SDL_Texture* feedbackTexture = nullptr;
SDL_Rect feedbackRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 50, 0, 0};
GameState gameState = GameState::MENU;
int selectedMenuItem = 0;
SDL_Texture* menuTitleTexture = nullptr;
SDL_Texture* menuStartTexture = nullptr;
SDL_Texture* menuHighScoreTexture = nullptr;
SDL_Texture* menuStoreTexture = nullptr;
SDL_Texture* menuExitTexture = nullptr;
SDL_Rect menuTitleRect, menuStartRect, menuHighScoreRect, menuStoreRect, menuExitRect;
int highScore = 0;
SDL_Texture* highScoreScreenTexture = nullptr;
SDL_Rect highScoreScreenRect;
int selectedStoreItem = 0;
SDL_Texture* storeTitleTexture = nullptr;
SDL_Texture* storeRocketTexture = nullptr;
SDL_Texture* storeImmortalTexture = nullptr;
SDL_Texture* storeSpeedBoostTexture = nullptr;
SDL_Texture* storeBackTexture = nullptr;
SDL_Rect storeTitleRect, storeRocketRect, storeImmortalRect, storeSpeedBoostRect, storeBackRect;
SDL_Texture* storeCoinCountTexture = nullptr;
SDL_Rect storeCoinCountRect;
int rocketCount = 0;
int immortalCount = 0;
int speedBoostCount = 0;
float flashTimer = 0;
bool playerVisible = true;
std::vector<Particle> particles;
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
float baseScrollSpeed = 6.0f;
float scrollSpeed = baseScrollSpeed;
float speedMultiplier = 0;
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
float initialPlayerX = 0;
bool isSpeedBoosted = false;
float speedBoostTimerValue = 0;
int currentFrame = 0;
float animationTimer = 0;
float slideTimer = 0;
bool justGotImmortal = false;
bool justGotSpeedBoost = false;
float glowTimer = 0;
bool wasOnGroundLastFrame = false;

void initGame() {
    player = {100, groundY - FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT, true, ObstacleType::SPIKE, 0, 0};
    obstacles.clear();
    particles.clear();
    groundX = 0;
    currentFrame = 0;
    animationTimer = 0;
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
    justGotImmortal = false;
    justGotSpeedBoost = false;
    glowTimer = 0;
    flashTimer = 0;
    playerVisible = true;
    wasOnGroundLastFrame = false;
    feedbackMessage = "";
    feedbackTimer = 0;
    backgroundChangeTimer = 0;
    currentBackgroundTextureIndex = 0;

    scoreRect = {10, 10, 0, 0};
    immortalTimerRect = {10, 40, 0, 0};
    coinCountRect = {10, 70, 0, 0};
    flyingTimerRect = {10, 100, 0, 0};
    speedBoostTimerRect = {10, 130, 0, 0};
}

void updateGame() {
    backgroundChangeTimer += 0.016f;
    if (backgroundChangeTimer >= BACKGROUND_CHANGE_DURATION) {
        currentBackgroundTextureIndex = (currentBackgroundTextureIndex + 1) % backgroundTextures.size();
        backgroundChangeTimer = 0;
    }

    glowTimer += 0.016f;
    flashTimer += 0.016f;
    bool shouldFlash = false;

    if (feedbackTimer > 0) {
        feedbackTimer -= 0.016f;
        if (feedbackTimer <= 0) {
            feedbackMessage = "";
        }
    }

    if (isSpeedBoosted && speedBoostTimerValue >= SPEED_BOOST_DURATION - 2.0f) {
        shouldFlash = true;
    }

    if (isImmortal && immortalityTimer >= IMMORTALITY_DURATION - 2.0f) {
        shouldFlash = true;
    }

    if (shouldFlash) {
        if (flashTimer >= FLASH_INTERVAL) {
            playerVisible = !playerVisible;
            flashTimer = 0;
        }
    } else {
        playerVisible = true;
        flashTimer = 0;
    }

    for (auto it = particles.begin(); it != particles.end();) {
        it->x += it->velocityX;
        it->y += it->velocityY;
        it->lifetime -= 0.016f;
        if (it->lifetime <= 0) {
            it = particles.erase(it);
        } else {
            ++it;
        }
    }

    if (!isFlying) {
        player.y += playerVelocityY;
        playerVelocityY += 0.5f;
        if (player.y >= groundY - player.h) {
            player.y = groundY - player.h;
            playerVelocityY = 0;
            if (isJumping) {
                isJumping = false;
                jumpCount = 0;
                hasDownJumped = false;
            }
            scrollSpeed = baseScrollSpeed * speedMultiplier;
            if (isSpeedBoosted) {
                scrollSpeed *= SPEED_BOOST_FACTOR;
            }
            player.w = FRAME_WIDTH;
            player.h = FRAME_HEIGHT;
            wasOnGroundLastFrame = true;
        } else {
            wasOnGroundLastFrame = false;
        }
    } else {
        player.y = groundY - player.h - FLYING_HEIGHT;
        flyingTimer += 0.016f;
        if (flyingTimer >= FLYING_DURATION) {
            isFlying = false;
            flyingTimer = 0;
            playerVelocityY = 0;
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
            scrollSpeed = baseScrollSpeed * speedMultiplier;
            if (isSpeedBoosted) {
                scrollSpeed *= SPEED_BOOST_FACTOR;
            }
            player.w = FRAME_WIDTH;
            player.h = FRAME_HEIGHT;
            wasOnGroundLastFrame = true;
        } else {
            wasOnGroundLastFrame = false;
        }
        player.w = PLANE_WIDTH;
        player.h = PLANE_HEIGHT;
    }

    if (isSliding) {
        slideTimer += 0.016f;
        if (slideTimer >= SLIDE_DURATION) {
            isSliding = false;
            player.h = FRAME_HEIGHT;
            player.y = groundY - FRAME_HEIGHT;
        }
    }

    if (isImmortal) {
        immortalityTimer += 0.016f;
        if (immortalityTimer >= IMMORTALITY_DURATION) {
            isImmortal = false;
            immortalityTimer = 0;
            justGotImmortal = false;
        }
    }

    if (isSpeedBoosted) {
        speedBoostTimerValue += 0.016f;
        if (speedBoostTimerValue >= SPEED_BOOST_DURATION) {
            isSpeedBoosted = false;
            speedBoostTimerValue = 0;
            justGotSpeedBoost = false;
            scrollSpeed = baseScrollSpeed * speedMultiplier;
        }
    }

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

    groundX -= scrollSpeed;
    if (groundX <= -SCREEN_WIDTH) groundX = 0;

    powerUpTimer += 0.00001f;
    if (powerUpTimer > 200000 && rand() % 100 < 0.05) {
        Entity powerUp = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                         COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::POWERUP, 0, -scrollSpeed};
        obstacles.push_back(powerUp);
        powerUpTimer = 0;
    }

    scoreBoosterTimer += 0.0001f;
    if (scoreBoosterTimer > 30000 && rand() % 100 < 0.1) {
        Entity scoreBooster = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                             COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SCORE_BOOSTER, 0, -scrollSpeed};
        obstacles.push_back(scoreBooster);
        scoreBoosterTimer = 0;
    }

    coinTimer += 0.1f;
    if (coinTimer > 5 && rand() % 100 < 75) {
        Entity coin = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                      COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::COIN, 0, -scrollSpeed};
        obstacles.push_back(coin);
        coinTimer = 0;
    }

    rocketTimer += 0.0000016f;
    if (rocketTimer > 30000000 && rand() % 10000000 < 5) {
        Entity rocket = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                        COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::ROCKET, 0, -scrollSpeed};
        obstacles.push_back(rocket);
        rocketTimer = 0;
    }

    speedBoostTimer += 0.0001f;
    if (speedBoostTimer > 250000 && rand() % 100 < 10) {
        Entity speedBoost = {SCREEN_WIDTH, isFlying ? (groundY - player.h - FLYING_HEIGHT) : (groundY - (rand() % 100 + 50)), 
                           COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::SPEED_BOOST, 0, -scrollSpeed};
        obstacles.push_back(speedBoost);
        speedBoostTimer = 0;
    }

    obstacleTimer += 0.1f;
    if (obstacleTimer > 10) {
        spawnObstacle();
        obstacleTimer = 0;
    }

    for (auto& obstacle : obstacles) {
        if (obstacle.active) {
            obstacle.x += obstacle.velocityX;
            if (obstacle.type == ObstacleType::MOVING) {
                obstacle.y += obstacle.velocityY;
                if (obstacle.y + obstacle.h < groundY - 180 || obstacle.y > groundY - 20) {
                    obstacle.velocityY = -obstacle.velocityY;
                }
            }

            if (obstacle.type != ObstacleType::COIN && obstacle.type != ObstacleType::ROCKET
                && obstacle.type != ObstacleType::POWERUP && obstacle.type != ObstacleType::SCORE_BOOSTER
                && obstacle.type != ObstacleType::SPEED_BOOST) {
                obstacle.x -= scrollSpeed;
            }

            bool collision = (player.x < obstacle.x + obstacle.w &&
                             player.x + player.w > obstacle.x &&
                             player.y < obstacle.y + obstacle.h &&
                             player.y + player.h > obstacle.y);

            if (obstacle.type == ObstacleType::POWERUP && collision) {
                obstacle.active = false;
                isImmortal = true;
                immortalityTimer = 0;
                if (!justGotImmortal) {
                    spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 255, 0, 255});
                    justGotImmortal = true;
                }
            } else if (obstacle.type == ObstacleType::SCORE_BOOSTER && collision) {
                obstacle.active = false;
                score += 100;
            } else if (obstacle.type == ObstacleType::COIN && collision) {
                obstacle.active = false;
                coinCount += 1;
            } else if (obstacle.type == ObstacleType::ROCKET && collision) {
                obstacle.active = false;
                isFlying = true;
                flyingTimer = 0;
                initialPlayerX = player.x;
            } else if (obstacle.type == ObstacleType::SPEED_BOOST && collision) {
                obstacle.active = false;
                isSpeedBoosted = true;
                speedBoostTimerValue = 0;
                if (isImmortal) {
                    isImmortal = false;
                    immortalityTimer = 0;
                    justGotImmortal = false;
                }
                if (!justGotSpeedBoost) {
                    spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 165, 0, 255});
                    justGotSpeedBoost = true;
                }
            } else if (obstacle.type != ObstacleType::GAP && collision && !isImmortal) {
                gameState = GameState::MENU;
            } else if (obstacle.type == ObstacleType::GAP && player.x < obstacle.x + obstacle.w &&
                       player.x + player.w > obstacle.x && player.y + player.h >= obstacle.y && !isJumping && !isFlying) {
                gameState = GameState::MENU;
            }

            if (obstacle.x < -obstacle.w) obstacle.active = false;
        }
    }

    scoreTimer += 0.016f;
    if (scoreTimer >= 0.1f) {
        score += 10;
        scoreTimer = 0;
    }

    if (!isJumping && !isFlying) {
        animationTimer += 0.016f;
        float effectiveFrameTime = FRAME_TIME;
        if (isSpeedBoosted) {
            effectiveFrameTime /= SPEED_BOOST_ANIMATION_FACTOR;
        }
        if (animationTimer >= effectiveFrameTime) {
            animationTimer = 0;
            currentFrame = (currentFrame + 1) % FRAME_COUNT;
        }
    } else {
        currentFrame = 1;
    }
}