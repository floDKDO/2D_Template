#include "enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur(100, BLEU, {0, 0, 64, 64}, VUE_DESSUS)
{
    (void)sing_syst;
    this->carte_actuelle = sing_syst->cartes["toto.map"];
    this->changement_de_carte = false;
}


void EnJeu::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    camera.x = this->joueur.position.x + this->joueur.position.w/2 - LONGUEUR_FENETRE/2;
    camera.y = this->joueur.position.y + this->joueur.position.h/2 - HAUTEUR_FENETRE/2;

    this->carte_actuelle->draw(rendu, camera);
    this->joueur.draw(rendu, camera);
}


void EnJeu::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    this->carte_actuelle->update(timeStep, sing_syst);
    this->joueur.update(timeStep, sing_syst, carte_actuelle->tuiles);


    //TODO : à simplifier !

    for(long long unsigned int i = 0; i < carte_actuelle->tuiles.size(); i++)
    {
        if(this->joueur.collision(joueur.position, this->carte_actuelle->tuiles[i]) == true && this->carte_actuelle->tuiles[i].isWarp == true)
        {
            //aller vers la nouvelle carte
            if(changement_de_carte == false)
            {
                //std::cout << "collision avec une porte" << std::endl;

                //récupérer la position du joueur qu'il avait quand il a pris la porte
                this->carte_actuelle->x_depart = this->carte_actuelle->tuiles[i].position.x;
                this->carte_actuelle->y_depart = this->carte_actuelle->tuiles[i].position.y;

                this->carte_actuelle = this->carte_actuelle->warp_cartes[this->carte_actuelle->tuiles[i].id_porte];

                //placer le joueur au bon endroit dans la nouvelle carte
                this->joueur.position.x = this->carte_actuelle->x_depart;
                this->joueur.position.y = this->carte_actuelle->y_depart;

                this->changement_de_carte = true;
            }
        }
    }

    //TODO : quand le joueur prend une porte, il arrive toujours à (limite bas - une tuile) en y sur la carte, le x pouvant varier
    //TODO : trouver une meilleure alternative à (D) => faire comme DewfordTown/map.json de PokeRuby de GitHub (trouver une librairie json)

    if(changement_de_carte == true) //si j'ai changé de carte et que je me trouve sur une porte, alors pas de changement immédiat de carte
    {
        if(this->joueur.position.x >= this->carte_actuelle->x_depart + 64 || this->joueur.position.x <= this->carte_actuelle->x_depart - 64
        || this->joueur.position.y >= this->carte_actuelle->y_depart + 64 || this->joueur.position.y <= this->carte_actuelle->y_depart - 64)
        {
            this->changement_de_carte = false;
        }
    }

    //on sort de la carte dans laquelle on est arrivé après avoir pris la porte en sortant par une extrémité
    //revenir dans la carte principale
    if(this->carte_actuelle->limite_haut == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_haut != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_haut;
        this->joueur.position.x = this->carte_actuelle->x_depart;
        this->joueur.position.y = this->carte_actuelle->y_depart;
        this->changement_de_carte = true;
    }
    else if(this->carte_actuelle->limite_bas == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_bas != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_bas;
        this->joueur.position.x = this->carte_actuelle->x_depart;
        this->joueur.position.y = this->carte_actuelle->y_depart;
        this->changement_de_carte = true;
    }
    else if(this->carte_actuelle->limite_gauche == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_gauche != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_gauche;
        this->joueur.position.x = this->carte_actuelle->x_depart;
        this->joueur.position.y = this->carte_actuelle->y_depart;
        this->changement_de_carte = true;
    }
    else if(this->carte_actuelle->limite_droite == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_droite != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_droite;
        this->joueur.position.x = this->carte_actuelle->x_depart;
        this->joueur.position.y = this->carte_actuelle->y_depart;
        this->changement_de_carte = true;
    }
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
