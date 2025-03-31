#include "game.h"
#include <sstream>
#include <iomanip>

void renderMenu() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

    SDL_RenderCopy(renderer, menuTitleTexture, nullptr, &menuTitleRect);

    SDL_Color normalColor = {255, 255, 255, 255};
    SDL_Color selectedColor = {255, 165, 0, 255};

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

    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

    SDL_RenderCopy(renderer, storeTitleTexture, nullptr, &storeTitleRect);

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

    SDL_Color normalColor = {255, 255, 255, 255};
    SDL_Color selectedColor = {255, 165, 0, 255};

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

    if (feedbackTexture) SDL_DestroyTexture(feedbackTexture);
    if (!feedbackMessage.empty()) {
        feedbackTexture = renderText(feedbackMessage, {255, 255, 255, 255});
        if (feedbackTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(feedbackTexture, nullptr, nullptr, &textWidth, &textHeight);
            feedbackRect.w = textWidth;
            feedbackRect.h = textHeight;
            feedbackRect.x = SCREEN_WIDTH / 2 - textWidth / 2;
            SDL_RenderCopy(renderer, feedbackTexture, nullptr, &feedbackRect);
        }
    }

    SDL_RenderPresent(renderer);
}

void renderHighScore() {
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

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

    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTextures[currentBackgroundTextureIndex], nullptr, &bgRect);

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

    for (const auto& particle : particles) {
        Uint8 alpha = (Uint8)(255 * (particle.lifetime / particle.maxLifetime));
        SDL_SetRenderDrawColor(renderer, particle.color.r, particle.color.g, particle.color.b, alpha);
        SDL_Rect particleRect = {(int)particle.x, (int)particle.y, 4, 4};
        SDL_RenderFillRect(renderer, &particleRect);
    }

    if (isImmortal) {
        SDL_Color glowColor = {255, 255, 0, 255};
        float remainingTime = IMMORTALITY_DURATION - immortalityTimer;
        float duration = IMMORTALITY_DURATION;
        Uint8 baseAlpha = (Uint8)(255 * (remainingTime / duration));
        float pulse = ((glowTimer * GLOW_PULSE_SPEED) + 1.0f) / 2.0f;
        Uint8 alpha = (Uint8)(baseAlpha * (0.5f + 0.5f * pulse));

        SDL_SetRenderDrawColor(renderer, glowColor.r, glowColor.g, glowColor.b, alpha / 4);
        SDL_Rect glowRect = {(int)player.x - 8, (int)player.y - 8, player.w + 16, player.h + 16};
        SDL_RenderDrawRect(renderer, &glowRect);
        
        SDL_SetRenderDrawColor(renderer, glowColor.r, glowColor.g, glowColor.b, alpha / 2);
        glowRect = {(int)player.x - 4, (int)player.y - 4, player.w + 8, player.h + 8};
        SDL_RenderDrawRect(renderer, &glowRect);
        
        SDL_SetRenderDrawColor(renderer, glowColor.r, glowColor.g, glowColor.b, alpha);
        glowRect = {(int)player.x - 2, (int)player.y - 2, player.w + 4, player.h + 4};
        SDL_RenderDrawRect(renderer, &glowRect);
    }

    if (playerVisible) {
        SDL_Rect playerSrc = {currentFrame * FRAME_WIDTH, 0, FRAME_WIDTH, isSliding ? SLIDE_HEIGHT : (isFlying ? PLANE_HEIGHT : FRAME_HEIGHT)};
        SDL_Rect playerDst = {(int)player.x, (int)player.y, player.w, isSliding ? SLIDE_HEIGHT : (isFlying ? PLANE_HEIGHT : player.h)};
        SDL_Texture* currentTexture = isFlying ? planeTexture : playerTexture;
        SDL_RenderCopy(renderer, currentTexture, &playerSrc, &playerDst);
    }

    for (const auto& obstacle : obstacles) {
        if (obstacle.active) {
            SDL_Rect obsRect = {(int)obstacle.x, (int)obstacle.y, obstacle.w, obstacle.h};
            SDL_Texture* texture = nullptr;

            switch (obstacle.type) {
                case ObstacleType::SPIKE: texture = spikeTexture; break;
                case ObstacleType::TALL: texture = tallObstacleTexture; break;
                case ObstacleType::MOVING: texture = movingObstacleTexture; break;
                case ObstacleType::GAP: texture = gapTexture; break;
                case ObstacleType::BIRD: texture = birdTexture; break;
                case ObstacleType::POWERUP: texture = powerUpTexture; break;
                case ObstacleType::SCORE_BOOSTER: texture = scoreBoosterTexture; break;
                case ObstacleType::COIN: texture = coinTexture; break;
                case ObstacleType::ROCKET: texture = rocketTexture; break;
                case ObstacleType::SPEED_BOOST: texture = speedBoostTexture; break;
            }

            if (texture) {
                SDL_RenderCopy(renderer, texture, nullptr, &obsRect);
            }
        }
    }

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

    if (rocketInventoryTexture) SDL_DestroyTexture(rocketInventoryTexture);
    std::stringstream rocketSs;
    rocketSs << "1: Rocket (" << rocketCount << ")";
    rocketInventoryTexture = renderText(rocketSs.str(), {255, 255, 255, 255});
    if (rocketInventoryTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(rocketInventoryTexture, nullptr, nullptr, &textWidth, &textHeight);
        rocketInventoryRect.w = textWidth;
        rocketInventoryRect.h = textHeight;
        SDL_RenderCopy(renderer, rocketInventoryTexture, nullptr, &rocketInventoryRect);
    }

    if (immortalInventoryTexture) SDL_DestroyTexture(immortalInventoryTexture);
    std::stringstream immortalSs;
    immortalSs << "2: Immortal (" << immortalCount << ")";
    immortalInventoryTexture = renderText(immortalSs.str(), {255, 255, 0, 255});
    if (immortalInventoryTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(immortalInventoryTexture, nullptr, nullptr, &textWidth, &textHeight);
        immortalInventoryRect.w = textWidth;
        immortalInventoryRect.h = textHeight;
        SDL_RenderCopy(renderer, immortalInventoryTexture, nullptr, &immortalInventoryRect);
    }

    if (speedBoostInventoryTexture) SDL_DestroyTexture(speedBoostInventoryTexture);
    std::stringstream speedBoostSs;
    speedBoostSs << "3: Speed Boost (" << speedBoostCount << ")";
    speedBoostInventoryTexture = renderText(speedBoostSs.str(), {255, 165, 0, 255});
    if (speedBoostInventoryTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(speedBoostInventoryTexture, nullptr, nullptr, &textWidth, &textHeight);
        speedBoostInventoryRect.w = textWidth;
        speedBoostInventoryRect.h = textHeight;
        SDL_RenderCopy(renderer, speedBoostInventoryTexture, nullptr, &speedBoostInventoryRect);
    }

    if (feedbackTexture) SDL_DestroyTexture(feedbackTexture);
    if (!feedbackMessage.empty()) {
        feedbackTexture = renderText(feedbackMessage, {255, 255, 255, 255});
        if (feedbackTexture) {
            int textWidth, textHeight;
            SDL_QueryTexture(feedbackTexture, nullptr, nullptr, &textWidth, &textHeight);
            feedbackRect.w = textWidth;
            feedbackRect.h = textHeight;
            feedbackRect.x = SCREEN_WIDTH / 2 - textWidth / 2;
            SDL_RenderCopy(renderer, feedbackTexture, nullptr, &feedbackRect);
        }
    }

    SDL_RenderPresent(renderer);
}