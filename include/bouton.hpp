#ifndef __BOUTON__HPP__
#define __BOUTON__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"
#include "texte.hpp"

enum etat_bouton
{
    IDLE,
    HOVERED,
    CLICKED
};

class Bouton
{
    public:
        SDL_Color couleur_idle;
        SDL_Color couleur_hover;
        SDL_Color couleur_click;
        SDL_Rect position;

        typedef void(*eventFunction)(SingletonSysteme*, Bouton*);
        eventFunction funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton

        etat_bouton etat;
        Texte texte;

        //hover sound, click sound
        Mix_Chunk* hover_sound;
        Mix_Chunk* click_sound;
        bool son_joue = false;

        Bouton(SDL_Color couleur_idle, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Rect position, eventFunction funcPtr, std::string texte);
        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);
        bool clicAvantCollision;

    protected:

    private:
};

#endif // __BOUTON__HPP__
