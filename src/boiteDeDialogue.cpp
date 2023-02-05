#include "boiteDeDialogue.hpp"

BoiteDeDialogue::BoiteDeDialogue(SDL_Color couleur, SDL_Rect position, std::string texte, int taille_police, SDL_Renderer* rendu, std::string name)
:texte_defilement(texte, "./font/lazy.ttf", taille_police, {255, 255, 255, 255}, position, rendu, "texte de " + name, true)
{
    this->position = position;
    this->name = name;
    this->couleur = couleur;
    this->texte_defilement.wrapLength = position.w;
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());
}



void BoiteDeDialogue::Draw(SDL_Renderer* rendu)
{
    CHK(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a), SDL_GetError());
    CHK(SDL_RenderFillRect(rendu, &(this->position)), SDL_GetError());
    this->texte_defilement.Draw(rendu);
}


void BoiteDeDialogue::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    //si le texte est complètement affiché
    if(this->texte_defilement.texteDefilement.length() == this->texte_defilement.texte.length())
    {
        (void)e;
        int x, y; //position x et y de la souris
        SDL_GetMouseState(&x, &y);
        if(collision(this->position, x, y) == true && e.type == SDL_MOUSEBUTTONDOWN)
        {
            if(sing_syst->son_active == true)
            {
                CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
            }
            this->texte_defilement.texteDefilement = "";
        }
    }
}


void BoiteDeDialogue::Update(Uint32& timeStep)
{
    this->texte_defilement.Update(timeStep);
}


bool BoiteDeDialogue::collision(SDL_Rect dest_joueur, int x, int y)
{
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
