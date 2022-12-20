#include "bouton.hpp"

Bouton::Bouton(SDL_Color couleur_idle, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Rect position, eventFunction funcPtr)
{
    this->couleur_idle = couleur_idle;
    this->couleur_hover = couleur_hover;
    this->couleur_click = couleur_click;
    this->position = position;
    this->funcPtr = funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton
    this->etat = IDLE; //etat de base
    this->clicAvantCollision = false; //protection
}


bool Bouton::collision(SDL_Rect dest_joueur, int x, int y)
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

void Bouton::Draw(SDL_Renderer* rendu)
{
    if(this->etat == IDLE)
    {
        if(SDL_SetRenderDrawColor(rendu, this->couleur_idle.r, this->couleur_idle.g, this->couleur_idle.b, this->couleur_idle.a) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(this->etat == HOVERED)
    {
        if(SDL_SetRenderDrawColor(rendu, this->couleur_hover.r, this->couleur_hover.g, this->couleur_hover.b, this->couleur_hover.a) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(this->etat == CLICKED)
    {
        if(SDL_SetRenderDrawColor(rendu, this->couleur_click.r, this->couleur_click.g, this->couleur_click.b, this->couleur_click.a) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if(SDL_RenderFillRect(rendu, &(this->position)) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Bouton::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    if(e.type == SDL_KEYDOWN)
    {

    }
    else if(e.type == SDL_MOUSEMOTION)
    {
        if(e.button.button != SDL_BUTTON_LEFT) //si on se trouve sur le bouton sans le clic enfonce
        {
            if(collision(this->position, x, y) == true)
            {
                this->etat = HOVERED;
            }
            else this->etat = IDLE;
        }
        else //si on se trouve sur le bouton avec le clic enfonce
        {
            if(clicAvantCollision == false)
            {
                if(collision(this->position, x, y) == true)
                {
                    this->etat = CLICKED;
                }
                else this->etat = IDLE;
            }
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(this->position, x, y) == true)
            {
                this->etat = CLICKED;
                clicAvantCollision = false; //protection
            }
            else clicAvantCollision = true; //protection
        }
    }
    else if(e.type == SDL_MOUSEBUTTONUP)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(this->position, x, y) == true && clicAvantCollision == false) //empecher de cliquer avant d'etre sur le bouton
            {
                if(funcPtr != nullptr)
                {
                    funcPtr(sing_syst);
                    this->etat = IDLE;
                }
            }
        }
    }
}
