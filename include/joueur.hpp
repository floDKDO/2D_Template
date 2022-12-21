#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

enum mode_affichage
{
    VUE_DESSUS,
    VUE_COTE
};

class Joueur
{
    public:
        Joueur(unsigned int pv, SDL_Color couleur, SDL_Rect position, mode_affichage mode);
        void ResetAllValues(bool dep[4]);
        void SetValue(bool dep[4], int indice);
        void Draw(SDL_Renderer* rendu);
        void HandleEvents(SDL_Event e);
        void Update(Uint32& timeStep);

        SDL_Color couleur;
        SDL_Texture* texture;
        SDL_Rect position;
        unsigned int pv;
        unsigned int multiplication_vitesse;
        mode_affichage mode;

        // Que pour mode vue de cote
        bool surSol;
        float vitesseDeChute;
        float hauteur_saut;
        ////////////////////////////

        bool dep[4]; //dep[0] = HAUT, dep[1] = BAS, dep[2] = GAUCHE, dep[3] = DROITE

    protected:

    private:
};

#endif // JOUEUR_HPP
