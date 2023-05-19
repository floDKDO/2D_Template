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

        //différence entre une méthode virtuelle et une méthode virtuelle pure
        // => la méthode virtuelle pure doit être obligatoirement redéfinie par les classes filles, pas la méthode uniquement virtuelle

        virtual void draw(SDL_Renderer* rendu) = 0;
        virtual void handleEvents(SDL_Event e, SingletonSysteme* sing_syst) = 0;
        virtual bool collision(SDL_Rect dest, int x, int y) = 0;
        virtual void setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight) = 0;
        virtual void setSelected(Selectionnable* ui) = 0;
        virtual void setUnselected(Selectionnable* previous) = 0;

        virtual void onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onPointerExit(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onPointerDown(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onClick(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onKeyPressed(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onKeyReleased(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onControllerButtonPressed(SDL_Event e, SingletonSysteme* sing_syst);
        virtual void onControllerButtonReleased(SDL_Event e, SingletonSysteme* sing_syst);

        SDL_Rect position;
        std::string name;

        etat etat;
        typedef void(*eventFunction)(SingletonSysteme*, Selectionnable*);
        eventFunction funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le selectionnable
        Selectionnable* selectOnUp;
        Selectionnable* selectOnDown;
        Selectionnable* selectOnLeft;
        Selectionnable* selectOnRight;

        static bool verrou; //pour empecher le double defilement (passer d'un ui à deux autres en un appuie)

    protected:

    private:
};

#endif // __SELECTIONNABLE__HPP__
