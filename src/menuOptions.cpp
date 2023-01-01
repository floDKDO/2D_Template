#include "menuOptions.hpp"

MenuOptions::MenuOptions(SingletonSysteme* sing_syst)
:texte_touche_haut("Haut", "./font/lazy.ttf", BLANC, {420, 100, 150, 60}, sing_syst->rendu)
,texte_touche_bas("Bas", "./font/lazy.ttf", BLANC, {420, 180, 150, 60}, sing_syst->rendu)
,texte_touche_gauche("Gauche", "./font/lazy.ttf", BLANC, {420, 260, 150, 60}, sing_syst->rendu)
,texte_touche_droite("Droite", "./font/lazy.ttf", BLANC, {420, 340, 150, 60}, sing_syst->rendu)
,mode_ecran("MODE ECRAN", "./font/lazy.ttf", BLANC, {575, 500, 200, 100}, sing_syst->rendu)
,bouton_options_fenetre(ROUGE, VERT, BLEU, GRIS, {575, 600, 200, 100}, &fonc_bouton_options_fenetre, "", sing_syst->rendu, "Bouton options fenetre")
,bouton_options_retour(ROUGE, VERT, BLEU, GRIS, {100, 600, 200, 100}, &fonc_bouton_options_retour, "RETOUR", sing_syst->rendu, "Bouton options retour")
,bouton_options_touche_haut(ROUGE, VERT, BLEU, GRIS, {600, 100, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_haut), sing_syst->rendu, "Bouton options touche haut")
,bouton_options_touche_bas(ROUGE, VERT, BLEU, GRIS, {600, 180, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_bas), sing_syst->rendu, "Bouton options touche bas")
,bouton_options_touche_gauche(ROUGE, VERT, BLEU, GRIS, {600, 260, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_gauche), sing_syst->rendu, "Bouton options touche gauche")
,bouton_options_touche_droite(ROUGE, VERT, BLEU, GRIS, {600, 340, 150, 60}, &fonc_choix_touche, SDL_GetKeyName(sing_syst->touches.dep_droite), sing_syst->rendu, "Bouton options touche droite")
,toggle_sound(BLANC, GRIS, GRIS, GRIS, {280, 100, 50, 50}, "SON", &fonc_toggle_son, sing_syst->son_active, sing_syst->rendu)
,toggle_musique(BLANC, GRIS, GRIS, GRIS, {1000, 100, 50, 50}, "MUSIQUE", &fonc_toggle_musique, sing_syst->musique_activee, sing_syst->rendu)
{
    if(SingletonSysteme::instance().mode_fenetre == PLEIN_ECRAN)
        bouton_options_fenetre.texte.texte = "PLEIN ECRAN";
    else if(SingletonSysteme::instance().mode_fenetre == FENETRE)
        bouton_options_fenetre.texte.texte = "FENETRE";
    //HAUT, BAS, GAUCHE, DROITE
    bouton_options_fenetre.setSelectedIfMove(&bouton_options_touche_droite, nullptr, &bouton_options_retour, nullptr);
    bouton_options_retour.setSelectedIfMove(nullptr, nullptr, nullptr, &bouton_options_fenetre);
    bouton_options_touche_haut.setSelectedIfMove(nullptr, &bouton_options_touche_bas, &toggle_sound, &toggle_musique);
    bouton_options_touche_bas.setSelectedIfMove(&bouton_options_touche_haut, &bouton_options_touche_gauche, nullptr, nullptr);
    bouton_options_touche_gauche.setSelectedIfMove(&bouton_options_touche_bas, &bouton_options_touche_droite, nullptr, nullptr);
    bouton_options_touche_droite.setSelectedIfMove(&bouton_options_touche_gauche, &bouton_options_fenetre, nullptr, nullptr);
    toggle_sound.setSelectedIfMove(nullptr, nullptr, nullptr, &bouton_options_touche_haut);
    toggle_musique.setSelectedIfMove(nullptr, nullptr, &bouton_options_touche_haut, nullptr);
    bouton_options_retour.setSelected(&bouton_options_retour);

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

void MenuOptions::Draw(SDL_Renderer* rendu, SingletonSysteme* sing_syst)
{
    for(Selectionnable* s : ui)
    {
        s->Draw(rendu);
    }
    texte_touche_haut.Draw(rendu);
    texte_touche_bas.Draw(rendu);
    texte_touche_gauche.Draw(rendu);
    texte_touche_droite.Draw(rendu);
    mode_ecran.Draw(rendu);
}


void MenuOptions::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    for(Selectionnable* s : ui)
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
        if(SDL_SetWindowFullscreen(sing_syst->fenetre, 0) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
        sing_syst->mode_fenetre = FENETRE;
    }
    else //si la fenetre est en mode fenetre
    {
        if(SDL_SetWindowFullscreen(sing_syst->fenetre, SDL_WINDOW_FULLSCREEN) < 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }
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
    if((texture = IMG_LoadTexture(sing_syst->rendu, "./img/pop_up_touche.png")) == nullptr)
    {
        std::cerr << IMG_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_Rect dest;
    int w, h;
    if(SDL_QueryTexture(texture, nullptr, nullptr, &w, &h) < 0)
    {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    dest = {300, 300, w, h};
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
                        if (b->tag.find("haut") != std::string::npos)
                            sing_syst->touches.dep_haut = e.key.keysym.sym;
                        else if (b->tag.find("bas") != std::string::npos)
                            sing_syst->touches.dep_bas = e.key.keysym.sym;
                        else if (b->tag.find("gauche") != std::string::npos)
                            sing_syst->touches.dep_gauche = e.key.keysym.sym;
                        else if (b->tag.find("droite") != std::string::npos)
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
