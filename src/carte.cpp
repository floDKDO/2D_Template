#include "carte.hpp"

#include <singleton_systeme.hpp>

Carte::Carte(std::string fichier_carte, bool est_carte_principale)
{
    this->est_carte_principale = est_carte_principale;
    this->fichier_carte = fichier_carte;

    this->connection_haut = nullptr;
    this->connection_bas = nullptr;
    this->connection_gauche = nullptr;
    this->connection_droite = nullptr;

    this->initTuiles(this->fichier_carte);
    this->limite_haut = 0;
    this->limite_gauche = 0;
}


void Carte::initJson(std::string fichier_carte, SingletonSysteme* sing_syst)
{
    /*
    - ID MAP : val
    - NOM_FICHIER_MAP : fichier.map
    - NOM MAP : val
    - MUSIQUE : val
    - CONNECTIONS : direction(nord, sud, ouest, est), carte atteinte, x et y de la direction
    - WARPS : x et y de la warp, carte atteinte
    - OBJETS / PERSOS: image, x et y de l'objet, mouvements si perso, longueur en x et y des mouvements
    */

    std::ifstream fichier(fichier_carte);
    json data = json::parse(fichier);

    std::string id_carte = data.value("id", "not found");
    if(sscanf(id_carte.c_str(), "%d", &(this->id_carte)) != 0)
    {
        std::cerr << "Erreur" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string nom_carte = data.value("name", "not found");
    this->nom_carte = nom_carte;
    std::string layout_carte = data.value("layout", "not found");
    this->fichier_carte = layout_carte;
    std::string music_carte = data.value("music", "not found");
    NCHK(this->musique = Mix_LoadMUS(music_carte.c_str()), Mix_GetError());;

    //<=>initConnections1
    json connections_carte = data["connections"];
    if(connections_carte.is_null() == false)
    {
        for(long long unsigned int i = 0; i < connections_carte.size(); i++)
        {
            if(connections_carte[i]["direction"] == "up")
                this->connection_haut = sing_syst->cartes[connections_carte[i]["map"]];
            else if(connections_carte[i]["direction"] == "down")
                this->connection_bas = sing_syst->cartes[connections_carte[i]["map"]];
            else if(connections_carte[i]["direction"] == "left")
                this->connection_gauche = sing_syst->cartes[connections_carte[i]["map"]];
            else if(connections_carte[i]["direction"] == "right")
                this->connection_droite = sing_syst->cartes[connections_carte[i]["map"]];
        }
    }

    //<=>initConnections
    json warp_events_carte = data["warp_events"];
    if(warp_events_carte.is_null() == false)
    {
        for(long long unsigned int i = 0; i < warp_events_carte.size(); i++)
            this->warp_cartes.push_back(sing_syst->cartes[warp_events_carte[i]["dest_map"]]);
    }

    fichier.close();
}


void Carte::initConnections(std::string fichier_carte, SingletonSysteme* sing_syst)
{
    std::ifstream fichier;
    fichier.open(fichier_carte);

    if(fichier.fail())
    {
		exit(EXIT_FAILURE);
    }

    std::string ligne;

    //lire le fichier ligne par ligne
    while(std::getline(fichier, ligne))
	{
	    if(ligne.find("Connection") != std::string::npos)
        {
            //récupérer ce qu'il y a après "Connection ... : "
            std::stringstream ss(ligne);
            std::string temp;
            std::vector<std::string> mots;
            while(std::getline(ss, temp, ':'))
            {
                mots.push_back(temp); //mettre les mots dans le tableau
            }

            if(mots[1].find(".map") != std::string::npos)
            {
                //enlever le " " au début
                mots[1].erase(0, 1);
                //std::cout << mots[1] << std::endl;

                this->warp_cartes.push_back(sing_syst->cartes[mots[1]]);
            }
        }
	}
	fichier.close();
}


void Carte::initConnections1(std::string fichier_carte, SingletonSysteme* sing_syst)
{
    std::ifstream fichier;
    fichier.open(fichier_carte);

    if(fichier.fail())
    {
		exit(EXIT_FAILURE);
    }

    std::string ligne;

    //lire le fichier ligne par ligne
    while(std::getline(fichier, ligne))
	{
	    if(ligne.find("Connection") != std::string::npos)
        {
            //récupérer ce qu'il y a après "Connection ... : "
            std::stringstream ss(ligne);
            std::string temp;
            std::vector<std::string> mots;
            while(std::getline(ss, temp, ':'))
            {
                mots.push_back(temp); //mettre les mots dans le tableau
            }

            if(ligne.find("haut") != std::string::npos)
            {
                //enlever le " " au début
                mots[1].erase(0, 1);
                std::cout << mots[1] << std::endl;

                this->connection_haut = sing_syst->cartes[mots[1]];
            }
            else if(ligne.find("bas") != std::string::npos)
            {
                //enlever le " " au début
                mots[1].erase(0, 1);
                std::cout << mots[1] << std::endl;

                this->connection_bas = sing_syst->cartes[mots[1]];
            }
            else if(ligne.find("gauche") != std::string::npos)
            {
                //enlever le " " au début
                mots[1].erase(0, 1);
                std::cout << mots[1] << std::endl;

                this->connection_gauche = sing_syst->cartes[mots[1]];
            }
            else if(ligne.find("droite") != std::string::npos)
            {
                //enlever le " " au début
                mots[1].erase(0, 1);
                std::cout << mots[1] << std::endl;

                this->connection_droite = sing_syst->cartes[mots[1]];
            }
        }
	}
	fichier.close();
}


void Carte::initTuiles(std::string fichier_carte)
{
    const int TILE_WIDTH = 16;
    const int TILE_HEIGHT = 16;

    //de combien la taille des tuiles (16*16) sera multipliée sur l'écran
    int facteur = 4;

    int x = 0, y = 0;

    int x_max = x;
    int y_max = y;

    unsigned int compteur_de_porte = 0;

    std::ifstream fichier;
    fichier.open(fichier_carte);

    if(fichier.fail())
    {
		exit(EXIT_FAILURE);
    }

    std::string ligne;
    std::vector<std::string> tuiles_fichier;

    //lire le fichier ligne par ligne
    while(std::getline(fichier, ligne))
	{
	    //pour ne pas fausser le y
	    if(ligne.find("Connection") != std::string::npos)
        {
            continue;
        }

        std::stringstream ss(ligne);
        std::string temp;

        //séparer la ligne en mots avec les espaces (un espace entre chaque mot) comme délimiteur
        while(std::getline(ss, temp, ' '))
        {
            tuiles_fichier.push_back(temp); //mettre les mots dans le tableau
        }

        //parcourir chaque mot du tableau => un mot est une tuile
        for(long long unsigned int i = 0; i < tuiles_fichier.size(); i++)
        {
            //SIMPLIFICATION POSSIBLE : nom des tuiles = tuiles_fichier[i]
            //=> this->tuiles.push_back(Tuile("./img/" + tuiles_fichier[i] + ".png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, 8, 17, false));

            if(x_max < x * facteur)
                x_max = x * facteur;

            if(y_max < y * facteur)
                y_max = y * facteur;

            if(tuiles_fichier[i].find("00") != std::string::npos) //00 ou 00(D)
            {
                this->tuiles.push_back(Tuile("./img/animated_water.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, 8, 17, false));
            }
            else if(tuiles_fichier[i].find("01") != std::string::npos) //01 ou 01(D)
            {
                this->tuiles.push_back(Tuile("./img/herbe.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, true, false));
            }
            else if(tuiles_fichier[i].find("02") != std::string::npos) //02 ou 02(D) (fait changer de zone)
            {
                this->tuiles.push_back(Tuile("./img/porte.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, true, true));
                this->tuiles.back().isDoor = true;
                this->tuiles.back().id_porte = compteur_de_porte; //indice de la carte à charger lors d'un changement de carte
                compteur_de_porte += 1;
            }
            else if(tuiles_fichier[i].find("03") != std::string::npos) //02 ou 02(D) (fait changer de zone)
            {
                this->tuiles.push_back(Tuile("./img/stairs.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, true, true));
                this->tuiles.back().isStairs = true;
                this->tuiles.back().id_porte = compteur_de_porte; //indice de la carte à charger lors d'un changement de carte
                compteur_de_porte += 1;
            }

            if(tuiles_fichier[i].find("(D)") != std::string::npos) //la tuile est déjà dans le vector
            {
                this->x_depart = tuiles.back().position.x;
                this->y_depart = tuiles.back().position.y;
            }

            //prochaine tuile
            x += TILE_WIDTH;
        }

        //comme on change de ligne, alors on change le x à 0 (debut de ligne) et le y (+= hauteur tuile)
        x = 0;
        y += TILE_HEIGHT;

        //on vide le tableau de string car cette ligne est finie
        tuiles_fichier.clear();
	}

	this->limite_droite = x_max + TILE_WIDTH * facteur; //on ajoute une tuile : quand on quitte la carte, le joueur change de carte
	this->limite_bas = y_max + TILE_HEIGHT * facteur; //idem

    fichier.close();
}


void Carte::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        this->tuiles[i].draw(rendu, camera);
    }
}


void Carte::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        this->tuiles[i].update(timeStep, sing_syst);
    }

    /*this->limite_haut += sing_syst->camera.y;
    this->limite_bas += sing_syst->camera.y;
    this->limite_gauche += sing_syst->camera.x;
    this->limite_droite += sing_syst->camera.x;

    std::cout << sing_syst->camera.y << std::endl;*/
}
