#ifndef __ELEMENT__CARTE__HPP__
#define __ELEMENT__CARTE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"


//classe virtuelle pure pour chaque élément constituant une carte (tuile, objet, pnj, monstre, ...)
class ElementCarte
{
    public:
        ElementCarte(){}

        virtual void draw(SDL_Renderer* rendu, SDL_Rect camera) = 0;
        virtual void handleEvents(SDL_Event e, SingletonSysteme* sing_syst) = 0;
        virtual void update(Uint32& timeStep, SingletonSysteme* sing_syst) = 0;

        SDL_Rect position;
        std::string chemin;
        SDL_Texture* texture;
        bool estPassable;
        bool collisionAvecJoueur;

    protected:

    private:
};

#endif // __ELEMENT__CARTE__HPP__
