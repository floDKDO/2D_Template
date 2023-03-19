#ifndef __CARTE__HPP__
#define __CARTE__HPP__

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <tuile.hpp>

#include <../json.hpp>
using json = nlohmann::json;


class SingletonSysteme;

struct carte_warp
{
    Carte* warp_carte;
    int x_warp;
    int y_warp;
    int x_arrive;
    int y_arrive;
};


class Carte
{
    public:
        Carte(std::string fichier_carte, bool est_carte_principale);

        //CHAMPS DU JSON
        unsigned int id_carte;
        std::string nom_carte;
        std::string fichier_carte;
        Mix_Music* musique;
        Carte* connection_haut;
        Carte* connection_bas;
        Carte* connection_gauche;
        Carte* connection_droite;

        std::vector<struct carte_warp> warp_cartes_test; //contient les cartes atteignables par warp, x_warp, y_warp, x_arrive et y_arrive
        ////////////////////////////////

        int limite_haut;
        int limite_bas;
        int limite_gauche;
        int limite_droite;

        bool une_fois;

        bool est_carte_principale; //ce booléen est inutile mais il permet de dire que la carte principale n'a pas de connexions

        std::vector<Tuile> tuiles;

        void initJson(std::string fichier_carte, SingletonSysteme* sing_syst);
        void initTuiles(std::string fichier_carte);
        void jouerMusique(void);
        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __CARTE__HPP__
