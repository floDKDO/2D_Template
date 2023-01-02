#include "texte.hpp"

Texte::Texte(std::string texte, std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, SDL_Renderer* rendu, std::string name)
{
    (void)rendu;
    NCHK(this->police = TTF_OpenFont(police.c_str(), taille_police), TTF_GetError());
    this->taille_police = taille_police;
    this->couleur = couleur;
    this->texte = texte;
    this->position = position;
    this->name = name;
}

void Texte::Draw(SDL_Renderer* rendu)
{
    if(texte.length() > 0)
    {
        NCHK(this->surface = TTF_RenderText_Solid(this->police, this->texte.c_str(), couleur), TTF_GetError());

    }
    else
    {
        NCHK(this->surface = TTF_RenderText_Solid(this->police, " ", couleur), TTF_GetError());
    }
    NCHK(texture = SDL_CreateTextureFromSurface(rendu, surface), SDL_GetError());
    CHK(TTF_SizeText(this->police, this->texte.c_str(), &(this->position.w), &(this->position.h)), TTF_GetError());
    CHK(SDL_RenderCopy(rendu, texture, nullptr, &position), SDL_GetError());
}
