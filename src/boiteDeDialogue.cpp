#include "../include/boiteDeDialogue.hpp"


BoiteDeDialogue::BoiteDeDialogue(SDL_Color couleur, SDL_Rect position, std::string textes, int taille_police, SDL_Renderer* rendu, std::string name)
{
    this->position = position;
    this->petit_carre = {position.x + position.w - 30, position.y + position.h - 30, 20, 20};
    this->un_dialogue_fini = false;
    this->tous_dialogues_finis = false;
    this->alpha_carre = 0;
    this->name = name;
    this->couleur = couleur;
    NCHK(this->click_sound = Mix_LoadWAV("./sound/select.ogg"), Mix_GetError());

    //obtenir chaque ligne de dialogue
    std::vector<std::string> temp = split(textes, "\n");

    //textes et textes_defilement ont la meme taille
    for(long long unsigned int i = 0; i < temp.size(); i++)
    {
        textes_defilement.push_back(Texte(temp[i], "./font/lazy.ttf", taille_police, BLANC, position, rendu, "texte de " + name, true));
        this->textes_defilement[i].wrapLength = position.w;
    }
    indice_texte_courant = 0; //indice du texte a afficher (d'abord = 0)
}

//cette fonction va prendre chaque ligne de "textes" séparée par un \n et mettre chacune dans le std::vector en retour de fonction
std::vector<std::string> BoiteDeDialogue::split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}


void BoiteDeDialogue::draw(SDL_Renderer* rendu)
{
    CHK(SDL_SetRenderDrawColor(rendu, this->couleur.r, this->couleur.g, this->couleur.b, this->couleur.a), SDL_GetError());
    CHK(SDL_RenderFillRect(rendu, &(this->position)), SDL_GetError());

    if(textes_defilement.size() - 1 >= indice_texte_courant)
        this->textes_defilement[indice_texte_courant].draw(rendu);

    if(this->un_dialogue_fini == true)
    {
        CHK(SDL_SetRenderDrawColor(rendu, 255, 255, 255, alpha_carre), SDL_GetError());
        CHK(SDL_RenderFillRect(rendu, &(this->petit_carre)), SDL_GetError());
    }
}

void BoiteDeDialogue::interactionBoite(SingletonSysteme* sing_syst)
{
    if(sing_syst->son_active == true)
    {
        CHK(Mix_PlayChannel(1, click_sound, 0), Mix_GetError());
    }
    if(textes_defilement.size() - 1 > indice_texte_courant) //on incrémente uniquement s'il y a encore au moins un élément dans le vector
        indice_texte_courant += 1;
    else indice_texte_courant = textes_defilement.size(); //dernier appui qui ferme la boite

    this->un_dialogue_fini = false;
}


void BoiteDeDialogue::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    //si le texte est complètement affiché
    if(un_dialogue_fini == true)
    {
        int x, y; //position x et y de la souris
        SDL_GetMouseState(&x, &y);
        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
        {
            if(collision(this->position, x, y) == true)
                interactionBoite(sing_syst);
        }
        else if(e.type == SDL_KEYDOWN)
        {
            if(e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE)
                interactionBoite(sing_syst);
        }
    }
}


void BoiteDeDialogue::update(Uint32& timeStep)
{
    if(textes_defilement.size() - 1 >= indice_texte_courant)
    {
        this->textes_defilement[indice_texte_courant].update(timeStep);
        if(this->textes_defilement[indice_texte_courant].texteDefilement.length() == this->textes_defilement[indice_texte_courant].texte.length())
        {
            this->un_dialogue_fini = true;

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
    else
    {
        //tous les dialogues ont été affichés donc on peut cacher la boite de dialogue
        this->tous_dialogues_finis = true;
    }
}


bool BoiteDeDialogue::collision(SDL_Rect dest, int x, int y)
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
