#include "singleton_systeme.hpp"


//initialise les librairies (SDL2, SDL_TTF, SDL_Mixer, SDL_IMG), créer la fenêtre et le rendu, met l'état du jeu en MENU_PRINCIPAL et active les entrées au clavier pour les inputfields
void SingletonSysteme::Init(void)
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

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() < 0)
    {
        std::cerr << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0)
    {
        std::cerr << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_CHANNELS, 1024) < 0)
    {
        std::cerr << Mix_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if((this->fenetre = SDL_CreateWindow("Mon jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LONGUEUR_FENETRE, HAUTEUR_FENETRE, flags)) == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if((this->rendu = SDL_CreateRenderer(this->fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(SDL_RenderSetLogicalSize(this->rendu, 1280, 720) < 0) //720!
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    //si dans menu où il y a l'inputfield, alors appeler cette fonction
    SDL_StartTextInput();


    this->manette = SDL_GameControllerOpen(0);
    if(this->manette == nullptr)
        std::cout << "pas de manette";

    //activer les evenements manette
    if(SDL_GameControllerEventState(SDL_ENABLE) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SingletonSysteme::Charger(void)
{
    fichier_sauvegarde.open("./fichier_sauvegarde.txt", std::ios::in); //ouvrir le fichier en lecture
    std::string ligne;
    unsigned int numero_ligne_effective = 0;
    if(fichier_sauvegarde.is_open() == true) //si le fichier est ouvert
    {
        if(fichier_sauvegarde.peek() != std::ifstream::traits_type::eof()) //si le fichier n'est pas vide
        {
            while(std::getline(fichier_sauvegarde, ligne))
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
    fichier_sauvegarde.close();
}


void SingletonSysteme::Sauvegarder(void)
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

void SingletonSysteme::Supprimmer(void)
{
    fichier_sauvegarde.open("./fichier_sauvegarde.txt", std::ios::out | std::ios::trunc); //ouvrir le fichier en écriture et supprimer ce qu'il contient s'il n'est pas vide
    this->nom_joueur = "";
    /*this->posX_joueur = 800;
    this->posY_joueur = 96;*/
    fichier_sauvegarde.close();
}


//nettoyage
void SingletonSysteme::Destroy(void)
{
    SDL_StopTextInput();
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
