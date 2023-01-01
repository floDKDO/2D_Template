#ifndef __MENU__CHOIX__NOM__HPP__
#define __MENU__CHOIX__NOM__HPP__

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "selectionnable.hpp"
#include "bouton.hpp"
#include "texte.hpp"
#include "toggle.hpp"
#include "inputfield.hpp"
#include "singleton_systeme.hpp"

class MenuChoixNom
{
    public:
        MenuChoixNom(SingletonSysteme* sing_syst);

        std::vector<Selectionnable*> ui; //contient tous les boutons du menu principal

        Inputfield inputfield;
        Texte demande_nom;
        Bouton bouton_valider;

        //static sinon probleme de type avec le pointeur de fonction
        static void fonc_bouton_fin_demande_nom(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_inputfield_nom_joueur(SingletonSysteme* sing_syst, Selectionnable* inputfield);

        void resetSelected(void);
        Selectionnable* getSelected(void);

        void Draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void Update(Uint32& timeStep);

    protected:

    private:
};

#endif // __MENU__CHOIX__NOM__HPP__
