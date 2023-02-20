#include "boiteDeDialogue.hpp"

BoiteDeDialogue::BoiteDeDialogue(SDL_Color couleur, SDL_Rect position, std::vector<std::string> textes, int taille_police, SDL_Renderer* rendu, std::string name)
{
    this->position = position;
    this->petit_carre = {position.x + position.w - 30, position.y + position.h - 30, 20, 20};
    this->dialogue_fini = false;
    this->alpha_carre = 0;
    this->name = name;
    this->couleur = couleur;
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());

    //textes et textes_defilement ont la meme taille
    for(long long unsigned int i = 0; i < textes.size(); i++)
    {
        textes_defilement.push_back(Texte(textes[i], "./font/lazy.ttf", taille_police, BLANC, position, rendu, "texte de " + name, true));
        this->textes_defilement[i].wrapLength = position.w;
    }
    indice_texte_courant = 0; //indice du texte a afficher (d'abord = 0)
}



void BoiteDeDialogue::Draw(SDL_Renderer* rendu)
{
    CHK(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a), SDL_GetError());
    CHK(SDL_RenderFillRect(rendu, &(this->position)), SDL_GetError());

    this->textes_defilement[indice_texte_courant].Draw(rendu);

    if(this->dialogue_fini == true)
    {
        CHK(SDL_SetRenderDrawColor(rendu, 255, 255, 255, alpha_carre), SDL_GetError());
        CHK(SDL_RenderFillRect(rendu, &(this->petit_carre)), SDL_GetError());
    }
}


void BoiteDeDialogue::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    //si le texte est complètement affiché
    if(dialogue_fini == true)
    {
        int x, y; //position x et y de la souris
        SDL_GetMouseState(&x, &y);
        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(this->position, x, y) == true)
            {
                if(sing_syst->son_active == true)
                {
                    CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
                }
                if(textes_defilement.size() - 1 > indice_texte_courant) //on incrémente uniquement s'il y a encore au moins un élément dans le vector
                    indice_texte_courant += 1;

                this->dialogue_fini = false;
            }
        }
    }
}


void BoiteDeDialogue::Update(Uint32& timeStep)
{
    this->textes_defilement[indice_texte_courant].Update(timeStep);
    if(this->textes_defilement[indice_texte_courant].texteDefilement.length() == this->textes_defilement[indice_texte_courant].texte.length())
    {
        this->dialogue_fini = true;

        //toutes les demis secondes, alterner opaque et transparent
        if(SDL_GetTicks() - timeStep > 500)
        {
            alpha_carre = 255;
        }
        if(SDL_GetTicks() - timeStep > 1000)
        {
            alpha_carre = 0;
            timeStep = SDL_GetTicks();
        }
    }
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
