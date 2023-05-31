#ifndef __OBJET__HPP__
#define __OBJET__HPP__

#include "singleton_systeme.hpp"
#include "elementCarte.hpp"


class Objet : public ElementCarte
{
    public:
        Objet(std::string chemin, SDL_Renderer* rendu, SDL_Rect position);
        Objet(SDL_Renderer* rendu, SDL_Color couleur, SDL_Rect position);

        std::string nom_objet;
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
