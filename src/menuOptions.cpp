#include "menuOptions.hpp"

MenuOptions::MenuOptions(SingletonSysteme* sing_syst)
:texte_touche_haut("Haut", "./font/lazy.ttf", 30, BLANC, {420, 100, 150, 60}, sing_syst->rendu, "texte touche_haut", false)
,texte_touche_bas("Bas", "./font/lazy.ttf", 30, BLANC, {420, 180, 150, 60}, sing_syst->rendu, "texte touche_bas", false)
,texte_touche_gauche("Gauche", "./font/lazy.ttf", 30, BLANC, {420, 260, 150, 60}, sing_syst->rendu, "texte touche_gauche", false)
,texte_touche_droite("Droite", "./font/lazy.ttf", 30, BLANC, {420, 340, 150, 60}, sing_syst->rendu, "texte touche_droite", false)
,mode_ecran("MODE ECRAN", "./font/lazy.ttf", 30, BLANC, {575, 500, 200, 100}, sing_syst->rendu, "texte mode_ecran", false)
,bouton_options_fenetre(ROUGE, VERT, BLEU, GRIS, {575, 600, 200, 100}, &fonc_bouton_options_fenetre, "", 30, sing_syst->rendu, "Bouton options fenetre")
,bouton_options_retour(ROUGE, VERT, BLEU, GRIS, {100, 600, 200, 100}, &fonc_bouton_options_retour, "RETOUR", 30, sing_syst->rendu, "Bouton options retour")
,bouton_options_touche_haut(ROUGE, VERT, BLEU, GRIS, {600, 100, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_haut), 30, sing_syst->rendu, "Bouton options touche haut")
,bouton_options_touche_bas(ROUGE, VERT, BLEU, GRIS, {600, 180, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_bas), 30, sing_syst->rendu, "Bouton options touche bas")
,bouton_options_touche_gauche(ROUGE, VERT, BLEU, GRIS, {600, 260, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_gauche), 30, sing_syst->rendu, "Bouton options touche gauche")
,bouton_options_touche_droite(ROUGE, VERT, BLEU, GRIS, {600, 340, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_droite), 30, sing_syst->rendu, "Bouton options touche droite")
,bouton_options_manette_haut(ROUGE, VERT, BLEU, GRIS, {775, 100, 150, 60}, &fonc_choix_touche_manette, SDL_GameControllerGetStringForButton(sing_syst->touches_1.dep_haut), 30, sing_syst->rendu, "Bouton options manette haut")
,bouton_options_manette_bas(ROUGE, VERT, BLEU, GRIS, {775, 180, 150, 60}, &fonc_choix_touche_manette, SDL_GameControllerGetStringForButton(sing_syst->touches_1.dep_bas), 30, sing_syst->rendu, "Bouton options manette bas")
,bouton_options_manette_gauche(ROUGE, VERT, BLEU, GRIS, {775, 260, 150, 60}, &fonc_choix_touche_manette, SDL_GameControllerGetStringForButton(sing_syst->touches_1.dep_gauche), 30, sing_syst->rendu, "Bouton options manette gauche")
,bouton_options_manette_droite(ROUGE, VERT, BLEU, GRIS, {775, 340, 150, 60}, &fonc_choix_touche_manette, SDL_GameControllerGetStringForButton(sing_syst->touches_1.dep_droite), 30, sing_syst->rendu, "Bouton options manette droite")
,texte_clavier("CLAVIER", "./font/lazy.ttf", 30, BLANC, {600, 50, 50, 50}, sing_syst->rendu,"texte clavier", false)
,texte_manette("MANETTE", "./font/lazy.ttf", 30, BLANC, {775, 50, 50, 50}, sing_syst->rendu, "texte manette", false)
,toggle_sound(BLANC, GRIS, GRIS, GRIS, {280, 100, 50, 50}, "SON", 30, &fonc_toggle_son, sing_syst->son_active, sing_syst->rendu, "toggle sound")
,toggle_musique(BLANC, GRIS, GRIS, GRIS, {1000, 100, 50, 50}, "MUSIQUE", 30, &fonc_toggle_musique, sing_syst->musique_activee, sing_syst->rendu, "toggle musique")
{
    if(sing_syst->mode_fenetre == PLEIN_ECRAN)
        bouton_options_fenetre.texte.texte = "PLEIN ECRAN";
    else if(sing_syst->mode_fenetre == FENETRE)
        bouton_options_fenetre.texte.texte = "FENETRE";

    if(sing_syst->manette != nullptr)
    {
        bouton_options_fenetre.setSelectedIfMove(&bouton_options_touche_droite, nullptr, &bouton_options_retour, nullptr);
        bouton_options_retour.setSelectedIfMove(nullptr, nullptr, nullptr, &bouton_options_fenetre);
        bouton_options_touche_haut.setSelectedIfMove(nullptr, &bouton_options_touche_bas, &toggle_sound, &bouton_options_manette_haut);
        bouton_options_touche_bas.setSelectedIfMove(&bouton_options_touche_haut, &bouton_options_touche_gauche, nullptr, &bouton_options_manette_bas);
        bouton_options_touche_gauche.setSelectedIfMove(&bouton_options_touche_bas, &bouton_options_touche_droite, nullptr, &bouton_options_manette_gauche);
        bouton_options_touche_droite.setSelectedIfMove(&bouton_options_touche_gauche, &bouton_options_fenetre, nullptr, &bouton_options_manette_droite);
        bouton_options_manette_haut.setSelectedIfMove(nullptr, &bouton_options_manette_bas, &bouton_options_touche_haut, &toggle_musique);
        bouton_options_manette_bas.setSelectedIfMove(&bouton_options_manette_haut, &bouton_options_manette_gauche, &bouton_options_touche_bas, nullptr);
        bouton_options_manette_gauche.setSelectedIfMove(&bouton_options_manette_bas, &bouton_options_manette_droite, &bouton_options_touche_gauche, nullptr);
        bouton_options_manette_droite.setSelectedIfMove(&bouton_options_manette_gauche, nullptr, &bouton_options_touche_droite, nullptr);
        bouton_options_retour.setSelected(&bouton_options_retour);

        toggle_sound.setSelectedIfMove(nullptr, nullptr, nullptr, &bouton_options_touche_haut);
        toggle_musique.setSelectedIfMove(nullptr, nullptr, &bouton_options_manette_haut, nullptr);

        this->ui.push_back(&(this->bouton_options_manette_haut));
        this->ui.push_back(&(this->bouton_options_manette_bas));
        this->ui.push_back(&(this->bouton_options_manette_gauche));
        this->ui.push_back(&(this->bouton_options_manette_droite));
    }
    else
    {
        bouton_options_fenetre.setSelectedIfMove(&bouton_options_touche_droite, nullptr, &bouton_options_retour, nullptr);
        bouton_options_retour.setSelectedIfMove(nullptr, nullptr, nullptr, &bouton_options_fenetre);
        bouton_options_touche_haut.setSelectedIfMove(nullptr, &bouton_options_touche_bas, &toggle_sound, &toggle_musique);
        bouton_options_touche_bas.setSelectedIfMove(&bouton_options_touche_haut, &bouton_options_touche_gauche, nullptr, nullptr);
        bouton_options_touche_gauche.setSelectedIfMove(&bouton_options_touche_bas, &bouton_options_touche_droite, nullptr, nullptr);
        bouton_options_touche_droite.setSelectedIfMove(&bouton_options_touche_gauche, &bouton_options_fenetre, nullptr, nullptr);
        toggle_sound.setSelectedIfMove(nullptr, nullptr, nullptr, &bouton_options_touche_haut);
        toggle_musique.setSelectedIfMove(nullptr, nullptr, &bouton_options_touche_haut, nullptr);
        bouton_options_retour.setSelected(&bouton_options_retour);
    }
    this->ui.push_back(&(this->bouton_options_fenetre));
    this->ui.push_back(&(this->bouton_options_retour));
    this->ui.push_back(&(this->bouton_options_touche_haut));
    this->ui.push_back(&(this->bouton_options_touche_bas));
    this->ui.push_back(&(this->bouton_options_touche_gauche));
    this->ui.push_back(&(this->bouton_options_touche_droite));
    this->ui.push_back(&(this->toggle_sound));
    this->ui.push_back(&(this->toggle_musique));
}


