#include "toggle.hpp"

Toggle::Toggle(SDL_Color couleur_normal, SDL_Color couleur_hover, SDL_Color couleur_click, SDL_Color couleur_selected, SDL_Rect position, std::string texte, int taille_police, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu, std::string name)
:texte(texte, "./font/lazy.ttf", taille_police, {255, 255, 255, 255}, position, rendu, "texte de " + name)
{
    this->isChecked = isCheckedSave;
    this->texte.position = {position.x, position.y - position.h, position.w, position.h};

    this->couleur_normal = couleur_normal;
    this->couleur_hover = couleur_hover;
    this->couleur_click = couleur_click;
    this->couleur_selected = couleur_selected;
    this->etat = NORMAL; //etat de base

    this->position = position;
    this->funcPtr = funcPtr; //pointeur sur la fonction qui sera lancée quand il y aura un clic sur le bouton
    this->clicAvantCollision = false; //protection
    NCHK(this->hover_sound = Mix_LoadWAV("./sound/hover.ogg"), Mix_GetError());
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());
    this->son_joue = false;
    this->hasImage = false;
    this->name = name;
    NCHK(this->check = IMG_LoadTexture(rendu, "./img/checked.png"), IMG_GetError());
}

Toggle::Toggle(std::string image_normal, std::string image_hover, std::string image_click, std::string image_selected, SDL_Rect position, std::string texte, int taille_police, eventFunction funcPtr, bool isCheckedSave, SDL_Renderer* rendu, std::string name)
:texte(texte, "./font/lazy.ttf", taille_police, {255, 255, 255, 255}, position, rendu, "texte de " + name)
{
    this->isChecked = isCheckedSave;
    this->texte.position = {position.x, position.y - position.h, position.w, position.h};

    this->etat = NORMAL; //etat de base

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

    NCHK(this->check = IMG_LoadTexture(rendu, "./img/checked.png"), IMG_GetError());
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


void Toggle::Draw(SDL_Renderer* rendu)
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
    texte.Draw(rendu);

    //mettre le coche ou non
    if(this->isChecked == true)
    {
        CHK(SDL_RenderCopy(rendu, check, nullptr, &(this->position)), SDL_GetError());
    }
}

void Toggle::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
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

void Toggle::onKeyPressed(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->etat == SELECTED && verrou == true)
    {
        if(e.key.keysym.sym == SDLK_UP)
        {
            if(this->selectOnUp != nullptr)
            {
                this->fonc(this->selectOnUp, sing_syst);
            }
        }
        else if(e.key.keysym.sym == SDLK_DOWN)
        {
            if(this->selectOnDown != nullptr)
            {
                this->fonc(this->selectOnDown, sing_syst);
            }
        }
        else if(e.key.keysym.sym == SDLK_LEFT)
        {
            if(this->selectOnLeft != nullptr)
            {
                this->fonc(this->selectOnLeft, sing_syst);
            }
        }
        else if(e.key.keysym.sym == SDLK_RIGHT)
        {
            if(this->selectOnRight != nullptr)
            {
                this->fonc(this->selectOnRight, sing_syst);
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
                isChecked = !isChecked;
            }
        }
        verrou = false;
    }
}


void Toggle::onKeyReleased(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    this->verrou = true;
}


void Toggle::onControllerButtonPressed(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->etat == SELECTED && verrou == true)
    {
        if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
        {
            if(this->selectOnUp != nullptr)
            {
                this->fonc(this->selectOnUp, sing_syst);
            }
        }
        else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
        {
            if(this->selectOnDown != nullptr)
            {
                this->fonc(this->selectOnDown, sing_syst);
            }
        }
        else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
        {
            if(this->selectOnLeft != nullptr)
            {
                this->fonc(this->selectOnLeft, sing_syst);
            }
        }
        else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
        {
            if(this->selectOnRight != nullptr)
            {
                this->fonc(this->selectOnRight, sing_syst);
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
                isChecked = !isChecked;
            }
        }
        verrou = false;
    }
}

void Toggle::onControllerButtonReleased(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    this->verrou = true;
}


void Toggle::onPointerEnter(SDL_Event e, SingletonSysteme* sing_syst)
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


void Toggle::onPointerExit(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    (void)sing_syst;
    son_joue = false;
    previousEtat = etat;
}


void Toggle::onClick(SDL_Event e, SingletonSysteme* sing_syst)
{
    (void)e;
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    if(collision(this->position, x, y) == true && clicAvantCollision == false) //empecher de cliquer avant d'etre sur le bouton
    {
        this->etat = SELECTED;
        isChecked = !isChecked;
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


void Toggle::onPointerDown(SDL_Event e, SingletonSysteme* sing_syst)
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


void Toggle::setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight)
{
    this->selectOnUp = selectOnUp;
    this->selectOnDown = selectOnDown;
    this->selectOnLeft = selectOnLeft;
    this->selectOnRight = selectOnRight;
}


void Toggle::setSelected(Selectionnable* ui)
{
    if(ui != nullptr)
        ui->etat = SELECTED;
}

void Toggle::setUnselected(Selectionnable* previous)
{
    if(previous != nullptr)
        previous->etat = NORMAL;
}

void Toggle::fonc(Selectionnable* ui, SingletonSysteme* sing_syst)
{
    this->setUnselected(this);
    this->setSelected(ui);
    if(sing_syst->son_active == true && etat != SELECTED)
    {
        CHK(Mix_PlayChannel(1, hover_sound, 0), Mix_GetError());
        son_joue = true;
    }
}
