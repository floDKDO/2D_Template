#include "singleton_systeme.hpp"
#include "inputfield.hpp"
#include "bouton.hpp"
#include "texte.hpp"
#include "joueur.hpp"
#include "toggle.hpp"
#include "menuPrincipal.hpp"
#include "menuOptions.hpp"
#include "menuChoixNom.hpp"
#include "tuile.hpp"
#include "enJeu.hpp"

SDL_Texture* init_texture(std::string image, SDL_Renderer* rendu)
{
    SDL_Texture* texture = nullptr;
    NCHK(texture = IMG_LoadTexture(rendu, image.c_str()), IMG_GetError());
    return texture;
}

SDL_Rect init_rect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

//cette fonction permet de creer un rect qui est de la taille de la texture, donc de l'image
SDL_Rect init_rect_from_image(int x, int y, SDL_Texture* texture)
{
    int w, h;
    CHK(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h), SDL_GetError());
    SDL_Rect rect = init_rect(x, y, w, h);
    return rect;
}


/* Algo de modification de taille de fenetre
1. Partir d'une taille de résolution de base et donc d'une taille de fenetre de base (ex : 1280 * 720)
=> fenetre = SDL_CreateWindow("Mon jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LONGUEUR_FENETRE, 1280, 720)
=> SDL_RenderSetLogicalSize(rendu, 1280, 720)
2. Quand la fenetre sera redimensionnée (soit avec SDL_WINDOW_RESIZABLE en flag de SDL_CreateWindow, ou avec SDL_SetWindowSize()) :
   tous les éléments seront eux aussi redimensionnés en accord avec la résolution
Il faudra créer les images en se basant sur la taille de l'étape 1 et cela sera bon pour toutes les autres tailles avec cet algo
*/


int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    SingletonSysteme::instance().charger();
    SingletonSysteme::instance().init();

    MenuPrincipal menuPrincipal(&SingletonSysteme::instance());
    MenuOptions menuOptions(&SingletonSysteme::instance());
    MenuChoixNom menuChoixNom(&SingletonSysteme::instance());
    EnJeu enJeu(&SingletonSysteme::instance());

    Uint32 timeStep = SDL_GetTicks();

    bool quitter = false;
    while(quitter == false)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_WINDOWEVENT:

                    if(e.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        SingletonSysteme::instance().sauvegarder();
                        quitter = true;
                    }
                    break;

                case SDL_KEYDOWN:

                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        SingletonSysteme::instance().sauvegarder();
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }

            if(SingletonSysteme::instance().etat == MENU_PRINCIPAL)
            {
                menuPrincipal.handleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == MENU_OPTIONS)
            {
                menuOptions.handleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == DEMANDE_NOM)
            {
                menuChoixNom.handleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == EN_JEU)
            {
                enJeu.handleEvents(e, &SingletonSysteme::instance());
            }
        }

        //std::cout << "camera : " << SingletonSysteme::instance().camera.x << std::endl;

        CHK(SDL_SetRenderDrawColor(SingletonSysteme::instance().rendu, 0, 0, 0, 255), SDL_GetError());
        CHK(SDL_RenderClear(SingletonSysteme::instance().rendu), SDL_GetError());

        if(SingletonSysteme::instance().etat == MENU_PRINCIPAL)
        {
            menuPrincipal.draw(&SingletonSysteme::instance());
            menuPrincipal.update(timeStep);
        }
        else if(SingletonSysteme::instance().etat == MENU_OPTIONS)
        {
            menuOptions.draw(&SingletonSysteme::instance());
        }
        else if(SingletonSysteme::instance().etat == DEMANDE_NOM)
        {
            menuChoixNom.draw(&SingletonSysteme::instance());
            menuChoixNom.update(timeStep);
        }
        else if(SingletonSysteme::instance().etat == EN_JEU)
        {
            enJeu.draw(&SingletonSysteme::instance());
            enJeu.update(timeStep, &SingletonSysteme::instance());
        }

        SDL_RenderPresent(SingletonSysteme::instance().rendu);
    }
    SingletonSysteme::instance().sauvegarder();
    SingletonSysteme::instance().destroy();
    return EXIT_SUCCESS;
}
