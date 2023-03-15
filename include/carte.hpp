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
        std::vector<Carte*> warp_cartes; //contient les cartes atteignables par warp
        ////////////////////////////////

        int limite_haut;
        int limite_bas;
        int limite_gauche;
        int limite_droite;

        bool est_carte_principale;

        std::vector<Tuile> tuiles;

        int x_depart, y_depart;

        void initJson(std::string fichier_carte, SingletonSysteme* sing_syst);
        void initConnections(std::string fichier_carte, SingletonSysteme* sing_syst);
        void initConnections1(std::string fichier_carte, SingletonSysteme* sing_syst);
        void initTuiles(std::string fichier_carte);
        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __CARTE__HPP__
