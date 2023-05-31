#ifndef __TOGGLE__HPP__
#define __TOGGLE__HPP__

#include "selectionnable.hpp"
#include "texte.hpp"
#include "singleton_systeme.hpp"


class Toggle : public Selectionnable
{
    public:
        Toggle(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, std::string texte, int taille_police, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu, std::string name);
        Toggle(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, std::string texte, int taille_police, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu, std::string name);

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
        SDL_Texture* check;
        bool isChecked;

        void draw(SDL_Renderer* rendu);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest, int x, int y);
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
        void selectNew(Selectionnable* ui, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __TOGGLE__HPP__
