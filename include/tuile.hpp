#ifndef __TUILE__HPP__
#define __TUILE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"
#include "elementCarte.hpp"


class Tuile : public ElementCarte
{
    public:
        //chemin : endroit o� la tuile est stock�e
        Tuile(std::string chemin, SDL_Rect position, bool estPassable, bool isWarp, SDL_Renderer* rendu);
        Tuile(std::string chemin, SDL_Rect position, unsigned int nb_images, int espacement_tuiles_x, bool estPassable, SDL_Renderer* rendu);

        unsigned int nb_images;

        //espacement des tuiles dans le tilesheet
        int espacement_tuiles_x;

        SDL_Rect srcRect;
        SDL_Texture* texture;
        bool isAnimated;

        bool isWarp;

        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);

        void update(Uint32& timeStep, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __TUILE__HPP__
