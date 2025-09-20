#include "../include/bouton.hpp"

Bouton::Bouton(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, eventFunction funcPtr, std::string texte, int taille_police, SDL_Renderer* rendu, std::string name)
:texte(texte, "./font/lazy.ttf", taille_police, {255, 255, 255, 255}, position, rendu, "texte de " + name, false)
{
    this->couleur_normal = couleur_normal;
    this->couleur_hover = couleur_hover;
    this->couleur_click = couleur_click;
    this->couleur_selected = couleur_selected;
    this->etat = NORMAL; //etat de base
    this->previousEtat = etat;
    this->position = position;
    this->funcPtr = funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton
    this->clicAvantCollision = false; //protection
    NCHK(this->hover_sound = Mix_LoadWAV("./sound/hover.ogg"), Mix_GetError());
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());
    this->son_joue = false;
    this->hasImage = false;
    this->name = name;
}

Bouton::Bouton(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, eventFunction funcPtr, std::string texte, int taille_police, SDL_Renderer* rendu, std::string name)
:texte(texte, "./font/lazy.ttf", taille_police, {255, 255, 255, 255}, position, rendu, "texte de " + name, false)
{
    this->etat = NORMAL; //etat de base
    this->previousEtat = etat;
    NCHK(this->image_normal = IMG_LoadTexture(rendu, image_normal.c_str()), IMG_GetError());
    NCHK(this->image_hover = IMG_LoadTexture(rendu, image_hover.c_str()), IMG_GetError());
    NCHK(this->image_click = IMG_LoadTexture(rendu, image_click.c_str()), IMG_GetError());
    NCHK(this->image_selected = IMG_LoadTexture(rendu, image_selected.c_str()), IMG_GetError());
    int w, h;
    CHK(SDL_QueryTexture(this->image_normal, nullptr, nullptr, &w, &h), SDL_GetError());
    this->position = {position.x, position.y, w, h};
    this->funcPtr = funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton
    this->clicAvantCollision = false; //protection
    NCHK(this->hover_sound = Mix_LoadWAV("./sound/hover.ogg"), Mix_GetError());
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());
    this->son_joue = false;
    this->hasImage = true;
    this->name = name;
}


bool Bouton::collision(SDL_Rect dest, int x, int y)
{
    return Selectionnable::collision(dest, x, y);
}

void Bouton::draw(SDL_Renderer* rendu)
{
    if(this->hasImage == false)
    {
        if(this->etat == NORMAL)
        {
            CHK(SDL_SetRenderDrawColor(rendu, this->couleur_normal.r, this->couleur_normal.g, this->couleur_normal.b, this->couleur_normal.a), SDL_GetError());
        }
        else if(this->etat == HOVERED)
        {
            CHK(SDL_SetRenderDrawColor(rendu, this->couleur_hover.r, this->couleur_hover.g, this->couleur_hover.b, this->couleur_hover.a), SDL_GetError());
        }
        else if(this->etat == CLICKED)
        {
            CHK(SDL_SetRenderDrawColor(rendu, this->couleur_click.r, this->couleur_click.g, this->couleur_click.b, this->couleur_click.a), SDL_GetError());
        }
        else if(this->etat == SELECTED)
        {
            CHK(SDL_SetRenderDrawColor(rendu, this->couleur_selected.r, this->couleur_selected.g, this->couleur_selected.b, this->couleur_selected.a), SDL_GetError());
        }
        CHK(SDL_RenderFillRect(rendu, &(this->position)), SDL_GetError());
    }
    else
    {
        if(this->etat == NORMAL)
        {
            CHK(SDL_RenderCopy(rendu, image_normal, nullptr, &(this->position)), SDL_GetError());
        }
        else if(this->etat == HOVERED)
        {
            CHK(SDL_RenderCopy(rendu, image_hover, nullptr, &(this->position)), SDL_GetError());
        }
        else if(this->etat == CLICKED)
        {
            CHK(SDL_RenderCopy(rendu, image_click, nullptr, &(this->position)), SDL_GetError());
        }
        else if(this->etat == SELECTED)
        {
            CHK(SDL_RenderCopy(rendu, image_selected, nullptr, &(this->position)), SDL_GetError());
        }
    }
    texte.draw(rendu);
}

