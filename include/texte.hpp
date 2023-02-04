#ifndef __TEXTE__HPP__
#define __TEXTE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"

class Texte
{
    public:
        Texte(std::string texte, std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, SDL_Renderer* rendu, std::string name, bool dialogue);
        void Draw(SDL_Renderer* rendu);
        void Update(Uint32& timeStep);

        SDL_Rect position;
        std::string texte;

        //si le texte est un dialogue avec un défilement
        std::string texteDefilement;
        bool dialogue;
        unsigned int i;
        ////////////////////////////////////////////////

        std::string name;
        TTF_Font* police;
        SDL_Color couleur;
        SDL_Surface* surface;
        SDL_Texture* texture;
        int taille_police;

    protected:

    private:
};

#endif // __TEXTE__HPP__
