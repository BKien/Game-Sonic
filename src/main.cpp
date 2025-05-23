#include "game.h"
#include <iostream>
#if defined(__GNUC__) && __GNUC__ < 9
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #include <filesystem>
    namespace fs = std::filesystem;
#endif
std::string getExecutablePath() {
    return std::filesystem::current_path().string();
}

// Function to get the resource path dynamically
std::string getResourcePath(const std::string& filename) {
    std::filesystem::path basePath = std::filesystem::current_path();
    std::filesystem::path fullPath = basePath / filename;
    return fullPath.string();
}

int main(int argc, char* argv[]) {
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 || !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << "\n";
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer Initialization failed: " << Mix_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf Initialization failed: " << TTF_GetError() << "\n";
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    window = SDL_CreateWindow("Subway Surfer 2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("D:/project/src/Font/Arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }
    
    std::string musicPath = getResourcePath("src/sound/background.mp3");
    std::string playerTexturePath = getResourcePath("src/img/sonic.png");
    std::string planeTexturePath = getResourcePath("src/img/plane.png");
    std::string spikeTexturePath = getResourcePath("src/img/spikes_2.png");
    std::string tallObstacleTexturePath = getResourcePath("src/img/tallObstacleTexture.png");
    std::string movingObstacleTexturePath = getResourcePath("src/img/moving_obstacle.png");
    std::string backgroundPath = getResourcePath("src/img/background.png");
    std::string background2Path = getResourcePath("src/img/background_2.jpg");
    std::string birdTexturePath = getResourcePath("src/img/blade_3.png");
    std::string powerUpTexturePath = getResourcePath("src/img/powerup.png");
    std::string scoreBoosteextur = getResourcePath("src/img/scorebooster.png");
    std::string coinTexturePath = getResourcePath("src/img/coin.png");
    std::string rocketTexturePath = getResourcePath("src/img/rocket.png");
    std::string speedBoostTexturePath = getResourcePath("src/img/speedboost.png");
    std::string gapTexturePath = getResourcePath("src/img/gap.jpg");
    std::string groundTexturePath = getResourcePath("src/img/ground.jpg");

    backgroundMusic = Mix_LoadMUS(musicPath.c_str());
    if (!backgroundMusic) {
        std::cerr << "Failed to load background music: " << Mix_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return -1;
    }

    playerTexture = loadTexture(playerTexturePath.c_str());
    planeTexture = loadTexture(planeTexturePath.c_str());
    spikeTexture = loadTexture(spikeTexturePath.c_str());
    tallObstacleTexture = loadTexture(tallObstacleTexturePath.c_str());
    movingObstacleTexture = loadTexture(movingObstacleTexturePath.c_str());
    gapTexture = loadTexture(gapTexturePath.c_str());
    groundTexture = loadTexture(groundTexturePath.c_str());
    backgroundTextures.push_back(loadTexture(backgroundPath.c_str()));
    backgroundTextures.push_back(loadTexture(background2Path.c_str()));
    birdTexture = loadTexture(birdTexturePath.c_str());
    powerUpTexture = loadTexture(powerUpTexturePath.c_str());
    scoreBoosterTexture = loadTexture(scoreBoosteextur.c_str());
    coinTexture = loadTexture(coinTexturePath.c_str());
    rocketTexture = loadTexture(rocketTexturePath.c_str());
    speedBoostTexture = loadTexture(speedBoostTexturePath.c_str());

    if (!playerTexture || !planeTexture || !spikeTexture || !tallObstacleTexture || 
        !movingObstacleTexture || !gapTexture || !groundTexture || 
        !birdTexture || !powerUpTexture || !scoreBoosterTexture || 
        !coinTexture || !rocketTexture || !speedBoostTexture) {
        std::cerr << "Failed to load textures!\n";
        Mix_FreeMusic(backgroundMusic);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        TTF_Quit();
        Mix_CloseAudio();
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
            Mix_FreeMusic(backgroundMusic);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            TTF_Quit();
            Mix_CloseAudio();
            SDL_Quit();
            return -1;
        }
    }

    menuTitleRect = {SCREEN_WIDTH / 2 - 100, 100, 0, 0};
    menuStartRect = {SCREEN_WIDTH / 2 - 50, 200, 0, 0};
    menuHighScoreRect = {SCREEN_WIDTH / 2 - 50, 250, 0, 0};
    menuStoreRect = {SCREEN_WIDTH / 2 - 50, 300, 0, 0};
    menuExitRect = {SCREEN_WIDTH / 2 - 50, 350, 0, 0};
    highScoreScreenRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 20, 0, 0};
    storeTitleRect = {SCREEN_WIDTH / 2 - 50, 100, 0, 0};
    storeCoinCountRect = {SCREEN_WIDTH / 2 - 50, 150, 0, 0};
    storeRocketRect = {SCREEN_WIDTH / 2 - 100, 200, 0, 0};
    storeImmortalRect = {SCREEN_WIDTH / 2 - 100, 250, 0, 0};
    storeSpeedBoostRect = {SCREEN_WIDTH / 2 - 100, 300, 0, 0};
    storeBackRect = {SCREEN_WIDTH / 2 - 50, 350, 0, 0};

    menuTitleTexture = renderText("Subway Surfer 2D", {255, 255, 255, 255});
    if (menuTitleTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(menuTitleTexture, nullptr, nullptr, &textWidth, &textHeight);
        menuTitleRect.w = textWidth;
        menuTitleRect.h = textHeight;
    }

    storeTitleTexture = renderText("Store", {255, 255, 255, 255});
    if (storeTitleTexture) {
        int textWidth, textHeight;
        SDL_QueryTexture(storeTitleTexture, nullptr, nullptr, &textWidth, &textHeight);
        storeTitleRect.w = textWidth;
        storeTitleRect.h = textHeight;
    }

    if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
        std::cerr << "Failed to play background music: " << Mix_GetError() << "\n";
    } else {
        isMusicPlaying = true;
    }

    bool running = true;
    SDL_Event event;
    const float FRAME_TIME = 0.016f;
    Uint32 lastTime = SDL_GetTicks();
    float accumulator = 0.0f;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

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

        switch (gameState) {
            case GameState::MENU:
            case GameState::PLAYING:
                if (!isMusicPlaying) {
                    if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
                        std::cerr << "Failed to play background music: " << Mix_GetError() << "\n";
                    } else {
                        isMusicPlaying = true;
                    }
                }
                break;
            case GameState::STORE:
            case GameState::HIGH_SCORE:
                if (isMusicPlaying) {
                    Mix_PauseMusic();
                    isMusicPlaying = false;
                }
                break;
            case GameState::EXIT:
                running = false;
                break;
        }

        switch (gameState) {
            case GameState::MENU:
                renderMenu();
                break;
            case GameState::STORE:
                renderStore();
                break;
            case GameState::PLAYING:
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

        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < 16) {
            SDL_Delay(16 - frameTime);
        }
    }

    Mix_FreeMusic(backgroundMusic);
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
    SDL_DestroyTexture(feedbackTexture);
    SDL_DestroyTexture(rocketInventoryTexture);
    SDL_DestroyTexture(immortalInventoryTexture);
    SDL_DestroyTexture(speedBoostInventoryTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
