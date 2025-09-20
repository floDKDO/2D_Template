#include "../include/enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur("./img/spritesheet.png", {0, 0, TAILLE_TUILE * FACTEUR_MULTIPLICATION, TAILLE_TUILE * FACTEUR_MULTIPLICATION}, VUE_DESSUS, sing_syst->rendu)
{
    (void)sing_syst;
    this->carte_actuelle = sing_syst->cartes["toto.map"];
    this->carre_noir = {0, 0, LONGUEUR_FENETRE, HAUTEUR_FENETRE};
    this->alpha = 255;
    this->dialogue = false;
    this->enTransition = false;
    this->une_fois = true;
}


bool EnJeu::estACote(Joueur joueur, SDL_Rect position)
{
    if((joueur.position.x + joueur.position.w == position.x && joueur.position.y == position.y && joueur.orientation == DROITE)
    || (joueur.position.y + joueur.position.h == position.y && joueur.position.x == position.x && joueur.orientation == BAS)
    || (joueur.position.x == position.x + position.w && joueur.position.y == position.y && joueur.orientation == GAUCHE)
    || (joueur.position.y == position.y + position.h && joueur.position.x == position.x && joueur.orientation == HAUT))
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
            if(SDL_GetTicks() - timeStep > 500)
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


bool EnJeu::replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}



void EnJeu::checkCollisionsPlayerMap(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    static bool est_colle = false; //si le joueur est collé contre un mur et se déplace dessus, cette variable vaut true

    if(this->joueur.dep[0] == true || this->joueur.dep[1] == true|| this->joueur.dep[2] == true || this->joueur.dep[3] == true)
    {
        SDL_Rect copie = this->joueur.position;
        if(this->joueur.dep[0] == true)
            copie.y -= TAILLE_TUILE * FACTEUR_MULTIPLICATION; //taille d'une tuile
        else if(this->joueur.dep[1] == true)
            copie.y += TAILLE_TUILE * FACTEUR_MULTIPLICATION;
        else if(this->joueur.dep[2] == true)
            copie.x -= TAILLE_TUILE * FACTEUR_MULTIPLICATION;
        else if(this->joueur.dep[3] == true)
            copie.x += TAILLE_TUILE * FACTEUR_MULTIPLICATION;

        for(long long unsigned int i = 0; i < carte_actuelle->elementCarte.size()/* && joueur.je_peux_marcher == true*/; i++)
        {
            if(carte_actuelle->elementCarte[i]->estPassable == false && SDL_HasIntersection(&copie, &carte_actuelle->elementCarte[i]->position) == true)
            {
                joueur.je_peux_marcher = false;
                est_colle = true;
            }
        }
    }
    else
    {
        if(est_colle == true) //quand le joueur avance contre un mur et qu'il le quitte, il est toujours collé au mur : remettre 'une_fois' à true
            this->une_fois = true;

        est_colle = false;
        joueur.je_peux_marcher = true;
        checkPlayerTakesObjects(sing_syst);
    }

    if(est_colle == true)
        this->playSoundCollision(timeStep, sing_syst);
}

void EnJeu::checkPlayerTakesObjects(SingletonSysteme* sing_syst)
{
    for(long long unsigned int i = 0; i < carte_actuelle->elementCarte.size(); i++)
    {
        Objet* o = dynamic_cast<Objet*>(carte_actuelle->elementCarte[i]);

        //si l'élément de la carte est un objet
        if(o != nullptr)
        {
            //si le joueur est à une case de l'objet
            if(estACote(this->joueur, this->carte_actuelle->elementCarte[i]->position) == true && this->joueur.interagit == true)
            {
                std::cout << "objet obtenu !" << std::endl;

                b = new BoiteDeDialogue({0, 255, 255, 255}, {300, 300, 300, 100}, sing_syst->string_obtention_objet, 30, sing_syst->rendu, "boite A");
                for(long long unsigned int i = 0; i < b->textes_defilement.size(); i++)
                    replace(b->textes_defilement[i].texte, "{ITEM_NAME}", o->nom_objet);

                this->dialogue = true;
                this->carte_actuelle->elementCarte.erase(this->carte_actuelle->elementCarte.begin() + i);
            }
        }
    }
}


