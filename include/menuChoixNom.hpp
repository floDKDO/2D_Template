#ifndef __MENU__CHOIX__NOM__HPP__
#define __MENU__CHOIX__NOM__HPP__

#include <vector>

#include "../include/selectionnable.hpp"
#include "../include/bouton.hpp"
#include "../include/texte.hpp"
#include "../include/toggle.hpp"
#include "../include/inputfield.hpp"
#include "../include/singleton_systeme.hpp"

class MenuChoixNom
{
    public:
        MenuChoixNom(SingletonSysteme* sing_syst);

        std::vector<Selectionnable*> ui; //contient tous les boutons du menu
        Inputfield inputfield;
        Texte demande_nom;
        Bouton bouton_valider;

        //static sinon probleme de type avec le pointeur de fonction
        static void fonc_bouton_fin_demande_nom(SingletonSysteme* sing_syst, Selectionnable* bouton);
        static void fonc_inputfield_nom_joueur(SingletonSysteme* sing_syst, Selectionnable* inputfield);
        void resetSelected(void);
        Selectionnable* getSelected(void);
        void draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void update(Uint32& timeStep);

    protected:

    private:
};

#endif // __MENU__CHOIX__NOM__HPP__
