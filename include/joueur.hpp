#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"

#include <vector>
#include "tuile.hpp"

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
        void resetAllValues();
        void setValue(int indice);
        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst/*, std::vector<Tuile> tuiles*/);

        SDL_Color couleur;
        SDL_Texture* texture;
        SDL_Rect position;
        unsigned int multiplication_vitesse;
        mode_affichage mode;
        std::string chemin;
        bool isAnimated;
        SDL_Rect srcRect;

        // Que pour mode vue de cote
        bool surSol;
        float vitesseDeChute;
        float hauteur_saut;
        ////////////////////////////

        bool dep[4]; //dep[0] = HAUT, dep[1] = BAS, dep[2] = GAUCHE, dep[3] = DROITE
        unsigned int orientation; //0 = HAUT, 1 = BAS, 2 = GAUCHE, 3 = DROITE
        bool interagit; //il appuie sur le bouton d'intéraction

    protected:

    private:
};

#endif // JOUEUR_HPP
