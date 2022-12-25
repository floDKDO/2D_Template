#ifndef __TOGGLE__HPP__
#define __TOGGLE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "texte.hpp"
#include "bouton.hpp"
#include "singleton_systeme.hpp"

typedef void(*eventFunction)(SingletonSysteme*, Bouton*);


class Toggle
{
    public:
        Toggle(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, std::string texte, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu);
        Toggle(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, std::string texte, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu);

        Bouton bouton;

        SDL_Texture* check;
        bool isChecked;

        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);

        void onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst);
        void onPointerExit(SDL_Event e, SingletonSysteme* sing_syst);
        void onPointerDown(SDL_Event e, SingletonSysteme* sing_syst);
        void onClick(SDL_Event e, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __TOGGLE__HPP__
