#include "enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur(100, BLEU, {0, 0, 64, 64}, VUE_DESSUS)
{
    (void)sing_syst;
    this->carte_actuelle = sing_syst->cartes["toto.map"];
    this->changement_de_carte = false;
    this->carre_noir = {0, 0, LONGUEUR_FENETRE, HAUTEUR_FENETRE};
    this->alpha = 255;
    this->dialogue = false;
}

bool EnJeu::collision(SDL_Rect dest_joueur, SDL_Rect position)
{
    if(dest_joueur.y + dest_joueur.h > position.y
    && dest_joueur.y < position.y + position.h
    && dest_joueur.x + dest_joueur.w > position.x
    && dest_joueur.x < position.x + position.w)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool EnJeu::estACote(SDL_Rect dest_joueur, SDL_Rect position)
{
    if((dest_joueur.x + dest_joueur.w == position.x && dest_joueur.y == position.y)
    || (dest_joueur.y + dest_joueur.h == position.y && dest_joueur.x == position.x)
    || (dest_joueur.x == position.x + position.w && dest_joueur.y == position.y)
    || (dest_joueur.y == position.y + position.h && dest_joueur.x == position.x))
    {
        return true;
    }
    else return false;
}


void EnJeu::playSoundCollision(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    if(sing_syst->son_active == true)
    {
        if(une_fois == true)
        {
            CHK(Mix_PlayChannel(1, sing_syst->son_collision, 0), Mix_GetError());
            une_fois = false;
        }
        else
        {
            if(SDL_GetTicks() - timeStep > 200)
            {
                CHK(Mix_PlayChannel(1, sing_syst->son_collision, 0), Mix_GetError());
                timeStep = SDL_GetTicks();
            }
        }
    }
}


void EnJeu::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    camera.x = this->joueur.position.x + this->joueur.position.w/2 - LONGUEUR_FENETRE/2;
    camera.y = this->joueur.position.y + this->joueur.position.h/2 - HAUTEUR_FENETRE/2;

    this->carte_actuelle->draw(rendu, camera);
    this->joueur.draw(rendu, camera);


    if(this->dialogue == true)
        b->draw(rendu);



    if(this->enTransition == true)
    {
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, this->alpha);
        SDL_RenderFillRect(rendu, &this->carre_noir);
    }
}


void EnJeu::checkCollisionsPlayerMap(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    static bool collisionEnCours = false;

    for(long long unsigned int i = 0; i < carte_actuelle->elementCarte.size(); i++)
    {
        //s'il y a une collision avec une tuile, on ne regarde pas les autres tuiles
        if(collisionEnCours == false || (collisionEnCours == true && carte_actuelle->elementCarte[i]->collisionAvecJoueur == true))
        {
            //on regarde les collisions uniquement avec les tuiles non passables
            if(carte_actuelle->elementCarte[i]->estPassable == false)
            {
                SDL_Rect copie = this->joueur.position;
                if(this->joueur.dep[0] == true)
                    copie.y -= 16 * 4; //taille d'une tuile
                else if(this->joueur.dep[1] == true)
                    copie.y += 16 * 4;
                else if(this->joueur.dep[2] == true)
                    copie.x -= 16 * 4;
                else if(this->joueur.dep[3] == true)
                    copie.x += 16 * 4;

                Objet* o = dynamic_cast<Objet*>(carte_actuelle->elementCarte[i]);
                //si l'élément de la carte est un objet
                if(o != nullptr)
                {
                    //si le joueur est à une case de l'objet
                    if(estACote(this->joueur.position, this->carte_actuelle->elementCarte[i]->position) == true
                    && this->joueur.interagit == true)
                    {
                        std::cout << "objet obtenu !" << std::endl;
                        b = new BoiteDeDialogue({0, 255, 255, 255}, {300, 300, 300, 100}, {"Vous avez obtenu objet X!", "Bravo!"}, 30, sing_syst->rendu, "boite A");
                        this->dialogue = true;
                        collisionEnCours = false;
                        this->carte_actuelle->elementCarte[i]->collisionAvecJoueur = false;
                        this->carte_actuelle->elementCarte.erase(this->carte_actuelle->elementCarte.begin() + i);
                    }
                }

                if(this->joueur.dep[0] == true || this->joueur.dep[1] == true|| this->joueur.dep[2] == true || this->joueur.dep[3] == true)
                {
                    if(collision(copie, carte_actuelle->elementCarte[i]->position) == true)
                    {
                        if(collisionEnCours == true)
                            this->playSoundCollision(timeStep, sing_syst);
                        this->joueur.resetAllValues();
                        carte_actuelle->elementCarte[i]->collisionAvecJoueur = true;
                        collisionEnCours = true;
                    }
                    else
                    {
                        //la collision est desactivée uniquement si le joueur arrête sa collision avec la tuile ayant provoquée la collision
                        carte_actuelle->elementCarte[i]->collisionAvecJoueur = false;
                        collisionEnCours = false;
                        une_fois = true;
                    }
                }
            }
        }
    }
}

