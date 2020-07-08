#include "engine.hpp"

#include <algorithm>
#include <iostream> // REMOVE
#include <random>
#include <stdexcept>

#include "hexvoid.hpp"

namespace hexvoid
{
    FPS::FPS()
    {
        frameCount_ = 0;
        lastSecond_ = SDL_GetTicks();
        fps_ = 0;
        font_ = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 12);
        if(!font_) throw std::runtime_error("Font not found! TTF_Error: " + std::string(TTF_GetError()));

        textColor_ = {255, 255, 127};
    }

    FPS::~FPS()
    {
        TTF_CloseFont(font_);
    }

    void FPS::Tick()
    {
        uint32_t tick = SDL_GetTicks();
        frameCount_++;
        if(tick - lastSecond_ > 1000)
        {
            fps_ = frameCount_;
            lastSecond_ = tick;
            frameCount_ = 0;
        }
    };

    void FPS::Draw(SDL_Renderer*& gRenderer)
    {
        char const* fpsTexst = std::to_string(fps_).c_str();
        SDL_Surface* textSurface = TTF_RenderText_Solid(font_, fpsTexst, textColor_);
        SDL_Rect textLocation = {5, 5, 30, 20};
        SDL_Texture* Message = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        SDL_RenderCopy(gRenderer, Message, NULL, &textLocation);
    };

    uint8_t Random(uint8_t min, uint8_t max)
    {
        static std::default_random_engine generator(time(0));
        std::uniform_int_distribution<uint8_t> distribution(min, max);
        return distribution(generator);
    }

    std::vector<Hexagon> GenerateGrid(int16_t gridColumns, int16_t gridRows, int16_t hexRadius, int16_t spacing)
    {
        std::vector<Hexagon> hexGrid;
        hexGrid.reserve(gridRows * gridColumns);

        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t columnSpacing = 3 * hexRadius / 2 + spacing;
        int16_t rowSpacing = cos30 * hexRadius + spacing;

        for(int r = 0; r < gridRows; r++)
        {
            for(int c = 0; c < gridColumns; c++)
            {
                int16_t x = columnSpacing + r * columnSpacing;
                int16_t y = rowSpacing + (2 * c + r % 2) * rowSpacing;
                uint8_t colorIndex = Random(1, 4);
                hexGrid.push_back(Hexagon{x, y, hexRadius, colorIndex});
            }
        }

        return std::move(hexGrid);
    }

    std::vector<Hexagon> GenerateHexagonGrid(int16_t gridRadius, int16_t hexRadius, int16_t spacing)
    {
        std::vector<Hexagon> hexGrid;

        int16_t gridSize = 2 * gridRadius - 1;
        for(int i = 1; i < gridRadius; i++)
            gridSize += 2 * (2 * gridRadius - 1 - i);

        hexGrid.reserve(gridSize);

        const double cos30 = std::cos(30.0 * M_PI / 180.0);

        int16_t columnSpacing = 3 * hexRadius / 2 + spacing;
        int16_t rowSpacing = cos30 * hexRadius + spacing;

        int16_t screenCenterX = WINDOW_WIDTH / 2;
        int16_t screenCenterY = WINDOW_HEIGHT / 2;

        for(int r = -gridRadius + 1; r < gridRadius; r++)
        {
            int columns;
            if(r < 0)
                columns = 2 * gridRadius + r - 1;
            else
                columns = 2 * gridRadius - r - 1;

            int columnStart = (2 * gridRadius - columns) / 2 - gridRadius + 1;

            for(int c = columnStart; c < columnStart + columns; c++)
            {
                int16_t x = screenCenterX + (2 * c - abs(r) % 2) * rowSpacing;
                int16_t y = screenCenterY + r * columnSpacing;
                uint8_t colorIndex = Random(1, 4);
                hexGrid.push_back(Hexagon{x, y, hexRadius, colorIndex});
            }
        }
        return std::move(hexGrid);
    }

    Hexagon FindClosestHexagon(const std::vector<Hexagon>& grid, int16_t x, int16_t y)
    {

        double minDistance = std::numeric_limits<double>::infinity();
        int bestIndex = -1;
        for(int i = 0; i < grid.size(); i++)
        {
            double distance = std::sqrt(std::pow(grid.at(i).x - x, 2) + std::pow(grid.at(i).y - y, 2));
            if(distance < minDistance)
            {
                minDistance = distance;
                bestIndex = i;
            }
        }
        return grid.at(bestIndex);
    }

} // namespace hexvoid
