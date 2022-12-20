#include "singleton_systeme.hpp"


//initialise les librairies (SDL2, SDL_TTF, SDL_Mixer, SDL_IMG), créer la fenêtre et le rendu, met l'état du jeu en MENU_PRINCIPAL et active les entrées au clavier pour les inputfields
void SingletonSysteme::Init(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
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

    if((this->fenetre = SDL_CreateWindow("Mon jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LONGUEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_RESIZABLE)) == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if((this->rendu = SDL_CreateRenderer(this->fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) == nullptr)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    if(SDL_RenderSetLogicalSize(this->rendu, 1280, 704) < 0) //720!
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    //si dans menu où il y a l'inputfield, alors appeler cette fonction
    SDL_StartTextInput();

    this->etat = MENU_PRINCIPAL;
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
