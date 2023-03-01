#ifndef __TUILE__HPP__
#define __TUILE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"

class Tuile
{
    public:
        //chemin : endroit où la tuile est stockée
        Tuile(std::string chemin, SDL_Rect position, bool estPassable);
        Tuile(std::string chemin, SDL_Rect position, unsigned int nb_images, int espacement_tuiles_x, bool estPassable);

        unsigned int nb_images;
        //espacement des tuiles dans le tilesheet
        int espacement_tuiles_x;

        SDL_Rect position;
        SDL_Rect srcRect;
        SDL_Texture* texture;
        bool estPassable;
        bool isAnimated;
        std::string chemin;

        void draw(SDL_Renderer* rendu);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest, int x, int y);

        void update(Uint32& timeStep, SingletonSysteme* sing_syst);
        void clean();

    protected:

    private:
};

#endif // __TUILE__HPP__
