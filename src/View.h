#pragma once
#include <SDL2/SDL.h>

class View {
public:
    float centerX = 0, centerY = 0;
    float width = 0, height = 0;

    View() = default;
    View(float centerX, float centerY, float width, float height)
        : centerX(centerX), centerY(centerY), width(width), height(height) {}

    void setCenter(float x, float y) { centerX = x; centerY = y; }
    void move(float dx, float dy) { centerX += dx; centerY += dy; }

    float left() const { return centerX - width / 2; }
    float top() const { return centerY - height / 2; }

    SDL_FPoint toScreen(float x, float y) const { return {x - left(), y - top()}; }
    SDL_FPoint toWorld(float x, float y) const { return {x + left(), y + top()}; }

    SDL_Rect toScreen(const SDL_FRect& r) const {
        SDL_FPoint p = toScreen(r.x, r.y);
        return {int(p.x), int(p.y), int(r.w), int(r.h)};
    }

    bool isVisible(const SDL_FRect& r) const {
        return r.x < left() + width && r.x + r.w > left() && r.y < top() + height && r.y + r.h > top();
    }
};
