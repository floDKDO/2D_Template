#include "tuile.hpp"

//Tuile fixe
Tuile::Tuile(std::string chemin, SDL_Rect position, bool estPassable, bool isWarp, SDL_Renderer* rendu)
{
    this->position = position;
    this->estPassable = estPassable;
    this->chemin = chemin;
    this->isAnimated = false;
    this->isWarp = isWarp;
    this->collisionAvecJoueur = false;

    NCHK(this->texture = IMG_LoadTexture(rendu, this->chemin.c_str()), IMG_GetError());
}


//Tuile animée
Tuile::Tuile(std::string chemin, SDL_Rect position, unsigned int nb_images, int espacement_tuiles_x, bool estPassable, SDL_Renderer* rendu)
{
    this->position = position;
    this->estPassable = estPassable;
    this->chemin = chemin;
    this->isAnimated = true;
    this->collisionAvecJoueur = false;

    this->nb_images = nb_images;
    this->espacement_tuiles_x = espacement_tuiles_x;

    NCHK(this->texture = IMG_LoadTexture(rendu, this->chemin.c_str()), IMG_GetError());
}


void Tuile::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    SDL_Rect temp = {this->position.x - camera.x, this->position.y - camera.y, this->position.w, this->position.h};

    if(isAnimated == true)
    {
        CHK(SDL_RenderCopy(rendu, this->texture, &(this->srcRect), &temp), SDL_GetError());
    }
    else
    {
        CHK(SDL_RenderCopy(rendu, this->texture, nullptr, &temp), SDL_GetError());
    }
}


void Tuile::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    (void)timeStep;
    (void)sing_syst;
    if(isAnimated == true)
    {
        Uint32 seconds = SDL_GetTicks() / 200; //200 => rapidité de l'animation
        int sprite = seconds % nb_images;
        this->srcRect = {espacement_tuiles_x * sprite, 0, 16, 16};
    }
}


void Tuile::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
}
