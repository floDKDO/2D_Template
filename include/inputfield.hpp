#ifndef __INPUTFIELD__HPP__
#define __INPUTFIELD__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "texte.hpp"
#include "singleton_systeme.hpp"

class Inputfield
{
    public:

        typedef void(*eventFunction)(SingletonSysteme*, Inputfield*);
        eventFunction funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un appuie sur entrée ou un clic en dehors de l'inputfield

        bool mode_edition;
        SDL_Rect zone_de_texte;
        SDL_Rect fond_de_texte;
        bool texte_modifie;
        SDL_Surface* surface;

        Texte texte;
        Texte texte_placeHolder;

        Inputfield(std::string police, SDL_Color couleur, SDL_Rect position, eventFunction funcPtr, SDL_Renderer* rendu);
        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);

    protected:

    private:
};

#endif // __INPUTFIELD__HPP__
