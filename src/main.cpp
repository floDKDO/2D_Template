#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "singleton_systeme.hpp"
#include "inputfield.hpp"
#include "bouton.hpp"
#include "texte.hpp"
#include "joueur.hpp"

//TODO pas de variables globales
std::string nom_joueur;
bool donner_nom = false;
////////////////////////////////

SDL_Texture* init_texture(std::string image, SDL_Renderer* rendu)
{
    SDL_Texture* texture = nullptr;
    if((texture = IMG_LoadTexture(rendu, image.c_str())) == nullptr)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
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
    if(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_Rect rect = init_rect(x, y, w, h);
    return rect;
}


void plein_ecran(SDL_Window* fenetre)
{
    if(SDL_SetWindowFullscreen(fenetre, SDL_WINDOW_FULLSCREEN) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void ecran_fenetre(SDL_Window* fenetre)
{
    if(SDL_SetWindowFullscreen(fenetre, 0) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void redimensionner_fenetre(SDL_Window* fenetre, int x, int y)
{
    SDL_SetWindowSize(fenetre, x, y);
    SDL_SetWindowPosition(fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


void fonc_bouton_jouer(SingletonSysteme* sing_syst)
{
    std::cout << "click jouer" << std::endl;
    sing_syst->etat = DEMANDE_NOM;
}

void fonc_bouton_fin_demande_nom(SingletonSysteme* sing_syst)
{
    std::cout << "click jouer" << std::endl;
    if(nom_joueur.length() > 0)
    {
        donner_nom = true;
        sing_syst->etat = EN_JEU;
    }
}

void fonc_bouton_options(SingletonSysteme* sing_syst)
{
    std::cout << "click options" << std::endl;
    sing_syst->etat = MENU_OPTIONS;
}

void fonc_bouton_options_retour(SingletonSysteme* sing_syst)
{
    std::cout << "click options retour" << std::endl;
    sing_syst->etat = MENU_PRINCIPAL;
}

void fonc_bouton_options_plein_ecran(SingletonSysteme* sing_syst)
{
    std::cout << "click options plein ecran" << std::endl;
    plein_ecran(sing_syst->fenetre); //=> passer le parametre
}

void fonc_bouton_options_mode_fenetre(SingletonSysteme* sing_syst)
{
    std::cout << "click options mode fenetre" << std::endl;
    ecran_fenetre(sing_syst->fenetre); //=> passer le parametre
}

void fonc_bouton_quitter(SingletonSysteme* sing_syst)
{
    std::cout << "click quitter" << std::endl;
    sing_syst->Destroy(); //il faut tout clean...
    exit(EXIT_SUCCESS);
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

    SingletonSysteme::instance().Init();

    Bouton bouton_jouer({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {800, 100, 200, 100}, &fonc_bouton_jouer, "JOUER");
    Bouton bouton_options({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {800, 250, 200, 100}, &fonc_bouton_options, "OPTIONS");
    Bouton bouton_quitter({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {800, 400, 200, 100}, &fonc_bouton_quitter, "QUITTER");

    Bouton bouton_options_retour({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {300, 250, 200, 100}, &fonc_bouton_options_retour, "RETOUR");
    Bouton bouton_options_plein_ecran({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {300, 400, 200, 100}, &fonc_bouton_options_plein_ecran, "PLEIN ECRAN");
    Bouton bouton_options_mode_fenetre({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {300, 550, 200, 100}, &fonc_bouton_options_mode_fenetre, "MODE FENETRE");

    Inputfield inputfield("./font/lazy.ttf", {255, 0, 0, 255}, {300, 400, 200, 50});
    Texte demande_nom("Ecrivez votre nom", "./font/lazy.ttf", {255, 255, 255, 255}, {300, 200, 300, 150});
    Bouton bouton_valider({255, 0, 0, 255}, {0, 255, 0, 255}, {0, 0, 255, 255}, {300, 550, 200, 100}, &fonc_bouton_fin_demande_nom, "VALIDER");

    Texte titre("Titre du jeu", "./font/lazy.ttf", {255, 255, 255, 255}, {300, 200, 300, 150});

    Joueur joueur(100, {0, 0, 255, 255}, {800, 96, 32, 32}, VUE_DESSUS);

    SDL_Texture* texture = init_texture("./img/fond.png", SingletonSysteme::instance().rendu);
    SDL_Rect dest = init_rect_from_image(0, 0, texture);

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
                        quitter = true;
                    break;

                case SDL_KEYDOWN:

                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        quitter = true;
                    break;

                default :
                    break;
            }

            if(SingletonSysteme::instance().etat == MENU_PRINCIPAL)
            {
                bouton_jouer.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options.HandleEvents(e, &SingletonSysteme::instance());
                bouton_quitter.HandleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == MENU_OPTIONS)
            {
                bouton_options_retour.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_plein_ecran.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_mode_fenetre.HandleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == DEMANDE_NOM)
            {
                bouton_valider.HandleEvents(e, &SingletonSysteme::instance());
                inputfield.HandleEvents(e);
            }
            else if(SingletonSysteme::instance().etat == EN_JEU)
            {
                joueur.HandleEvents(e);
            }
        }

        // A ameliorer/////////////////
        nom_joueur = inputfield.texte;
        if(donner_nom == true)
        {
            joueur.nom = nom_joueur;
            donner_nom = false;
        }
        ///////////////////////////////

        //fond
        if(SDL_SetRenderDrawColor(SingletonSysteme::instance().rendu, 0, 0, 0, 255) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        if(SDL_RenderClear(SingletonSysteme::instance().rendu) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        if(SingletonSysteme::instance().etat == MENU_PRINCIPAL)
        {
            bouton_jouer.Draw(SingletonSysteme::instance().rendu);
            bouton_options.Draw(SingletonSysteme::instance().rendu);
            bouton_quitter.Draw(SingletonSysteme::instance().rendu);
            titre.Draw(SingletonSysteme::instance().rendu);
        }
        else if(SingletonSysteme::instance().etat == MENU_OPTIONS)
        {
            bouton_options_retour.Draw(SingletonSysteme::instance().rendu);
            bouton_options_plein_ecran.Draw(SingletonSysteme::instance().rendu);
            bouton_options_mode_fenetre.Draw(SingletonSysteme::instance().rendu);
        }
        else if(SingletonSysteme::instance().etat == DEMANDE_NOM)
        {
            bouton_valider.Draw(SingletonSysteme::instance().rendu);
            demande_nom.Draw(SingletonSysteme::instance().rendu);
            inputfield.Draw(SingletonSysteme::instance().rendu);
        }
        else if(SingletonSysteme::instance().etat == EN_JEU)
        {
            if(SDL_RenderCopy(SingletonSysteme::instance().rendu, texture, nullptr, &dest) < 0)
            {
                std::cerr << SDL_GetError() << std::endl;
                exit(EXIT_FAILURE);
            }
            joueur.Draw(SingletonSysteme::instance().rendu);
            joueur.Update(timeStep);
        }

        SDL_RenderPresent(SingletonSysteme::instance().rendu);
    }
    SingletonSysteme::instance().Destroy();
    return EXIT_SUCCESS;
}
