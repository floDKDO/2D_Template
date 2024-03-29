#ifndef __SELECTIONNABLE__HPP__
#define __SELECTIONNABLE__HPP__

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

        SDL_Rect position;
        std::string name;
        etat etat;
        typedef void(*eventFunction)(SingletonSysteme*, Selectionnable*);
        eventFunction funcPtr; //pointeur sur la fonction qui sera lanc�e quand il y aura un clic sur le selectionnable
        Selectionnable* selectOnUp;
        Selectionnable* selectOnDown;
        Selectionnable* selectOnLeft;
        Selectionnable* selectOnRight;
        static bool verrou; //pour empecher le double defilement (passer d'un ui � deux autres en un appuie)

        //diff�rence entre une m�thode virtuelle et une m�thode virtuelle pure
        // => la m�thode virtuelle pure doit �tre obligatoirement red�finie par les classes filles, pas la m�thode uniquement virtuelle
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

    protected:

    private:
};

#endif // __SELECTIONNABLE__HPP__
