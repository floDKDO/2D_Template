#include "enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur(100, BLEU, {0, 0, 64, 64}, VUE_DESSUS)
{
    carte_actuelle = sing_syst->cartes["toto.map"];
    (void)sing_syst;
    this->changement_de_carte = false;
}


void EnJeu::draw(SingletonSysteme* sing_syst)
{
    sing_syst->camera.x = this->joueur.position.x + this->joueur.position.w/2 - LONGUEUR_FENETRE/2;
    sing_syst->camera.y = this->joueur.position.y + this->joueur.position.h/2 - HAUTEUR_FENETRE/2;
    this->carte_actuelle->draw(sing_syst->rendu, sing_syst);
    this->joueur.draw(sing_syst->rendu, sing_syst);
}


void EnJeu::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    this->carte_actuelle->update(timeStep, sing_syst);
    this->joueur.update(timeStep, sing_syst, carte_actuelle->tuiles);

    for(long long unsigned int i = 0; i < carte_actuelle->tuiles.size(); i++)
    {
        if(joueur.collision(joueur.position, this->carte_actuelle->tuiles[i]) && this->carte_actuelle->tuiles[i].isWarp == true)
        {
            if(changement_de_carte == false)
            {
                std::cout << "collision avec une porte" << std::endl;

                this->carte_actuelle->x_depart = this->carte_actuelle->tuiles[i].position.x;
                this->carte_actuelle->y_depart = this->carte_actuelle->tuiles[i].position.y;

                this->carte_actuelle = this->carte_actuelle->cartes[this->carte_actuelle->tuiles[i].id_porte];

                this->joueur.position.x = this->carte_actuelle->x_depart;
                this->joueur.position.y = this->carte_actuelle->y_depart;

                this->changement_de_carte = true;
            }
        }
    }
    if(changement_de_carte == true) //si j'ai chang� de carte et que je me trouve sur une porte, alors pas de changement imm�diat de carte
    {
        if(this->joueur.position.x >= this->carte_actuelle->x_depart + 64 || this->joueur.position.x <= this->carte_actuelle->x_depart - 64
        || this->joueur.position.y >= this->carte_actuelle->y_depart + 64 || this->joueur.position.y <= this->carte_actuelle->y_depart - 64)
        {
            //std::cout << this->carte_actuelle->x_depart << " et " << this->joueur.position.x << std::endl;
            changement_de_carte = false;
        }
    }
    //std::cout << this->carte_actuelle->x_depart << " et " << this->joueur.position.x << std::endl;
    //std::cout << changement_de_carte << std::endl;
}


void EnJeu::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    this->joueur.handleEvents(e, sing_syst);
    for(long long unsigned int i = 0; i < this->carte_actuelle->tuiles.size(); i++)
    {
        //this->tuiles[i].handleEvents(e, sing_syst);
    }
}
