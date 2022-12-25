#include "toggle.hpp"

Toggle::Toggle(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, std::string texte, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu)
:bouton(couleur_normal, couleur_hover, couleur_click, couleur_selected, position, funcPtr, texte, rendu)
{
    this->isChecked = isCheckedSave;
    this->bouton.texte.position = {position.x, position.y - position.h, position.w, position.h};

    if((this->check = IMG_LoadTexture(rendu, "./img/checked.png")) == nullptr)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

Toggle::Toggle(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, std::string texte, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu)
:bouton(image_normal, image_hover, image_click, image_selected, position, funcPtr, texte, rendu)
{
    this->isChecked = isCheckedSave;
    this->bouton.texte.position = {position.x, position.y - position.h, position.w, position.h};

    if((this->check = IMG_LoadTexture(rendu, "./img/checked.png")) == nullptr)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}


bool Toggle::collision(SDL_Rect dest_joueur, int x, int y)
{
    return this->bouton.collision(dest_joueur, x, y);
}


void Toggle::Draw(SDL_Renderer* rendu)
{
    this->bouton.Draw(rendu);

    //mettre le coche ou non
    if(this->isChecked == true)
    {
        if(SDL_RenderCopy(rendu, check, nullptr, &(this->bouton.position)) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
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
        if(collision(this->bouton.position, x, y) == true)
        {
            this->bouton.onPointerEnter(e, sing_syst);
        }
        else
        {
            this->bouton.onPointerExit(e, sing_syst);
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            this->bouton.onPointerDown(e, sing_syst);
        }
    }
    else if(e.type == SDL_MOUSEBUTTONUP)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            this->onClick(e, sing_syst);
        }
    }
}


void Toggle::onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst)
{
    this->bouton.onPointerEnter(e, sing_syst);
}


void Toggle::onPointerExit(SDL_Event e, SingletonSysteme* sing_syst)
{
    this->bouton.onPointerExit(e, sing_syst);
}


void Toggle::onClick(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    this->bouton.onClick(e, sing_syst);
    if(collision(this->bouton.position, x, y) == true && this->bouton.clicAvantCollision == false) //empecher de cliquer avant d'etre sur le bouton
    {
        isChecked = !isChecked;
    }
}


void Toggle::onPointerDown(SDL_Event e, SingletonSysteme* sing_syst)
{
    this->bouton.onPointerDown(e, sing_syst);
}
