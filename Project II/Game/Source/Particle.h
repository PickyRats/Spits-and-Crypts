#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "SDL/include/SDL.h"

class Particle {
public:
    Particle(SDL_Texture* texture, int x, int y, float lifetime, int xOffset, int type);

    void Update(float dt);
    void Draw(SDL_Renderer* renderer);

    bool IsAlive() const;

private:
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    Uint8 currentGreen = 255;
    float lifetime;
    float age;
    float velocityX, velocityY;
    float alpha;
    int xOffset;
    int type;
};

#endif // __PARTICLE_H__
