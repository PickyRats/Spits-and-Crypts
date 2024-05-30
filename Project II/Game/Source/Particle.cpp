#include "Particle.h"
#include "App.h"
#include <iostream>

Particle::Particle(SDL_Texture* texture, int x, int y, float lifetime, int xOffset)
    : texture(texture), lifetime(lifetime), xOffset(xOffset), age(0.0f), alpha(255.0f)
{
    destRect.x = x;
    destRect.y = y;

    destRect.w = 32;
    destRect.h = 32;

    velocityX = 0.03f;
    velocityY = 1.0f;
}

void Particle::Update(float dt) {
    age += dt;
    if (age < lifetime) {

        if (destRect.x <= xOffset) {
			destRect.x += velocityX * dt;
		}
        else if (destRect.x >= xOffset) {
			destRect.x -= velocityX * dt;
		}

        if (std::abs(destRect.x - xOffset) < 5) {
            velocityX += (rand() % 200 - 100) / 1500.0f;
        }
        
        destRect.y -= (velocityY / 8) * dt;

        if (currentGreen > 0) currentGreen = 180 * (1.0f - age / (lifetime));
        else currentGreen = 0;

        alpha = 255 * (1.0f - age / lifetime);
    }
}



void Particle::Draw(SDL_Renderer* renderer) {
    if (IsAlive()) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
        SDL_SetTextureColorMod(texture, 255, currentGreen, 0);
        SDL_SetTextureAlphaMod(texture, (Uint8)alpha);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
}

bool Particle::IsAlive() const {
    return age < lifetime;
}
