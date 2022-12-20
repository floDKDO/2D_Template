#ifndef __TEXTE__HPP__
#define __TEXTE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class Texte
{
    public:
        Texte(std::string texte, std::string police, SDL_Color couleur, SDL_Rect position);
        void Draw(SDL_Renderer* rendu);

        std::string texte;
        TTF_Font* police;
        SDL_Color couleur;
        SDL_Rect position;
        SDL_Surface* surface;
        SDL_Texture* texture;

    protected:

    private:
};

#endif // __TEXTE__HPP__
