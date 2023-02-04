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

        Texte texte;

        Mix_Chunk* hover_sound;
        Mix_Chunk* click_sound;
        bool son_joue;
        int previousEtat;

        bool clicAvantCollision;


        Bouton(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, eventFunction funcPtr, std::string texte, int taille_police, SDL_Renderer* rendu, std::string name);
        Bouton(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, eventFunction funcPtr, std::string texte, int taille_police, SDL_Renderer* rendu, std::string name);
        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        //void Update(SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest_joueur, int x, int y);

        void onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst);
        void onPointerExit(SDL_Event e, SingletonSysteme* sing_syst);
        void onPointerDown(SDL_Event e, SingletonSysteme* sing_syst);
        void onClick(SDL_Event e, SingletonSysteme* sing_syst);
        void onKeyPressed(SDL_Event e, SingletonSysteme* sing_syst);
        void onKeyReleased(SDL_Event e, SingletonSysteme* sing_syst);
        void onControllerButtonPressed(SDL_Event e, SingletonSysteme* sing_syst);
        void onControllerButtonReleased(SDL_Event e, SingletonSysteme* sing_syst);

        void setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight);
        void setSelected(Selectionnable* ui);
        void setUnselected(Selectionnable* previous);
        void fonc(Selectionnable* ui, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __BOUTON__HPP__
