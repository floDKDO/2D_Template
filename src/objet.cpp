#include "../include/objet.hpp"

Objet::Objet(std::string chemin, SDL_Renderer* rendu, SDL_Rect position)
{
    this->chemin = chemin;
    this->has_image = true;
    this->position = position;
    this->estPassable = false;

    this->nom_objet = "TEST";

    NCHK(this->texture = IMG_LoadTexture(rendu, this->chemin.c_str()), IMG_GetError());
}

Objet::Objet(SDL_Renderer* rendu, SDL_Color couleur, SDL_Rect position)
{
    (void)rendu;
    this->couleur = couleur;
    this->has_image = false;
    this->position = position;
    this->estPassable = false;

    this->nom_objet = "TEST";
}


void Objet::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    SDL_Rect temp = {this->position.x - camera.x, this->position.y - camera.y, this->position.w, this->position.h};
    if(this->has_image == true)
    {
        CHK(SDL_RenderCopy(rendu, this->texture, nullptr, &temp), SDL_GetError());
    }
    else
    {
        CHK(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a), SDL_GetError());
        CHK(SDL_RenderFillRect(rendu, &temp), SDL_GetError());
    }
}



void Objet::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
}

void Objet::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    (void)timeStep;
    (void)sing_syst;
}
