#ifndef __TUILE__HPP__
#define __TUILE__HPP__

#include "singleton_systeme.hpp"
#include "elementCarte.hpp"


class Tuile : public ElementCarte
{
    public:
        //chemin : endroit où la tuile est stockée
        Tuile(std::string chemin, SDL_Rect position, bool estPassable, bool isWarp, SDL_Renderer* rendu);
        Tuile(std::string chemin, SDL_Rect position, unsigned int nb_images, int espacement_tuiles_x, bool estPassable, SDL_Renderer* rendu);
        Tuile(std::string chemin, SDL_Rect position, SDL_Rect srcRect, bool estPassable, SingletonSysteme* sing_syst);

        unsigned int nb_images;
        //espacement des tuiles dans le tilesheet
        int espacement_tuiles_x;
        SDL_Rect srcRect;
        SDL_Texture* texture;
        bool isAnimated;
        bool isWarp;
        bool isTileset;

        void draw(SDL_Renderer* rendu, SDL_Rect camera);
        void handleEvents(SDL_Event e, SingletonSysteme* sing_syst);
        void update(Uint32& timeStep, SingletonSysteme* sing_syst);

    protected:

    private:
};

#endif // __TUILE__HPP__
