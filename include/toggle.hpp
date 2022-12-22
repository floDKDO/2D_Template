#ifndef __TOGGLE__HPP__
#define __TOGGLE__HPP__

#include "texte.hpp"
#include "singleton_systeme.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

typedef void(*eventFunction)(SingletonSysteme*);

enum etat_toggle
{
    CHECKED,
    UNCHECKED,
    HOVERED1
};


class Toggle
{
    public:
        Toggle(SDL_Color couleur_checked, SDL_Color couleur_unchecked, SDL_Color couleur_hover, SDL_Rect position, std::string texte, eventFunction funcPtr, bool isCheckedSave);
        Toggle(std::string image_checked, std::string image_unchecked, std::string image_hover_checked, std::string image_hover_unchecked, SDL_Rect position, std::string texte, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu);

        Texte texte;
        SDL_Color couleur_checked;
        SDL_Color couleur_unchecked;
        SDL_Color couleur_hover;

        SDL_Texture* image_checked;
        SDL_Texture* image_unchecked;
        SDL_Texture* image_hover_checked;
        SDL_Texture* image_hover_unchecked;

        bool hasImage;

        SDL_Rect position;
        bool isChecked;
        etat_toggle etat;
        eventFunction funcPtr;
        //hover sound, click sound
        Mix_Chunk* hover_sound;
        Mix_Chunk* click_sound;
        bool son_joue = false;
        bool clicAvantCollision;

        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);

    protected:

    private:
};

#endif // __TOGGLE__HPP__
