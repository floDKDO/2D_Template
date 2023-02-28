#include "enJeu.hpp"

EnJeu::EnJeu(SingletonSysteme* sing_syst)
:joueur(100, BLEU, {160, 160, 32, 32}, VUE_DESSUS)
{
    (void)sing_syst;
    this->initTuiles();
}


void EnJeu::initTuiles(void)
{
    const int TILE_WIDTH = 16;
    const int TILE_HEIGHT = 16;

    int x = 0, y = 0;

    std::ifstream fichier;
    fichier.open("toto.map");

    if(fichier.fail())
    {
		exit(EXIT_FAILURE);
    }

    std::string ligne;
    std::vector<std::string> tuiles_fichier;

    //lire le fichier ligne par ligne
    while(std::getline(fichier, ligne))
	{
        std::stringstream ss(ligne);
        std::string temp;

        //séparer la ligne en mots avec les espaces (un espace entre chaque mot) comme délimiteur
        while(getline(ss, temp, ' '))
        {
            tuiles_fichier.push_back(temp); //mettre les mots dans le tableau
        }

        //parcourir chaque mot du tableau => un mot est une tuile
        for(long long unsigned int i = 0; i < tuiles_fichier.size(); i++)
        {
            if(tuiles_fichier[i] == "00")
            {
                this->tuiles.push_back(Tuile("./img/animated_water.png", {x, y, 16, 16}, 8, 17));
            }
            else if(tuiles_fichier[i] == "01")
            {
                this->tuiles.push_back(Tuile("./img/herbe.png", {x, y, 16, 16}));
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
    fichier.close();
}



void EnJeu::draw(SingletonSysteme* sing_syst)
{
    this->joueur.draw(sing_syst->rendu);
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        this->tuiles[i].draw(sing_syst->rendu);
    }
}


void EnJeu::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    this->joueur.update(timeStep, sing_syst, tuiles);
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        this->tuiles[i].update(timeStep, sing_syst);
    }
}


void EnJeu::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    this->joueur.handleEvents(e, sing_syst);
    for(long long unsigned int i = 0; i < tuiles.size(); i++)
    {
        //this->tuiles[i].handleEvents(e, sing_syst);
    }
}
