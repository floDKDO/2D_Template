#include "singleton_systeme.hpp"

#include <carte.hpp>


//initialise les librairies (SDL2, SDL_TTF, SDL_Mixer, SDL_IMG), créer la fenêtre et le rendu, met l'état du jeu en MENU_PRINCIPAL et active les entrées au clavier pour les inputfields
void SingletonSysteme::init(void)
{
    this->etat = MENU_PRINCIPAL;

    Uint32 flags;
    if(mode_fenetre == PLEIN_ECRAN)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    else if(mode_fenetre == FENETRE)
    {
        flags = SDL_WINDOW_RESIZABLE;
    }

    CHK(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER), SDL_GetError());
    CHK(TTF_Init(), TTF_GetError());
    CHK(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG), IMG_GetError());
    CHK(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG), Mix_GetError());
    CHK(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 1024), Mix_GetError());
    NCHK(this->fenetre = SDL_CreateWindow("Mon jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LONGUEUR_FENETRE, HAUTEUR_FENETRE, flags), SDL_GetError());
    NCHK(this->rendu = SDL_CreateRenderer(this->fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_GetError());
    CHK(SDL_SetRenderDrawBlendMode(this->rendu, SDL_BLENDMODE_BLEND), SDL_GetError()); //permettre la transparence
    CHK(SDL_RenderSetLogicalSize(this->rendu, 1280, 720), SDL_GetError()); //720!

    this->camera = {0, 0, LONGUEUR_FENETRE, HAUTEUR_FENETRE};

    /* Algo de modification de taille de fenetre
1. Partir d'une taille de résolution de base et donc d'une taille de fenetre de base (ex : 1280 * 720)
=> fenetre = SDL_CreateWindow("Mon jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LONGUEUR_FENETRE, 1280, 720)
=> SDL_RenderSetLogicalSize(rendu, 1280, 720)
2. Quand la fenetre sera redimensionnée (soit avec SDL_WINDOW_RESIZABLE en flag de SDL_CreateWindow, ou avec SDL_SetWindowSize()) :
   tous les éléments seront eux aussi redimensionnés en accord avec la résolution
Il faudra créer les images en se basant sur la taille de l'étape 1 et cela sera bon pour toutes les autres tailles avec cet algo
*/


    this->manette = SDL_GameControllerOpen(0);
    if(this->manette == nullptr)
        std::cout << "pas de manette" << std::endl;

    //activer les evenements manette
    CHK(SDL_GameControllerEventState(SDL_ENABLE), SDL_GetError());
    //pour les inputfield
    SDL_StartTextInput();


    this->cartes.insert(std::make_pair("toto.map", new Carte("toto.map", true)));
    this->cartes.insert(std::make_pair("tutu.map", new Carte("tutu.map", false)));
    this->cartes.insert(std::make_pair("titi.map", new Carte("titi.map", false)));

    this->cartes["toto.map"]->initJson("toto.json", this);
    this->cartes["tutu.map"]->initJson("tutu.json", this);
    this->cartes["titi.map"]->initJson("titi.json", this);

}

void SingletonSysteme::charger(void)
{
    this->fichier_sauvegarde.open("./fichier_sauvegarde.txt", std::ios::in); //ouvrir le fichier en lecture
    std::string ligne;
    unsigned int numero_ligne_effective = 0;
    if(this->fichier_sauvegarde.is_open() == true) //si le fichier est ouvert
    {
        if(this->fichier_sauvegarde.peek() != std::ifstream::traits_type::eof()) //si le fichier n'est pas vide
        {
            while(std::getline(this->fichier_sauvegarde, ligne))
            {
                if(ligne.find("//") != std::string::npos) //si la ligne contient un commentaire
                    continue; //passer à la ligne suivante

                numero_ligne_effective += 1;

                switch(numero_ligne_effective)
                {
                    case 1:
                        this->nom_joueur = ligne;
                        break;

                    case 2: //mode affichage de la fenetre
                        this->mode_fenetre = static_cast<enum mode_fenetre>(std::stoi(ligne));
                        break;

                    case 3: //musique activee ?
                        this->musique_activee = std::stoi(ligne);
                        break;

                    case 4: //son active ?
                        this->son_active = std::stoi(ligne);
                        break;

                    case 5: //touche deplacement haut clavier
                        if((this->touches.dep_haut = SDL_GetKeyFromName(ligne.c_str())) == SDLK_UNKNOWN)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 6: //touche deplacement bas clavier
                        if((this->touches.dep_bas = SDL_GetKeyFromName(ligne.c_str())) == SDLK_UNKNOWN)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 7: //touche deplacement gauche clavier
                        if((this->touches.dep_gauche = SDL_GetKeyFromName(ligne.c_str())) == SDLK_UNKNOWN)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 8: //touche deplacement droite clavier
                        if((this->touches.dep_droite = SDL_GetKeyFromName(ligne.c_str())) == SDLK_UNKNOWN)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 9: //touche deplacement haut manette
                        if((this->touches_1.dep_haut = SDL_GameControllerGetButtonFromString(ligne.c_str())) == SDL_CONTROLLER_BUTTON_INVALID)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 10: //touche deplacement bas manette
                        if((this->touches_1.dep_bas = SDL_GameControllerGetButtonFromString(ligne.c_str())) == SDL_CONTROLLER_BUTTON_INVALID)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 11: //touche deplacement gauche manette
                        if((this->touches_1.dep_gauche = SDL_GameControllerGetButtonFromString(ligne.c_str())) == SDL_CONTROLLER_BUTTON_INVALID)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;

                    case 12: //touche deplacement droite manette
                        if((this->touches_1.dep_droite = SDL_GameControllerGetButtonFromString(ligne.c_str())) == SDL_CONTROLLER_BUTTON_INVALID)
                        {
                            std::cerr << SDL_GetError() << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        break;


                    /*case 9: //position joueur en x
                        this->posX_joueur = std::stoi(ligne);
                        break;

                    case 10: //position joueur en y
                        this->posY_joueur = std::stoi(ligne);
                        break;*/

                    default:
                        break;
                }
            }
        }
        else //si le fichier est vide (en cas de premiere partie)
        {
            this->nom_joueur = "";
            this->mode_fenetre = FENETRE;
            this->son_active = true;
            this->musique_activee = true;

            this->touches.dep_haut = SDLK_UP;
            this->touches.dep_bas = SDLK_DOWN;
            this->touches.dep_gauche = SDLK_LEFT;
            this->touches.dep_droite = SDLK_RIGHT;

            this->touches_1.dep_haut = SDL_CONTROLLER_BUTTON_DPAD_UP;
            this->touches_1.dep_bas = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
            this->touches_1.dep_gauche = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
            this->touches_1.dep_droite = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;

            /*this->posX_joueur = 800;
            this->posY_joueur = 96;*/
        }
    }
    this->fichier_sauvegarde.close();
}


void SingletonSysteme::sauvegarder(void)
{
    fichier_sauvegarde.open("./fichier_sauvegarde.txt", std::ios::out | std::ios::trunc); //ouvrir le fichier en écriture et supprimer ce qu'il contient s'il n'est pas vide
    if(fichier_sauvegarde.is_open() == true)
    {
        fichier_sauvegarde << "//Nom joueur\n";
        fichier_sauvegarde << this->nom_joueur << "\n";
        fichier_sauvegarde << "//Mode fenetre\n";
        fichier_sauvegarde << this->mode_fenetre << "\n";
        fichier_sauvegarde << "//Musique activee ?\n";
        fichier_sauvegarde << this->musique_activee << "\n";
        fichier_sauvegarde << "//Son active ?\n";
        fichier_sauvegarde << this->son_active << "\n";

        fichier_sauvegarde << "//Touche déplacement haut\n";
        fichier_sauvegarde << SDL_GetKeyName(this->touches.dep_haut) << "\n";
        fichier_sauvegarde << "//Touche déplacement bas\n";
        fichier_sauvegarde << SDL_GetKeyName(this->touches.dep_bas) << "\n";
        fichier_sauvegarde << "//Touche déplacement gauche\n";
        fichier_sauvegarde << SDL_GetKeyName(this->touches.dep_gauche) << "\n";
        fichier_sauvegarde << "//Touche déplacement droite\n";
        fichier_sauvegarde << SDL_GetKeyName(this->touches.dep_droite) << "\n";

        fichier_sauvegarde << "//Touche déplacement haut\n";
        fichier_sauvegarde << SDL_GameControllerGetStringForButton(this->touches_1.dep_haut) << "\n";
        fichier_sauvegarde << "//Touche déplacement bas\n";
        fichier_sauvegarde << SDL_GameControllerGetStringForButton(this->touches_1.dep_bas) << "\n";
        fichier_sauvegarde << "//Touche déplacement gauche\n";
        fichier_sauvegarde << SDL_GameControllerGetStringForButton(this->touches_1.dep_gauche) << "\n";
        fichier_sauvegarde << "//Touche déplacement droite\n";
        fichier_sauvegarde << SDL_GameControllerGetStringForButton(this->touches_1.dep_droite) << "\n";

        /*fichier_sauvegarde << "//Position joueur en x\n";
        fichier_sauvegarde << this->posX_joueur << "\n";
        fichier_sauvegarde << "////Position joueur en y\n";
        fichier_sauvegarde << this->posY_joueur << "\n";*/
    }
    fichier_sauvegarde.close();
}

void SingletonSysteme::supprimmer(void)
{
    fichier_sauvegarde.open("./fichier_sauvegarde.txt", std::ios::out | std::ios::trunc); //ouvrir le fichier en écriture et supprimer ce qu'il contient s'il n'est pas vide
    this->nom_joueur = "";
    /*this->posX_joueur = 800;
    this->posY_joueur = 96;*/
    fichier_sauvegarde.close();
}


//nettoyage
void SingletonSysteme::destroy(void)
{
    SDL_StopTextInput();
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
