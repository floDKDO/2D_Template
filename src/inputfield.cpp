#include "inputfield.hpp"

Inputfield::Inputfield(std::string police, SDL_Color couleur, SDL_Rect position)
{
    if((this->police = TTF_OpenFont(police.c_str(), 50)) == nullptr)
    {
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    this->couleur = couleur;
    this->fond_de_texte = position;
    this->zone_de_texte = {fond_de_texte.x, fond_de_texte.y, 0, 0};
    this->mode_edition = false;
    this->texte_modifie = false;

    SDL_SetTextInputRect(&(this->zone_de_texte));
}

bool Inputfield::collision(SDL_Rect dest_joueur, int x, int y)
{
    //si pas de collision
    if(dest_joueur.y + dest_joueur.h > y
    && dest_joueur.y < y
    && dest_joueur.x + dest_joueur.w > x
    && dest_joueur.x < x)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Inputfield::Draw(SDL_Renderer* rendu)
{
    if(texte.length() > 0)
    {
        if((surface = TTF_RenderText_Solid(police, texte.c_str(), couleur)) == nullptr)
        {
            std::cerr << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        if((surface = TTF_RenderText_Solid(police, " ", couleur)) == nullptr)
        {
            std::cerr << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }


    if(surface != nullptr)
    {
        if((texture = SDL_CreateTextureFromSurface(rendu, surface)) == nullptr)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        zone_de_texte.w = surface->w;
        zone_de_texte.h = surface->h;
    }
    if(SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if(SDL_RenderFillRect(rendu, &fond_de_texte) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if(SDL_RenderCopy(rendu, texture, nullptr, &zone_de_texte) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Inputfield::HandleEvents(SDL_Event e)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    if(e.type == SDL_KEYDOWN)
    {
        if(e.key.keysym.sym == SDLK_BACKSPACE && texte.length() > 0 && mode_edition == true)
        {   //si on appuie sur la touche <- (supprimer), que la chaine n'est pas vide et qu'on est en mode édition
            texte.pop_back();
            texte_modifie = true;
        }
        else if(e.key.keysym.sym == SDLK_RETURN && mode_edition == true)
        {   //si on appuie sur entrée lors de la modification de la chaine, quitte l'inputfield
            mode_edition = false;
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT && collision(fond_de_texte, x, y) == true && mode_edition == false)
        {   //si on clique sur l'inputfield et qu'on est pas en mode édition
            mode_edition = true;
        }
        else if(e.button.button == SDL_BUTTON_LEFT && collision(fond_de_texte, x, y) == false && mode_edition == true)
        {   //si on clique autre part que sur l'inputfield et qu'on est en mode édition
            mode_edition = false;
        }
    }
    else if(e.type == SDL_TEXTINPUT)
    {
        if(mode_edition == true && texte.length() < 7) //max 7 lettres
        {
            texte += e.text.text;
            texte_modifie = true;
        }
    }
}

