#ifndef __INPUTFIELD__HPP__
#define __INPUTFIELD__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class Inputfield
{
    public:
        std::string texte;
        bool mode_edition;
        TTF_Font* police;
        SDL_Color couleur;
        SDL_Surface* surface;
        SDL_Texture* texture;
        SDL_Rect zone_de_texte;
        SDL_Rect fond_de_texte;
        bool texte_modifie;

        Inputfield(std::string police, SDL_Color couleur, SDL_Rect position);
        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e);
        bool collision(SDL_Rect dest_joueur, int x, int y);

    protected:

    private:
};

#endif // __INPUTFIELD__HPP__
