#ifndef __BOUTON__HPP__
#define __BOUTON__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "selectionnable.hpp"
#include "singleton_systeme.hpp"
#include "texte.hpp"

class Bouton : public Selectionnable
{
    public:
        SDL_Color couleur_normal;
        SDL_Color couleur_hover;
        SDL_Color couleur_click;
        SDL_Color couleur_selected;

        SDL_Texture* image_normal;
        SDL_Texture* image_hover;
        SDL_Texture* image_click;
        SDL_Texture* image_selected;

        bool hasImage;

        SDL_Rect position;

        //typedef void(*eventFunction)(SingletonSysteme*, Bouton*);
        //eventFunction funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton

        //etat etat;
        Texte texte;

        Mix_Chunk* hover_sound;
        Mix_Chunk* click_sound;
        bool son_joue = false;

        std::string tag;

        Bouton(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, eventFunction funcPtr, std::string texte, SDL_Renderer* rendu, std::string tag);
        Bouton(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, eventFunction funcPtr, std::string texte, SDL_Renderer* rendu);
        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        //void Update(SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);
        bool clicAvantCollision;

        void onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst);
        void onPointerExit(SDL_Event e, SingletonSysteme* sing_syst);
        void onPointerDown(SDL_Event e, SingletonSysteme* sing_syst);
        void onClick(SDL_Event e, SingletonSysteme* sing_syst);
        void onKeyPressed(SDL_Event e, SingletonSysteme* sing_syst);
        void onKeyReleased(SDL_Event e, SingletonSysteme* sing_syst);

        void setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight);
        void setSelected(Selectionnable* ui);
        void setUnselected(Selectionnable* previous);
        void fonc(Selectionnable* ui, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __BOUTON__HPP__