void EnJeu::checkPlayerTakesObjects()
{
    /*for(long long unsigned int i = 0; i < carte_actuelle->elementCarte.size(); i++)
    {
        Objet* o = dynamic_cast<Objet*>(carte_actuelle->elementCarte[i]);

        //si l'élément de la carte est un objet
        if(o != nullptr)
        {
            //si le joueur est à une case de l'objet
            if((this->joueur.position.x + this->joueur.position.w == this->carte_actuelle->elementCarte[i]->position.x && this->joueur.position.y == this->carte_actuelle->elementCarte[i]->position.y)
            || (this->joueur.position.y + this->joueur.position.h == this->carte_actuelle->elementCarte[i]->position.y && this->joueur.position.x == this->carte_actuelle->elementCarte[i]->position.x)
            || (this->joueur.position.x == this->carte_actuelle->elementCarte[i]->position.x + this->carte_actuelle->elementCarte[i]->position.w && this->joueur.position.y == this->carte_actuelle->elementCarte[i]->position.y)
            || (this->joueur.position.y == this->carte_actuelle->elementCarte[i]->position.y + this->carte_actuelle->elementCarte[i]->position.h && this->joueur.position.x == this->carte_actuelle->elementCarte[i]->position.x))
            {
                if(this->joueur.interagit == true)
                {
                    std::cout << "objet obtenu !" << std::endl;
                    this->carte_actuelle->elementCarte[i]->collisionAvecJoueur = false;
                    this->carte_actuelle->elementCarte.erase(this->carte_actuelle->elementCarte.begin() + i);
                    //this->carte_actuelle->objects.erase(this->carte_actuelle->objects.begin() + i);
                }
            }
        }
    }*/
}


void EnJeu::gereJoueurPrendWarp()
{
    //position que le joueur a lors du changement de carte
    //sert de référence pour savoir si le joueur s'est déplacé ou pas pour modifier en conséquence changement_de_porte
    static int previous_x = 0;
    static int previous_y = 0;

    for(long long unsigned int i = 0; i < carte_actuelle->tuiles.size(); i++)
    {
        if(this->collision(joueur.position, this->carte_actuelle->tuiles[i].position) == true && this->carte_actuelle->tuiles[i].isWarp == true)
        {
            //aller vers la nouvelle carte
            if(changement_de_carte == false)
            {
                for(long long unsigned int k = 0; k < this->carte_actuelle->warp_cartes.size(); k++)
                {
                    //std::cout << joueur.position.x << " et " << this->carte_actuelle->warp_cartes[k].x_depart;

                    //trouver le warp_event que le joueur a déclenché
                    if(this->carte_actuelle->warp_cartes[k].x_warp >= joueur.position.x
                    && this->carte_actuelle->warp_cartes[k].x_warp <= joueur.position.x + joueur.position.w
                    && this->carte_actuelle->warp_cartes[k].y_warp >= joueur.position.y
                    && this->carte_actuelle->warp_cartes[k].y_warp <= joueur.position.y + joueur.position.h)
                    {
                        //placer le joueur au bon endroit dans la nouvelle carte
                        this->joueur.position.x = this->carte_actuelle->warp_cartes[k].x_arrive;
                        this->joueur.position.y = this->carte_actuelle->warp_cartes[k].y_arrive;

                        previous_x = this->joueur.position.x;
                        previous_y = this->joueur.position.y;

                        this->enTransition = true;

                        this->carte_actuelle = this->carte_actuelle->warp_cartes[k].warp_carte;
                    }
                }
                this->changement_de_carte = true;
            }
        }
    }

    //TODO : quand le joueur prend une porte, il arrive toujours à (limite bas - une tuile) en y sur la carte, le x pouvant varier (maintenant toujours à la moitié)
    if(changement_de_carte == true) //si j'ai changé de carte et que je me trouve sur une porte, alors pas de changement immédiat de carte
    {
        if(this->joueur.position.x >= previous_x + (16 * 4) || this->joueur.position.x <= previous_x - (16 * 4)
        || this->joueur.position.y >= previous_y + (16 * 4) || this->joueur.position.y <= previous_y - (16 * 4))
        {
            this->changement_de_carte = false;
        }
    }
}


