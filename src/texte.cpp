#include "texte.hpp"

Texte::Texte(std::string texte, std::string police, SDL_Color couleur, SDL_Rect position)
{
    if((this->police = TTF_OpenFont(police.c_str(), 50)) == nullptr)
    {
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    this->couleur = couleur;
    this->texte = texte;
    this->position = position;
}

void Texte::Draw(SDL_Renderer* rendu)
{
    this->surface = TTF_RenderText_Solid(police, texte.c_str(), couleur);

    if(surface != nullptr)
    {
        if((texture = SDL_CreateTextureFromSurface(rendu, surface)) == nullptr)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        position.w = surface->w;
        position.h = surface->h;
    }
    SDL_RenderCopy(rendu, texture, nullptr, &position);
}
