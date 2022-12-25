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
#include "toggle.hpp"

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

//cette fonction se lance lorsque le bouton de changement de mode d'affichage est enclenché
void reglage_fenetre(SingletonSysteme* sing_syst)
{
    Uint32 mask_window = SDL_GetWindowFlags(sing_syst->fenetre);
    if(mask_window & SDL_WINDOW_FULLSCREEN) //si la fenetre est en plein ecran
    {
        ecran_fenetre(sing_syst->fenetre);
        sing_syst->mode_fenetre = FENETRE;
    }
    else //si la fenetre est en mode fenetre
    {
        plein_ecran(sing_syst->fenetre);
        sing_syst->mode_fenetre = PLEIN_ECRAN;
    }
}

/*void redimensionner_fenetre(SDL_Window* fenetre, int x, int y)
{
    SDL_SetWindowSize(fenetre, x, y);
    SDL_SetWindowPosition(fenetre, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}*/


void fonc_bouton_nouvelle_partie(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click nouvelle partie" << std::endl;
    if(sing_syst->nom_joueur.empty() == false)
    {
        sing_syst->Supprimmer();
    }
    sing_syst->etat = DEMANDE_NOM;
}

void fonc_bouton_continuer(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click jouer" << std::endl;
    sing_syst->etat = EN_JEU;
}

void fonc_bouton_fin_demande_nom(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click jouer" << std::endl;
    if(sing_syst->nom_joueur.length() > 0)
    {
        sing_syst->etat = EN_JEU;
    }
}

void fonc_inputfield_nom_joueur(SingletonSysteme* sing_syst, Inputfield* inputfield)
{
    sing_syst->nom_joueur = inputfield->texte.texte;
}

void fonc_bouton_options(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click options" << std::endl;
    sing_syst->etat = MENU_OPTIONS;
}

void fonc_bouton_options_retour(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click options retour" << std::endl;
    sing_syst->etat = MENU_PRINCIPAL;
}

void fonc_bouton_options_fenetre(SingletonSysteme* sing_syst, Bouton* bouton)
{
    std::cout << "click options fenetre" << std::endl;
    reglage_fenetre(sing_syst); //=> passer le parametre
    if(sing_syst->mode_fenetre == FENETRE)
    {
        bouton->texte.texte = "FENETRE";
    }
    else if(sing_syst->mode_fenetre == PLEIN_ECRAN)
    {
        bouton->texte.texte = "PLEIN-ECRAN";
    }
}

void fonc_bouton_quitter(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click quitter" << std::endl;
    sing_syst->Sauvegarder();
    sing_syst->Destroy(); //il faut tout clean...
    exit(EXIT_SUCCESS);
}

void fonc_choix_touche_haut(SingletonSysteme* sing_syst, Bouton* bouton)
{
    bool quitter = false;
    SDL_Texture* texture = init_texture("./img/pop_up_touche.png", sing_syst->rendu);
    SDL_Rect dest = init_rect_from_image(300, 300, texture);
    while(quitter == false)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:

                    if(e.key.keysym.sym != sing_syst->touches.dep_haut
                    && e.key.keysym.sym != sing_syst->touches.dep_bas
                    && e.key.keysym.sym != sing_syst->touches.dep_gauche
                    && e.key.keysym.sym != sing_syst->touches.dep_droite)
                    {
                        if((bouton->texte.texte = SDL_GetKeyName(e.key.keysym.sym)).empty() == true)
                        {
                            std::cerr << "No name" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        sing_syst->touches.dep_haut = e.key.keysym.sym;
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }
        }
        if(SDL_RenderCopy(sing_syst->rendu, texture, nullptr, &dest) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_RenderPresent(sing_syst->rendu);
    }
    SDL_DestroyTexture(texture);
}

