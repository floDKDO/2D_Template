#include "selectionnable.hpp"


bool Selectionnable::verrou = true;



bool Selectionnable::collision(SDL_Rect dest, int x, int y)
{
    if(dest.y + dest.h > y
    && dest.y < y
    && dest.x + dest.w > x
    && dest.x < x)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Selectionnable::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    if(e.type == SDL_KEYDOWN)
    {
        this->onKeyPressed(e, sing_syst);
    }
    else if(e.type == SDL_KEYUP)
    {
        this->onKeyReleased(e, sing_syst);
    }
    else if(e.type == SDL_CONTROLLERBUTTONDOWN)
    {
        this->onControllerButtonPressed(e, sing_syst);
    }
    else if(e.type == SDL_CONTROLLERBUTTONUP)
    {
        this->onControllerButtonReleased(e, sing_syst);
    }
    else if(e.type == SDL_MOUSEMOTION)
    {
        if(collision(this->position, x, y) == true)
        {
            this->onPointerEnter(e, sing_syst);
        }
        else
        {
            this->onPointerExit(e, sing_syst);
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            this->onPointerDown(e, sing_syst);
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


void Selectionnable::setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight)
{
    this->selectOnUp = selectOnUp;
    this->selectOnDown = selectOnDown;
    this->selectOnLeft = selectOnLeft;
    this->selectOnRight = selectOnRight;
}

void Selectionnable::setSelected(Selectionnable* ui)
{
    if(ui != nullptr)
        ui->etat = SELECTED;
}

void Selectionnable::setUnselected(Selectionnable* previous)
{
    if(previous != nullptr)
        previous->etat = NORMAL;
}


void Selectionnable::selectNew(Selectionnable* ui, SingletonSysteme* sing_syst)
{
    this->setUnselected(this);
    this->setSelected(ui);
    if(sing_syst->son_active == true && etat != SELECTED)
    {
        CHK(Mix_PlayChannel(1, hover_sound, 0), Mix_GetError());
    }
}


void Selectionnable::onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    if(son_joue == false && sing_syst->son_active == true && previousEtat != SELECTED)
    {
        CHK(Mix_PlayChannel(1, hover_sound, 0), Mix_GetError());
        son_joue = true;
    }
    this->etat = SELECTED;
    this->previousEtat = etat;
}

void Selectionnable::onPointerExit(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    son_joue = false;
    previousEtat = etat;
}

void Selectionnable::onPointerDown(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    if(collision(this->position, x, y) == true)
    {
        this->etat = CLICKED;
        clicAvantCollision = false; //protection
    }
    else
    {
        clicAvantCollision = true; //protection
    }
}

void Selectionnable::onClick(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
}

void Selectionnable::onKeyPressed(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
}

void Selectionnable::onKeyReleased(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    this->verrou = true;
}

void Selectionnable::onControllerButtonPressed(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
}

void Selectionnable::onControllerButtonReleased(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    this->verrou = true;
}
