#include "tuile.hpp"

//Tuile fixe
Tuile::Tuile(std::string chemin, SDL_Rect position)
{
    this->position = position;
    this->estPassable = false;
    this->chemin = chemin;
    this->isAnimated = false;
}


//Tuile animée
Tuile::Tuile(std::string chemin, SDL_Rect position, unsigned int nb_images, int espacement_tuiles_x)
{
    this->position = position;
    this->estPassable = false;
    this->chemin = chemin;
    this->isAnimated = true;

    this->nb_images = nb_images;
    this->espacement_tuiles_x = espacement_tuiles_x;
}


void Tuile::draw(SDL_Renderer* rendu)
{
    if((this->texture = IMG_LoadTexture(rendu, this->chemin.c_str())) == nullptr)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(SDL_RenderCopy(rendu, texture, &srcRect, &position) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}


void Tuile::update(Uint32& timeStep, SingletonSysteme* sing_syst)
{
    (void)timeStep;
    if(isAnimated == true)
    {
        Uint32 seconds = SDL_GetTicks() / 200; //200 => rapidité de l'animation
        int sprite = seconds % nb_images;
        srcRect = {espacement_tuiles_x * sprite, 0, 16, 16};
    }
    this->position = {position.x + sing_syst->camera.x, position.y + sing_syst->camera.y, position.w, position.h};
}



/*
//The dimensions of the level
const int LEVEL_WIDTH = 4608;
const int LEVEL_HEIGHT = 3248;

//Tile constants
const int TILE_WIDTH = 16*8;
const int TILE_HEIGHT = 16*7;
const int TOTAL_TILES = 928;
const int TOTAL_TILE_SPRITES = 12;

void fonction(std::vector<Tuile> tuiles)
{
    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream fichier;
    fichier.open("toto.map");

    //If the map couldn't be loaded
    if(fichier.fail())
    {
		exit(EXIT_FAILURE);
    }

	//Initialize the tiles
	for(int i = 0; i < TOTAL_TILES; ++i)
	{
		//Determines what kind of tile will be made
		std::string tileType;

		//Read tile from map file
		fichier >> tileType;

		//If the number is a valid tile number
		//if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
		//{
			//std::cout << tileType << std::endl;
		//}

		if(tileType == "00")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_liaison_gauche_arbre.png");
		}
		else if(tileType == "01")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_liaison_droite_arbre.png");
		}
		else if(tileType == "02")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_bas_gauche.png");
		}
		else if(tileType == "03")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_bas_droite.png");
		}
		else if(tileType == "04")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_liaison_gauche_herbe.png");
		}
		else if(tileType == "05")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_liaison_droite_herbe.png");
		}
		else if(tileType == "06")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_haut_gauche.png");
		}
		else if(tileType == "07")
		{
		    tuiles[i] = Tuile(rendu, x, y, "arbre_haut_droite.png");
		}
		else if(tileType == "08")
		{
		    tuiles[i] = Tuile(rendu, x, y, "herbe.png");
		}
		else if(tileType == "09")
		{
		    tuiles[i] = Tuile(rendu, x, y, "fleur.png");
		}
		else if(tileType == "10")
		{
		    tuiles[i] = Tuile(rendu, x, y, "terre.png");
		}
		else if(tileType == "11")
		{
		    tuiles[i] = Tuile(rendu, x, y, "panneau.png");
		}

		//Move to next tile spot
		x += TILE_WIDTH;

		//If we've gone too far
		if(x >= LEVEL_WIDTH)
		{
			//Move back
			x = 0;

			//Move to the next row
			y += TILE_HEIGHT;
		}
	}

    //Close the file
    fichier.close();
}*/
