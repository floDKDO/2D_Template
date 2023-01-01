#include "joueur.hpp"

Joueur::Joueur(unsigned int pv, SDL_Color couleur, SDL_Rect position, mode_affichage mode)
{
    this->pv = pv;
    this->couleur = couleur;
    this->position = position;
    for(int i = 0; i < 4; i++)
    {
        this->dep[i] = false;
    }
    this->multiplication_vitesse = 1; //de base, vitesse*1
    this->mode = mode;

    // Que pour mode vue de cote
    this->surSol = true;
    this->vitesseDeChute = 0;
    this->hauteur_saut = 12;
    ////////////////////////////
}

void Joueur::ResetAllValues(bool dep[4])
{
    for(int i = 0; i < 4; i++)
    {
        dep[i] = false;
    }
}

void Joueur::SetValue(bool dep[4], int indice)
{
    ResetAllValues(dep);
    dep[indice] = true;
}

void Joueur::Draw(SDL_Renderer* rendu)
{
    if(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if(SDL_RenderFillRect(rendu, &(this->position)) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Joueur::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->mode == VUE_DESSUS)
    {
        if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == sing_syst->touches.dep_haut)
            {
                SetValue(dep, 0);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_bas)
            {
                SetValue(dep, 1);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_gauche)
            {
                SetValue(dep, 2);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_droite)
            {
                SetValue(dep, 3);
            }
            sing_syst->posX_joueur = this->position.x;
            sing_syst->posY_joueur = this->position.y;
        }
        else if(e.type == SDL_KEYUP)
        {
            ResetAllValues(this->dep);
        }
        else if(e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if(e.cbutton.button == sing_syst->touches_1.dep_haut)
            {
                SetValue(dep, 0);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_bas)
            {
                SetValue(dep, 1);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_gauche)
            {
                SetValue(dep, 2);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_droite)
            {
                SetValue(dep, 3);
            }
            sing_syst->posX_joueur = this->position.x;
            sing_syst->posY_joueur = this->position.y;
        }
        else if(e.type == SDL_CONTROLLERBUTTONUP)
        {
            ResetAllValues(this->dep);
        }
    }
    else if(this->mode == VUE_COTE)
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
            else if(e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)
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
    }
}

void Joueur::Update(Uint32& timeStep)
{
    if(this->mode == VUE_DESSUS)
    {
        if(SDL_GetTicks() - timeStep > (80 / this->multiplication_vitesse))
        {
            if(dep[0] == true)
            {
                this->position.y -= 32; //taille d'une tuile
            }
            else if(dep[1] == true)
            {
                this->position.y += 32;
            }
            else if(dep[2] == true)
            {
                this->position.x -= 32;
            }
            else if(dep[3] == true)
            {
                this->position.x += 32;
            }

            timeStep = SDL_GetTicks();
        }
    }
    else if(this->mode == VUE_COTE)
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
    }
}