void Bouton::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    Selectionnable::handleEvents(e, sing_syst);
}


void Bouton::onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst)
{
    Selectionnable::onPointerEnter(e, sing_syst);
}


void Bouton::onPointerExit(SDL_Event e, SingletonSysteme* sing_syst)
{
    Selectionnable::onPointerExit(e, sing_syst);
}

void Bouton::onClick(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    if(collision(this->position, x, y) == true && clicAvantCollision == false) //empecher de cliquer avant d'etre sur le bouton
    {
        this->etat = SELECTED;
        if(sing_syst->son_active == true)
        {
            CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
        }
        if(funcPtr != nullptr)
        {
            funcPtr(sing_syst, this);
        }
    }
}


void Bouton::onPointerDown(SDL_Event e, SingletonSysteme* sing_syst)
{
    Selectionnable::onPointerDown(e, sing_syst);
}


void Bouton::onKeyPressed(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->etat == SELECTED && verrou == true)
    {
        if(e.key.keysym.sym == SDLK_UP)
        {
            if(this->selectOnUp != nullptr)
            {
                this->selectNew(this->selectOnUp, sing_syst);
            }
        }
        else if(e.key.keysym.sym == SDLK_DOWN)
        {
            if(this->selectOnDown != nullptr)
            {
                this->selectNew(this->selectOnDown, sing_syst);
            }
        }
        else if(e.key.keysym.sym == SDLK_LEFT)
        {
            if(this->selectOnLeft != nullptr)
            {
                this->selectNew(this->selectOnLeft, sing_syst);
            }
        }
        else if(e.key.keysym.sym == SDLK_RIGHT)
        {
            if(this->selectOnRight != nullptr)
            {
                this->selectNew(this->selectOnRight, sing_syst);
            }
        }

        if(e.key.keysym.sym == SDLK_RETURN)
        {
            if(this->etat == SELECTED)
            {
                if(sing_syst->son_active == true)
                {
                    CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
                }
                this->funcPtr(sing_syst, this);
            }
        }
        verrou = false;
    }
}


void Bouton::onKeyReleased(SDL_Event e, SingletonSysteme* sing_syst)
{
    Selectionnable::onKeyReleased(e, sing_syst);
}


void Bouton::onControllerButtonPressed(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->etat == SELECTED && verrou == true)
    {
        if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
        {
            if(this->selectOnUp != nullptr)
            {
                this->selectNew(this->selectOnUp, sing_syst);
            }
        }
        else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        {
            if(this->selectOnDown != nullptr)
            {
                this->selectNew(this->selectOnDown, sing_syst);
            }
        }
        else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        {
            if(this->selectOnLeft != nullptr)
            {
                this->selectNew(this->selectOnLeft, sing_syst);
            }
        }
        else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
            if(this->selectOnRight != nullptr)
            {
                this->selectNew(this->selectOnRight, sing_syst);
            }
        }

        if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A) //a ameliorer selon le type de manette
        {
            if(this->etat == SELECTED)
            {
                if(sing_syst->son_active == true)
                {
                    CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
                }
                this->funcPtr(sing_syst, this);
            }
        }
        verrou = false;
    }
}

void Bouton::onControllerButtonReleased(SDL_Event e, SingletonSysteme* sing_syst)
{
    Selectionnable::onControllerButtonReleased(e, sing_syst);
}


void Bouton::setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight)
{
    Selectionnable::setSelectedIfMove(selectOnUp, selectOnDown, selectOnLeft, selectOnRight);
}


void Bouton::setSelected(Selectionnable* ui)
{
    Selectionnable::setSelected(ui);
}


void Bouton::setUnselected(Selectionnable* previous)
{
    Selectionnable::setUnselected(previous);
}

void Bouton::selectNew(Selectionnable* ui, SingletonSysteme* sing_syst)
{
    Selectionnable::selectNew(ui, sing_syst);
}
