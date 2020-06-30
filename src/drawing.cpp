#include "drawing.hpp"

#include "color.hpp"

void hexvoid::DrawHexagon(SDL_Renderer*& gRenderer, const Hexagon& hexagon)
{
    Color color = GetColorFromTheme(hexvoid::PsychedelicPattern, hexagon.color);

    printf("Color: %i %i %i\n", color.r, color.g, color.b);

    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, 255);

    SDL_Rect rect;

    rect.x = hexagon.x;
    rect.y = hexagon.y;
    rect.w = hexagon.x + hexagon.diagonal;
    rect.h = hexagon.y + hexagon.diagonal;

    SDL_RenderDrawRect(gRenderer, &rect);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
}
