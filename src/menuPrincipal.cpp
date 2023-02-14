#include "menuPrincipal.hpp"

MenuPrincipal::MenuPrincipal(SingletonSysteme* sing_syst)
:bouton_continuer(ROUGE, VERT, BLEU, GRIS, {550, 225, 200, 100}, &fonc_bouton_continuer, "Continuer", 30, sing_syst->rendu, "Bouton continuer")
,texte_nom_joueur(sing_syst->nom_joueur, "./font/lazy.ttf", 30, BLANC, {300, 225, 200, 100}, sing_syst->rendu, "texte nom_joueur", false)
,titre("Titre du jeu", "./font/lazy.ttf", 30, BLANC, {400, 0, 500, 200}, sing_syst->rendu, "texte titre", false)
,bouton_nouvelle_partie(ROUGE, VERT, BLEU, GRIS, {550, 350, 200, 100}, &fonc_bouton_nouvelle_partie, "NOUVELLE PARTIE", 30, sing_syst->rendu, "Bouton nouvelle partie")
,bouton_options(ROUGE, VERT, BLEU, GRIS, {550, 475, 200, 100}, &fonc_bouton_options, "OPTIONS", 30, sing_syst->rendu, "Bouton options")
,bouton_quitter(ROUGE, VERT, BLEU, GRIS, {550, 600, 200, 100}, &fonc_bouton_quitter, "QUITTER", 30, sing_syst->rendu, "Bouton quitter")
,boite(GRIS, {100, 600, 200, 100}, {"Bonjour joueur\n, comment allez vous? Il est bien mon jeu, n'est-ce pas? Dis OUIIIIIIIIIIIIIIIIIIIIIII!",
                                    "kdkjerdherhnderhjncdkxejnrkfcekckrfrkcf,kr,k dssssssssssssssssssssssssssssssssssssssssssssssssss"
                                    }
       , 15, sing_syst->rendu, "Boite de dialogue")
{
    bouton_options.setSelectedIfMove(&bouton_nouvelle_partie, &bouton_quitter, nullptr, nullptr);
    bouton_continuer.setSelectedIfMove(nullptr, &bouton_nouvelle_partie, nullptr, nullptr);

    if(SingletonSysteme::instance().nom_joueur.empty() == true)
    {
        bouton_nouvelle_partie.setSelected(&bouton_nouvelle_partie);
        bouton_nouvelle_partie.setSelectedIfMove(nullptr, &bouton_options, nullptr, nullptr);
    }
    else
    {
        bouton_continuer.setSelected(&bouton_continuer);
        bouton_nouvelle_partie.setSelectedIfMove(&bouton_continuer, &bouton_options, nullptr, nullptr);
    }

    bouton_quitter.setSelectedIfMove(&bouton_options, nullptr, nullptr, nullptr);

    if(sing_syst->nom_joueur.empty() == false)
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

void MenuPrincipal::Draw(SingletonSysteme* sing_syst)
{
    if(sing_syst->nom_joueur.empty() == false)
    {
        bouton_continuer.Draw(sing_syst->rendu);
        texte_nom_joueur.Draw(sing_syst->rendu);
    }

    bouton_nouvelle_partie.Draw(sing_syst->rendu);
    bouton_options.Draw(sing_syst->rendu);
    bouton_quitter.Draw(sing_syst->rendu);
    this->titre.Draw(sing_syst->rendu);
    this->boite.Draw(sing_syst->rendu);
}

void MenuPrincipal::Update(Uint32& timeStep)
{
    bouton_nouvelle_partie.texte.Update(timeStep);
    bouton_options.texte.Update(timeStep);
    bouton_quitter.texte.Update(timeStep);
    boite.Update(timeStep);
}


void MenuPrincipal::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    for(Selectionnable* s : ui)
    {
        if(s->name == "Bouton continuer" && sing_syst->nom_joueur.empty() == true)
            continue; //si le nom est vide, alors on ne gère pas les événements du bouton continuer

        if(e.type == SDL_MOUSEMOTION)
        {
            if(s->collision(s->position, x, y) == true && s->inOnPointerEnter == false) //CA SERT A QUELQUE CHOSE???/////////////////////////////////////////////////////////////////
                this->resetSelected(); //seul ajout
        }
        s->HandleEvents(e, sing_syst);
    }

    this->boite.HandleEvents(e, sing_syst);
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
