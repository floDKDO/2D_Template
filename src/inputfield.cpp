#include "inputfield.hpp"

Inputfield::Inputfield(std::string police, SDL_Color couleur, SDL_Rect position, eventFunction funcPtr, SDL_Renderer* rendu)
:texte("", police, couleur, position, rendu), texte_placeHolder("Votre nom...", police, {127, 127, 127, 255}, position, rendu)
{
    this->fond_de_texte = position;
    this->zone_de_texte = {fond_de_texte.x, fond_de_texte.y, 0, 0};
    this->mode_edition = false;
    this->texte_modifie = false;
    this->funcPtr = funcPtr;

    if(TTF_SetFontSize(this->texte.police, 50) < 0)
    {
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

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
    texte.Draw(rendu);

    zone_de_texte.w = texte.surface->w;
    zone_de_texte.h = texte.surface->h;

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
    if(SDL_RenderCopy(rendu, texte.texture, nullptr, &zone_de_texte) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(texte.texte.length() == 0)
        texte_placeHolder.Draw(rendu);
}

void Inputfield::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
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
            if(e.key.keysym.sym == SDLK_UP)
            {
                if(this->selectOnUp != nullptr)
                {
                    this->setUnselected(this);
                    this->setSelected(this->selectOnUp);
                    /*if(sing_syst->son_active == true)
                    {
                        if(Mix_PlayChannel(1, this->bouton.hover_sound, 0) < 0)
                        {
                            std::cerr << Mix_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }*/
                }
            }
            else if(e.key.keysym.sym == SDLK_DOWN)
            {
                if(this->selectOnDown != nullptr)
                {
                    this->setUnselected(this);
                    this->setSelected(this->selectOnDown);
                }
            }
            else if(e.key.keysym.sym == SDLK_LEFT)
            {
                if(this->selectOnLeft != nullptr)
                {
                    this->setUnselected(this);
                    this->setSelected(this->selectOnLeft);
                }
            }
            else if(e.key.keysym.sym == SDLK_RIGHT)
            {
                if(this->selectOnRight != nullptr)
                {
                    this->setUnselected(this);
                    this->setSelected(this->selectOnRight);
                }
            }

            if(e.key.keysym.sym == SDLK_RETURN)
            {
                if(this->etat == SELECTED)
                {
                    /*if(sing_syst->son_active == true)
                    {
                        if(Mix_PlayChannel(1, click_sound, 0) < 0)
                        {
                            std::cerr << Mix_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }*/
                    mode_edition = true;
                }
            }
        }
    }
    else if(e.type == SDL_MOUSEBUTTONDOWN)
    {
        if(e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(fond_de_texte, x, y) == true && mode_edition == false)
            {   //si on clique sur l'inputfield et qu'on est pas en mode édition
                mode_edition = true;
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
