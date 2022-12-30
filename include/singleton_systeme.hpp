#ifndef __SINGLETON__SYSTEME__HPP__
#define __SINGLETON__SYSTEME__HPP__

#define LONGUEUR_FENETRE 1280
#define HAUTEUR_FENETRE 720 //720!

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

enum etat_jeu
{
    MENU_PRINCIPAL,
    MENU_OPTIONS,
    DEMANDE_NOM,
    EN_JEU,
    TEST
};

enum mode_fenetre
{
    FENETRE,
    PLEIN_ECRAN
};

struct touches_clavier
{
    SDL_Keycode dep_haut;
    SDL_Keycode dep_bas;
    SDL_Keycode dep_gauche;
    SDL_Keycode dep_droite;
};

class SingletonSysteme
{
    public:
        static SingletonSysteme& instance()
        {
            static SingletonSysteme* instance = new SingletonSysteme();
            return *instance;
        }

        void Init();
        void Sauvegarder();
        void Charger();
        void Supprimmer();
        void Destroy();

        touches_clavier touches;

        SDL_Window* fenetre;
        SDL_Renderer* rendu;
        etat_jeu etat;
        mode_fenetre mode_fenetre;
        bool son_active;
        bool musique_activee;
        std::string nom_joueur;
        int posX_joueur;
        int posY_joueur;

        //MenuPrincipal menuPrincipal;

        //TODO : attribut Etat*

        std::fstream fichier_sauvegarde;

    protected:

    private:
        SingletonSysteme(){}
};

#endif // __SINGLETON__SYSTEME__HPP__