void MenuOptions::resetSelected(void)
{
    for(Selectionnable* s : ui)
    {
        s->etat = NORMAL;
    }
}

Selectionnable* MenuOptions::getSelected(void)
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

void MenuOptions::draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst)
{
    for(Selectionnable* s : ui)
    {
        s->draw(rendu);
    }
    texte_touche_haut.draw(rendu);
    texte_touche_bas.draw(rendu);
    texte_touche_gauche.draw(rendu);
    texte_touche_droite.draw(rendu);
    mode_ecran.draw(rendu);

    if(sing_syst->manette != nullptr)
    {
        texte_clavier.draw(rendu);
        texte_manette.draw(rendu);
    }
}

void MenuOptions::update(Uint32& timeStep)
{
    (void)timeStep;
}


void MenuOptions::handleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    for(Selectionnable* s : ui)
    {
        if(e.type == SDL_MOUSEMOTION)
        {
            if(s->collision(s->position, x, y) == true /*&& s->inOnPointerEnter == false*/)
                this->resetSelected(); //seul ajout
        }
        s->handleEvents(e, sing_syst);
    }
}


void MenuOptions::fonc_bouton_options_retour(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click options retour" << std::endl;
    sing_syst->etat = MENU_PRINCIPAL;
}

void MenuOptions::fonc_bouton_options_fenetre(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    std::cout << "click options fenetre" << std::endl;

    Uint32 mask_window = SDL_GetWindowFlags(sing_syst->fenetre);
    if(mask_window & SDL_WINDOW_FULLSCREEN) //si la fenetre est en plein ecran
    {
        CHK(SDL_SetWindowFullscreen(sing_syst->fenetre, 0), SDL_GetError());
        sing_syst->mode_fenetre = FENETRE;
    }
    else //si la fenetre est en mode fenetre
    {
        CHK(SDL_SetWindowFullscreen(sing_syst->fenetre, SDL_WINDOW_FULLSCREEN), SDL_GetError());
        sing_syst->mode_fenetre = PLEIN_ECRAN;
    }
    if(sing_syst->mode_fenetre == FENETRE)
    {
        dynamic_cast<Bouton*>(bouton)->texte.texte = "FENETRE";
    }
    else if(sing_syst->mode_fenetre == PLEIN_ECRAN)
    {
        dynamic_cast<Bouton*>(bouton)->texte.texte = "PLEIN-ECRAN";
    }
}