void EnJeu::gereJoueurPrendWarp()
{
    //position que le joueur a lors du changement de carte
    //sert de référence pour savoir si le joueur s'est déplacé ou pas pour modifier en conséquence vient_de_changer_de_carte
    static int arrivee_x = 0;
    static int arrivee_y = 0;
    static bool vient_de_changer_de_carte = false;

    for(long long unsigned int i = 0; i < carte_actuelle->tuiles.size(); i++)
    {
        if(this->carte_actuelle->tuiles[i].isWarp == true && SDL_HasIntersection(&joueur.position, &this->carte_actuelle->tuiles[i].position) == true)
        {
            //aller vers la nouvelle carte
            if(vient_de_changer_de_carte == false)
            {
                for(long long unsigned int j = 0; j < this->carte_actuelle->warp_cartes.size(); j++)
                {
                    //trouver le warp_event que le joueur a déclenché
                    if(this->carte_actuelle->warp_cartes[j].x_warp >= joueur.position.x
                    && this->carte_actuelle->warp_cartes[j].x_warp <= joueur.position.x + joueur.position.w
                    && this->carte_actuelle->warp_cartes[j].y_warp >= joueur.position.y
                    && this->carte_actuelle->warp_cartes[j].y_warp <= joueur.position.y + joueur.position.h)
                    {
                        //placer le joueur au bon endroit dans la nouvelle carte
                        this->joueur.position.x = this->carte_actuelle->warp_cartes[j].x_arrive;
                        this->joueur.position.y = this->carte_actuelle->warp_cartes[j].y_arrive;
                        arrivee_x = this->carte_actuelle->warp_cartes[j].x_arrive;
                        arrivee_y = this->carte_actuelle->warp_cartes[j].y_arrive;
                        this->enTransition = true;
                        this->carte_actuelle = this->carte_actuelle->warp_cartes[j].warp_carte;
                    }
                }
                vient_de_changer_de_carte = true;
            }
        }
    }

    //TODO : quand le joueur prend une porte, il arrive toujours à (limite bas - une tuile) en y sur la carte, le x pouvant varier (maintenant toujours à la moitié)

    if(vient_de_changer_de_carte == true) //si j'ai changé de carte et que je me trouve sur une porte, alors pas de changement immédiat de carte
    {
        //si j'ai bougé
        if(this->joueur.position.x >= arrivee_x + (TAILLE_TUILE * FACTEUR_MULTIPLICATION) || this->joueur.position.x <= arrivee_x - (TAILLE_TUILE * FACTEUR_MULTIPLICATION)
        || this->joueur.position.y >= arrivee_y + (TAILLE_TUILE * FACTEUR_MULTIPLICATION) || this->joueur.position.y <= arrivee_y - (TAILLE_TUILE * FACTEUR_MULTIPLICATION))
        {
            vient_de_changer_de_carte = false;
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


        /*void newCarteExtremite(Carte* connection_dest, int cote_extremite)
        {
            this->carte_actuelle = connection_dest;
        }*/


        int val = this->carte_actuelle->limite_droite / 2;
        if(val % (TAILLE_TUILE * FACTEUR_MULTIPLICATION) != 0)
        {
            val /= (TAILLE_TUILE * FACTEUR_MULTIPLICATION); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = val;
        this->joueur.position.y = this->carte_actuelle->limite_bas;
        //this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_bas == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_bas != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_bas;

        //arrive centré sur la carte d'arrivée en x et tout en haut de la carte en y

        int val = this->carte_actuelle->limite_droite / 2;
        if(val % (TAILLE_TUILE * FACTEUR_MULTIPLICATION) != 0)
        {
            val /= (TAILLE_TUILE * FACTEUR_MULTIPLICATION); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = val;
        this->joueur.position.y = this->carte_actuelle->limite_haut;
        //this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_gauche == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_gauche != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_gauche;

        //arrive tout à droite sur la carte d'arrivée en x et centré en y

        int val = this->carte_actuelle->limite_bas / 2;
        if(val % (TAILLE_TUILE * FACTEUR_MULTIPLICATION) != 0)
        {
            val /= (TAILLE_TUILE * FACTEUR_MULTIPLICATION); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = this->carte_actuelle->limite_droite;
        this->joueur.position.y = val;
        //this->changement_de_carte = true;
        this->enTransition = true;
    }
    else if(this->carte_actuelle->limite_droite == this->joueur.position.y && this->carte_actuelle->est_carte_principale == false && this->carte_actuelle->connection_droite != nullptr)
    {
        this->carte_actuelle = this->carte_actuelle->connection_droite;

        //arrive tout à gauche sur la carte d'arrivée en x et centré en y

        int val = this->carte_actuelle->limite_bas / 2;
        if(val % (TAILLE_TUILE * FACTEUR_MULTIPLICATION) != 0)
        {
            val /= (TAILLE_TUILE * FACTEUR_MULTIPLICATION); //ramener à la valeur inférieure (ex : val == 224 et 224 / 64 = 3.5, on a que val = 3)
        }

        this->joueur.position.x = this->carte_actuelle->limite_gauche;
        this->joueur.position.y = val;
        //this->changement_de_carte = true;
        this->enTransition = true;
    }
    //std::cout << changement_de_carte << std::endl;
}


void EnJeu::gereSelonTransition(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    if(this->enTransition == false)
    {
        this->carte_actuelle->update(timeStep, sing_syst);
        this->joueur.update(timeStep, sing_syst);
    }
    else
    {
        // TODO : pour simuler la nuit ou l'après midi, mettre un carré de la couleur adaptée
        if(alpha > 0)
        {
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
            Mix_FreeChunk(b->click_sound);
            delete b;
        }
    }
    else
    {
        checkCollisionsPlayerMap(timeStep, sing_syst);
        gereSelonTransition(timeStep, sing_syst);
        gereJoueurPrendWarp();
        gereJoueurSortExtremite();
    }
}


void EnJeu::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

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
