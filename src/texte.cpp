#include "texte.hpp"

Texte::Texte(std::string texte, std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, SDL_Renderer* rendu, std::string name, bool dialogue)
{
    (void)rendu;
    NCHK(this->police = TTF_OpenFont(police.c_str(), taille_police), TTF_GetError());
    this->taille_police = taille_police;
    this->couleur = couleur;
    this->texte = texte;
    this->position = position;
    this->name = name;
    // dialogue //////////////
    this->dialogue = dialogue;
    this->i = 0;
    this->texteDefilement = "";
    //////////////////////////
}

void Texte::Draw(SDL_Renderer* rendu)
{
    if(dialogue == false)
    {
        if(this->texte.length() > 0)
        {
            NCHK(this->surface = TTF_RenderText_Solid(this->police, this->texte.c_str(), couleur), TTF_GetError());
        }
        else
        {
            NCHK(this->surface = TTF_RenderText_Solid(this->police, " ", couleur), TTF_GetError());
        }
        CHK(TTF_SizeText(this->police, this->texte.c_str(), &(this->position.w), &(this->position.h)), TTF_GetError());
    }
    else
    {
        if(this->texteDefilement.length() > 0)
        {
            NCHK(this->surface = TTF_RenderText_Solid(this->police, this->texteDefilement.c_str(), couleur), TTF_GetError());
        }
        else
        {
            NCHK(this->surface = TTF_RenderText_Solid(this->police, " ", couleur), TTF_GetError());
        }
        CHK(TTF_SizeText(this->police, this->texteDefilement.c_str(), &(this->position.w), &(this->position.h)), TTF_GetError());
    }
    NCHK(texture = SDL_CreateTextureFromSurface(rendu, surface), SDL_GetError());
    CHK(SDL_RenderCopy(rendu, texture, nullptr, &position), SDL_GetError());
}


void Texte::Update(Uint32& timeStep)
{
    if(dialogue == true)
    {
        if(SDL_GetTicks() - timeStep > 200 && i < texte.length()) //200 => vitesse de défilement (200 ms par lettre)
        {
            this->texteDefilement.push_back(this->texte[i]);
            i += 1;
            timeStep = SDL_GetTicks();
        }
    }
}
