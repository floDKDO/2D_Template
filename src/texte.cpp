#include "texte.hpp"

Texte::Texte(std::string texte, std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, SDL_Renderer* rendu, std::string name, bool isDialogue)
{
    (void)rendu;
    NCHK(this->police = TTF_OpenFont(police.c_str(), taille_police), TTF_GetError());
    this->taille_police = taille_police;
    this->couleur = couleur;
    this->texte = texte;
    this->position = position;
    this->name = name;
    // dialogue //////////////
    this->isDialogue = isDialogue;
    this->indice = 0;
    this->texteDefilement = "";
    //////////////////////////

    if(isDialogue == false)
    {
        if(this->texte.length() > 0)
        {
            NCHK(this->surface = TTF_RenderText_Solid(this->police, this->texte.c_str(), couleur), TTF_GetError());
        }
        else NCHK(this->surface = TTF_RenderText_Solid(this->police, " ", couleur), TTF_GetError());

        position.w = surface->w;
        position.h = surface->h;
        NCHK(texture = SDL_CreateTextureFromSurface(rendu, surface), SDL_GetError());
    }
}


void Texte::draw(SDL_Renderer* rendu)
{
    if(isDialogue == true)
    {
        if(this->texteDefilement.length() > 0)
        {
            NCHK(this->surface = TTF_RenderText_Solid_Wrapped(this->police, this->texteDefilement.c_str(), couleur, wrapLength), TTF_GetError());
        }
        else NCHK(this->surface = TTF_RenderText_Solid_Wrapped(this->police, " ", couleur, wrapLength), TTF_GetError());

        NCHK(texture = SDL_CreateTextureFromSurface(rendu, surface), SDL_GetError());
    }

    position.w = surface->w;
    position.h = surface->h;
    CHK(SDL_RenderCopy(rendu, texture, nullptr, &position), SDL_GetError());

    if(isDialogue == true)
    {
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}


void Texte::editText(std::string nouveau_texte, SDL_Renderer* rendu)
{
    if(isDialogue == false)
    {
        if(this->texte.length() > 0)
        {
            NCHK(this->surface = TTF_RenderText_Solid(this->police, nouveau_texte.c_str(), couleur), TTF_GetError());
        }
        else NCHK(this->surface = TTF_RenderText_Solid(this->police, " ", couleur), TTF_GetError());
    }
    else
    {
        if(this->texteDefilement.length() > 0)
        {
            NCHK(this->surface = TTF_RenderText_Solid_Wrapped(this->police, nouveau_texte.c_str(), couleur, wrapLength), TTF_GetError());
        }
        else NCHK(this->surface = TTF_RenderText_Solid_Wrapped(this->police, " ", couleur, wrapLength), TTF_GetError());
    }
    position.w = surface->w;
    position.h = surface->h;
    NCHK(texture = SDL_CreateTextureFromSurface(rendu, surface), SDL_GetError());
}


void Texte::update(Uint32& timeStep)
{
    if(isDialogue == true)
    {
        if(SDL_GetTicks() - timeStep > 50 && indice < texte.length()) //50 => vitesse de défilement (200 ms par lettre)
        {
            this->texteDefilement.push_back(this->texte[indice]);
            indice += 1;
            timeStep = SDL_GetTicks();
        }
    }
}
