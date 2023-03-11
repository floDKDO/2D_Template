#include "tuile.hpp"

//Tuile fixe
Tuile::Tuile(std::string chemin, SDL_Rect position, bool estPassable, bool isWarp/*, int connection_carte*/)
{
    this->position = position;
    this->estPassable = estPassable;
    this->chemin = chemin;
    this->isAnimated = false;
    this->isWarp = isWarp;

    //this->connection_carte = carte;
}


//Tuile animée
Tuile::Tuile(std::string chemin, SDL_Rect position, unsigned int nb_images, int espacement_tuiles_x, bool estPassable)
{
    this->position = position;
    this->estPassable = estPassable;
    this->chemin = chemin;
    this->isAnimated = true;

    this->nb_images = nb_images;
    this->espacement_tuiles_x = espacement_tuiles_x;
}


void Tuile::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    if((this->texture = IMG_LoadTexture(rendu, this->chemin.c_str())) == nullptr)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_Rect temp = {this->position.x - camera.x, this->position.y - camera.y, this->position.w, this->position.h};

    if(isAnimated == true)
    {
        if(SDL_RenderCopy(rendu, this->texture, &(this->srcRect), &temp) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if(SDL_RenderCopy(rendu, this->texture, nullptr, &temp) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
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
