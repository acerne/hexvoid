#pragma once

#include <condition_variable>
#include <cstddef>
#include <map>
#include <mutex>
#include <queue>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDL_FontCache.h"

namespace hex
{
    class Core
    {

    protected:
        static FC_Font* font_;
        static SDL_Window* gWindow_;
        static SDL_Surface* gSurface_;
        static SDL_Surface* gBackground_;
        static SDL_Renderer* gRenderer_;
        static uint16_t windowWidth_;
        static uint16_t windowHeight_;
        static bool quit_;
        static const char* fontPath_;

        static bool animationActive_;

        static void SDL(int error);
    };

    class Engine : public Core
    {
    public:
        enum class GameState
        {
            LOAD,
            SPLASH_SCREEN,
            MAIN_MENU,
            NEW_GAME_MENU,
            OPTIONS_MENU,
            GAME,
            GAME_OVER
        };

        static void Initialize(const std::string& title, uint16_t windowWidth, uint16_t windowHeight);
        static void Terminate();

        static void PresentingThread();

        static bool GetQuitFlag();

        static GameState GetGameState();
        static void SetGameState(GameState state);

        static void ChangeResolution(uint16_t windowWidth, uint16_t windowHeight);

        static void WaitDisplay();
        static void ReadyToDisplay();

    private:
        Engine() {}
        static std::mutex renderMutex_;
        static std::condition_variable renderCondition_;
        static bool renderReady_;
        static bool drawingReady_;

        static GameState state_;

        static std::thread inputThread_;
        static std::thread displayThread_;
    };

    class Input : public Core
    {
    public:
        static uint16_t cursorX;
        static uint16_t cursorY;

        static void PollingThread();

        static void AddClick(const SDL_Event& event);
        static void AddKeypress(const SDL_Event& event);
        static void UpdateMouse(const SDL_Event& event);

        static SDL_Event GetClick();
        static SDL_Event GetKeypress();

    private:
        static std::mutex clickMutex_;
        static std::mutex keyMutex_;
        static std::mutex mouseMutex_;
        static std::queue<SDL_Event> clicks_;
        static std::queue<SDL_Event> keys_;
    };

    class Background : public Core
    {
    public:
        Background() {}

        virtual void UpdatePhysics() = 0;
        virtual void Draw() = 0;

    private:
    };

    class Menu : public Core
    {
    public:
        class MenuList
        {
        public:
            MenuList() {}

            void AddItem(const std::string& title, uint8_t index);
            void AddOption(const std::string& title, uint8_t index, const std::vector<std::string>& valueNames);

            int8_t Click(int16_t cursorX, int16_t cursorY);
            std::string GetSelection(const std::string& item) const;
            void Draw(int16_t cursorX, int16_t cursorY) const;

        private:
            enum class MenuItemType
            {
                Item,
                Option
            };
            struct MenuItem
            {
                MenuItemType type;
                std::string title;
                uint8_t index;
                SDL_Rect box;
                std::vector<std::string> options;
                u_int8_t selected;
            };

            std::map<std::string, MenuItem> items_;
            uint16_t spacing_ = 60;

            int8_t MouseOverItemIndex(int16_t cursorX, int16_t cursorY) const;
            std::string MouseOverItemName(int16_t cursorX, int16_t cursorY) const;
            bool IsMouseInside(int16_t cursorX, int16_t cursorY, const SDL_Rect& box) const;
            void NextOption(const std::string& itemName);
        };

        static void Initialize();
        static void Update();
        static void Draw();

    private:
        Menu() {}

        static MenuList mainMenu_;
        static MenuList optionsMenu_;
        static MenuList newGameMenu_;
    };

    class Palette : public Core
    {
    public:
        enum class Element
        {
            Background,
            Foreground,
            A,
            B,
            C,
            D
        };

        struct Color
        {
            Color(uint8_t r, uint8_t g, uint8_t b) : a(255) {}
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {}
            Color(int32_t hexValue)
            {
                if(hexValue > 0xFFFFFF)
                {
                    r = ((hexValue >> 24) & 0xFF);
                    g = ((hexValue >> 16) & 0xFF);
                    b = ((hexValue >> 8) & 0xFF);
                    a = ((hexValue)&0xFF);
                }
                else
                {
                    r = ((hexValue >> 16) & 0xFF);
                    g = ((hexValue >> 8) & 0xFF);
                    b = ((hexValue)&0xFF);
                    a = 255;
                }
            }
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        struct Theme
        {
            Color background;
            Color foreground;
            Color A;
            Color B;
            Color C;
            Color D;
        };

        static void ChangeTheme(const std::string& themeName);
        static Color GetColor(Element id);
        static Color RandomColor();
        static std::string GetThemeName();
        static std::vector<std::string> GetThemeNames();

    private:
        Palette();

        static std::string selectedTheme_;
        static const std::map<std::string, Theme> themes_;
    };

    class Randomizer : public Core
    {
    public:
        typedef std::pair<uint8_t, uint8_t> ValueAndWeight;

        static uint8_t Random(uint8_t min, uint8_t max);
        static uint8_t WeightedRandom(const std::vector<ValueAndWeight>& candidates);
        static bool Chance(uint32_t oneInA);

    private:
        Randomizer() {}
    };

    class Score : public Core
    {
    public:
        static void Start();
        static bool IsGameOver();

        static void AddScore(uint16_t points);
        static void TakeScore(uint16_t points);
        static void AddMoves(uint16_t moves);
        static void TakeMoves(uint16_t moves);
        static void RegisterMove();

        static void Draw();

    private:
        Score();
        static size_t score_;
        static size_t moves_;
        static uint16_t movesLeft_;
    };

    class SystemInfo : public Core
    {
    public:
        static void Update();
        static void Draw();

    private:
        SystemInfo();

        static uint16_t frameCount_;
        static uint32_t lastSecond_;
        static uint16_t fps_;
        static uint16_t ram_;
    };

} // namespace hex
