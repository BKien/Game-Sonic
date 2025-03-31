#include "game.h"
#include <iostream>
#include <cmath>

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
    const int NUM_PARTICLES = 20;
    const float PARTICLE_LIFETIME = 1.0f;
    const float PARTICLE_SPEED = 2.0f;

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Particle particle;
        particle.x = centerX;
        particle.y = centerY;
        particle.maxLifetime = PARTICLE_LIFETIME;
        particle.lifetime = PARTICLE_LIFETIME;
        particle.color = color;

        float angle = (float)(rand() % 360) * (M_PI / 180.0f);
        particle.velocityX = cos(angle) * PARTICLE_SPEED;
        particle.velocityY = sin(angle) * PARTICLE_SPEED;

        particles.push_back(particle);
    }
}

void spawnObstacle() {
    ObstacleType type = static_cast<ObstacleType>(rand() % 10); // Updated to include SPEED_BOOST
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
                       COLLECTABLE_SIZE, COLLECTABLE_SIZE, true, ObstacleType::COIN, 0, -scrollSpeed};
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
        ObstacleType secondType = static_cast<ObstacleType>(rand() % 10); // Updated to include SPEED_BOOST
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