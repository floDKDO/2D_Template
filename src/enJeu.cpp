#include "enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur(100, BLEU, {0, 0, 64, 64}, VUE_DESSUS)
{
    (void)sing_syst;
    this->carte_actuelle = sing_syst->cartes["toto.map"];
    this->changement_de_carte = false;
    this->carre_noir = {0, 0, LONGUEUR_FENETRE, HAUTEUR_FENETRE};
    this->alpha = 255;
}


void EnJeu::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    camera.x = this->joueur.position.x + this->joueur.position.w/2 - LONGUEUR_FENETRE/2;
    camera.y = this->joueur.position.y + this->joueur.position.h/2 - HAUTEUR_FENETRE/2;

    this->carte_actuelle->draw(rendu, camera);
    this->joueur.draw(rendu, camera);

    if(this->enTransition == true)
    {
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, this->alpha);
        SDL_RenderFillRect(rendu, &this->carre_noir);
    }
}


void EnJeu::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    if(this->enTransition == false)
    {
        this->carte_actuelle->update(timeStep, sing_syst);
        this->joueur.update(timeStep, sing_syst, carte_actuelle->tuiles);
    }
    else
    {
        // TODO : pour simuler la nuit ou l'apr�s midi, mettre un carr� de la couleur adapt�e
        if(alpha > 0)
        {
            //std::cout << alpha << std::endl;
            this->alpha -= 15; //15 est un diviseur de 255
        }
        else
        {
            this->enTransition = false;
            alpha = 255;
        }
    }


    //position que le joueur a lors du changement de carte
    //sert de r�f�rence pour savoir si le joueur s'est d�plac� ou pas pour modifier en cons�quence changement_de_porte
    static int previous_x = 0;
    static int previous_y = 0;


    for(long long unsigned int i = 0; i < carte_actuelle->tuiles.size(); i++)
    {
        if(this->joueur.collision(joueur.position, this->carte_actuelle->tuiles[i]) == true && this->carte_actuelle->tuiles[i].isWarp == true)
        {
            //aller vers la nouvelle carte
            if(changement_de_carte == false)
            {
                for(long long unsigned int k = 0; k < this->carte_actuelle->warp_cartes_test.size(); k++)
                {
                    //std::cout << joueur.position.x << " et " << this->carte_actuelle->warp_cartes_test[k].x_depart;

                    //trouver le warp_event que le joueur a d�clench�
                    if(this->carte_actuelle->warp_cartes_test[k].x_warp >= joueur.position.x
                    && this->carte_actuelle->warp_cartes_test[k].x_warp <= joueur.position.x + joueur.position.w
                    && this->carte_actuelle->warp_cartes_test[k].y_warp >= joueur.position.y
                    && this->carte_actuelle->warp_cartes_test[k].y_warp <= joueur.position.y + joueur.position.h)
                    {
                        //placer le joueur au bon endroit dans la nouvelle carte
                        this->joueur.position.x = this->carte_actuelle->warp_cartes_test[k].x_arrive;
                        this->joueur.position.y = this->carte_actuelle->warp_cartes_test[k].y_arrive;

                        previous_x = this->joueur.position.x;
                        previous_y = this->joueur.position.y;

                        this->enTransition = true;

                        this->carte_actuelle = this->carte_actuelle->warp_cartes_test[k].warp_carte;
                    }
                }
                this->changement_de_carte = true;
            }
        }
    }

    //std::cout << joueur.position.x << " et " << previous_x << std::endl;

    //TODO : quand le joueur prend une porte, il arrive toujours � (limite bas - une tuile) en y sur la carte, le x pouvant varier (maintenant toujours � la moiti�)

    if(changement_de_carte == true) //si j'ai chang� de carte et que je me trouve sur une porte, alors pas de changement imm�diat de carte
    {
        if(this->joueur.position.x >= previous_x + (16 * 4) || this->joueur.position.x <= previous_x - (16 * 4)
        || this->joueur.position.y >= previous_y + (16 * 4) || this->joueur.position.y <= previous_y - (16 * 4))
        {
            this->changement_de_carte = false;
        }
    }

    //on sort de la carte dans laquelle on est arriv� apr�s avoir pris la porte en sortant par une extr�mit�
    //revenir dans la carte principale
    if(this->carte_actuelle->limite_haut == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_haut != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_haut;

        //arrive centr� sur la carte d'arriv�e en x et tout en bas de la carte en y

        int val = this->carte_actuelle->limite_droite / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener � la valeur inf�rieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = val;


        this->joueur.position.y = this->carte_actuelle->limite_bas;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_bas == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_bas != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_bas;

        //arrive centr� sur la carte d'arriv�e en x et tout en haut de la carte en y

        int val = this->carte_actuelle->limite_droite / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener � la valeur inf�rieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = val;
        this->joueur.position.y = this->carte_actuelle->limite_haut;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_gauche == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_gauche != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_gauche;

        //arrive tout � droite sur la carte d'arriv�e en x et centr� en y

        int val = this->carte_actuelle->limite_bas / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener � la valeur inf�rieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = this->carte_actuelle->limite_droite;
        this->joueur.position.y = val;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_droite == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_droite != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_droite;

        //arrive tout � gauche sur la carte d'arriv�e en x et centr� en y

        int val = this->carte_actuelle->limite_bas / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener � la valeur inf�rieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = this->carte_actuelle->limite_gauche;
        this->joueur.position.y = val;
        this->changement_de_carte = true;
        this->enTransition = true;
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
