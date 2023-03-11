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


class SingletonSysteme;


class Carte
{
    public:
        Carte(std::string fichier_carte);

        std::string fichier_carte;

        std::vector<Carte*> cartes;
        std::vector<Tuile> tuiles;

        int x_depart, y_depart;

        void initConnections(std::string fichier_carte, SingletonSysteme* sing_syst);
        void initTuiles(std::string fichier_carte);
        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __CARTE__HPP__