void EnJeu::gereJoueurSortExtremite()
{
    //on sort de la carte dans laquelle on est arrivé après avoir pris la porte en sortant par une extrémité
    //revenir dans la carte principale
    if(this->carte_actuelle->limite_haut == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_haut != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_haut;

        //arrive centré sur la carte d'arrivée en x et tout en bas de la carte en y

        int val = this->carte_actuelle->limite_droite / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = val;


        this->joueur.position.y = this->carte_actuelle->limite_bas;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_bas == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_bas != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_bas;

        //arrive centré sur la carte d'arrivée en x et tout en haut de la carte en y

        int val = this->carte_actuelle->limite_droite / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = val;
        this->joueur.position.y = this->carte_actuelle->limite_haut;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_gauche == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_gauche != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_gauche;

        //arrive tout à droite sur la carte d'arrivée en x et centré en y

        int val = this->carte_actuelle->limite_bas / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = this->carte_actuelle->limite_droite;
        this->joueur.position.y = val;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_droite == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_droite != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_droite;

        //arrive tout à gauche sur la carte d'arrivée en x et centré en y

        int val = this->carte_actuelle->limite_bas / 2;
        if(val % (16 * 4) != 0)
        {
            val /= (16 * 4); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = this->carte_actuelle->limite_gauche;
        this->joueur.position.y = val;
        this->changement_de_carte = true;
        this->enTransition = true;
    }
    //std::cout << changement_de_carte << std::endl;
}


void EnJeu::gereSelonTransition(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    if(this->enTransition == false)
    {
        this->carte_actuelle->update(timeStep, sing_syst);
        this->joueur.update(timeStep, sing_syst/*, carte_actuelle->tuiles*/);
    }
    else
    {
        // TODO : pour simuler la nuit ou l'après midi, mettre un carré de la couleur adaptée
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
}


void EnJeu::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    if(this->dialogue == true)
    {
        b->update(timeStep);
        if(b->tous_dialogues_finis == true)
        {
            this->dialogue = false;
            delete b;
        }
    }
    else
    {
        checkCollisionsPlayerMap(timeStep, sing_syst);
        //checkPlayerTakesObjects();
        gereSelonTransition(timeStep, sing_syst);
        gereJoueurPrendWarp();
        gereJoueurSortExtremite();
    }
}


void EnJeu::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    //test : avancer le joueur d'une position selon la touche appuyée
    //si pas de collision avec une tuile, alors le joueur avance

    //test : si le joueur est en face d'un objet, alors il pourra l'attraper



    if(this->dialogue == true)
        b->handleEvents(e, sing_syst);
    else
    {
        this->joueur.handleEvents(e, sing_syst);
        for(long long unsigned int i = 0; i < this->carte_actuelle->tuiles.size(); i++)
        {
            //this->tuiles[i].handleEvents(e, sing_syst);
        }
    }
}
