#include "menuPrincipal.hpp"

MenuPrincipal::MenuPrincipal(SingletonSysteme* sing_syst)
:bouton_continuer(ROUGE, VERT, BLEU, GRIS, {550, 225, 200, 100}, &fonc_bouton_continuer, "Continuer", sing_syst->rendu, "Bouton continuer")
,texte_nom_joueur(sing_syst->nom_joueur, "./font/lazy.ttf", BLANC, {300, 225, 200, 100}, sing_syst->rendu)
,titre("Titre du jeu", "./font/lazy.ttf", BLANC, {400, 0, 500, 200}, sing_syst->rendu)
,bouton_nouvelle_partie(ROUGE, VERT, BLEU, GRIS, {550, 350, 200, 100}, &fonc_bouton_nouvelle_partie, "NOUVELLE PARTIE", sing_syst->rendu, "Bouton nouvelle partie")
,bouton_options(ROUGE, VERT, BLEU, GRIS, {550, 475, 200, 100}, &fonc_bouton_options, "OPTIONS", sing_syst->rendu, "Bouton options")
,bouton_quitter(ROUGE, VERT, BLEU, GRIS, {550, 600, 200, 100}, &fonc_bouton_quitter, "QUITTER", sing_syst->rendu, "Bouton quitter")
{
    bouton_options.setSelected(&bouton_options);
    bouton_options.setSelectedIfMove(&bouton_nouvelle_partie, &bouton_quitter, nullptr, nullptr);

    bouton_continuer.setSelectedIfMove(nullptr, &bouton_nouvelle_partie, nullptr, nullptr);

    if(SingletonSysteme::instance().nom_joueur.empty() == true)
        bouton_nouvelle_partie.setSelectedIfMove(nullptr, &bouton_options, nullptr, nullptr);
    else bouton_nouvelle_partie.setSelectedIfMove(&bouton_continuer, &bouton_options, nullptr, nullptr);

    bouton_quitter.setSelectedIfMove(&bouton_options, nullptr, nullptr, nullptr);

    this->ui.push_back(&(this->bouton_continuer));
    this->ui.push_back(&(this->bouton_nouvelle_partie));
    this->ui.push_back(&(this->bouton_options));
    this->ui.push_back(&(this->bouton_quitter));
}


void MenuPrincipal::resetSelected(void)
{
    for(Selectionnable* s : ui)
    {
        s->etat = NORMAL;
    }
}

Selectionnable* MenuPrincipal::getSelected(void)
{
    Selectionnable* ui_selected;
    for(Selectionnable* s : ui)
    {
        if(s->etat == SELECTED)
        {
            ui_selected = s;
            break;
        }
    }
    return ui_selected;
}

void MenuPrincipal::Draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst)
{
    if(sing_syst->nom_joueur.empty() == false)
    {
        bouton_continuer.Draw(rendu);
        texte_nom_joueur.Draw(rendu);
    }

    bouton_nouvelle_partie.Draw(rendu);
    bouton_options.Draw(rendu);
    bouton_quitter.Draw(rendu);
    this->titre.Draw(rendu);
}


void MenuPrincipal::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    for(Selectionnable* s0 : ui)
    {
        Bouton* s = dynamic_cast<Bouton*>(s0);
        if(s->tag == "Bouton continuer")
        {
            if(sing_syst->nom_joueur.empty() == false)
                bouton_continuer.HandleEvents(e, sing_syst);
        }
        else
        {
            int x, y; //position x et y de la souris
            SDL_GetMouseState(&x, &y);

            if(e.type == SDL_KEYDOWN)
            {
                s->onKeyPressed(e, sing_syst);
            }
            else if(e.type == SDL_KEYUP)
            {
                s->onKeyReleased(e, sing_syst);
            }
            else if(e.type == SDL_MOUSEMOTION)
            {
                if(s->collision(s->position, x, y) == true)
                {
                    this->resetSelected(); //seul ajout
                    s->onPointerEnter(e, sing_syst);
                }
                else
                {
                    s->onPointerExit(e, sing_syst);
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(e.button.button == SDL_BUTTON_LEFT)
                {
                    s->onPointerDown(e, sing_syst);
                }
            }
            else if(e.type == SDL_MOUSEBUTTONUP)
            {
                if(e.button.button == SDL_BUTTON_LEFT)
                {
                   s->onClick(e, sing_syst);
                }
            }
        }
    }
}


void MenuPrincipal::fonc_bouton_quitter(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click quitter" << std::endl;
    sing_syst->Sauvegarder();
    sing_syst->Destroy(); //il faut tout clean...
    exit(EXIT_SUCCESS);
}

void MenuPrincipal::fonc_bouton_options(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click options" << std::endl;
    sing_syst->etat = MENU_OPTIONS;
}

void MenuPrincipal::fonc_bouton_nouvelle_partie(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click nouvelle partie" << std::endl;
    if(sing_syst->nom_joueur.empty() == false)
    {
        sing_syst->Supprimmer();
    }
    sing_syst->etat = DEMANDE_NOM;
}

void MenuPrincipal::fonc_bouton_continuer(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click jouer" << std::endl;
    sing_syst->etat = EN_JEU;
}