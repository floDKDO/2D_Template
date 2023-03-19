#ifndef __EN__JEU__HPP__
#define __EN__JEU__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"
#include "joueur.hpp"
#include "tuile.hpp"
#include "carte.hpp"
#include <vector>

class EnJeu
{
    public:
        EnJeu(SingletonSysteme* sing_syst);

        Carte* carte_actuelle;

        SDL_Rect carre_noir;
        bool enTransition;
        int alpha;

        Joueur joueur;

        bool changement_de_carte;

        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __EN__JEU__HPP__