void fonc_choix_touche_bas(SingletonSysteme* sing_syst, Bouton* bouton)
{
    bool quitter = false;
    SDL_Texture* texture = init_texture("./img/pop_up_touche.png", sing_syst->rendu);
    SDL_Rect dest = init_rect_from_image(300, 300, texture);
    while(quitter == false)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:

                    if(e.key.keysym.sym != sing_syst->touches.dep_haut
                    && e.key.keysym.sym != sing_syst->touches.dep_bas
                    && e.key.keysym.sym != sing_syst->touches.dep_gauche
                    && e.key.keysym.sym != sing_syst->touches.dep_droite)
                    {
                        if((bouton->texte.texte = SDL_GetKeyName(e.key.keysym.sym)).empty() == true)
                        {
                            std::cerr << "No name" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        sing_syst->touches.dep_bas = e.key.keysym.sym;
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }
        }
        if(SDL_RenderCopy(sing_syst->rendu, texture, nullptr, &dest) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_RenderPresent(sing_syst->rendu);
    }
    SDL_DestroyTexture(texture);
}

void fonc_choix_touche_gauche(SingletonSysteme* sing_syst, Bouton* bouton)
{
    bool quitter = false;
    SDL_Texture* texture = init_texture("./img/pop_up_touche.png", sing_syst->rendu);
    SDL_Rect dest = init_rect_from_image(300, 300, texture);
    while(quitter == false)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:

                    if(e.key.keysym.sym != sing_syst->touches.dep_haut
                    && e.key.keysym.sym != sing_syst->touches.dep_bas
                    && e.key.keysym.sym != sing_syst->touches.dep_gauche
                    && e.key.keysym.sym != sing_syst->touches.dep_droite)
                    {
                        if((bouton->texte.texte = SDL_GetKeyName(e.key.keysym.sym)).empty() == true)
                        {
                            std::cerr << "No name" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        sing_syst->touches.dep_gauche = e.key.keysym.sym;
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }
        }
        if(SDL_RenderCopy(sing_syst->rendu, texture, nullptr, &dest) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_RenderPresent(sing_syst->rendu);
    }
    SDL_DestroyTexture(texture);
}

void fonc_choix_touche_droite(SingletonSysteme* sing_syst, Bouton* bouton)
{
    bool quitter = false;
    SDL_Texture* texture = init_texture("./img/pop_up_touche.png", sing_syst->rendu);
    SDL_Rect dest = init_rect_from_image(300, 300, texture);
    while(quitter == false)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_KEYDOWN:

                    if(e.key.keysym.sym != sing_syst->touches.dep_haut
                    && e.key.keysym.sym != sing_syst->touches.dep_bas
                    && e.key.keysym.sym != sing_syst->touches.dep_gauche
                    && e.key.keysym.sym != sing_syst->touches.dep_droite)
                    {
                        if((bouton->texte.texte = SDL_GetKeyName(e.key.keysym.sym)).empty() == true)
                        {
                            std::cerr << "No name" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        sing_syst->touches.dep_droite = e.key.keysym.sym;
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }
        }
        if(SDL_RenderCopy(sing_syst->rendu, texture, nullptr, &dest) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        SDL_RenderPresent(sing_syst->rendu);
    }
    SDL_DestroyTexture(texture);
}


void fonc_toggle_son(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click toggle son" << std::endl;
    sing_syst->son_active = !(sing_syst->son_active);
}

