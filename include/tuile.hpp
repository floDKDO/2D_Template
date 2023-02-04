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
        Tuile(std::string chemin, SDL_Rect position);
        SDL_Rect position;
        SDL_Texture* texture;
        bool estPassable;
        std::string chemin;

        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);

        void Update(float timeStep);
        void Clean();

    protected:

    private:
};

#endif // __TUILE__HPP__
