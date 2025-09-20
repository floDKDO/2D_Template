#ifndef __INPUTFIELD__HPP__
#define __INPUTFIELD__HPP__

#include "selectionnable.hpp"
#include "texte.hpp"
#include "singleton_systeme.hpp"

class Inputfield : public Selectionnable
{
    public:

        Inputfield(std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, eventFunction funcPtr, SDL_Renderer* rendu, std::string name);

        bool mode_edition;
        SDL_Rect zone_de_texte;
        SDL_Rect fond_de_texte;
        bool texte_modifie;
        SDL_Surface* surface;
        Texte texte;
        Texte texte_placeHolder;
        Texte curseur;
        int place; //place prise par un seul caractère
        bool affiche_curseur;
        //Mix_Chunk* hover_sound;
        //Mix_Chunk* click_sound;

        void draw(SDL_Renderer* rendu);
        void update(Uint32& timeStep);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        bool collision(SDL_Rect dest, int x, int y);
        void setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight);
        void setSelected(Selectionnable* ui);
        void setUnselected(Selectionnable* previous);
        void selectNew(Selectionnable* ui, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __INPUTFIELD__HPP__
