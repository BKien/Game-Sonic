#include "game.h"

void handleMenuInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        gameState = GameState::EXIT;
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
            selectedMenuItem--;
            if (selectedMenuItem < 0) selectedMenuItem = 3;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            selectedMenuItem++;
            if (selectedMenuItem > 3) selectedMenuItem = 0;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            if (selectedMenuItem == 0) {
                gameState = GameState::PLAYING;
                initGame();
            } else if (selectedMenuItem == 1) {
                gameState = GameState::HIGH_SCORE;
            } else if (selectedMenuItem == 2) {
                gameState = GameState::STORE;
                selectedStoreItem = 0;
            } else if (selectedMenuItem == 3) {
                gameState = GameState::EXIT;
            }
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (mouseX >= menuStartRect.x && mouseX <= menuStartRect.x + menuStartRect.w &&
            mouseY >= menuStartRect.y && mouseY <= menuStartRect.y + menuStartRect.h) {
            gameState = GameState::PLAYING;
            initGame();
        }
        else if (mouseX >= menuHighScoreRect.x && mouseX <= menuHighScoreRect.x + menuHighScoreRect.w &&
                 mouseY >= menuHighScoreRect.y && mouseY <= menuHighScoreRect.y + menuHighScoreRect.h) {
            gameState = GameState::HIGH_SCORE;
        }
        else if (mouseX >= menuStoreRect.x && mouseX <= menuStoreRect.x + menuStoreRect.w &&
                 mouseY >= menuStoreRect.y && mouseY <= menuStoreRect.y + menuStoreRect.h) {
            gameState = GameState::STORE;
            selectedStoreItem = 0;
        }
        else if (mouseX >= menuExitRect.x && mouseX <= menuExitRect.x + menuExitRect.w &&
                 mouseY >= menuExitRect.y && mouseY <= menuExitRect.y + menuExitRect.h) {
            gameState = GameState::EXIT;
        }
    }
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

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
            if (selectedStoreItem < 0) selectedStoreItem = 3;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            selectedStoreItem++;
            if (selectedStoreItem > 3) selectedStoreItem = 0;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            if (selectedStoreItem == 0) {
                if (coinCount >= ROCKET_COST) {
                    coinCount -= ROCKET_COST;
                    rocketCount++;
                    feedbackMessage = "Rocket Purchased!";
                    feedbackTimer = FEEDBACK_DURATION;
                } else {
                    feedbackMessage = "Not Enough Coins for Rocket!";
                    feedbackTimer = FEEDBACK_DURATION;
                }
            } else if (selectedStoreItem == 1) {
                if (coinCount >= IMMORTAL_COST) {
                    coinCount -= IMMORTAL_COST;
                    immortalCount++;
                    feedbackMessage = "Immortal Purchased!";
                    feedbackTimer = FEEDBACK_DURATION;
                } else {
                    feedbackMessage = "Not Enough Coins for Immortal!";
                    feedbackTimer = FEEDBACK_DURATION;
                }
            } else if (selectedStoreItem == 2) {
                if (coinCount >= SPEED_BOOST_COST) {
                    coinCount -= SPEED_BOOST_COST;
                    speedBoostCount++;
                    feedbackMessage = "Speed Boost Purchased!";
                    feedbackTimer = FEEDBACK_DURATION;
                } else {
                    feedbackMessage = "Not Enough Coins for Speed Boost!";
                    feedbackTimer = FEEDBACK_DURATION;
                }
            } else if (selectedStoreItem == 3) {
                gameState = GameState::MENU;
                feedbackMessage = "";
            }
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gameState = GameState::MENU;
            feedbackMessage = "";
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        if (mouseX >= storeRocketRect.x && mouseX <= storeRocketRect.x + storeRocketRect.w &&
            mouseY >= storeRocketRect.y && mouseY <= storeRocketRect.y + storeRocketRect.h) {
            if (coinCount >= ROCKET_COST) {
                coinCount -= ROCKET_COST;
                rocketCount++;
                feedbackMessage = "Rocket Purchased!";
                feedbackTimer = FEEDBACK_DURATION;
            } else {
                feedbackMessage = "Not Enough Coins for Rocket!";
                feedbackTimer = FEEDBACK_DURATION;
            }
        }
        else if (mouseX >= storeImmortalRect.x && mouseX <= storeImmortalRect.x + storeImmortalRect.w &&
                 mouseY >= storeImmortalRect.y && mouseY <= storeImmortalRect.y + storeImmortalRect.h) {
            if (coinCount >= IMMORTAL_COST) {
                coinCount -= IMMORTAL_COST;
                immortalCount++;
                feedbackMessage = "Immortal Purchased!";
                feedbackTimer = FEEDBACK_DURATION;
            } else {
                feedbackMessage = "Not Enough Coins for Immortal!";
                feedbackTimer = FEEDBACK_DURATION;
            }
        }
        else if (mouseX >= storeSpeedBoostRect.x && mouseX <= storeSpeedBoostRect.x + storeSpeedBoostRect.w &&
                 mouseY >= storeSpeedBoostRect.y && mouseY <= storeSpeedBoostRect.y + storeSpeedBoostRect.h) {
            if (coinCount >= SPEED_BOOST_COST) {
                coinCount -= SPEED_BOOST_COST;
                speedBoostCount++;
                feedbackMessage = "Speed Boost Purchased!";
                feedbackTimer = FEEDBACK_DURATION;
            } else {
                feedbackMessage = "Not Enough Coins for Speed Boost!";
                feedbackTimer = FEEDBACK_DURATION;
            }
        }
        else if (mouseX >= storeBackRect.x && mouseX <= storeBackRect.x + storeBackRect.w &&
                 mouseY >= storeBackRect.y && mouseY <= storeBackRect.y + storeBackRect.h) {
            gameState = GameState::MENU;
            feedbackMessage = "";
        }
    }
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

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
        gameState = GameState::MENU;
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
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN && isJumping && !hasDownJumped) {
            downPressedThisFrame = true;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_S && !isSliding && !isJumping && !isFlying) {
            isSliding = true;
            slideTimer = 0;
            player.h = SLIDE_HEIGHT;
            player.y = groundY - SLIDE_HEIGHT;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            gameState = GameState::MENU;
            return;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_1 && rocketCount > 0 && !isFlying) {
            rocketCount--;
            isFlying = true;
            flyingTimer = 0;
            initialPlayerX = player.x;
            feedbackMessage = "Rocket Used!";
            feedbackTimer = FEEDBACK_DURATION;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_2 && immortalCount > 0 && !isImmortal) {
            immortalCount--;
            isImmortal = true;
            immortalityTimer = 0;
            if (!justGotImmortal) {
                spawnParticles(player.x + player.w / 2, player.y + player.h / 2, {255, 255, 0, 255});
                justGotImmortal = true;
            }
            feedbackMessage = "Immortal Activated!";
            feedbackTimer = FEEDBACK_DURATION;
        }
        if (event.key.keysym.scancode == SDL_SCANCODE_3 && speedBoostCount > 0 && !isSpeedBoosted) {
            speedBoostCount--;
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
            feedbackMessage = "Speed Boost Activated!";
            feedbackTimer = FEEDBACK_DURATION;
        }
    }

    if (spacePressedThisFrame && !spacePressedLastFrame) {
        if (!isJumping && jumpCount == 0 && !isFlying) {
            playerVelocityY = -8.0f;
            isJumping = true;
            jumpCount = 1;
            hasDownJumped = false;
        } else if (isJumping && jumpCount == 1 && !isFlying) {
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