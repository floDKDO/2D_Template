#ifndef __SELECTIONNABLE__HPP__
#define __SELECTIONNABLE__HPP__

#include "../include/singleton_systeme.hpp"

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
        virtual ~Selectionnable(){}

        SDL_Rect position;
        etat etat;

        bool clicAvantCollision;
        Mix_Chunk* hover_sound;
        Mix_Chunk* click_sound;
        bool son_joue;
        int previousEtat;
        std::string name;
        typedef void(*eventFunction)(SingletonSysteme*, Selectionnable*);
        eventFunction funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le selectionnable
        Selectionnable* selectOnUp;
        Selectionnable* selectOnDown;
        Selectionnable* selectOnLeft;
        Selectionnable* selectOnRight;
        static bool verrou; //pour empecher le double defilement (passer d'un ui à deux autres en un appui)

        //différence entre une méthode virtuelle et une méthode virtuelle pure
        // => la méthode virtuelle pure doit être obligatoirement redéfinie par les classes filles, pas la méthode uniquement virtuelle
        virtual void draw(SDL_Renderer* rendu) = 0;
        virtual void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        virtual bool collision(SDL_Rect dest, int x, int y);
        virtual void setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight);
        virtual void setSelected(Selectionnable* ui);
        virtual void setUnselected(Selectionnable* previous);
        virtual void selectNew(Selectionnable* ui, SingletonSysteme* sing_syst);
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
