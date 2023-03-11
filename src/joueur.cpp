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
    /*this->surSol = true;
    this->vitesseDeChute = 0;
    this->hauteur_saut = 12;*/
    ////////////////////////////
}

bool Joueur::collision(SDL_Rect dest_joueur, Tuile tuile)
{
    //if(tuile.isWarp == true)
        //std::cout << tuile.position.x << std::endl;
    //si pas de collision
    if(dest_joueur.y + dest_joueur.h > tuile.position.y
    && dest_joueur.y < tuile.position.y +  + tuile.position.h
    && dest_joueur.x + dest_joueur.w > tuile.position.x
    && dest_joueur.x < tuile.position.x + tuile.position.w)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Joueur::resetAllValues(bool dep[4])
{
    for(int i = 0; i < 4; i++)
    {
        dep[i] = false;
    }
}

void Joueur::setValue(bool dep[4], int indice)
{
    resetAllValues(dep);
    dep[indice] = true;
}

void Joueur::draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst)
{
    CHK(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a), SDL_GetError());
    SDL_Rect temp = {position.x - sing_syst->camera.x, position.y - sing_syst->camera.y, position.w, position.h};
    CHK(SDL_RenderFillRect(rendu, &(temp)), SDL_GetError());
}

void Joueur::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    if(this->mode == VUE_DESSUS)
    {
        if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == sing_syst->touches.dep_haut)
            {
                setValue(dep, 0);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_bas)
            {
                setValue(dep, 1);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_gauche)
            {
                setValue(dep, 2);
            }
            else if(e.key.keysym.sym == sing_syst->touches.dep_droite)
            {
                setValue(dep, 3);
            }
        }
        else if(e.type == SDL_CONTROLLERBUTTONDOWN)
        {
            if(e.cbutton.button == sing_syst->touches_1.dep_haut)
            {
                setValue(dep, 0);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_bas)
            {
                setValue(dep, 1);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_gauche)
            {
                setValue(dep, 2);
            }
            else if(e.cbutton.button == sing_syst->touches_1.dep_droite)
            {
                setValue(dep, 3);
            }
        }
        else if(e.type == SDL_KEYUP || e.type == SDL_CONTROLLERBUTTONUP)
        {
            resetAllValues(this->dep);
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

void Joueur::update(Uint32& timeStep, SingletonSysteme* sing_syst, std::vector<Tuile> tuiles)
{
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        //on regarde les collisions uniquement avec les tuiles non passables
        if(tuiles[i].estPassable == false)
        {
            SDL_Rect copie = this->position;
            if(dep[0] == true)
            {
                copie.y -= 16 * 4; //taille d'une tuile
                if(collision(copie, tuiles[i]) == true)
                    dep[0] = false;
            }
            else if(dep[1] == true)
            {
                copie.y += 16 * 4;
                if(collision(copie, tuiles[i]) == true)
                    dep[1] = false;
            }
            else if(dep[2] == true)
            {
                copie.x -= 16 * 4;
                if(collision(copie, tuiles[i]) == true)
                    dep[2] = false;
            }
            else if(dep[3] == true)
            {
                copie.x += 16 * 4;
                if(collision(copie, tuiles[i]) == true)
                    dep[3] = false;
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
            if(dep[0] == true)
            {
                this->position.y -= 16 * 4; //taille d'une tuile
            }
            else if(dep[1] == true)
            {
                this->position.y += 16 * 4;
            }
            else if(dep[2] == true)
            {
                this->position.x -= 16 * 4;
            }
            else if(dep[3] == true)
            {
                this->position.x += 16 * 4;
            }

            timeStep = SDL_GetTicks();
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
