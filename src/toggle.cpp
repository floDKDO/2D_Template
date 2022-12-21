#include "toggle.hpp"

Toggle::Toggle(SDL_Color couleur_checked, SDL_Color couleur_unchecked, SDL_Color couleur_hover, SDL_Rect position, std::string texte, eventFunction funcPtr)
:texte(texte, "./font/lazy.ttf", {255, 255, 255, 255}, {position.x, position.y - position.h, position.w, position.h})
{
    this->isChecked = true;
    this->etat = CHECKED;
    this->couleur_checked = couleur_checked;
    this->couleur_unchecked = couleur_unchecked;
    this->couleur_hover = couleur_hover;
    this->position = position;
    this->funcPtr = funcPtr;
    this->hover_sound = Mix_LoadWAV("./hover.ogg");
    this->click_sound = Mix_LoadWAV("./select.ogg");
}


void Toggle::Draw(SDL_Renderer* rendu)
{
    if(this->etat == CHECKED)
    {
        if(SDL_SetRenderDrawColor(rendu, this->couleur_checked.r, this->couleur_checked.g, this->couleur_checked.b, this->couleur_checked.a) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(this->etat == UNCHECKED)
    {
        if(SDL_SetRenderDrawColor(rendu, this->couleur_unchecked.r, this->couleur_unchecked.g, this->couleur_unchecked.b, this->couleur_unchecked.a) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if(this->etat == HOVERED1)
    {
        if(SDL_SetRenderDrawColor(rendu, this->couleur_hover.r, this->couleur_hover.g, this->couleur_hover.b, this->couleur_hover.a) < 0)
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

    texte.Draw(rendu);
}


void Toggle::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
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
                this->etat = HOVERED1;
                if(son_joue == false && sing_syst->son_active == true)
                {
                    Mix_PlayChannel(1, hover_sound, 0);
                    son_joue = true;
                }
            }
            else
            {
                if(isChecked == true)
                    this->etat = CHECKED;
                else this->etat = UNCHECKED;
                son_joue = false;
            }
        }
        else //si on se trouve sur le bouton avec le clic enfonce
        {
            if(clicAvantCollision == false)
            {
                if(collision(this->position, x, y) == true)
                {
                    this->etat = HOVERED1;
                    if(son_joue == false && sing_syst->son_active == true)
                    {
                        Mix_PlayChannel(1, hover_sound, 0);
                        son_joue = true;
                    }
                }
                else
                {
                    if(isChecked == true)
                        this->etat = CHECKED;
                    else this->etat = UNCHECKED;
                    son_joue = false;
                }
            }
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(this->position, x, y) == true)
            {
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
                isChecked = !isChecked;
                if(isChecked == true)
                    this->etat = CHECKED;
                else this->etat = UNCHECKED;
                if(sing_syst->son_active == true)
                    Mix_PlayChannel(1, click_sound, 0);
                if(funcPtr != nullptr)
                {
                    funcPtr(sing_syst);
                }
            }
        }
    }
}


bool Toggle::collision(SDL_Rect dest_joueur, int x, int y)
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
