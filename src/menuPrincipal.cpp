#include "menuPrincipal.hpp"

MenuPrincipal::MenuPrincipal(SingletonSysteme* sing_syst)
:bouton_continuer(ROUGE, VERT, BLEU, GRIS, {550, 225, 200, 100}, &fonc_bouton_continuer, "Continuer", 30, sing_syst->rendu, "Bouton continuer")
,texte_nom_joueur(sing_syst->nom_joueur, "./font/lazy.ttf", 30, BLANC, {300, 225, 200, 100}, sing_syst->rendu, "texte nom_joueur", false)
,titre("Titre du jeu", "./font/lazy.ttf", 30, BLANC, {400, 0, 500, 200}, sing_syst->rendu, "texte titre", false)
,bouton_nouvelle_partie(ROUGE, VERT, BLEU, GRIS, {550, 350, 200, 100}, &fonc_bouton_nouvelle_partie, "NOUVELLE PARTIE", 30, sing_syst->rendu, "Bouton nouvelle partie")
,bouton_options(ROUGE, VERT, BLEU, GRIS, {550, 475, 200, 100}, &fonc_bouton_options, "OPTIONS", 30, sing_syst->rendu, "Bouton options")
,bouton_quitter(ROUGE, VERT, BLEU, GRIS, {550, 600, 200, 100}, &fonc_bouton_quitter, "QUITTER", 30, sing_syst->rendu, "Bouton quitter")
{
    if(sing_syst->nom_joueur.empty() == true)
    {
        bouton_nouvelle_partie.setSelected(&bouton_nouvelle_partie);
        bouton_nouvelle_partie.setSelectedIfMove(nullptr, &bouton_options, nullptr, nullptr);
    }
    else
    {
        bouton_nouvelle_partie.setSelectedIfMove(&bouton_continuer, &bouton_options, nullptr, nullptr);
        bouton_continuer.setSelected(&bouton_continuer);
        bouton_continuer.setSelectedIfMove(nullptr, &bouton_nouvelle_partie, nullptr, nullptr);
        this->ui.push_back(&(this->bouton_continuer));
    }

    bouton_options.setSelectedIfMove(&bouton_nouvelle_partie, &bouton_quitter, nullptr, nullptr);
    bouton_quitter.setSelectedIfMove(&bouton_options, nullptr, nullptr, nullptr);

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

void MenuPrincipal::draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst)
{
    if(sing_syst->nom_joueur.empty() == false)
    {
        bouton_continuer.draw(rendu);
        texte_nom_joueur.draw(rendu);
    }

    bouton_nouvelle_partie.draw(rendu);
    bouton_options.draw(rendu);
    bouton_quitter.draw(rendu);
    this->titre.draw(rendu);
}

void MenuPrincipal::update(Uint32& timeStep)
{
    (void)timeStep;
}


void MenuPrincipal::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    for(Selectionnable* s : ui)
    {
        if(s->name == "Bouton continuer" && sing_syst->nom_joueur.empty() == true)
            continue; //si le nom est vide, alors on ne gère pas les événements du bouton continuer

        if(e.type == SDL_MOUSEMOTION)
        {
            if(s->collision(s->position, x, y) == true)
                this->resetSelected(); //seul ajout
        }
        s->handleEvents(e, sing_syst);
    }
}


void MenuPrincipal::fonc_bouton_quitter(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click quitter" << std::endl;
    sing_syst->sauvegarder();
    sing_syst->destroy(); //il faut tout clean...
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
        sing_syst->supprimmer();
    }
    sing_syst->etat = DEMANDE_NOM;
}

void MenuPrincipal::fonc_bouton_continuer(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click jouer" << std::endl;
    sing_syst->etat = EN_JEU;
}
