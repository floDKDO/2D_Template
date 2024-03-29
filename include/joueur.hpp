#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include "tuile.hpp"
#include "singleton_systeme.hpp"

#include <vector>


enum mode_affichage
{
    VUE_DESSUS,
    VUE_COTE
};

class Joueur
{
    public:

        Joueur(SDL_Color couleur, SDL_Rect position, mode_affichage mode);
        Joueur(std::string chemin, SDL_Rect position, mode_affichage mode, SDL_Renderer* rendu);

        SDL_Color couleur;
        SDL_Texture* texture;
        SDL_Rect position;
        unsigned int multiplication_vitesse;
        mode_affichage mode;
        std::string chemin;
        bool isAnimated;
        SDL_Rect srcRect;
        bool je_peux_marcher;
        bool dep[4]; //dep[0] = HAUT, dep[1] = BAS, dep[2] = GAUCHE, dep[3] = DROITE
        unsigned int orientation; //0 = HAUT, 1 = BAS, 2 = GAUCHE, 3 = DROITE
        bool interagit; //il appuie sur le bouton d'intéraction
        // Que pour mode vue de cote
        bool surSol;
        float vitesseDeChute;
        float hauteur_saut;
        ////////////////////////////

        void resetAllValues();
        void setValue(int indice);
        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // JOUEUR_HPP
