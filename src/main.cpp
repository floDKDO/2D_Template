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


int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    SingletonSysteme::instance().charger();
    SingletonSysteme::instance().init();

    SDL_Renderer* rendu = SingletonSysteme::instance().rendu;
    SingletonSysteme* sing_syst = &SingletonSysteme::instance();

    MenuPrincipal menuPrincipal(sing_syst);
    MenuOptions menuOptions(sing_syst);
    MenuChoixNom menuChoixNom(sing_syst);
    EnJeu enJeu(sing_syst);

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
                        sing_syst->sauvegarder();
                        quitter = true;
                    }
                    break;

                case SDL_KEYDOWN:

                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        sing_syst->sauvegarder();
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }

            if(sing_syst->etat == MENU_PRINCIPAL)
            {
                menuPrincipal.handleEvents(e, sing_syst);
            }
            else if(sing_syst->etat == MENU_OPTIONS)
            {
                menuOptions.handleEvents(e, sing_syst);
            }
            else if(sing_syst->etat == DEMANDE_NOM)
            {
                menuChoixNom.handleEvents(e, sing_syst);
            }
            else if(sing_syst->etat == EN_JEU)
            {
                enJeu.handleEvents(e, sing_syst);
            }
        }

        CHK(SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255), SDL_GetError());
        CHK(SDL_RenderClear(rendu), SDL_GetError());

        if(sing_syst->etat == MENU_PRINCIPAL)
        {
            menuPrincipal.draw(rendu, sing_syst);
            menuPrincipal.update(timeStep);
        }
        else if(sing_syst->etat == MENU_OPTIONS)
        {
            menuOptions.draw(rendu, sing_syst);
        }
        else if(sing_syst->etat == DEMANDE_NOM)
        {
            menuChoixNom.draw(rendu, sing_syst);
            menuChoixNom.update(timeStep);
        }
        else if(sing_syst->etat == EN_JEU)
        {
            enJeu.draw(rendu, sing_syst->camera);
            enJeu.update(timeStep, sing_syst);
        }

        SDL_RenderPresent(rendu);
    }
    sing_syst->sauvegarder();
    sing_syst->destroy();
    return EXIT_SUCCESS;
}
