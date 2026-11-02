#pragma once
#include <string>
#include "BodyComponent.h"
#include "Textures.h"

class SpriteComponent : public Component {
public:
    bool visible = true;
    Uint8 alpha = 255;
    int offsetX = 0;

    SpriteComponent(GameObject& parent, const std::string& textureKey, bool flip, bool tile)
        : Component(parent), texture(Textures::get(textureKey)), flip(flip), tile(tile) {}

    void setTexture(const std::string& key) { texture = Textures::get(key); }

    void update() override {}

    void draw() override {
        auto body = parent().get<BodyComponent>();
        if (!visible || !body || !texture) return;

        if (body->vx < -1) facingLeft = true;
        if (body->vx > 1)  facingLeft = false;

        SDL_SetTextureAlphaMod(texture, alpha);
        int x = int(body->x) + offsetX, y = int(body->y), w = int(body->w), h = int(body->h);
        if (tile) {
            for (int tx = 0; tx < w; tx += h) {
                SDL_Rect src{0, 0, 0, 0};
                SDL_QueryTexture(texture, nullptr, nullptr, &src.w, &src.h);
                int pw = std::min(h, w - tx);
                src.w = src.w * pw / h;
                SDL_Rect dst{x + tx, y, pw, h};
                SDL_RenderCopy(Engine::renderer, texture, &src, &dst);
            }
        } else {
            SDL_Rect dst{x, y, w, h};
            SDL_RendererFlip f = (flip && facingLeft) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(Engine::renderer, texture, nullptr, &dst, body->angle, nullptr, f);
        }
    }

private:
    SDL_Texture* texture;
    bool flip, tile;
    bool facingLeft = false;
};
