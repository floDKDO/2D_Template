#include "carte.hpp"

#include <singleton_systeme.hpp>

Carte::Carte(std::string fichier_carte, bool est_carte_principale, SingletonSysteme* sing_syst)
{
    this->est_carte_principale = est_carte_principale;
    this->fichier_carte = fichier_carte;

    this->connection_haut = nullptr;
    this->connection_bas = nullptr;
    this->connection_gauche = nullptr;
    this->connection_droite = nullptr;

    //this->initTuiles(this->fichier_carte, sing_syst);
    this->limite_haut = 0;
    this->limite_gauche = 0;
    this->une_fois = true;
}


void Carte::initJson(std::string fichier_carte, SingletonSysteme* sing_syst)
{
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
    NCHK(this->musique = Mix_LoadMUS(music_carte.c_str()), Mix_GetError());

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

            int offset_x = connections_carte[i]["x_offset"];
            offset_x = (offset_x * (16 * 4)) - (16 * 4); //16*4 = largeur d'une tuile, -16*4 car on veut l'indice (tuile 0 position 0,0)
            int offset_y = connections_carte[i]["y_offset"];
            offset_y = (offset_y * (16 * 4)) - (16 * 4);
        }
    }

    json warp_events_carte = data["warp_events"];
    if(warp_events_carte.is_null() == false)
    {
        for(long long unsigned int i = 0; i < warp_events_carte.size(); i++)
        {
            int warp_x = warp_events_carte[i]["x_warp"];
            warp_x = (warp_x * (16 * 4)) - (16 * 4); //16*4 = largeur d'une tuile, -16*4 car on veut l'indice (tuile 0 position 0,0)
            int warp_y = warp_events_carte[i]["y_warp"];
            warp_y = (warp_y * (16 * 4)) - (16 * 4);
            int arrive_x = warp_events_carte[i]["x_dest"];
            arrive_x = (arrive_x * (16 * 4)) - (16 * 4); //16*4 = largeur d'une tuile, -16*4 car on veut l'indice (tuile 0 position 0,0)
            int arrive_y = warp_events_carte[i]["y_dest"];
            arrive_y = (arrive_y * (16 * 4)) - (16 * 4);

            struct carte_warp s = {sing_syst->cartes[warp_events_carte[i]["dest_map"]], warp_x, warp_y, arrive_x, arrive_y};
            this->warp_cartes.push_back(s);
        }
    }

    json object_events_carte = data["object_events"];
    if(object_events_carte.is_null() == false)
    {
        for(long long unsigned int i = 0; i < object_events_carte.size(); i++)
        {
            //std::string id_objet = object_events_carte[i]["id_objet"];
            std::string graphics = object_events_carte[i]["graphics"];
            std::string name = object_events_carte[i]["name"];
            int pos_x = object_events_carte[i]["pos_x"];
            pos_x = (pos_x * (16 * 4)) - (16 * 4); //16*4 = largeur d'une tuile, -16*4 car on veut l'indice (tuile 0 position 0,0)
            int pos_y = object_events_carte[i]["pos_y"];
            pos_y = (pos_y * (16 * 4)) - (16 * 4);
            SDL_Rect position = {pos_x, pos_y, 64, 64};

            Objet o(graphics, sing_syst->rendu, position);
            o.nom_objet = name;
            //o.id_objet = id_objet;

            this->objects.push_back(o);
        }
    }


    /*
    int num = 89;
    int nb_colonnes = 88;
    int nb_lignes = 69;
    int taille_tuile = 16;

    int indice_ligne = (num-1)/nb_colonnes;
    int y = indice_ligne * taille_tuile;

    int ieme_tuile = (num-1) - indice_ligne*nb_colonnes;
    int x = ieme_tuile * taille_tuile;
    printf("ligne : %d et elem : %d\n", indice_ligne, ieme_tuile);
    printf("x : %d et y : %d\n", x, y);
    */

    json object_carte = data["map"];
    if(object_carte.is_null() == false)
    {
        for(long long unsigned int i = 0; i < object_carte.size(); i++)
        {
            std::string chemin_tileset = object_carte[i]["tileset"];
            //on ne peut pas se permettre de chager une texture du tileset par tuile
            //on donne une unique texture du tileset à toutes les tuiles
            NCHK(this->tileset = IMG_LoadTexture(sing_syst->rendu, chemin_tileset.c_str()), IMG_GetError());

            int taille_tuile = object_carte[i]["taille_tuile"];
            int nombre_tuile_x = object_carte[i]["width"];
            int nombre_tuile_y = object_carte[i]["height"];
            json object_tuiles = object_carte[i]["tuiles"];
            if(object_tuiles.is_null() == false)
            {
                for(long long unsigned int i = 0; i < object_tuiles.size(); i++)
                {
                    static int dest_x = 0;
                    static int dest_y = 0;

                    int num = object_tuiles[i];
                    int nb_colonnes = 88;
                    int nb_lignes = 69;

                    int indice_ligne = (num-1)/nb_colonnes;
                    int src_y = indice_ligne * taille_tuile;

                    int ieme_tuile = (num-1) - indice_ligne*nb_colonnes;
                    int src_x = ieme_tuile * taille_tuile;

                    int facteur = 4;
                    this->tuiles.push_back(Tuile(chemin_tileset, {dest_x, dest_y, taille_tuile * facteur, taille_tuile * facteur},{src_x, src_y, taille_tuile, taille_tuile}, true, sing_syst->rendu, this->tileset));

                    dest_x += taille_tuile * facteur;
                    if(dest_x == nombre_tuile_x * taille_tuile * facteur)
                    {
                        dest_x = 0;
                        dest_y += taille_tuile * facteur;
                    }
                }
            }
        }
    }

    for(long long unsigned int i = 0; i < this->tuiles.size(); i++)
    {
        this->elementCarte.push_back(&this->tuiles[i]);
    }

    for(long long unsigned int i = 0; i < this->objects.size(); i++)
    {
        this->elementCarte.push_back(&this->objects[i]);
    }

    fichier.close();
}