void fonc_toggle_musique(SingletonSysteme* sing_syst, Bouton* bouton)
{
    (void)bouton;
    std::cout << "click toggle musique" << std::endl;
    sing_syst->musique_activee = !(sing_syst->musique_activee);
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

    const SDL_Color ROUGE = {255, 0, 0, 255};
    const SDL_Color VERT = {0, 255, 0, 255};
    const SDL_Color BLEU = {0, 0, 255, 255};
    const SDL_Color BLANC = {255, 255, 255, 255};
    const SDL_Color GRIS = {127, 127, 127, 255};
    //const SDL_Color NOIR = {0, 0, 0, 255};

    SingletonSysteme::instance().Charger();
    SingletonSysteme::instance().Init();

    std::string mode;
    if(SingletonSysteme::instance().mode_fenetre == PLEIN_ECRAN)
        mode = "PLEIN ECRAN";
    else if(SingletonSysteme::instance().mode_fenetre == FENETRE)
        mode = "FENETRE";

    // MENU PRINCIPAL /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Bouton bouton_continuer(ROUGE, VERT, BLEU, BLANC, {550, 225, 200, 100}, &fonc_bouton_continuer, "Continuer", SingletonSysteme::instance().rendu);
    Texte texte_nom_joueur(SingletonSysteme::instance().nom_joueur, "./font/lazy.ttf", BLANC, {300, 225, 200, 100}, SingletonSysteme::instance().rendu);
    Texte titre("Titre du jeu", "./font/lazy.ttf", BLANC, {400, 0, 500, 200}, SingletonSysteme::instance().rendu);
    Bouton bouton_nouvelle_partie(ROUGE, VERT, BLEU, BLANC, {550, 350, 200, 100}, &fonc_bouton_nouvelle_partie, "NOUVELLE PARTIE", SingletonSysteme::instance().rendu);
    Bouton bouton_options(ROUGE, VERT, BLEU, BLANC, {550, 475, 200, 100}, &fonc_bouton_options, "OPTIONS", SingletonSysteme::instance().rendu);
    Bouton bouton_quitter(ROUGE, VERT, BLEU, BLANC, {550, 600, 200, 100}, &fonc_bouton_quitter, "QUITTER", SingletonSysteme::instance().rendu);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // MENU OPTIONS ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Bouton bouton_options_retour(ROUGE, VERT, BLEU, BLANC, {100, 600, 200, 100}, &fonc_bouton_options_retour, "RETOUR", SingletonSysteme::instance().rendu);
    Texte mode_ecran("MODE ECRAN", "./font/lazy.ttf", BLANC, {1000, 500, 200, 100}, SingletonSysteme::instance().rendu);
    Bouton bouton_options_fenetre(ROUGE, VERT, BLEU, BLANC, {1000, 600, 200, 100}, &fonc_bouton_options_fenetre, mode, SingletonSysteme::instance().rendu);

    Bouton bouton_options_touche_haut(ROUGE, VERT, BLEU, BLANC, {600, 150, 200, 100}, &fonc_choix_touche_haut, SDL_GetKeyName(SingletonSysteme::instance().touches.dep_haut), SingletonSysteme::instance().rendu);
    Bouton bouton_options_touche_bas(ROUGE, VERT, BLEU, BLANC, {600, 300, 200, 100}, &fonc_choix_touche_bas, SDL_GetKeyName(SingletonSysteme::instance().touches.dep_bas), SingletonSysteme::instance().rendu);
    Bouton bouton_options_touche_gauche(ROUGE, VERT, BLEU, BLANC, {600, 450, 200, 100}, &fonc_choix_touche_gauche, SDL_GetKeyName(SingletonSysteme::instance().touches.dep_gauche), SingletonSysteme::instance().rendu);
    Bouton bouton_options_touche_droite(ROUGE, VERT, BLEU, BLANC, {600, 600, 200, 100}, &fonc_choix_touche_droite, SDL_GetKeyName(SingletonSysteme::instance().touches.dep_droite), SingletonSysteme::instance().rendu);
    Texte texte_touche_haut("Haut", "./font/lazy.ttf", BLANC, {400, 150, 150, 100}, SingletonSysteme::instance().rendu);
    Texte texte_touche_bas("Bas", "./font/lazy.ttf", BLANC, {400, 300, 150, 100}, SingletonSysteme::instance().rendu);
    Texte texte_touche_gauche("Gauche", "./font/lazy.ttf", BLANC, {400, 450, 150, 100}, SingletonSysteme::instance().rendu);
    Texte texte_touche_droite("Droite", "./font/lazy.ttf", BLANC, {400, 600, 150, 100}, SingletonSysteme::instance().rendu);

    Toggle toggle_sound(BLANC, GRIS, BLANC, BLANC, {100, 100, 50, 50}, "SON", &fonc_toggle_son, SingletonSysteme::instance().son_active, SingletonSysteme::instance().rendu);
    Toggle toggle_musique(BLANC, GRIS, BLANC, BLANC, {1000, 100, 50, 50}, "MUSIQUE", &fonc_toggle_musique, SingletonSysteme::instance().musique_activee, SingletonSysteme::instance().rendu);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // MENU CHOIX DU NOM //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Inputfield inputfield("./font/lazy.ttf", ROUGE, {300, 400, 200, 50}, &fonc_inputfield_nom_joueur, SingletonSysteme::instance().rendu);
    Texte demande_nom("Ecrivez votre nom", "./font/lazy.ttf", BLANC, {300, 200, 300, 150}, SingletonSysteme::instance().rendu);
    Bouton bouton_valider(ROUGE, VERT, BLEU, BLANC, {300, 550, 200, 100}, &fonc_bouton_fin_demande_nom, "VALIDER", SingletonSysteme::instance().rendu);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // EN JEU ////////////////////////////////////////////////////////////////////////////////
    Joueur joueur(100, BLEU, {160, 160, 32, 32}, VUE_DESSUS);
    SDL_Texture* texture = init_texture("./img/fond.png", SingletonSysteme::instance().rendu);
    SDL_Rect dest = init_rect_from_image(0, 0, texture);
    //////////////////////////////////////////////////////////////////////////////////////////

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
                        SingletonSysteme::instance().Sauvegarder();
                        quitter = true;
                    }
                    break;

                case SDL_KEYDOWN:

                    if(e.key.keysym.sym == SDLK_ESCAPE)
                    {
                        SingletonSysteme::instance().Sauvegarder();
                        quitter = true;
                    }
                    break;

                default :
                    break;
            }

            if(SingletonSysteme::instance().etat == MENU_PRINCIPAL)
            {
                if(SingletonSysteme::instance().nom_joueur.empty() == false)
                    bouton_continuer.HandleEvents(e, &SingletonSysteme::instance());
                bouton_nouvelle_partie.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options.HandleEvents(e, &SingletonSysteme::instance());
                bouton_quitter.HandleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == MENU_OPTIONS)
            {
                bouton_options_retour.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_fenetre.HandleEvents(e, &SingletonSysteme::instance());
                toggle_sound.HandleEvents(e, &SingletonSysteme::instance());
                toggle_musique.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_touche_haut.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_touche_bas.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_touche_gauche.HandleEvents(e, &SingletonSysteme::instance());
                bouton_options_touche_droite.HandleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == DEMANDE_NOM)
            {
                bouton_valider.HandleEvents(e, &SingletonSysteme::instance());
                inputfield.HandleEvents(e, &SingletonSysteme::instance());
            }
            else if(SingletonSysteme::instance().etat == EN_JEU)
            {
                joueur.HandleEvents(e, &SingletonSysteme::instance());
            }
        }

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
            if(SingletonSysteme::instance().nom_joueur.empty() == false)
            {
                bouton_continuer.Draw(SingletonSysteme::instance().rendu);
                texte_nom_joueur.Draw(SingletonSysteme::instance().rendu);
            }
            bouton_nouvelle_partie.Draw(SingletonSysteme::instance().rendu);
            bouton_options.Draw(SingletonSysteme::instance().rendu);
            bouton_quitter.Draw(SingletonSysteme::instance().rendu);
            titre.Draw(SingletonSysteme::instance().rendu);
        }
        else if(SingletonSysteme::instance().etat == MENU_OPTIONS)
        {
            bouton_options_retour.Draw(SingletonSysteme::instance().rendu);
            mode_ecran.Draw(SingletonSysteme::instance().rendu);
            bouton_options_fenetre.Draw(SingletonSysteme::instance().rendu);
            toggle_sound.Draw(SingletonSysteme::instance().rendu);
            toggle_musique.Draw(SingletonSysteme::instance().rendu);

            bouton_options_touche_haut.Draw(SingletonSysteme::instance().rendu);
            bouton_options_touche_bas.Draw(SingletonSysteme::instance().rendu);
            bouton_options_touche_gauche.Draw(SingletonSysteme::instance().rendu);
            bouton_options_touche_droite.Draw(SingletonSysteme::instance().rendu);
            texte_touche_haut.Draw(SingletonSysteme::instance().rendu);
            texte_touche_bas.Draw(SingletonSysteme::instance().rendu);
            texte_touche_gauche.Draw(SingletonSysteme::instance().rendu);
            texte_touche_droite.Draw(SingletonSysteme::instance().rendu);
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
    SDL_DestroyTexture(texture);
    SingletonSysteme::instance().Sauvegarder();
    SingletonSysteme::instance().Destroy();
    return EXIT_SUCCESS;
}
