#ifndef __MENU__PRINCIPAL__HPP__
#define __MENU__PRINCIPAL__HPP__


#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "selectionnable.hpp"
#include "bouton.hpp"
#include "texte.hpp"
#include "boiteDeDialogue.hpp"

#include "singleton_systeme.hpp"

class MenuPrincipal
{
    public:
        MenuPrincipal(SingletonSysteme* sing_syst);
        std::vector<Selectionnable*> ui; //contient tous les boutons du menu

        Bouton bouton_continuer;
        Texte texte_nom_joueur;
        Texte titre;
        Bouton bouton_nouvelle_partie;
        Bouton bouton_options;
        Bouton bouton_quitter;

        //BoiteDeDialogue b;

        //static sinon probleme de type avec le pointeur de fonction
        static void fonc_bouton_quitter(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_bouton_options(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_bouton_nouvelle_partie(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_bouton_continuer(SingletonSysteme* sing_syst, Selectionnable* bouton);

        void resetSelected(void);
        Selectionnable* getSelected(void);

        void draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst);
        void update(Uint32& timeStep);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __MENU__PRINCIPAL__HPP__