void Carte::initTuiles(std::string fichier_carte, SingletonSysteme* sing_syst)
{
    /*const int TILE_WIDTH = 16;
    const int TILE_HEIGHT = 16;

    //de combien la taille des tuiles (16*16) sera multipliée sur l'écran
    int facteur = 4;

    int x = 0, y = 0;

    int x_max = x;
    int y_max = y;

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
                this->tuiles.push_back(Tuile("./img/animated_water.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, 8, 17, false, sing_syst->rendu));
            }
            else if(tuiles_fichier[i].find("01") != std::string::npos) //01 ou 01(D)
            {
                this->tuiles.push_back(Tuile("./img/herbe.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, true, false, sing_syst->rendu));
            }
            else if(tuiles_fichier[i].find("02") != std::string::npos) //02 ou 02(D) (fait changer de zone)
            {
                this->tuiles.push_back(Tuile("./img/porte.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, true, true, sing_syst->rendu));
            }
            else if(tuiles_fichier[i].find("03") != std::string::npos) //02 ou 02(D) (fait changer de zone)
            {
                this->tuiles.push_back(Tuile("./img/stairs.png", {x * facteur, y * facteur, TILE_WIDTH * facteur, TILE_HEIGHT * facteur}, true, true, sing_syst->rendu));
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

	this->limite_droite = x_max + (TILE_WIDTH * facteur); //on ajoute une tuile : quand on quitte la carte, le joueur change de carte
	this->limite_bas = y_max + (TILE_HEIGHT * facteur); //idem

    for(long long unsigned int i = 0; i < this->tuiles.size(); i++)
    {
        this->elementCarte.push_back(&this->tuiles[i]);
    }

    fichier.close();*/
}

void Carte::jouerMusique(void)
{
    if(une_fois == true)
    {
        Mix_PlayMusic(this->musique, 1);
        une_fois = false;
    }
}


void Carte::draw(SDL_Renderer* rendu, SDL_Rect camera)
{
    for(long long unsigned int i = 0; i < elementCarte.size(); i++)
    {
        this->elementCarte[i]->draw(rendu, camera);
    }
}


void Carte::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    //jouerMusique();
    for(long long unsigned int i = 0; i < elementCarte.size(); i++)
    {
        this->elementCarte[i]->update(timeStep, sing_syst);
    }
}
