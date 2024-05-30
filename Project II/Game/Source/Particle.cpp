#include "Particle.h"
#include "App.h"
#include <iostream>
#include <cmath>
#include <cstdlib>


Particle::Particle(SDL_Texture* texture, int x, int y, float lifetime, int xOffset, int type)
    : texture(texture), lifetime(lifetime), xOffset(xOffset), type(type), age(0.0f), alpha(255.0f), currentGreen(255)
{
    destRect.x = x;
    destRect.y = y;

    destRect.w = 32;
    destRect.h = 32;
    if (type == 0)
    {
        velocityX = 0.03f;
        velocityY = 1.0f;
    }
    else if (type == 1)
    {
        velocityX = (rand() % 200 - 100) / 200.0f;
        velocityY = -1.0f;
    }
}

void Particle::Update(float dt) {
    age += dt;
    if (age < lifetime) {
        if (type == 0)
        {
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
        }
        if (type == 1)
        {
            destRect.x += velocityX * dt;

            velocityY += 0.004f * dt;
            destRect.y += velocityY * dt; 

            if (currentGreen > 0) {
                currentGreen = 180 * (1.0f - age / (lifetime / 0.2f));
            }
            else {
                currentGreen = 0;
            }
        }

       

        alpha = 255 * (1.0f - age / lifetime);
    }
}

void Particle::Draw(SDL_Renderer* renderer) {
    if (IsAlive()) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);

        if (type == 0)
        {
			SDL_SetTextureColorMod(texture, 255, currentGreen, 0);
		}
        else if (type == 1)
        {
            SDL_SetTextureColorMod(texture, 0, currentGreen, 255);
        }
        
        SDL_SetTextureAlphaMod(texture, (Uint8)alpha);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
}

bool Particle::IsAlive() const {
    return age < lifetime;
}
