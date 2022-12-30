#ifndef __SELECTIONNABLE__HPP__
#define __SELECTIONNABLE__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"

enum etat
{
    NORMAL,
    HOVERED,
    CLICKED,
    SELECTED
};

//classe mere virtuelle pure (pour polymorphisme) de Toggle, Bouton et Inputfield
class Selectionnable
{
    public:
        Selectionnable(){}
        virtual void Draw(SDL_Renderer* rendu) = 0;
        virtual void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst) = 0;
        virtual bool collision(SDL_Rect dest_joueur, int x, int y) = 0;
        virtual void setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight) = 0;
        virtual void setSelected(Selectionnable* ui) = 0;
        virtual void setUnselected(Selectionnable* previous) = 0;
        etat etat;
        typedef void(*eventFunction)(SingletonSysteme*, Selectionnable*);
        eventFunction funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton
        Selectionnable* selectOnUp;
        Selectionnable* selectOnDown;
        Selectionnable* selectOnLeft;
        Selectionnable* selectOnRight;

        static bool verrou; //pour empecher le double defilement (passer d'un ui à deux autres en un appuie)

    protected:

    private:
};

#endif // __SELECTIONNABLE__HPP__
