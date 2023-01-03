#include "menuChoixNom.hpp"

MenuChoixNom::MenuChoixNom(SingletonSysteme* sing_syst)
:inputfield("./font/lazy.ttf", 30, ROUGE, {300, 400, 200, 50}, &fonc_inputfield_nom_joueur, sing_syst->rendu, "inputfield choix nom")
,demande_nom("Ecrivez votre nom", "./font/lazy.ttf", 30, BLANC, {300, 200, 300, 150}, sing_syst->rendu, "texte dmande_nom")
,bouton_valider(ROUGE, VERT, BLEU, GRIS, {300, 550, 200, 100}, &fonc_bouton_fin_demande_nom, "VALIDER", 30, sing_syst->rendu, "Bouton valider")
{
    inputfield.setSelectedIfMove(nullptr, &bouton_valider, nullptr, nullptr);
    bouton_valider.setSelectedIfMove(&inputfield, nullptr, nullptr, nullptr);
    bouton_valider.setSelected(&bouton_valider);

    this->ui.push_back(&(this->inputfield));
    this->ui.push_back(&(this->bouton_valider));
}


void MenuChoixNom::fonc_bouton_fin_demande_nom(SingletonSysteme* sing_syst, Selectionnable* bouton)
{
    (void)bouton;
    std::cout << "click jouer" << std::endl;
    if(sing_syst->nom_joueur.length() > 0)
    {
        sing_syst->etat = EN_JEU;
    }
}

void MenuChoixNom::fonc_inputfield_nom_joueur(SingletonSysteme* sing_syst, Selectionnable* inputfield)
{
    sing_syst->nom_joueur = dynamic_cast<Inputfield*>(inputfield)->texte.texte;
}


void MenuChoixNom::resetSelected(void)
{
    for(Selectionnable* s : ui)
    {
        s->etat = NORMAL;
    }
}

Selectionnable* MenuChoixNom::getSelected(void)
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


void MenuChoixNom::Draw(SingletonSysteme* sing_syst)
{
    inputfield.Draw(sing_syst->rendu);
    demande_nom.Draw(sing_syst->rendu);
    bouton_valider.Draw(sing_syst->rendu);
    if(inputfield.etat == SELECTED)
        bouton_valider.etat = NORMAL;
}


void MenuChoixNom::Update(Uint32& timeStep)
{
    inputfield.Update(timeStep);
}


void MenuChoixNom::HandleEvents(SDL_Event e, SingletonSysteme* sing_syst)
{
    int x, y; //position x et y de la souris
    SDL_GetMouseState(&x, &y);

    for(Selectionnable* s : ui)
    {
        if(dynamic_cast<Inputfield*>(s) != nullptr)
        {
            s->HandleEvents(e, sing_syst);
        }
        else
        {
            if(e.type == SDL_MOUSEMOTION)
            {
                if(s->collision(s->position, x, y) == true && s->inOnPointerEnter == false)
                    this->resetSelected(); //seul ajout
            }
            s->HandleEvents(e, sing_syst);
        }
    }
}
