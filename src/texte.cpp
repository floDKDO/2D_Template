#include "texte.hpp"

Texte::Texte(std::string texte, std::string police, SDL_Color couleur, SDL_Rect position)
{
    if((this->police = TTF_OpenFont(police.c_str(), 30)) == nullptr)
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
    if((this->surface = TTF_RenderText_Solid(police, texte.c_str(), couleur)) == nullptr)
    {
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if((texture = SDL_CreateTextureFromSurface(rendu, surface)) == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(SDL_RenderCopy(rendu, texture, nullptr, &position) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}
