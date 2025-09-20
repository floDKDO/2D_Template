#include "../include/joueur.hpp"

Joueur::Joueur(SDL_Color couleur, SDL_Rect position, mode_affichage mode)
{
    this->couleur = couleur;
    this->position = position;
    for(int i = 0; i < 4; i++)
    {
        this->dep[i] = false;
    }
    this->multiplication_vitesse = 1; //de base, vitesse*1
    this->mode = mode;
    this->interagit = false;
    this->orientation = HAUT;

    this->isAnimated = false;
    this->je_peux_marcher = true;

    // Que pour mode vue de cote
    /*this->surSol = true;
    this->vitesseDeChute = 0;
    this->hauteur_saut = 12;*/
    ////////////////////////////
}

Joueur::Joueur(std::string chemin, SDL_Rect position, mode_affichage mode, SDL_Renderer* rendu)
{
    this->chemin = chemin;
    this->position = position;
    for(int i = 0; i < 4; i++)
    {
        this->dep[i] = false;
    }
    this->multiplication_vitesse = 1; //de base, vitesse*1
    this->mode = mode;
    this->interagit = false;
    this->orientation = HAUT;

    this->isAnimated = true;
    this->je_peux_marcher = true;

    NCHK(this->texture = IMG_LoadTexture(rendu, this->chemin.c_str()), IMG_GetError());

    // Que pour mode vue de cote
    /*this->surSol = true;
    this->vitesseDeChute = 0;
    this->hauteur_saut = 12;*/
    ////////////////////////////
}

void Joueur::resetAllValues()
{
    for(int i = 0; i < 4; i++)
    {
        dep[i] = false;
    }
}

void Joueur::setValue(int indice)
{
    resetAllValues();
    dep[indice] = true;
    if(indice == 0)
        orientation = HAUT;
    else if(indice == 1)
        orientation = BAS;
    else if(indice == 2)
        orientation = GAUCHE;
    else if(indice == 3)
        orientation = DROITE;
}

void Joueur::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    SDL_Rect temp = {position.x - camera.x, position.y - camera.y, position.w, position.h};

    if(this->isAnimated == false)
    {
        CHK(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a), SDL_GetError());
        CHK(SDL_RenderFillRect(rendu, &(temp)), SDL_GetError());
    }
    else
    {
        if(orientation == DROITE)
            CHK(SDL_RenderCopyEx(rendu, this->texture, &(this->srcRect), &temp, 0, nullptr, SDL_FLIP_HORIZONTAL), SDL_GetError());
        else CHK(SDL_RenderCopy(rendu, this->texture, &(this->srcRect), &temp), SDL_GetError());
    }
}

void Joueur::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->mode == VUE_DESSUS)
    {
        if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == sing_syst->touches.dep_haut)
            {
                setValue(0);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_bas)
            {
                setValue(1);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_gauche)
            {
                setValue(2);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_droite)
            {
                setValue(3);
            }
            else if(e.key.keysym.sym == SDLK_SPACE)
            {
                interagit = true;
            }
        }
        else if(e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if(e.cbutton.button == sing_syst->touches_1.dep_haut)
            {
                setValue(0);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_bas)
            {
                setValue(1);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_gauche)
            {
                setValue(2);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_droite)
            {
                setValue(3);
            }
        }
        else if(e.type == SDL_KEYUP || e.type == SDL_CONTROLLERBUTTONUP)
        {
            resetAllValues();
            this->interagit = false;
        }
    }
    /*else if(this->mode == VUE_COTE)
    {
        if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == sing_syst->touches.dep_gauche)
            {
                SetValue(dep, 2);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_droite)
            {
                SetValue(dep, 3);
            }
            else if(e.key.keysym.sym == SDLK_SPACE)
            {
                if(surSol == true)
                {
                    surSol = false;
                    vitesseDeChute = -(this->hauteur_saut);
                }
            }
            sing_syst->posX_joueur = this->position.x;
            sing_syst->posY_joueur = this->position.y;
        }
        else if(e.type == SDL_KEYUP)
        {
            if(e.key.keysym.sym == SDLK_SPACE)
            {
                //permet de jauger le saut quand on relache espace si on a pas atteint la vitesse maximale d'hauteur
                if(vitesseDeChute < -(this->hauteur_saut / 2))
                    vitesseDeChute = -(this->hauteur_saut / 2);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_gauche || e.key.keysym.sym == sing_syst->touches.dep_droite)
            {
                ResetAllValues(this->dep);
            }
        }
        else if(e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if(e.cbutton.button == sing_syst->touches_1.dep_gauche)
            {
                SetValue(dep, 2);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_droite)
            {
                SetValue(dep, 3);
            }
            else if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                if(surSol == true)
                {
                    surSol = false;
                    vitesseDeChute = -(this->hauteur_saut);
                }
            }
            sing_syst->posX_joueur = this->position.x;
            sing_syst->posY_joueur = this->position.y;
        }
        else if(e.type == SDL_CONTROLLERBUTTONUP)
        {
            if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                //permet de jauger le saut quand on relache espace si on a pas atteint la vitesse maximale d'hauteur
                if(vitesseDeChute < -(this->hauteur_saut / 2))
                    vitesseDeChute = -(this->hauteur_saut / 2);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_gauche || e.cbutton.button == sing_syst->touches_1.dep_droite)
            {
                ResetAllValues(this->dep);
            }
        }
    }*/
}