void MenuOptions::fonc_choix_touche(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    bool quitter = false;
    SDL_Texture* texture;
    NCHK(texture = IMG_LoadTexture(sing_syst->rendu, "./img/pop_up_touche.png"), IMG_GetError());
    int w, h;
    CHK(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h), SDL_GetError());
    SDL_Rect dest = {300, 300, w, h};
    Bouton* b = dynamic_cast<Bouton*>(bouton);
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
                        if((b->texte.texte = SDL_GetKeyName(e.key.keysym.sym)).empty() == true)
                        {
                            std::cerr << "No name" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        if (b->name.find("haut") != std::string::npos)
                            sing_syst->touches.dep_haut = e.key.keysym.sym;
                        else if (b->name.find("bas") != std::string::npos)
                            sing_syst->touches.dep_bas = e.key.keysym.sym;
                        else if (b->name.find("gauche") != std::string::npos)
                            sing_syst->touches.dep_gauche = e.key.keysym.sym;
                        else if (b->name.find("droite") != std::string::npos)
                            sing_syst->touches.dep_droite = e.key.keysym.sym;

                        quitter = true;
                    }
                    break;

                default :
                    break;
            }
        }
        CHK(SDL_RenderCopy(sing_syst->rendu, texture, nullptr, &dest), SDL_GetError());
        SDL_RenderPresent(sing_syst->rendu);
    }
    SDL_DestroyTexture(texture);
}


void MenuOptions::fonc_choix_touche_manette(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    bool quitter = false;
    SDL_Texture* texture;
    NCHK(texture = IMG_LoadTexture(sing_syst->rendu, "./img/pop_up_touche.png"), IMG_GetError());
    int w, h;
    CHK(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h), SDL_GetError());
    SDL_Rect dest = {300, 300, w, h};
    Bouton* b = dynamic_cast<Bouton*>(bouton);
    while(quitter == false)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0)
        {
            switch(e.type)
            {
                case SDL_CONTROLLERBUTTONDOWN:

                    if(e.cbutton.button != sing_syst->touches_1.dep_haut
                    && e.cbutton.button != sing_syst->touches_1.dep_bas
                    && e.cbutton.button != sing_syst->touches_1.dep_gauche
                    && e.cbutton.button != sing_syst->touches_1.dep_droite)
                    {
                        if((b->texte.texte = SDL_GameControllerGetStringForButton((SDL_GameControllerButton)e.cbutton.button)).empty() == true)
                        {
                            std::cerr << "No name" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                        if (b->name.find("haut") != std::string::npos)
                            sing_syst->touches_1.dep_haut = (SDL_GameControllerButton)e.cbutton.button;
                        else if (b->name.find("bas") != std::string::npos)
                            sing_syst->touches_1.dep_bas = (SDL_GameControllerButton)e.cbutton.button;
                        else if (b->name.find("gauche") != std::string::npos)
                            sing_syst->touches_1.dep_gauche = (SDL_GameControllerButton)e.cbutton.button;
                        else if (b->name.find("droite") != std::string::npos)
                            sing_syst->touches_1.dep_droite = (SDL_GameControllerButton)e.cbutton.button;

                        quitter = true;
                    }
                    break;

                default :
                    break;
            }
        }
        CHK(SDL_RenderCopy(sing_syst->rendu, texture, nullptr, &dest), SDL_GetError());
        SDL_RenderPresent(sing_syst->rendu);
    }
    SDL_DestroyTexture(texture);
}


void MenuOptions::fonc_toggle_son(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click toggle son" << std::endl;
    sing_syst->son_active = !(sing_syst->son_active);
}

void MenuOptions::fonc_toggle_musique(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click toggle musique" << std::endl;
    sing_syst->musique_activee = !(sing_syst->musique_activee);
}
