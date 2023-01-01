#ifndef __MENU__OPTIONS__HPP__
#define __MENU__OPTIONS__HPP__

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
#include "singleton_systeme.hpp"

class MenuOptions
{
    public:
        MenuOptions(SingletonSysteme* sing_syst);
        std::vector<Selectionnable*> ui; //contient tous les boutons du menu principal

        Texte texte_touche_haut;
        Texte texte_touche_bas;
        Texte texte_touche_gauche;
        Texte texte_touche_droite;
        Texte mode_ecran;

        Bouton bouton_options_fenetre;
        Bouton bouton_options_retour;

        Bouton bouton_options_touche_haut;
        Bouton bouton_options_touche_bas;
        Bouton bouton_options_touche_gauche;
        Bouton bouton_options_touche_droite;

        Toggle toggle_sound;
        Toggle toggle_musique;

        //static sinon probleme de type avec le pointeur de fonction
        static void fonc_bouton_options_retour(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_choix_touche(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_toggle_son(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_toggle_musique(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_bouton_options_fenetre(SingletonSysteme* sing_syst, Selectionnable* bouton);

        void resetSelected(void);
        Selectionnable* getSelected(void);

        void Draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst);
        void HandleEvents(SDL_Event e, SingletonSysteme* sing_syst);

    protected:

    private:
};


#endif // MENUOPTIONS_HPP