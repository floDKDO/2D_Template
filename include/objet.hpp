#ifndef __OBJET__HPP__
#define __OBJET__HPP__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"


class Objet
{
    public:
        Objet(std::string chemin, SDL_Renderer* rendu, SDL_Rect position);
        Objet(SDL_Renderer* rendu, SDL_Color couleur, SDL_Rect position);

        std::string nom_objet;
        std::string chemin;
        SDL_Rect position; //pos_x et pos_y
        SDL_Rect srcRect;
        SDL_Texture* texture;
        SDL_Color couleur;

        bool has_image;

        std::string id_objet;
        unsigned int movement_type;
        int movement_range_x;
        int movement_range_y;

        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);

        void update(Uint32& timeStep, SingletonSysteme* sing_syst);


    protected:

    private:
};

#endif // __OBJET__HPP__