void Joueur::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    if(isAnimated == true)
    {
        if(dep[0] == true || dep[1] == true || dep[2] == true || dep[3] == true)
        {
            Uint32 seconds = SDL_GetTicks() / 200; //200 => rapidité de l'animation
            int sprite = seconds % 3;
            switch(orientation)
            {
                case 0:
                    this->srcRect = {15 * 1, 22 * sprite, 14, 21};
                    break;

                case 1:
                    this->srcRect = {15 * 0, 22 * sprite, 14, 21};
                    break;

                case 2:
                    this->srcRect = {15 * 2, 22 * sprite, 14, 21};
                    break;

                case 3:
                    this->srcRect = {15 * 2, 22 * sprite, 14, 21};
                    break;

                default:
                    break;
            }
        }
        else
        {
            switch(orientation)
            {
                case 0:
                    this->srcRect = {15 * 1, 0, 14, 21};
                    break;

                case 1:
                    this->srcRect = {15 * 0, 0, 14, 21};
                    break;

                case 2:
                    this->srcRect = {15 * 2, 0, 14, 21};
                    break;

                case 3:
                    this->srcRect = {15 * 2, 0, 14, 21};
                    break;

                default:
                    break;
            }
        }
    }

    //pour la sauvegarde de la position du joueur
    sing_syst->posX_joueur = this->position.x;
    sing_syst->posY_joueur = this->position.y;

    if(this->mode == VUE_DESSUS)
    {
        if(SDL_GetTicks() - timeStep > (80 / this->multiplication_vitesse))
        {
            if(je_peux_marcher == true)
            {
                if(this->dep[0] == true)
                {
                    this->position.y -= TAILLE_TUILE * FACTEUR_MULTIPLICATION; //taille d'une tuile
                    timeStep = SDL_GetTicks();
                }
                else if(this->dep[1] == true)
                {
                    this->position.y += TAILLE_TUILE * FACTEUR_MULTIPLICATION;
                    timeStep = SDL_GetTicks();
                }
                else if(this->dep[2] == true)
                {
                    this->position.x -= TAILLE_TUILE * FACTEUR_MULTIPLICATION;
                    timeStep = SDL_GetTicks();
                }
                else if(this->dep[3] == true)
                {
                    this->position.x += TAILLE_TUILE * FACTEUR_MULTIPLICATION;
                    timeStep = SDL_GetTicks();
                }

            }
        }
    }
    /*else if(this->mode == VUE_COTE)
    {
        if(SDL_GetTicks() - timeStep > (80 / this->multiplication_vitesse))
        {
            if(dep[2] == true)
            {
                this->position.x -= 32;
            }
            else if(dep[3] == true)
            {
                this->position.x += 32;
            }
            timeStep = SDL_GetTicks();
        }
        position.y += vitesseDeChute;
        vitesseDeChute += 0.5f; //gravite

        if(position.y > 704 - 32) //si on touche le sol
        {
            position.y = 672;
            surSol = true;
            vitesseDeChute = 0;
        }
    }*/
}
