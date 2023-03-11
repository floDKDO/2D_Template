#include "inputfield.hpp"

Inputfield::Inputfield(std::string police, int taille_police, SDL_Color couleur, SDL_Rect position, eventFunction funcPtr, SDL_Renderer* rendu, std::string name)
:texte("", police, taille_police, couleur, position, rendu, "texte de " + name, false), texte_placeHolder("Votre nom...", police, 30, {127, 127, 127, 255}, position, rendu, "texte placeholder de " + name, false)
,curseur("|", police, taille_police, couleur, position, rendu, "curseur de " + name, false)
{
    this->fond_de_texte = position;
    this->zone_de_texte = {fond_de_texte.x, fond_de_texte.y, 0, 0};

    this->mode_edition = false;
    this->texte_modifie = false;
    this->funcPtr = funcPtr;

    NCHK(this->hover_sound = Mix_LoadWAV("./sound/hover.ogg"), Mix_GetError());
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());

    CHK(TTF_SizeText(this->texte.police, " ", &place, nullptr), TTF_GetError());

    place /= 2;

    this->name = name;

    SDL_SetTextInputRect(&(this->zone_de_texte));
}

bool Inputfield::collision(SDL_Rect dest, int x, int y)
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

void Inputfield::draw(SDL_Renderer* rendu)
{
    texte.draw(rendu);

    zone_de_texte.w = texte.surface->w;
    zone_de_texte.h = texte.surface->h;

    CHK(SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255), SDL_GetError());
    CHK(SDL_RenderFillRect(rendu, &fond_de_texte), SDL_GetError());
    CHK(SDL_RenderCopy(rendu, texte.texture, nullptr, &zone_de_texte), SDL_GetError());

    if(texte.texte.length() == 0)
    {
        if(this->mode_edition == false)
            texte_placeHolder.draw(rendu);

        curseur.position.x = fond_de_texte.x - place;
    }
    else curseur.position.x = fond_de_texte.x + zone_de_texte.w - place;

    if(this->mode_edition == true && affiche_curseur == true)
        curseur.draw(rendu);
}


void Inputfield::update(Uint32& timeStep)
{
    //toutes les demis secondes, alterner affichage et non affichage
    if(SDL_GetTicks() - timeStep > 500)
    {
        affiche_curseur = true; //reste afficher pendant ecriture
    }
    if(SDL_GetTicks() - timeStep > 1000)
    {
        affiche_curseur = false;
        timeStep = SDL_GetTicks();
    }
}


void Inputfield::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);
    if(e.type == SDL_KEYDOWN)
    {
        if(mode_edition == true)
        {
            if(e.key.keysym.sym == SDLK_BACKSPACE && texte.texte.length() > 0)
            {   //si on appuie sur la touche <- (supprimer), que la chaine n'est pas vide et qu'on est en mode édition
                texte.texte.pop_back();
                texte_modifie = true;
            }
            else if(e.key.keysym.sym == SDLK_RETURN)
            {   //si on appuie sur entrée lors de la modification de la chaine, quitte l'inputfield
                mode_edition = false;
                if(funcPtr != nullptr)
                    funcPtr(sing_syst, this); //si on appui sur entree, la fonction se lance
            }
        }
        else
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
                        mode_edition = true;
                        if(sing_syst->son_active == true)
                        {
                            CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
                        }
                    }
                }
                verrou = false;
            }
        }
    }
    else if(e.type == SDL_KEYUP)
    {
        verrou = true;
    }
    else if(e.type == SDL_CONTROLLERBUTTONDOWN)
    {
        if(mode_edition == true)
        {
            if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {   //si on appuie sur entrée lors de la modification de la chaine, quitte l'inputfield
                mode_edition = false;
                if(funcPtr != nullptr)
                    funcPtr(sing_syst, this); //si on appui sur entree, la fonction se lance
            }
        }
        else
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

                if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    if(this->etat == SELECTED)
                    {
                        mode_edition = true;
                        if(sing_syst->son_active == true)
                        {
                            CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
                        }
                    }
                }
                verrou = false;
            }
        }
    }
    else if(e.type == SDL_CONTROLLERBUTTONUP)
    {
        verrou = true;
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(fond_de_texte, x, y) == true && mode_edition == false)
            {   //si on clique sur l'inputfield et qu'on est pas en mode édition
                this->etat = SELECTED;
                mode_edition = true;
                if(sing_syst->son_active == true)
                {
                    CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
                }
            }
            else if(collision(fond_de_texte, x, y) == false && mode_edition == true)
            {   //si on clique autre part que sur l'inputfield et qu'on est en mode édition
                mode_edition = false;
                if(funcPtr != nullptr)
                    funcPtr(sing_syst, this); //si on clic en dehors de l'inputfield alors qu'on le modifie, la fonction se lance
            }
        }
    }
    else if(e.type == SDL_TEXTINPUT)
    {
        if(mode_edition == true && texte.texte.length() < 7) //max 7 lettres
        {
            texte.texte += e.text.text;
            texte_modifie = true;
        }
    }
}


void Inputfield::setSelectedIfMove(Selectionnable* selectOnUp, Selectionnable* selectOnDown, Selectionnable* selectOnLeft, Selectionnable* selectOnRight)
{
    this->selectOnUp = selectOnUp;
    this->selectOnDown = selectOnDown;
    this->selectOnLeft = selectOnLeft;
    this->selectOnRight = selectOnRight;
}

void Inputfield::setSelected(Selectionnable* ui)
{
    if(ui != nullptr)
        ui->etat = SELECTED;
}

void Inputfield::setUnselected(Selectionnable* previous)
{
    if(previous != nullptr)
        previous->etat = NORMAL;
}


void Inputfield::selectNew(Selectionnable* ui, SingletonSysteme* sing_syst)
{
    this->setUnselected(this);
    this->setSelected(ui);
    if(sing_syst->son_active == true && etat != SELECTED)
    {
        CHK(Mix_PlayChannel(1, hover_sound, 0), Mix_GetError());
    }
}
